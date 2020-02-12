#pragma once

#include "Object.h"
#include "Mesh.h"

Mesh::Mesh()
{
}

void Mesh::Load(const char * filename)
{
	tmpdata.LoadFromFileObj(filename, true);
}

void Mesh::Init()
{
	// Get number of point data
	int facenum = tmpdata.NF();
	int vertnum = tmpdata.NV();
	int normalnum = tmpdata.NVN();

	index.resize(facenum);
	data.resize(vertnum);

	for (int i = 0; i < vertnum; i++)
	{
		// Get normal data
		data[i].vertex = tmpdata.V(i);
	}

	for (int i = 0; i < facenum; i++)
	{
		// Get index data
		index[i].v[0] = tmpdata.F(i).v[0];
		index[i].v[1] = tmpdata.F(i).v[1];
		index[i].v[2] = tmpdata.F(i).v[2];

		// Sort normal to vertex index
		data[tmpdata.F(i).v[0]].normal = tmpdata.VN(tmpdata.FN(i).v[0]);
		data[tmpdata.F(i).v[1]].normal = tmpdata.VN(tmpdata.FN(i).v[1]);
		data[tmpdata.F(i).v[2]].normal = tmpdata.VN(tmpdata.FN(i).v[2]);

		// Sort uv coordinate to vertex index
		data[tmpdata.F(i).v[0]].uv = cy::Point2f(tmpdata.VT(tmpdata.FT(i).v[0]));
		data[tmpdata.F(i).v[1]].uv = cy::Point2f(tmpdata.VT(tmpdata.FT(i).v[1]));
		data[tmpdata.F(i).v[2]].uv = cy::Point2f(tmpdata.VT(tmpdata.FT(i).v[2]));
	}

	// Set material info
	material.Ns = tmpdata.M(0).Ns;
	for (int i = 0; i < 3; i++)
	{
		material.Ka[i] = tmpdata.M(0).Ka[i];
		material.Kd[i] = tmpdata.M(0).Kd[i];
		material.Ks[i] = tmpdata.M(0).Ks[i];
	}
}

void Mesh::Update()
{
	translation_mat = glm::translate(glm::mat4(1.0), owner->pos);

	rotation_mat = glm::rotate(glm::mat4(1.0), glm::radians(owner->rot.x), glm::vec3(1, 0, 0));
	rotation_mat = glm::rotate(rotation_mat, glm::radians(owner->rot.y), glm::vec3(0, 1, 0));
	rotation_mat = glm::rotate(rotation_mat, glm::radians(owner->rot.z), glm::vec3(0, 0, 1));

	scale_mat = glm::scale(glm::mat4(1.0), owner->scale);


	model_pos_mat = translation_mat * rotation_mat * scale_mat;
	model_vec_mat = glm::transpose(glm::inverse(glm::mat3(model_pos_mat)));
}
