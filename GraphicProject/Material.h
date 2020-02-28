#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"

class Material
{
public:
	glm::vec3 Ka = glm::vec3(1.0, 1.0, 1.0); //!< Ambient  color
	glm::vec3 Kd = glm::vec3(1.0, 1.0, 1.0); //!< Diffuse  color
	glm::vec3 Ks = glm::vec3(1.0, 1.0, 1.0); //!< Specular color
	float Ns = 0.0f;	                     //!< Specular exponent

	// Functions for openGL
public:
	GLuint programid = 0;
	GLuint uniformid_skybox;
	std::vector<Texture> texturelist;

	void LoadShader(const char * vert, const char * frag);
	void BindSkyBox(class CubeMap);
	void BindShadowMap();
	void BindShader();

	void LoadTexture(const char *);
	void LoadCubeMapTexture(const char **, int);


private:
	bool ReadShaderSource(const char* filename, std::vector<GLchar> &buffer);
};
