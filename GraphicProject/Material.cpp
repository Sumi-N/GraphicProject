#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Material.h"
#include "lodepng.h"
#include "Utility.h"

void Material::Load(const char * vert, const char * frag)
{
	std::vector<GLchar> vertsrc;
	std::vector<GLchar> fragsrc;

	ReadShaderSource(vert, vertsrc);
	ReadShaderSource(frag, fragsrc);

	// Create program;
	if (programid != 0)
	{
		glDeleteProgram(programid);		
	}

	programid = glCreateProgram();

	// Attach the shaders to the program

	// Vertex Shader
	if (vertsrc.data() != nullptr)
	{
		const GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
		const char * vertsrcponter = vertsrc.data();
		glShaderSource(vshader, 1, &vertsrcponter, NULL);
		glCompileShader(vshader);

		if (Utility::printShaderInfoLog(vshader, "vertex shader"))
		{
			glAttachShader(programid, vshader);
		}
		glDeleteShader(vshader);
	}

	// Fragment Shader
	if (fragsrc.data() != nullptr)
	{
		const GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
		const char * fragsrcpointer = fragsrc.data();
		glShaderSource(fshader, 1, &fragsrcpointer, NULL);
		glCompileShader(fshader);

		if (Utility::printShaderInfoLog(fshader, "fragment shader"))
		{
			glAttachShader(programid, fshader);
		}
		glDeleteShader(fshader);
	}

	// Link the program
	glBindAttribLocation(programid, 0, "position");
	glBindAttribLocation(programid, 1, "color");
	glBindFragDataLocation(programid, 0, "fragment");
	glLinkProgram(programid);

	if (Utility::printProgramInfoLog(programid))
	{
		printf("suceed the shader compiling process\n");
	}
	//glDeleteProgram(programid);
}

void Material::BindShader()
{
	glUseProgram(programid);
}

bool Material::ReadShaderSource(const char* filename, std::vector<GLchar> &buffer)
{
	if (filename == nullptr)
	{
		return false;
	}

	std::ifstream file(filename, std::ios::binary);
	if (file.fail())
	{
		std::cerr << "Error: Can't open source file" << filename << std::endl;
		return false;
	}

	file.seekg(0L, std::ios::end);
	GLsizei size = static_cast<GLsizei>(file.tellg());

	buffer.resize(size + 1);
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), size);
	buffer[size] = '\0';

	if (file.fail())
	{
		std::cerr << "Error: Could not read source file:" << filename << std::endl;
		file.close();
		return false;
	}

	file.close();
	return true;
}

Texture::Texture()
{
}


Texture::~Texture()
{
}

bool Texture::Load(char const * filename)
{
	data.clear();
	width = 0;
	height = 0;
	char const *name = filename ? filename : "";
	if (name[0] == '\0') return false;

	int len = (int)strlen(name);
	if (len < 3) return false;

	bool success = false;

	char ext[3] = { (char)tolower(name[len - 3]), (char)tolower(name[len - 2]), (char)tolower(name[len - 1]) };

	if (strncmp(ext, "png", 3) == 0)
	{
		std::vector<unsigned char> d;
		unsigned int w, h;
		unsigned int error = lodepng::decode(d, w, h, name, LCT_RGB);
		if (error == 0)
		{
			width = w;
			height = h;
			data.resize(width*height);
			memcpy(data.data(), d.data(), width*height * 3);
		}
		success = (error == 0);
	}

	return success;
}
