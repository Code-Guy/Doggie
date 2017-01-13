#ifndef TEXTURESHADER_H
#define TEXTURESHADER_H

#include "shader.h"
#include <glm/glm.hpp>
#include "lvluoapi.h"

class LVLUO_API TextureShader : public Shader
{
public:
	virtual bool init();

	void setMVP(glm::mat4 mvp);
	void setTextureUnit(GLint textureUnit);
	void setColorID(glm::vec3 colorID);
	void setPickOn(bool pickOn);
	void setBlendOn(bool blendOn);
	void setInverse(bool inverse);

private:
	GLuint MVPLocation;
	GLuint textureLocation;
	GLuint colorIDLocation;
	GLuint pickOnLocation;
	GLuint blendOnLocation;
	GLuint inverseLocation;
};

#endif // TEXTURESHADER_H