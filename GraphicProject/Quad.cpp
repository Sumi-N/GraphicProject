#include "Quad.h"


Quad::Quad()
{
}


Quad::~Quad()
{
}

void Quad::Init()
{
	mesh = new Mesh();
	this->SetMesh(mesh);
	mesh->Load("../Assets/Meshes/quad.obj");
	mesh->Init();

	mesh->material = new Material();
	mesh->material->LoadShader("../Assets/Shaders/quad.vert.glsl", "../Assets/Shaders/quad.frag.glsl");
}

void Quad::Bind(FrameBuffer framebuffer)
{

}