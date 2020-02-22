#pragma once

#include "Object.h"
#include "Material.h"
#include "Texture.h"

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
	mesh->Load("../Assets/Meshes/cubemap.obj");
	mesh->Init();

	////////////////////////////////////
	glActiveTexture(GL_TEXTURE0 + 5);
	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);

	int tmpwidth, tmpheight;

	std::vector<cy::Color24> data;
	tmptexture.Load("../Assets/Textures/cubemap_posx.png", data, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	tmptexture.Load("../Assets/Textures/cubemap_negx.png", data, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	tmptexture.Load("../Assets/Textures/cubemap_posy.png", data, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	tmptexture.Load("../Assets/Textures/cubemap_negy.png", data, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	tmptexture.Load("../Assets/Textures/cubemap_posz.png", data, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	tmptexture.Load("../Assets/Textures/cubemap_negz.png", data, tmpwidth, tmpheight);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, tmpwidth, tmpheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	////////////////////////////////////
	mesh->material = new Material();
	mesh->material->Load("../Assets/Shaders/environmentmap.vert.glsl", "../Assets/Shaders/environmentmap.frag.glsl");

	tmptexture.uniformid = glGetUniformLocation(mesh->material->programid, "skybox");
	if (tmptexture.uniformid == -1)
	{
		std::cerr << "The sampler cube variable doesn't exist in the shader file" << std::endl;
	}

}