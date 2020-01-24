#pragma once

namespace Utiliti
{

	GLboolean printShaderInfoLog(GLuint shader, const char *str)
	{
		// Get compile status
		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) std::cerr << "Compile Error in " << str << std::endl;
		// Get the length of the compile log
		GLsizei bufSize;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
		if (bufSize > 1)
		{
			// Get the contents of the compile log
			std::vector<GLchar> infoLog(bufSize);
			GLsizei length;
			glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
			std::cerr << &infoLog[0] << std::endl;
		}
		return static_cast<GLboolean>(status);
	}

	GLboolean printProgramInfoLog(GLuint program)
	{
		// Get a link result
		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;
		// Get the log of the link result
		GLsizei bufSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
		if (bufSize > 1)
		{
			// Get the contents of the link result
			std::vector<GLchar> infoLog(bufSize);
			GLsizei length;
			glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
			std::cerr << &infoLog[0] << std::endl;
		}
		return static_cast<GLboolean>(status);
	}
}