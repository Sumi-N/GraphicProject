#pragma once
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include "Material.h"

namespace FileLoader
{
	void recompileShader();

	Texture* ReadTexture(char const *texName);
}