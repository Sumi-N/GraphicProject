#pragma once

#include "Object.h"
#include "Material.h"
#include "Texture.h"

class CubeMap : public Object
{
public:
	void Init();
	void Bind();

	Texture* GetCubeMapTexture();
	GLint uniformid_vp_matrix;
	glm::mat4 vp;
};

inline void CubeMap::Init()
{
	mesh = new Mesh();
	this->SetMesh(mesh);
	mesh->Load("../Assets/Meshes/cubemap.obj");
	mesh->Init();

	mesh->material = new Material();
	mesh->material->LoadShader("../Assets/Shaders/skybox.vert.glsl", "../Assets/Shaders/skybox.frag.glsl");

	const char ** filenames = new const char*[6];
	filenames[0] = "../Assets/Textures/cubemap_posx.png";
	filenames[1] = "../Assets/Textures/cubemap_negx.png";
	filenames[2] = "../Assets/Textures/cubemap_posy.png";
	filenames[3] = "../Assets/Textures/cubemap_negy.png";
	filenames[4] = "../Assets/Textures/cubemap_posz.png";
	filenames[5] = "../Assets/Textures/cubemap_negz.png";

	mesh->material->LoadCubeMapTexture(filenames, 3);

	uniformid_vp_matrix = glGetUniformLocation(mesh->material->programid, "view_perspective_matrix");
}

inline Texture* CubeMap::GetCubeMapTexture()
{
	return &mesh->material->texturelist[0];
}

inline void CubeMap::Bind()
{
	mesh->material->BindShader();
	glUniformMatrix4fv(uniformid_vp_matrix, 1, GL_FALSE, &vp[0][0]);
}