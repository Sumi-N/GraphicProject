#pragma once
#include <vector>
#include "cyCodeBase/cyColor.h"

class Texture
{

public:
	Texture();
	~Texture();

	std::vector<cy::Color24> data;
	int width, height;

	bool Load(char const *);
};

class Material
{
public:
	float Ka[3];  //!< Ambient  color
	float Kd[3];  //!< Diffuse  color
	float Ks[3];  //!< Specular color
	float Ns;     //!< Specular exponent
};



