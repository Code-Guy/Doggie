#ifndef BILLBOARDSHADER_H
#define BILLBOARDSHADER_H

#include "shader.h"
#include <glm/glm.hpp>
#include "lvluoapi.h"

class LVLUO_API BillboardShader : public Shader
{
public:
	virtual bool init();

	void setMV(glm::mat4 mv);
	void setP(glm::mat4 p);
	void setTextureUnit(unsigned int textureUnit);
	void setColor(glm::vec3 color);
	void setSize(float size);
	void setColorID(glm::vec3 colorID);
	void setPickOn(bool pickOn);
	void setBlendOn(bool blendOn);

private:
	GLuint MVLocation;
	GLuint PLocation;
	GLuint textureLocation;
	GLuint colorLocation;
	GLuint sizeLocation;
	GLuint colorIDLocation;
	GLuint pickOnLocation;
	GLuint blendOnLocation;
};

#endif //BILLBOARDSHADER_H