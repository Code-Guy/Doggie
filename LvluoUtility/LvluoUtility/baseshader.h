#ifndef BASESHADER_H
#define BASESHADER_H

#include "shader.h"
#include <glm/glm.hpp>
#include "lvluoapi.h"

class LVLUO_API BaseShader : public Shader
{
public:
	virtual bool init();

	void setMVP(glm::mat4 mvp);
	void setColor(glm::vec3 color);

private:
	GLuint MVPLocation;
	GLuint colorLocation;
};

#endif // BASESHADER_H