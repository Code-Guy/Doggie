#ifndef TEXTUREMASKSHADER_H
#define TEXTUREMASKSHADER_H

#include "shader.h"
#include <glm/glm.hpp>
#include "lvluoapi.h"

class LVLUO_API TextureMaskShader : public Shader
{
public:
	virtual bool init();

	void setMVP(glm::mat4 mvp);
	void setTextureUnit(GLint textureUnit);
	void setColor(glm::vec3 color);
	void setColorID(glm::vec3 colorID);
	void setPickOn(bool pickOn);
	void setBlendOn(bool blendOn);

private:
	GLuint MVPLocation;
	GLuint textureLocation;
	GLuint colorLocation;
	GLuint colorIDLocation;
	GLuint pickOnLocation;
	GLuint blendOnLocation;
};

#endif // TEXTUREMASKSHADER_H