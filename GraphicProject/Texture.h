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
	void Init(int unitnum, GLint programid);
	void InitCubeMap(int unitnum, GLint programid, const char ** filenames);
	void BindTexture();

	void CreateTexture(GLint internalformat, GLuint width, GLuint height);

	std::string uniformname;
	GLuint textureid;
	GLuint unitnumber;
	GLint uniformid;
};