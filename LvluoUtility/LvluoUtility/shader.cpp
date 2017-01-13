#include "shader.h"
#include "tool.h"

Shader::Shader()
{
	shaderProg = 0;
}

Shader::~Shader()
{
	for (std::list<GLuint>::iterator it = shaderObjList.begin() ; it != shaderObjList.end() ; it++)
	{
		glDeleteShader(*it);
	}

	if (shaderProg != 0)
	{
		glDeleteProgram(shaderProg);
		shaderProg = 0;
	}
}

bool Shader::init()
{
	shaderProg = glCreateProgram();

	if (shaderProg == 0) 
	{
		printf("Error creating shader program\n");
		return false;
	}
	
	return true;
}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::addShader(GLenum ShaderType, const char *fileName)
{
	std::string s;

	if (!Tool::readFile(fileName, s)) {
		return false;
	}

	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		printf("Error creating shader type %d\n", ShaderType);
		return false;
	}

	// Save the shader object - will be deleted in the destructor
	shaderObjList.push_back(ShaderObj);

	const GLchar* p[1];
	p[0] = s.c_str();
	GLint Lengths[1] = { (GLint)s.size() };

	glShaderSource(ShaderObj, 1, p, Lengths);

	glCompileShader(ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	if (!success) 
	{
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		printf("Error compiling '%s': '%s'\n", fileName, InfoLog);
		return false;
	}

	glAttachShader(shaderProg, ShaderObj);
	return true;
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::finalize(bool isValidate)
{
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glLinkProgram(shaderProg);
	glGetProgramiv(shaderProg, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		printf("Error linking shader program: '%s'\n", ErrorLog);
		return false;
	}
	if (isValidate)
	{
		validate();
	}
	// Delete the intermediate shader objects that have been added to the program
	for (std::list<GLuint>::iterator it = shaderObjList.begin() ; it != shaderObjList.end() ; it++) {
		glDeleteShader(*it);
	}

	shaderObjList.clear();

	return glGetError() == GL_NO_ERROR;
}

bool Shader::validate()
{
	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };
	glValidateProgram(shaderProg);
	glGetProgramiv(shaderProg, GL_VALIDATE_STATUS, &Success);
	if (!Success)
	{
		glGetProgramInfoLog(shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
		printf("Invalid shader program: '%s'\n", ErrorLog);
		return false;
	}
	return true;
}

void Shader::enable()
{
	glUseProgram(shaderProg);
}

GLint Shader::getUniformLocation(const char *uniformName)
{
	GLuint Location = glGetUniformLocation(shaderProg, uniformName);

	if (Location == INVALID_UNIFORM_LOCATION) 
	{
		printf("Warning! Unable to get the location of uniform '%s'\n", uniformName);
	}

	return Location;
}

GLint Shader::getProgramParam(GLint param)
{
	GLint ret;
	glGetProgramiv(shaderProg, param, &ret);
	return ret;
}