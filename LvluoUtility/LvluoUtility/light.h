#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "lvluoapi.h"

struct LVLUO_API Light
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct LVLUO_API DirectionalLight : public Light
{
	glm::vec3 direction;
};

struct LVLUO_API PointLight : public Light
{
	glm::vec3 position;

	float constant;
	float linear;
	float quadratic;
};

struct LVLUO_API SpotLight : public PointLight
{
	glm::vec3 direction;

	float innerCutoff;
	float outerCutoff;
};

#endif // LIGHT_H
