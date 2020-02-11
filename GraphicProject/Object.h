#pragma once

#include <cyCodeBase/cyTriMesh.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"

class Object
{
public:
	Mesh * mesh;

	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 rot;

	glm::vec3 vel;
	glm::vec3 acc;

	glm::vec3 angvel;
	glm::vec3 angacc;

	Object()
	{
		pos      = glm::vec3(0.0, 0.0, 0.0);
		scale    = glm::vec3(1.0, 1.0, 1.0);
		rot      = glm::vec3(0.0, 0.0, 0.0);

		vel      = glm::vec3(0.0, 0.0, 0.0);
		acc      = glm::vec3(0.0, 0.0, 0.0);
		angvel   = glm::vec3(0.0, 0.0, 0.0);
		angacc   = glm::vec3(0.0, 0.0, 0.0);
	}

	void Translate(glm::vec3 & pos)
	{
		this->pos = pos;
	}
};

