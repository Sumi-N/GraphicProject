#pragma once

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Material.h"
#include "Texture.h"
#include "CubeMap.h"
#include "lodepng.h"
#include "Utility.h"

void Material::LoadShader(const char * vert, const char * frag)
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
	glBindAttribLocation(programid, 0, "model_position");
	glBindAttribLocation(programid, 1, "model_normal");
	glBindAttribLocation(programid, 2, "model_texcoord");
	glBindFragDataLocation(programid, 0, "color");
	glLinkProgram(programid);

	if (Utility::printProgramInfoLog(programid))
	{
		//printf("suceed the shader compiling process\n");
	}
	//glDeleteProgram(programid);

	// Get uniform cubemap texture skybox
	uniformid_skybox = glGetUniformLocation(programid, "skybox");
	if (uniformid_skybox == -1)
	{
		std::cerr << "The skybox variable doesn't exist in the shader file" << std::endl;
	}
}

void Material::BindSkyBox(CubeMap cubemap)
{
	Texture * cubetexture = cubemap.GetCubeMapTexture();
	glActiveTexture(GL_TEXTURE0 + cubetexture->unitnumber);
	glBindTexture(GL_TEXTURE_2D, cubetexture->textureid);
	glUniform1i(uniformid_skybox, cubetexture->unitnumber);
}

void Material::BindShadowMap()
{

}

void Material::BindShader()
{
	glUseProgram(programid);
	if (texturelist.size() != 0)
	{
		for (int i = 0; i < texturelist.size(); i++)
		{
			texturelist[i].BindTexture();
		}
	}
}

void Material::LoadTexture(const char * filename)
{
	Texture texture;
	texture.Load(filename);
	int unitnumber = static_cast<int>(texturelist.size());
	texture.Init(unitnumber, programid);
	texturelist.push_back(texture);
}

void Material::LoadCubeMapTexture(const char ** filenames, int unitnum)
{
	Texture cubemap;
	cubemap.InitCubeMap(unitnum, programid, filenames);
	texturelist.push_back(cubemap);
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


