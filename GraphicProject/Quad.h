#pragma once
#include "Object.h"

class Quad : public Object
{
public:
	Quad();
	~Quad();

public:
	void Initialize();
};

inline void Quad::Initialize()
{
	mesh = new Mesh();
	this->SetMesh(mesh);
	mesh->data.resize(6);
	mesh->index.resize(2);
	mesh->data[0].vertex = cy::Point3f(-1.0, -1.0, 0.0);
	mesh->data[1].vertex = cy::Point3f(1.0, -1.0, 0.0);
	mesh->data[2].vertex = cy::Point3f(-1.0, 1.0, 0.0);
	mesh->data[3].vertex = cy::Point3f(-1.0, 1.0, 0.0);
	mesh->data[4].vertex = cy::Point3f(1.0, -1.0, 0.0);
	mesh->data[5].vertex = cy::Point3f(1.0, 1.0, 0.0);
	
	mesh->data[0].uv = cy::Point2f(0.0, 0.0);
	mesh->data[1].uv = cy::Point2f(1.0, 0.0);
	mesh->data[2].uv = cy::Point2f(0.0, 1.0);
	mesh->data[3].uv = cy::Point2f(0.0, 1.0);
	mesh->data[4].uv = cy::Point2f(1.0, 0.0);
	mesh->data[5].uv = cy::Point2f(1.0, 1.0);
	
	mesh->index[0].v[0] = 0;
	mesh->index[0].v[1] = 1;
	mesh->index[0].v[2] = 2;
	mesh->index[1].v[0] = 3;
	mesh->index[1].v[1] = 4;
	mesh->index[1].v[2] = 5;

	mesh->InitializeBuffer();

	mesh->material = new Material();
	mesh->material->LoadShader("../Assets/Shaders/quad.vert.glsl", "../Assets/Shaders/quad.frag.glsl");
}

