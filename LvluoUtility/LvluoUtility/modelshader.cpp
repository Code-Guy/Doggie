#include "modelshader.h"

#include <glm/gtc/type_ptr.hpp>

bool ModelShader::init()
{
	if (!Shader::init())
	{
		return false;
	}

	if (!addShader(GL_VERTEX_SHADER, "Resources/Shaders/model.vert"))
	{
		return false;
	}
	if (!addShader(GL_FRAGMENT_SHADER, "Resources/Shaders/model.frag"))
	{
		return false;
	}
	if (!finalize())
	{
		return false;
	}

	mLocation = getUniformLocation("model");
	vpLocation = getUniformLocation("viewProjection");
	cameraPositionLocation = getUniformLocation("cameraPosition");

	materialLocation.diffuseColor = getUniformLocation("material.diffuseColor");
	materialLocation.specularColor = getUniformLocation("material.specularColor");
	materialLocation.diffuseTexture = getUniformLocation("material.diffuseTexture");
	materialLocation.specularTexture = getUniformLocation("material.specularTexture");
	materialLocation.normalTexture = getUniformLocation("material.normalTexture");
	materialLocation.useDiffuseTexture = getUniformLocation("material.useDiffuseTexture");
	materialLocation.useSpecularTexture = getUniformLocation("material.useSpecularTexture");
	materialLocation.useNormalTexture = getUniformLocation("material.useNormalTexture");
	materialLocation.shininess = getUniformLocation("material.shininess");

	directionalLightLocation.direction = getUniformLocation("directionalLight.direction");
	directionalLightLocation.ambient = getUniformLocation("directionalLight.ambient");
	directionalLightLocation.diffuse = getUniformLocation("directionalLight.diffuse");
	directionalLightLocation.specular = getUniformLocation("directionalLight.specular");

	for (int i = 0; i < PointLightNum; i++)
	{
		pointLightLocations[i].position = getUniformLocation(("pointLights[" + std::to_string(i) + "].position").c_str());
		pointLightLocations[i].constant = getUniformLocation(("pointLights[" + std::to_string(i) + "].constant").c_str());
		pointLightLocations[i].linear = getUniformLocation(("pointLights[" + std::to_string(i) + "].linear").c_str());
		pointLightLocations[i].quadratic = getUniformLocation(("pointLights[" + std::to_string(i) + "].quadratic").c_str());
		pointLightLocations[i].ambient = getUniformLocation(("pointLights[" + std::to_string(i) + "].ambient").c_str());
		pointLightLocations[i].diffuse = getUniformLocation(("pointLights[" + std::to_string(i) + "].diffuse").c_str());
		pointLightLocations[i].specular = getUniformLocation(("pointLights[" + std::to_string(i) + "].specular").c_str());
	}

	spotLightLocation.position = getUniformLocation("spotLight.position");
	spotLightLocation.direction = getUniformLocation("spotLight.direction");
	spotLightLocation.innerCutoff = getUniformLocation("spotLight.innerCutoff");
	spotLightLocation.outerCutoff = getUniformLocation("spotLight.outerCutoff");
	spotLightLocation.constant = getUniformLocation("spotLight.constant");
	spotLightLocation.linear = getUniformLocation("spotLight.linear");
	spotLightLocation.quadratic = getUniformLocation("spotLight.quadratic");
	spotLightLocation.ambient = getUniformLocation("spotLight.ambient");
	spotLightLocation.diffuse = getUniformLocation("spotLight.diffuse");
	spotLightLocation.specular = getUniformLocation("spotLight.specular");

	if (mLocation == INVALID_UNIFORM_LOCATION ||
		vpLocation == INVALID_UNIFORM_LOCATION ||
		cameraPositionLocation == INVALID_UNIFORM_LOCATION ||
		materialLocation.diffuseColor == INVALID_UNIFORM_LOCATION ||
		materialLocation.specularColor == INVALID_UNIFORM_LOCATION ||
		materialLocation.diffuseTexture == INVALID_UNIFORM_LOCATION ||
		materialLocation.specularTexture == INVALID_UNIFORM_LOCATION ||
		materialLocation.normalTexture == INVALID_UNIFORM_LOCATION ||
		materialLocation.useDiffuseTexture == INVALID_UNIFORM_LOCATION ||
		materialLocation.useSpecularTexture == INVALID_UNIFORM_LOCATION ||
		materialLocation.useNormalTexture == INVALID_UNIFORM_LOCATION ||
		materialLocation.shininess == INVALID_UNIFORM_LOCATION ||
		directionalLightLocation.direction == INVALID_UNIFORM_LOCATION ||
		directionalLightLocation.ambient == INVALID_UNIFORM_LOCATION ||
		directionalLightLocation.diffuse == INVALID_UNIFORM_LOCATION ||
		directionalLightLocation.specular == INVALID_UNIFORM_LOCATION ||
		spotLightLocation.position == INVALID_UNIFORM_LOCATION ||
		spotLightLocation.direction == INVALID_UNIFORM_LOCATION ||
		spotLightLocation.innerCutoff == INVALID_UNIFORM_LOCATION ||
		spotLightLocation.outerCutoff == INVALID_UNIFORM_LOCATION ||
		spotLightLocation.constant == INVALID_UNIFORM_LOCATION ||
		spotLightLocation.linear == INVALID_UNIFORM_LOCATION ||
		spotLightLocation.quadratic == INVALID_UNIFORM_LOCATION ||
		spotLightLocation.ambient == INVALID_UNIFORM_LOCATION ||
		spotLightLocation.diffuse == INVALID_UNIFORM_LOCATION ||
		spotLightLocation.specular == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	for (int i=0; i<3; i++)
	{
		if (pointLightLocations[i].position == INVALID_UNIFORM_LOCATION ||
			pointLightLocations[i].constant == INVALID_UNIFORM_LOCATION ||
			pointLightLocations[i].linear == INVALID_UNIFORM_LOCATION ||
			pointLightLocations[i].quadratic == INVALID_UNIFORM_LOCATION ||
			pointLightLocations[i].ambient == INVALID_UNIFORM_LOCATION ||
			pointLightLocations[i].diffuse == INVALID_UNIFORM_LOCATION ||
			pointLightLocations[i].specular == INVALID_UNIFORM_LOCATION)
		{
			return false;
		}
	}

	colorIDLocation = getUniformLocation("colorID");
	pickOnLocation = getUniformLocation("pickOn");
	blendOnLocation = getUniformLocation("blendOn");

	if (colorIDLocation == INVALID_UNIFORM_LOCATION ||
		pickOnLocation == INVALID_UNIFORM_LOCATION ||
		blendOnLocation == INVALID_UNIFORM_LOCATION)
	{
		return false;
	}

	enable();
	glUniform1i(materialLocation.diffuseTexture, 0);
	glUniform1i(materialLocation.specularTexture, 1);
	glUniform1i(materialLocation.normalTexture, 2);

	return true;
}

void ModelShader::setModel(glm::mat4 m)
{
	glUniformMatrix4fv(mLocation, 1, false, glm::value_ptr(m));
}

void ModelShader::setViewProjection(glm::mat4 vp)
{
	glUniformMatrix4fv(vpLocation, 1, false, glm::value_ptr(vp));
}

void ModelShader::setCameraPosition(glm::vec3 cameraPosition)
{
	glUniform3fv(cameraPositionLocation, 1, glm::value_ptr(cameraPosition));
}

void ModelShader::setMaterial(Material &material)
{
	// diffuse
	if (material.useDiffuseTexture)
	{
		glUniform1i(materialLocation.useDiffuseTexture, 1);
		material.diffuseTexture->bind(GL_TEXTURE0);
	}
	else
	{
		glUniform1i(materialLocation.useDiffuseTexture, 0);
		glUniform4fv(materialLocation.diffuseColor, 1, glm::value_ptr(material.diffuseColor));
	}

	// specular 
	if (material.useSpecularTexture)
	{
		glUniform1i(materialLocation.useSpecularTexture, 1);
		material.specularTexture->bind(GL_TEXTURE1);
	}
	else
	{
		glUniform1i(materialLocation.useSpecularTexture, 0);
		glUniform4fv(materialLocation.specularColor, 1, glm::value_ptr(material.specularColor));
	}

	// normal map
	if (material.useNormalTexture)
	{
		glUniform1i(materialLocation.useNormalTexture, 1);
		material.normalTexture->bind(GL_TEXTURE2);
	}
	else
	{
		glUniform1i(materialLocation.useNormalTexture, 0);
	}

	// shininess
	glUniform1f(materialLocation.shininess, material.shininess);
}

void ModelShader::setDirectionalLight(const DirectionalLight &dl)
{
	glUniform3fv(directionalLightLocation.direction, 1, glm::value_ptr(dl.direction));
	glUniform3fv(directionalLightLocation.ambient, 1, glm::value_ptr(dl.ambient));
	glUniform3fv(directionalLightLocation.diffuse, 1, glm::value_ptr(dl.diffuse));
	glUniform3fv(directionalLightLocation.specular, 1, glm::value_ptr(dl.specular));
}

void ModelShader::setPointLights(PointLight pls[PointLightNum])
{
	for (int i=0; i<PointLightNum; i++)
	{
		glUniform3fv(pointLightLocations[i].position, 1, glm::value_ptr(pls[i].position));
		glUniform1f(pointLightLocations[i].constant, pls[i].constant);
		glUniform1f(pointLightLocations[i].linear, pls[i].linear);
		glUniform1f(pointLightLocations[i].quadratic, pls[i].quadratic);
		glUniform3fv(pointLightLocations[i].ambient, 1, glm::value_ptr(pls[i].ambient));
		glUniform3fv(pointLightLocations[i].diffuse, 1, glm::value_ptr(pls[i].diffuse));
		glUniform3fv(pointLightLocations[i].specular, 1, glm::value_ptr(pls[i].specular));
	}
}

void ModelShader::setSpotLight(const SpotLight &sl)
{
	glUniform3fv(spotLightLocation.position, 1, glm::value_ptr(sl.position));
	glUniform3fv(spotLightLocation.direction, 1, glm::value_ptr(sl.direction));
	glUniform1f(spotLightLocation.innerCutoff, sl.innerCutoff);
	glUniform1f(spotLightLocation.outerCutoff, sl.outerCutoff);
	glUniform1f(spotLightLocation.constant, sl.constant);
	glUniform1f(spotLightLocation.linear, sl.linear);
	glUniform1f(spotLightLocation.quadratic, sl.quadratic);
	glUniform3fv(spotLightLocation.ambient, 1, glm::value_ptr(sl.ambient));
	glUniform3fv(spotLightLocation.diffuse, 1, glm::value_ptr(sl.diffuse));
	glUniform3fv(spotLightLocation.specular, 1, glm::value_ptr(sl.specular));
}

void ModelShader::setColorID(glm::vec3 colorID)
{
	glUniform3fv(colorIDLocation, 1, glm::value_ptr(colorID));
}

void ModelShader::setPickOn(bool pickOn)
{
	glUniform1i(pickOnLocation, pickOn);
}

void ModelShader::setBlendOn(bool blendOn)
{
	glUniform1i(blendOnLocation, blendOn);
}