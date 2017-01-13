#include "baseshader.h"

#include <glm/gtc/type_ptr.hpp>

bool BaseShader::init()
{
	if (!Shader::init())
	{
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, "Resources/Shaders/base.vert"))
	{
		return false;
	}
	if (!addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/base.frag"))
	{
		return false;
	}
	if (!finalize())
	{
		return false;
	}

	MVPLocation = getUniformLocation("mvp");
	colorLocation = getUniformLocation("color");

	if (MVPLocation == INVALID_UNIFORM_LOCATION ||
		colorLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void BaseShader::setMVP(glm::mat4 mvp)
{
	glUniformMatrix4fv(MVPLocation, 1, false, glm::value_ptr(mvp));
}

void BaseShader::setColor(glm::vec3 color)
{
	glUniform3fv(colorLocation, 1, glm::value_ptr(color));
}