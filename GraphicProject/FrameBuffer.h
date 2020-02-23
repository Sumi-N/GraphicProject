#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Texture.h"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	Texture color;
	Texture depth;

	GLuint bufferid;
	GLsizei width;
	GLsizei height;

	void Init(int width, int height);
};

