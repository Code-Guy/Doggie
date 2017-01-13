#ifndef PICKFBO_H
#define PICKFBO_H

#include <GL/glew.h>

class PickFBO
{
public:
	PickFBO();
	~PickFBO();

	void bind();
	void unBind();

	void drawBuffer(GLenum buffer);
	void readBuffer(GLenum buffer);

	void resize(int w, int h);

private:
	GLuint fbo;
	GLuint pickTex;
	GLuint depthRBO;
};

#endif // PICKFBO_H