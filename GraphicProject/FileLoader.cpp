#pragma once

#include "FileLoader.h"
#include "Constatnt.h"
#include "Utility.h"

namespace FileLoader
{
	bool readShaderSource(const char *name, std::vector<GLchar> &buffer)
	{
		if (name == nullptr)
		{
			return false;
		}

		std::ifstream file(name, std::ios::binary);
		if (file.fail())
		{
			std::cerr << "Error: Can't open source file" << name << std::endl;
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
			std::cerr << "Error: Could not read source file:" << name << std::endl;
			file.close();
			return false;
		}

		file.close();
		return true;
	}

	GLuint createShaderProgram(const char *vertscr, const char * fragsrc)
	{
		const GLuint program(glCreateProgram());

		if (vertscr != nullptr)
		{
			const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
			glShaderSource(vobj, 1, &vertscr, NULL);
			glCompileShader(vobj);

			if (Utility::printShaderInfoLog(vobj, "vertex shader"))
			{
				glAttachShader(program, vobj);
			}
			glDeleteShader(vobj);
		}

		if (fragsrc != nullptr)
		{
			const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
			glShaderSource(fobj, 1, &fragsrc, NULL);
			glCompileShader(fobj);

			if (Utility::printShaderInfoLog(fobj, "fragment shader"))
			{
				glAttachShader(program, fobj);
			}
			glDeleteShader(fobj);
		}

		glBindAttribLocation(program, 0, "position");
		glBindAttribLocation(program, 1, "color");
		glBindFragDataLocation(program, 0, "fragment");
		glLinkProgram(program);

		if (Utility::printProgramInfoLog(program))
		{
			return program;
		}
		glDeleteProgram(program);
		return 0;
	}

	GLuint loadShaderProgram(const char *vert, const char *frag)
	{
		std::vector<GLchar> vertsrc;
		std::vector<GLchar> fragsrc;

		return readShaderSource(vert, vertsrc) && readShaderSource(frag, fragsrc) ? createShaderProgram(vertsrc.data(), fragsrc.data()) : 0;
	}

	void recompileShader()
	{
		GLuint program;
		program = loadShaderProgram(VERTEXSHADERPATH, FRAGMENTSHADERPATH);
		glUseProgram(program);
	}
}