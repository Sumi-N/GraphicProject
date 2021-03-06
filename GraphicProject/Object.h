#pragma once

#include <cyCodeBase/cyTriMesh.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"

class Object
{
public:
	cy::TriMesh data;
	cy::Point3f * sortedvn;
	cy::Point2f * sortedvt;

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
		// Convert normal data
		sortedvn = new cy::Point3f[data.NF()];
		for (int i = 0; i < data.NF(); i++)
		{
			sortedvn[data.F(i).v[0]] = data.VN(data.FN(i).v[0]);
			sortedvn[data.F(i).v[1]] = data.VN(data.FN(i).v[1]);
			sortedvn[data.F(i).v[2]] = data.VN(data.FN(i).v[2]);
		}

		// Convert UV data
		sortedvt = new cy::Point2f[data.NF()];
		for (int i = 0; i < data.NF(); i++)
		{
			sortedvt[data.F(i).v[0]] = cy::Point2f(data.VT(data.FT(i).v[0]));
			sortedvt[data.F(i).v[1]] = cy::Point2f(data.VT(data.FT(i).v[1]));
			sortedvt[data.F(i).v[2]] = cy::Point2f(data.VT(data.FT(i).v[2]));
		}


		for (int i = 0; i < data.NF(); i++)
		{
			printf("The index for texture face is %d, %d, %d \n", data.FT(i).v[0], data.FT(i).v[1], data.FT(i).v[2]);
		}

		diffuse = glm::vec3(data.M(0).Kd[0], data.M(0).Kd[1], data.M(0).Kd[2]);
		specular = glm::vec4(data.M(0).Ks[0], data.M(0).Ks[1], data.M(0).Ks[2], data.M(0).Ns);
	}
};

