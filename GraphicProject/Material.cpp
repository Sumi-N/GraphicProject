#pragma once
#include "Material.h"
#include "lodepng.h"


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
