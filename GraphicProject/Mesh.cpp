#pragma once

#include "Object.h"
#include "Mesh.h"

void Mesh::Load(const char * filename)
{
	tmpdata.LoadFromFileObj(filename);
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

		// Sort uv coord to vertex index
		data[tmpdata.F(i).v[0]].uv = cy::Point2f(tmpdata.VT(tmpdata.FN(i).v[0]));
		data[tmpdata.F(i).v[1]].uv = cy::Point2f(tmpdata.VT(tmpdata.FN(i).v[1]));
		data[tmpdata.F(i).v[2]].uv = cy::Point2f(tmpdata.VT(tmpdata.FN(i).v[2]));
	}

}

void Mesh::Update()
{
	translation_mat = glm::translate(glm::mat4(1.0), owner->pos);
	scale_mat = glm::scale(glm::mat4(1.0), owner->scale);
	// Just make it like this for now
	rotation_mat = glm::mat4(1.0);
	model_pos_mat = translation_mat * rotation_mat * scale_mat;
	model_vec_mat = glm::transpose(glm::inverse(glm::mat3(model_pos_mat)));
}
