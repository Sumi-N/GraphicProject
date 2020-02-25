#pragma once
#include "Object.h"
#include "FrameBuffer.h"

class Quad : public Object
{
public:
	void Init();
	void Bind(FrameBuffer framebuffer);
};

inline void Quad::Init()
{
	mesh = new Mesh();
	this->SetMesh(mesh);
	mesh->Load("../Assets/Meshes/quad.obj");
	mesh->Init();

	mesh->material = new Material();
	mesh->material->LoadShader("../Assets/Shaders/quad.vert.glsl", "../Assets/Shaders/quad.frag.glsl");
}

inline void Quad::Bind(FrameBuffer framebuffer)
{
	glUseProgram(mesh->material->programid);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, framebuffer.color.textureid);
	glUniform1i(glGetUniformLocation(mesh->material->programid, "texture0"), 0);
}
