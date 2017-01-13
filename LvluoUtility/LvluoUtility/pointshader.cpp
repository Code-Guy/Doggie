#include "pointshader.h"

#include <glm/gtc/type_ptr.hpp>

bool PointShader::init()
{
	if (!Shader::init())
	{
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, "Resources/Shaders/point.vert"))
	{
		return false;
	}
	if (!addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/point.frag"))
	{
		return false;
	}
	if (!finalize())
	{
		return false;
	}

	MVPLocation = getUniformLocation("mvp");
	colorLocation = getUniformLocation("color");
	colorIDLocation = getUniformLocation("colorID");
	pickOnLocation = getUniformLocation("pickOn");
	blendOnLocation = getUniformLocation("blendOn");

	if (MVPLocation == INVALID_UNIFORM_LOCATION ||
		colorLocation == INVALID_UNIFORM_LOCATION ||
		colorIDLocation == INVALID_UNIFORM_LOCATION ||
		pickOnLocation == INVALID_UNIFORM_LOCATION ||
		blendOnLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void PointShader::setMVP(glm::mat4 mvp)
{
	glUniformMatrix4fv(MVPLocation, 1, false, glm::value_ptr(mvp));
}

void PointShader::setColor(glm::vec3 color)
{
	glUniform3fv(colorLocation, 1, glm::value_ptr(color));
}

void PointShader::setColorID(glm::vec3 colorID)
{
	glUniform3fv(colorIDLocation, 1, glm::value_ptr(colorID));
}

void PointShader::setPickOn(bool pickOn)
{
	glUniform1i(pickOnLocation, pickOn);
}

void PointShader::setBlendOn(bool blendOn)
{
	glUniform1i(blendOnLocation, blendOn);
}