#include "pickfbo.h"

PickFBO::PickFBO()
{
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &pickTex);
	glBindTexture(GL_TEXTURE_2D, pickTex);

	glGenRenderbuffers(1, &depthRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pickTex, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PickFBO::~PickFBO()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &pickTex);
	glDeleteRenderbuffers(1, &depthRBO);
}

void PickFBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void PickFBO::unBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PickFBO::drawBuffer(GLenum buffer)
{
	glDrawBuffer(buffer);
}

void PickFBO::readBuffer(GLenum buffer)
{
	glReadBuffer(buffer);
}

void PickFBO::resize(int w, int h)
{
	glBindTexture(GL_TEXTURE_2D, pickTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, depthRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
}