#include "FrameBuffer.h"



FrameBuffer::FrameBuffer()
{
}


FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::Init(int width, int height)
{
	this->width = width;
	this->height = height;

	// Create frame buffer
	glGenFramebuffers(1, &bufferid);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferid);

	// Create color buffer
	color.CreateTexture(GL_RGBA, width, height);
	// Create depth buffer
	depth.CreateTexture(GL_DEPTH_COMPONENT, width, height);

	// bind textures to framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color.textureid, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth.textureid, 0);

	GLenum drawbuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawbuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{  //Check for FBO completeness
		std::cout << "Error! FrameBuffer is not complete" << std::endl;
		std::cin.get();
		std::terminate();
	}

	// Set back to original back buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
