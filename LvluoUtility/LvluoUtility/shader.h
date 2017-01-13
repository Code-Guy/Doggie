#ifndef SHADER_H
#define SHADER_H

#include <list>
#include <string>
#include <GL/glew.h>

#include "lvluoapi.h"

class LVLUO_API Shader
{
public:
	Shader();
	virtual ~Shader();

	virtual bool init();

	void enable();

	bool validate();

protected:
	bool addShader(GLenum ShaderType, const char *fileName);
	bool finalize(bool isValidate = true);

	GLint getUniformLocation(const char *uniformName);
	GLint getProgramParam(GLint param);

	GLuint shaderProg;    

private:
#define INVALID_UNIFORM_LOCATION 0xffffffff

	std::list<GLuint> shaderObjList;
};

#endif // SHADER_H