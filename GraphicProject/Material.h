#pragma once

#include <GL/glew.h>
#include <vector>
#include "cyCodeBase/cyColor.h"
#include <glm/gtc/matrix_transform.hpp>

class Material
{
public:
	float Ka[3];  //!< Ambient  color
	float Kd[3];  //!< Diffuse  color
	float Ks[3];  //!< Specular color
	float Ns;     //!< Specular exponent

	glm::vec3 diffuse;
	glm::vec4 specular;

	// Functions for openGL
public:
	GLuint programid = 0;
	void Load(const char * vert, const char * frag);
	void BindShader();

private:
	bool ReadShaderSource(const char* filename, std::vector<GLchar> &buffer);
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



