#ifndef GRIDSHADER_H
#define GRIDSHADER_H

#include "shader.h"
#include <glm/glm.hpp>
#include "lvluoapi.h"

class LVLUO_API GridShader : public Shader
{
public:
	virtual bool init();

	void setColor(glm::vec3 color);

private:
	GLuint colorLocation;
};

#endif // GRIDSHADER_H