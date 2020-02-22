#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include "cyCodeBase/cyColor.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"

class Material
{
public:
	float Ka[3];  //!< Ambient  color
	float Kd[3];  //!< Diffuse  color
	float Ks[3];  //!< Specular color
	float Ns;     //!< Specular exponent

	// Functions for openGL
public:
	GLuint programid = 0;
	std::vector<Texture> texturelist;

	void LoadShader(const char * vert, const char * frag);
	void BindShader();

	void LoadTexture(const char *);


private:
	bool ReadShaderSource(const char* filename, std::vector<GLchar> &buffer);
};
