#include "GLTexture.h"
#include "../../GRender.h"

static unsigned int lastTextureNumber = 0;
const char* GLTexture::TAG = "OpenGL Texture";

#include <sstream>
using namespace std;

GLTexture::GLTexture() {
	m_glTextureID = 0;
	glGenTextures(1, &m_glTextureID);

	textureDebugIdentifier = lastTextureNumber;
	lastTextureNumber++;
}

GLTexture::~GLTexture() {
	glDeleteTextures(1, &m_glTextureID);
}

void GLTexture::loadTextureData(InputDataFormat format, unsigned int width,
		unsigned int height, bool generateMipMaps, void* inputData) {

	if (width == 0 || height == 0) {

		stringstream dbgIdStrm;
		dbgIdStrm << textureDebugIdentifier;

		GRenderer::getRenderer()->getRenderLog()->writeLine(
				TAG,
				string("Texture ID: ") + dbgIdStrm.str()
						+ " called with a bad with or height");

		return;
	}

	//Test if the width is a power of two (Width BITWISEAND (Width - 1)) will only equal 0 when it is a power of two
	if ((width & (width - 1)) != 0) {

		stringstream dbgIdStrm;
		dbgIdStrm << textureDebugIdentifier;

		GRenderer::getRenderer()->getRenderLog()->writeLine(
				TAG,
				string("Texture ID: ") + dbgIdStrm.str()
						+ " Attempting to buffer texture with no pow 2 texture");

		return;
	}

	GLuint internalFormat = 0;
	GLuint dataFormat = 0;

	switch (format) {
	case input_data_byte_rgb: {
		internalFormat = GL_RGB;
		dataFormat = GL_UNSIGNED_BYTE;

		break;
	}
		;

	case input_data_float_rgb: {
		internalFormat = GL_RGB;
		dataFormat = GL_FLOAT;

		break;
	}
		;

	case input_data_byte_rgba: {

		internalFormat = GL_RGBA;
		dataFormat = GL_UNSIGNED_BYTE;

		break;
	}
		;

	case input_data_float_rgba: {
		internalFormat = GL_RGBA;
		dataFormat = GL_FLOAT;

		break;
	}
		;

	case input_data_byte_luminance: {
		internalFormat = GL_LUMINANCE;
		dataFormat = GL_UNSIGNED_BYTE;

		break;
	}

	case input_data_float_luminance: {

		internalFormat = GL_LUMINANCE;
		dataFormat = GL_FLOAT;

		break;
	}
		;

	case input_data_byte_luminance_alpha: {
		internalFormat = GL_LUMINANCE_ALPHA;
		dataFormat = GL_UNSIGNED_BYTE;

		break;
	}
		;

	case input_data_float_luminance_alpha: {
		internalFormat = GL_LUMINANCE_ALPHA;
		dataFormat = GL_FLOAT;
		break;
	}
		;

	default: {

		printf("Unknown format");
		return;

	}
		;

	}

	glBindTexture(GL_TEXTURE_2D, m_glTextureID);

	if (generateMipMaps == true)
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0,
			internalFormat, dataFormat, inputData);

	if (generateMipMaps == true) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_LINEAR_MIPMAP_LINEAR);
	} else {

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	glBindTexture(GL_TEXTURE_2D, 0);

	m_textureWidth = width;
	m_textureHeight = height;
}

void GLTexture::makeActive() {

	if (m_glTextureID != 0) {

		glBindTexture(GL_TEXTURE_2D, m_glTextureID);

	}

}
