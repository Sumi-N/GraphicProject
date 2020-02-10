#pragma once
#include <vector>
#include "cyCodeBase/cyColor.h"

using namespace cy;

class Texture
{
	struct MyColor
	{
		float r;
		float g;
		float b;
	};

public:
	Texture();
	~Texture();

	char * name;
	bool Load();
	void SetName(char const *);
	std::vector<Color24> data;
	std::vector<MyColor> cleaneddata;
	int width, height;

	void Organize();
};

