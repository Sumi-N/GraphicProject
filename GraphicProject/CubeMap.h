#pragma once

#include "Object.h"
#include "Material.h"

class CubeMap : public Object
{
public:
	void Initialize();

	GLuint textureid;
	Texture tmptexture;
};

inline void CubeMap::Initialize()
{
	mesh = new Mesh();
	this->SetMesh(mesh);
	mesh->Load("../Objfiles/cubemap.obj");
	mesh->Init();

	////////////////////////////////////
	glActiveTexture(GL_TEXTURE0 + 5);
	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);

	int tmpwidth, tmpheight;

	std::vector<cy::Color24> data;
	tmptexture.Load("../Objfiles/cubemap_posx.png", data, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	std::vector<cy::Color24> data2;
	tmptexture.Load("../Objfiles/cubemap_negx.png", data, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	std::vector<cy::Color24> data3;
	tmptexture.Load("../Objfiles/cubemap_posy.png", data3, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data3.data());

	std::vector<cy::Color24> data4;
	tmptexture.Load("../Objfiles/cubemap_negy.png", data4, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data4.data());

	std::vector<cy::Color24> data5;
	tmptexture.Load("../Objfiles/cubemap_posz.png", data5, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data5.data());

	std::vector<cy::Color24> data6;
	tmptexture.Load("../Objfiles/cubemap_negz.png", data6, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data6.data());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	////////////////////////////////////
	mesh->material = new Material();
	mesh->material->Load("environmentmap.vert.glsl", "environmentmap.frag.glsl");

	tmptexture.uniformid = glGetUniformLocation(mesh->material->programid, "skybox");
	if (tmptexture.uniformid == -1)
	{
		std::cerr << "The sampler cube variable doesn't exist in the shader file" << std::endl;
	}

}