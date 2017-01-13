#include "textureshader.h"

#include <glm/gtc/type_ptr.hpp>

bool TextureShader::init()
{
	if (!Shader::init()) 
	{
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, "Resources/Shaders/texture.vert"))
	{
		return false;
	}
	if (!addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/texture.frag"))
	{
		return false;
	}
	if (!finalize()) 
	{
		return false;
	}

	MVPLocation = getUniformLocation("mvp");
	textureLocation = getUniformLocation("diffuseTexture");
	colorIDLocation = getUniformLocation("colorID");
	pickOnLocation = getUniformLocation("pickOn");
	blendOnLocation = getUniformLocation("blendOn");
	inverseLocation = getUniformLocation("inverse");

	if (MVPLocation == INVALID_UNIFORM_LOCATION || 
		textureLocation == INVALID_UNIFORM_LOCATION ||
		colorIDLocation == INVALID_UNIFORM_LOCATION ||
		pickOnLocation == INVALID_UNIFORM_LOCATION ||
		inverseLocation == INVALID_UNIFORM_LOCATION ||
		blendOnLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	enable();
	glUniform1i(textureLocation, 0);

	return true;
}

void TextureShader::setMVP(glm::mat4 mvp)
{
	glUniformMatrix4fv(MVPLocation, 1, false, glm::value_ptr(mvp));
}

void TextureShader::setTextureUnit(GLint textureUnit)
{
	glUniform1i(textureLocation, textureUnit);
}

void TextureShader::setColorID(glm::vec3 colorID)
{
	glUniform3fv(colorIDLocation, 1, glm::value_ptr(colorID));
}

void TextureShader::setPickOn(bool pickOn)
{
	glUniform1i(pickOnLocation, pickOn);
}

void TextureShader::setBlendOn(bool blendOn)
{
	glUniform1i(blendOnLocation, blendOn);
}

void TextureShader::setInverse(bool inverse)
{
	glUniform1i(inverseLocation, inverse);
}