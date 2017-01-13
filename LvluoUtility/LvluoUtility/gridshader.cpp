#include "gridshader.h"

#include <glm/gtc/type_ptr.hpp>

bool GridShader::init()
{
	if (!Shader::init())
	{
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, "Resources/Shaders/grid.vert"))
	{
		return false;
	}
	if (!addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/grid.frag"))
	{
		return false;
	}
	if (!finalize())
	{
		return false;
	}

	colorLocation = getUniformLocation("color");
	if (colorLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	return true;
}

void GridShader::setColor(glm::vec3 color)
{
	glUniform3fv(colorLocation, 1, glm::value_ptr(color));
}