#pragma once
#include <cyCodeBase/cyTriMesh.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Object.h"

class Mesh
{
public:

	Object * owner;

	glm::mat4 translation_mat;
	glm::mat4 scale_mat;
	glm::mat4 rotation_mat;

	glm::mat4 model_pos_mat;
	glm::mat3 model_vec_mat;

	Mesh()
	{
		
	}

	void Load()
	{

	}

	void Init()
	{

	}

	void Update()
	{
		translation_mat = glm::translate(glm::mat4(1.0), owner->pos);
		scale_mat       = glm::scale(glm::mat4(1.0), owner->scale);
		// Just make it like this for now
		rotation_mat    = glm::mat4(1.0);
		model_pos_mat   = translation_mat * rotation_mat * scale_mat;
		model_vec_mat   = glm::transpose(glm::inverse(glm::mat3(model_pos_mat)));
	}
};