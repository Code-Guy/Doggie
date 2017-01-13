#ifndef POINTSHADER_H
#define POINTSHADER_H

#include "shader.h"
#include <glm/glm.hpp>
#include "lvluoapi.h"

class LVLUO_API PointShader : public Shader
{
public:
	virtual bool init();

	void setMVP(glm::mat4 mvp);
	void setColor(glm::vec3 color);
	void setColorID(glm::vec3 colorID);
	void setPickOn(bool pickOn);
	void setBlendOn(bool blendOn);

private:
	GLuint MVPLocation;
	GLuint colorLocation;
	GLuint colorIDLocation;
	GLuint pickOnLocation;
	GLuint blendOnLocation;
};

#endif // POINTSHADER_H
