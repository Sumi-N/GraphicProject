#pragma once

#include <cyCodeBase/cyTriMesh.h>
#include <glm/gtc/matrix_transform.hpp>

// Forward declaration
class Mesh;

class Object
{
public:
	cy::TriMesh data;
	cy::Point3f * sortedvn;

	Mesh * mesh;

	glm::vec3 pos;
	glm::vec3 scale;
	glm::vec3 rot;

	glm::vec3 vel;
	glm::vec3 acc;

	glm::vec3 angvel;
	glm::vec3 angacc;



	glm::mat4 modelcoordinate = glm::mat4(1.0);
	glm::mat3 modelinversetranspose;

	glm::vec3 diffuse;
	glm::vec4 specular;

	Object()
	{
		pos      = glm::vec3(0.0, 0.0, 0.0);
		scale    = glm::vec3(1.0, 1.0, 1.0);
		rot      = glm::vec3(0.0, 0.0, 0.0);

		vel      = glm::vec3(0.0, 0.0, 0.0);
		acc      = glm::vec3(0.0, 0.0, 0.0);
		angvel   = glm::vec3(0.0, 0.0, 0.0);
		angacc   = glm::vec3(0.0, 0.0, 0.0);

		//glm::mat4 rotatemat = glm::rotate();
		glm::mat4 scalemat = glm::scale(glm::mat4(1.0), scale);
		glm::mat4 translatemat = glm::translate(glm::mat4(1.0), pos);
		//modelcoordinate = translatemat * rotatemat *scalemat;
		modelcoordinate = translatemat * scalemat;
	}

	void Translate(glm::vec3 & pos)
	{
		this->pos = pos;
	}

	//virtual void Update(float dt);


	void update()
	{
		//glm::mat4 rotatemat = glm::rotate();
		glm::mat4 scalemat = glm::scale(glm::mat4(1.0), scale);
		glm::mat4 translatemat = glm::translate(glm::mat4(1.0), pos);
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