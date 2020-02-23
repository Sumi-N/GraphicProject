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
	//bool Load(char const *, std::vector<cy::Color24> &, int &, int &);
	void Init(int unitnum, GLint programid);
	void BindTexture();

	void CreateTexture(GLint internalformat, GLuint width, GLuint height);

	std::string uniformname;
	GLuint textureobj;
	GLuint unitnumber;
	GLint uniformid;
};