#include "billboardshader.h"

#include <glm/gtc/type_ptr.hpp>

bool BillboardShader::init()
{
	if (!Shader::init()) {
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, "Resources/Shaders/billboard.vert")) {
		return false;
	}
	if (!addShader(GL_GEOMETRY_SHADER, "Resources/Shaders/billboard.geom")) {
		return false;
	}
	if (!addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/billboard.frag")) {
		return false;
	}
	if (!finalize()) {
		return false;
	}

	MVLocation = getUniformLocation("mv");
	PLocation = getUniformLocation("p");
	textureLocation = getUniformLocation("diffuseTexture");
	colorLocation = getUniformLocation("color");
	sizeLocation = getUniformLocation("size");
	colorIDLocation = getUniformLocation("colorID");
	pickOnLocation = getUniformLocation("pickOn");
	blendOnLocation = getUniformLocation("blendOn");

	if (MVLocation == INVALID_UNIFORM_LOCATION ||
		PLocation == INVALID_UNIFORM_LOCATION ||
		textureLocation == INVALID_UNIFORM_LOCATION ||
		colorLocation == INVALID_UNIFORM_LOCATION ||
		sizeLocation == INVALID_UNIFORM_LOCATION ||
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

void BillboardShader::setMV(glm::mat4 mv)
{
	glUniformMatrix4fv(MVLocation, 1, false, glm::value_ptr(mv));
}

void BillboardShader::setP(glm::mat4 p)
{
	glUniformMatrix4fv(PLocation, 1, false, glm::value_ptr(p));
}

void BillboardShader::setTextureUnit(unsigned int textureUnit)
{
	glUniform1i(textureLocation, textureUnit);
}

void BillboardShader::setColor(glm::vec3 color)
{
	glUniform3fv(colorLocation, 1, glm::value_ptr(color));
}

void BillboardShader::setSize(float size)
{
	glUniform1f(sizeLocation, size);
}

void BillboardShader::setColorID(glm::vec3 colorID)
{
	glUniform3fv(colorIDLocation, 1, glm::value_ptr(colorID));
}

void BillboardShader::setPickOn(bool pickOn)
{
	glUniform1i(pickOnLocation, pickOn);
}

void BillboardShader::setBlendOn(bool blendOn)
{
	glUniform1i(blendOnLocation, blendOn);
}