#pragma once

#include "Object.h"
#include "Material.h"
#include "Texture.h"

class CubeMap : public Object
{
public:
	void Init();
	Texture* GetCubeMapTexture();
};

inline void CubeMap::Init()
{
	mesh = new Mesh();
	this->SetMesh(mesh);
	mesh->Load("../Assets/Meshes/cubemap.obj");
	mesh->Init();

	mesh->material = new Material();
	mesh->material->LoadShader("../Assets/Shaders/environmentmap.vert.glsl", "../Assets/Shaders/environmentmap.frag.glsl");

	const char ** filenames = new const char*[6];
	filenames[0] = "../Assets/Textures/cubemap_posx.png";
	filenames[1] = "../Assets/Textures/cubemap_negx.png";
	filenames[2] = "../Assets/Textures/cubemap_posy.png";
	filenames[3] = "../Assets/Textures/cubemap_negy.png";
	filenames[4] = "../Assets/Textures/cubemap_posz.png";
	filenames[5] = "../Assets/Textures/cubemap_negz.png";

	mesh->material->LoadCubeMapTexture(filenames, 3);
}

inline Texture* CubeMap::GetCubeMapTexture()
{
	return &mesh->material->texturelist[0];
}