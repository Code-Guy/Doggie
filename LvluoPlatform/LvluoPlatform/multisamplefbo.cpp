#include "multisamplefbo.h"
#include "appconfig.h"
#include "sceneproperty.h"

MultiSampleFBO::MultiSampleFBO()
{
	lastMSAA = -1;
	lastViewport = glm::ivec4(-1, -1, -1, -1);

	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &tex);
	glGenRenderbuffers(1, &rbo);
}

MultiSampleFBO::~MultiSampleFBO()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &tex);
	glDeleteRenderbuffers(1, &rbo);
}

void MultiSampleFBO::logic(glm::ivec4 viewport)
{
	int msaa = AppConfig::getInstance()->getMSAA();
	if (msaa != lastMSAA || 
		viewport != lastViewport)
	{
		lastMSAA = msaa;
		lastViewport = viewport;

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaa, GL_RGB, viewport.z, viewport.w, GL_TRUE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, GL_DEPTH24_STENCIL8, viewport.z, viewport.w);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void MultiSampleFBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void MultiSampleFBO::unBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MultiSampleFBO::blit()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, lastViewport.z, lastViewport.w, 0, 0, lastViewport.z, lastViewport.w, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}