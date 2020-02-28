#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Material.h"
#include "lodepng.h"
#include "Utility.h"

#include "Texture.h"

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

void Texture::Init(int unitnum, GLint programid)
{
	unitnumber = unitnum;

	glGenTextures(1, &textureid);
	glActiveTexture(GL_TEXTURE0 + unitnumber);
	glBindTexture(GL_TEXTURE_2D, textureid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	uniformname = "texture" + std::to_string(unitnum);
	uniformid = glGetUniformLocation(programid, uniformname.c_str());
	if (uniformid == -1)
	{
		std::cerr << "The texture variable doesn't exist in the shader file" << std::endl;
	}
}

void Texture::InitCubeMap(int unitnum, GLint programid, const char ** filenames)
{
	unitnumber = unitnum;

	glGenTextures(1, &textureid);
	glActiveTexture(GL_TEXTURE0 + unitnumber);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);

	Load(filenames[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	Load(filenames[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	Load(filenames[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	Load(filenames[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	Load(filenames[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	Load(filenames[5]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	uniformid = glGetUniformLocation(programid, "skybox");
	if (uniformid == -1)
	{
		std::cerr << "The cubemap variable doesn't exist in the shader file" << std::endl;
	}
}

void Texture::BindTexture()
{
	glActiveTexture(GL_TEXTURE0 + unitnumber);
	glBindTexture(GL_TEXTURE_2D, textureid);
	glUniform1i(uniformid, unitnumber);
}

void Texture::CreateTexture(GLint internalformat, GLuint width, GLuint height, GLenum type)
{
	this->width = width;
	this->height = height;

	glGenTextures(1, &textureid);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureid);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, internalformat, type, data.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}