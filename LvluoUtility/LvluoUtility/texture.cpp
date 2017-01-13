#include "texture.h"

Texture::Texture(QString fileName, GLuint minFilter, GLuint magFilter) : minFilter(minFilter), magFilter(magFilter)
{
	QImage image(fileName);
	if (image.isNull())
	{
		printf("opening the texture failed\n");
		valid = false;
	}
	else
	{
		valid = load(image);
	}
}

Texture::Texture(int w, int h, GLuint minFilter, GLuint magFilter) : minFilter(minFilter), magFilter(magFilter)
{
	QImage image(w, h, QImage::Format_RGBA8888);
	image.fill(Qt::white);
	valid = load(image);
}

Texture::~Texture()
{
	glDeleteTextures(1, &texture);
}

bool Texture::load(const QImage &image)
{
	QImage img = image.convertToFormat(QImage::Format_RGBA8888);
	img = img.mirrored();

	width = img.width();
	height = img.height();

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (glGetError() != GL_NO_ERROR)
	{
		printf("loading the texture failed: %d\n", glGetError());
		return false;
	}

	return true;
}

void Texture::bind(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

bool Texture::isValid()
{
	return valid;
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}

void Texture::setBits(unsigned char *bits, GLenum format)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,
		format, GL_UNSIGNED_BYTE, bits);
}