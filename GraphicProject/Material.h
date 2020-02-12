#pragma once

#include <GL/glew.h>
#include <vector>
#include "cyCodeBase/cyColor.h"

class Material
{
public:
	float Ka[3];  //!< Ambient  color
	float Kd[3];  //!< Diffuse  color
	float Ks[3];  //!< Specular color
	float Ns;     //!< Specular exponent

	// Functions for openGL
public:
	GLuint s_programId = 0;
};

class Texture
{

public:
	Texture();
	~Texture();

	std::vector<cy::Color24> data;
	int width, height;

	bool Load(char const *);
};



