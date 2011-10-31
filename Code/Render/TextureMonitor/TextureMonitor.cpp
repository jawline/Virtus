#include "TextureMonitor.h"
#include "../GRender.h"

const char* TextureMonitor::TAG = "TextureMonitor";

TextureMonitor::TextureMonitor() {
	printf("Texture Monitor initialized\n");
}

TextureMonitor::~TextureMonitor() {
	printf("Destroying texture monitor\n");

	for (unsigned int i = 0; i < m_resourceList.size(); ++i) {
		f_freeEntity(m_resourceList[i].m_Entity);
	}

	m_resourceList.clear();
}

void TextureMonitor::f_freeEntity(void* m_Entity) {
	Texture* texture = (Texture*) m_Entity;
	delete texture;

	return;
}

void* TextureMonitor::f_loadEntity(std::string Name) {

	Texture* newTexture =
			GRenderer::getRenderer()->getGraphicsAPI()->newTexture();

	if (newTexture == 0) {
		GRenderer::getRenderer()->getRenderLog()->writeLine(
				TAG,
				string("Unable to load texture") + Name
						+ " due to being unable to create a texture object");
		return 0;
	}

	FilesystemNode* node = Filesystem::evaluateChild(Filesystem::getRoot(),
			Name);

	if (node == 0) {
		GRenderer::getRenderer()->getRenderLog()->writeLine(
				TAG,
				string("Unable to load texture") + Name
						+ " due to being unable to locate the file (FILE NOT FOUND)");
		return 0;
	}

	if (node->getType() != fileId) {
		GRenderer::getRenderer()->getRenderLog()->writeLine(
				TAG,
				string("Unable to load texture") + Name
						+ " as the specified filesystem node is not a readable file");

		return 0;
	}

	File* castToFile = static_cast<File*> (node);

	DataInputStream* newStream = castToFile->createInputStream();
	newStream->open(false);

	if (m_Loader.load(newTexture, newStream) == false) {

		GRenderer::getRenderer()->getRenderLog()->writeLine(
				TAG,
				string("Unable to load texture") + Name
						+ " due to the texture loader being unable to load this file");

		delete newTexture;

		//Delete the input stream once its been used
		delete newStream;

		return 0;

	} else {

		f_addEntity(Name, (void*) newTexture);

		//Delete the input stream once its been used
		delete newStream;

		return (void*) newTexture;
	}
}

Texture* TextureMonitor::getTexture(const char* Filename) {
	return (Texture*) rmGetResource(Filename);
}

void TextureMonitor::releaseTexture(Texture* Texture) {
	rmReleaseResource((void*) Texture);
}
