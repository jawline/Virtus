#include <GL/glew.h>
#include "GLShader.h"
#include "../../GRender.h"
#include "GLTexture.h"
#include <sstream>
using namespace std;

unsigned int GLShader::m_numCreated = 0;
const char* GLShader::TAG = "OpenGL Shader";

GLShader::GLShader() {

	m_debugID = m_numCreated;
	m_numCreated++;

	stringstream dbgIdStrm;
	dbgIdStrm << m_debugID;

	GRenderer::getRenderer()->getRenderLog()->writeLine(TAG,
			string("Creating unique shader number ") + dbgIdStrm.str());

	m_vertexShaderID = 0;
	m_fragmentShaderID = 0;
	m_shaderProgramID = 0;

	m_textureLocations = 0;
	m_numTextures = 0;
}

GLShader::~GLShader() {

	if (m_numTextures != 0) {
		delete[] m_textureLocations;
		m_textureLocations = 0;
		m_numTextures = 0;
	}

	glDetachShader(m_shaderProgramID, m_fragmentShaderID);
	glDetachShader(m_shaderProgramID, m_vertexShaderID);

	glDeleteShader(m_fragmentShaderID);
	glDeleteShader(m_vertexShaderID);

	glDeleteProgram(m_shaderProgramID);

	m_shaderProgramID = 0;
	m_fragmentShaderID = 0;
	m_vertexShaderID = 0;

	stringstream dbgIdStrm;
	dbgIdStrm << m_debugID;

	GRenderer::getRenderer()->getRenderLog()->writeLine(TAG,
			string("Destroying shader unique number ") + dbgIdStrm.str());

}

bool GLShader::loadShader(const char* vert, const char* frag) {
	while (glGetError() != 0) {
		//Get rid of existing GL errors
	}

	if (m_shaderProgramID != 0) {
		printf("Deleting existing shader\n");

		glDetachShader(m_shaderProgramID, m_fragmentShaderID);
		glDetachShader(m_shaderProgramID, m_vertexShaderID);

		glDeleteShader(m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);

		glDeleteProgram(m_shaderProgramID);

		m_shaderProgramID = 0;
		m_fragmentShaderID = 0;
		m_vertexShaderID = 0;

	}

	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(m_vertexShaderID, 1, (const GLchar**) &vert, 0);

	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragmentShaderID, 1, (const GLchar**) &frag, 0);

	glCompileShader(m_vertexShaderID);
	glCompileShader(m_fragmentShaderID);

	int isCompiledVS = 0;
	int isCompiledFS = 0;

	glGetShaderiv(m_vertexShaderID, GL_COMPILE_STATUS, &isCompiledVS);
	glGetShaderiv(m_fragmentShaderID, GL_COMPILE_STATUS, &isCompiledFS);

	if (isCompiledFS == false || isCompiledVS == false) {
		char Buffer[1024];
		sprintf(
				Buffer,
				"Could not compiler shaders. fragment result %i vertex result %i",
				isCompiledFS, isCompiledVS);

		if (isCompiledFS == false) {

			GRenderer::getRenderer()->getRenderLog()->writeLine(TAG,
					"Fragment error in passed shader");

			int len;
			glGetShaderInfoLog(m_fragmentShaderID, 1024, &len, Buffer);

			GRenderer::getRenderer()->getRenderLog()->writeLine(TAG,
					string("Error: ") + Buffer);
		}

		if (isCompiledVS == false) {

			GRenderer::getRenderer()->getRenderLog()->writeLine(TAG,
					"Vertex error in passed shader");

			int len;
			glGetShaderInfoLog(m_vertexShaderID, 1024, &len, Buffer);

			GRenderer::getRenderer()->getRenderLog()->writeLine(TAG,
					string("Error: ") + Buffer);
		}

		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);

		m_vertexShaderID = 0;
		m_fragmentShaderID = 0;
		m_shaderProgramID = 0;

		return false;
	}

	m_shaderProgramID = glCreateProgram();

	glAttachShader(m_shaderProgramID, m_vertexShaderID);
	glAttachShader(m_shaderProgramID, m_fragmentShaderID);

	glLinkProgram(m_shaderProgramID);

	int isLinked = 0;

	glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, (int *) &isLinked);

	if (isLinked == 0) {
		glDetachShader(m_shaderProgramID, m_fragmentShaderID);
		glDetachShader(m_shaderProgramID, m_vertexShaderID);

		glDeleteShader(m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);

		glDeleteProgram(m_shaderProgramID);

		m_shaderProgramID = 0;
		m_fragmentShaderID = 0;
		m_vertexShaderID = 0;
	}

	unsigned int errorMessage = glGetError();

	if (errorMessage != 0) {
		GRenderer::getRenderer()->getRenderLog()->writeLine(TAG,
				"Error occured compiling shader");
	}

	printf("Compiled shader results %i %i %i\n", isLinked, isCompiledVS,
			isCompiledFS);

	return true;
}

void GLShader::setNumberOfTextures(unsigned int number) {
	if (number == 0) {
		if (m_numTextures != 0) {
			delete[] m_textureLocations;
			m_textureLocations = 0;
			m_numTextures = 0;
		}

	} else {

		if (m_numTextures != 0) {
			delete[] m_textureLocations;
			m_textureLocations = 0;
			m_numTextures = 0;
		}

		m_textureLocations = new GLuint[number];

		for (unsigned int i = 0; i < number; ++i) {
			m_textureLocations[i] = 0;
		}

		m_numTextures = number;

	}
}

void GLShader::setTextureName(unsigned int i, const char* Name) {
	if (m_numTextures <= i)
		return;

	if (m_shaderProgramID == 0)
		return;

	activateShader();

	m_textureLocations[i] = glGetUniformLocation(m_shaderProgramID, Name);

	glUniform1i(m_textureLocations[i], static_cast<GLint> (i));
}

void GLShader::setTexture(unsigned int i, Texture* tex) {
	if (m_numTextures <= i)
		return;

	if (m_shaderProgramID == 0)
		return;

	activateShader();

	glActiveTexture(GL_TEXTURE0 + i);

	if (tex == 0)
		glBindTexture(GL_TEXTURE_2D, 0);
	else
		glBindTexture(GL_TEXTURE_2D, ((GLTexture*) (tex))->getTextureID());

	glActiveTexture(GL_TEXTURE0);
}

void GLShader::activateShader() {
	glUseProgram(m_shaderProgramID);
}

void GLShader::deactivateShader() {
	glUseProgram(0);
}
