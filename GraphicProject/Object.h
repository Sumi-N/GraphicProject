#pragma once

#include <cyCodeBase/cyTriMesh.h>
#include <glm/gtc/matrix_transform.hpp>

class Object
{
public:
	cy::TriMesh data;

	glm::vec3 position;
	glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
	//glm::vec3 rotation;

	glm::mat4 modelcoordinate = glm::mat4(1.0);
	glm::mat3 modeltranspose;

	Object()
	{
		//glm::mat4 rotatemat = glm::rotate();
		glm::mat4 scalemat = glm::scale(glm::mat4(1.0), scale);
		glm::mat4 translatemat = glm::translate(glm::mat4(1.0), position);
		//modelcoordinate = translatemat * rotatemat *scalemat;
		modelcoordinate = translatemat * scalemat;
	}

	void update()
	{
		//glm::mat4 rotatemat = glm::rotate();
		glm::mat4 scalemat = glm::scale(glm::mat4(1.0), scale);
		glm::mat4 translatemat = glm::translate(glm::mat4(1.0), position);
		//modelcoordinate = translatemat * rotatemat *scalemat;
		modelcoordinate = translatemat * scalemat;

		modeltranspose = glm::transpose(glm::mat3(modelcoordinate));
	}
};