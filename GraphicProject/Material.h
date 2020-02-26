#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
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
	GLuint uniformid_skybox;
	std::vector<Texture> texturelist;

	void LoadShader(const char * vert, const char * frag);
	void BindSkyBox(class CubeMap);
	void BindShader();

	void LoadTexture(const char *);
	void LoadCubeMapTexture(const char **, int);


private:
	bool ReadShaderSource(const char* filename, std::vector<GLchar> &buffer);
};
