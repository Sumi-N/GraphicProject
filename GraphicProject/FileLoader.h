#pragma once
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include "Texture.h"

namespace FileLoader
{
	bool readShaderSource(const char *name, std::vector<GLchar> &buffer);

	GLuint createShaderProgram(const char *vertscr, const char * fragsrc);

	GLuint loadShaderProgram(const char *vert, const char *frag);

	void recompileShader();

	Texture* ReadTexture(char const *texName);
}