#ifndef MULTISAMPLEFBO_H
#define MULTISAMPLEFBO_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class MultiSampleFBO
{
public:
	MultiSampleFBO();
	~MultiSampleFBO();

	void logic(glm::ivec4 viewport);

	void bind();
	void unBind();

	void blit();

private:
	GLuint fbo;
	GLuint tex;
	GLuint rbo;

	int lastMSAA;
	glm::ivec4 lastViewport;
};

#endif // MULTISAMPLEFBO_H