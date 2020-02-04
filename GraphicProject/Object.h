#pragma once

#include <cyCodeBase/cyTriMesh.h>
#include <glm/gtc/matrix_transform.hpp>

class Object
{
public:
	cy::TriMesh data;
	cy::Point3f * sortedvn;

	glm::vec3 position;
	glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0);
	//glm::vec3 rotation;

	glm::mat4 modelcoordinate = glm::mat4(1.0);
	glm::mat3 modelinversetranspose;

	glm::vec3 diffuse;
	glm::vec4 specular;

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

		modelinversetranspose = glm::transpose(glm::inverse(glm::mat3(modelcoordinate)));
	}

	void organizeindiceorder()
	{
		int nubnu = data.NVN();
		sortedvn = new cy::Point3f[data.NVN()];

		int numberofface = data.NF();

		for (int i = 0; i < numberofface; i++)
		{
			sortedvn[data.F(i).v[0]] = data.VN(data.FN(i).v[0]);
			sortedvn[data.F(i).v[1]] = data.VN(data.FN(i).v[1]);
			sortedvn[data.F(i).v[2]] = data.VN(data.FN(i).v[2]);
		}
	}
};