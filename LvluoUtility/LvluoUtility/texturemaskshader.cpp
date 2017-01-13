#include "texturemaskshader.h"

#include <glm/gtc/type_ptr.hpp>

bool TextureMaskShader::init()
{
	if (!Shader::init()) 
	{
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, "Resources/Shaders/texturemask.vert"))
	{
		return false;
	}
	if (!addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/texturemask.frag"))
	{
		return false;
	}
	if (!finalize()) 
	{
		return false;
	}

	MVPLocation = getUniformLocation("mvp");
	textureLocation = getUniformLocation("diffuseTexture");
	colorLocation = getUniformLocation("color");
	colorIDLocation = getUniformLocation("colorID");
	pickOnLocation = getUniformLocation("pickOn");
	blendOnLocation = getUniformLocation("blendOn");

	if (MVPLocation == INVALID_UNIFORM_LOCATION || 
		textureLocation == INVALID_UNIFORM_LOCATION || 
		colorLocation == INVALID_UNIFORM_LOCATION ||
		colorIDLocation == INVALID_UNIFORM_LOCATION ||
		pickOnLocation == INVALID_UNIFORM_LOCATION ||
		blendOnLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	enable();
	glUniform1i(textureLocation, 0);

	return true;
}

void TextureMaskShader::setMVP(glm::mat4 mvp)
{
	glUniformMatrix4fv(MVPLocation, 1, false, glm::value_ptr(mvp));
}

void TextureMaskShader::setTextureUnit(GLint textureUnit)
{
	glUniform1i(textureLocation, textureUnit);
}

void TextureMaskShader::setColor(glm::vec3 color)
{
	glUniform3fv(colorLocation, 1, glm::value_ptr(color));
}

void TextureMaskShader::setColorID(glm::vec3 colorID)
{
	glUniform3fv(colorIDLocation, 1, glm::value_ptr(colorID));
}

void TextureMaskShader::setPickOn(bool pickOn)
{
	glUniform1i(pickOnLocation, pickOn);
}

void TextureMaskShader::setBlendOn(bool blendOn)
{
	glUniform1i(blendOnLocation, blendOn);
}