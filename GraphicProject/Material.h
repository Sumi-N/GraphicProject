#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include "cyCodeBase/cyColor.h"
#include <glm/gtc/matrix_transform.hpp>

class Texture
{

public:
	Texture();
	~Texture();

	std::vector<cy::Color24> data;
	int width, height;

	bool Load(char const *);
	bool Load(char const *, std::vector<cy::Color24> &, int &, int &);
	void Init(int unitnum, GLint programid);
	void BindUniformData();

	std::string uniformname;
	GLuint textureobj;
	GLint uniformid;
	int unitnumber;
};


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

	void Load(const char * vert, const char * frag);
	void BindShader();

	void LoadTexture(const char *);


private:
	bool ReadShaderSource(const char* filename, std::vector<GLchar> &buffer);
};
