#ifndef MODELSHADER_H
#define MODELSHADER_H

#include "shader.h"
#include "light.h"
#include "mesh.h"
#include <glm/glm.hpp>

#include "lvluoapi.h"

class LVLUO_API ModelShader : public Shader
{
#define PointLightNum 3
public:
	virtual bool init();

	void setModel(glm::mat4 m);
	void setViewProjection(glm::mat4 vp);
	void setCameraPosition(glm::vec3 cameraPosition);
	void setMaterial(Material &material);
	void setDirectionalLight(const DirectionalLight &dl);
	void setPointLights(PointLight pls[PointLightNum]);
	void setSpotLight(const SpotLight &sl);
	void setColorID(glm::vec3 colorID);
	void setPickOn(bool pickOn);
	void setBlendOn(bool blendOn);

private:
	GLuint mLocation;
	GLuint vpLocation;
	GLuint cameraPositionLocation;

	struct
	{
		GLuint diffuseColor;
		GLuint specularColor;
		GLuint diffuseTexture;
		GLuint specularTexture;
		GLuint normalTexture;
		GLuint useDiffuseTexture;
		GLuint useSpecularTexture;
		GLuint useNormalTexture;

		GLuint shininess;
	} materialLocation;

	struct
	{
		GLuint direction;
		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
	} directionalLightLocation;

	struct
	{
		GLuint position;

		GLuint constant;
		GLuint linear;
		GLuint quadratic;

		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
	} pointLightLocations[PointLightNum];

	struct
	{
		GLuint position;
		GLuint direction;

		GLuint innerCutoff;
		GLuint outerCutoff;

		GLuint constant;
		GLuint linear;
		GLuint quadratic;

		GLuint ambient;
		GLuint diffuse;
		GLuint specular;
	} spotLightLocation;

	GLuint colorIDLocation;
	GLuint pickOnLocation;
	GLuint blendOnLocation;
};

#endif // MODELSHADER_H
