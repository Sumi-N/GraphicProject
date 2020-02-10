#pragma once
#include <vector>
#include "cyCodeBase/cyColor.h"

using namespace cy;

class Texture
{
public:
	Texture();
	~Texture();

	char * name;
	bool Load();
	void SetName(char const *);
	std::vector<Color24> data;
	int width, height;
};

