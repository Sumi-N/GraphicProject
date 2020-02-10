#pragma once
#include <GL/glew.h>

namespace Utility
{

	GLboolean printShaderInfoLog(GLuint shader, const char *str);

	GLboolean printProgramInfoLog(GLuint program);
}