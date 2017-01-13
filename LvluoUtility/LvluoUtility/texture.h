#ifndef TEXTURE_H
#define TEXTURE_H

#include <QString>
#include <QImage>
#include <GL/glew.h>

#include "lvluoapi.h"

class LVLUO_API Texture
{
public:
	Texture(QString fileName, GLuint minFilter = GL_LINEAR, GLuint magFilter = GL_LINEAR);
	Texture(int w, int h, GLuint minFilter = GL_LINEAR, GLuint magFilter = GL_LINEAR);

	~Texture();

	void bind(GLenum textureUnit = GL_TEXTURE0);

	bool isValid();

	int getWidth();
	int getHeight();

	void setBits(unsigned char *bits, GLenum format);

private:
	bool valid;

	GLuint minFilter;
	GLuint magFilter;

	GLuint texture;

	int width;
	int height;

	bool load(const QImage &image);
};

#endif //TEXTURE_H