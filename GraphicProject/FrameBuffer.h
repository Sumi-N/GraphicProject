#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	GLuint bufferid;
	GLuint targettexture;
	GLuint targetdepth;

	GLsizei width;
	GLsizei height;

	void Init(int width, int height);
};

inline void FrameBuffer::Init(int width,int height)
{
	this->width = width;
	this->height = height;

	// Create frame buffer
	glGenFramebuffers(1, &bufferid);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferid);

	// Create empty textures
	glGenTextures(1, &targettexture);
	glBindTexture(GL_TEXTURE_2D, targettexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Create empty texture for depth
	glGenTextures(1, &targetdepth);
	glBindTexture(GL_TEXTURE_2D, targetdepth);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, width, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	// bind texture to framebuffer
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 0, targettexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, targetdepth, 0);//optional

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

