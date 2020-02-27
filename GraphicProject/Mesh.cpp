#pragma once

#include <GL/glew.h>

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

		if(tmpdata.NVN() != 0)
		{
			// Sort normal to vertex index
			data[tmpdata.F(i).v[0]].normal = tmpdata.VN(tmpdata.FN(i).v[0]);
			data[tmpdata.F(i).v[1]].normal = tmpdata.VN(tmpdata.FN(i).v[1]);
			data[tmpdata.F(i).v[2]].normal = tmpdata.VN(tmpdata.FN(i).v[2]);
		}

		if (tmpdata.NVT() != 0)
		{
			// Sort uv coordinate to vertex index
			data[tmpdata.F(i).v[0]].uv = cy::Point2f(tmpdata.VT(tmpdata.FT(i).v[0]));
			data[tmpdata.F(i).v[1]].uv = cy::Point2f(tmpdata.VT(tmpdata.FT(i).v[1]));
			data[tmpdata.F(i).v[2]].uv = cy::Point2f(tmpdata.VT(tmpdata.FT(i).v[2]));
		}
	}

	InitializeBuffer();
}

void Mesh::SetMaterial(Material * material)
{
	if (material)
	{
		this->material = material;

		// Set material info
		if (tmpdata.m)
		{
			material->Ns = tmpdata.M(0).Ns;
			material->Ka = glm::vec3(tmpdata.M(0).Ka[0], tmpdata.M(0).Ka[1], tmpdata.M(0).Ka[2]);
			material->Kd = glm::vec3(tmpdata.M(0).Kd[0], tmpdata.M(0).Kd[1], tmpdata.M(0).Kd[2]);
			material->Ks = glm::vec3(tmpdata.M(0).Ks[0], tmpdata.M(0).Ks[1], tmpdata.M(0).Ks[2]);
		}
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
	model_inverse_transpose_matrix = glm::transpose(glm::inverse(model_pos_mat));
}

void Mesh::Draw()
{
	glBindVertexArray(bufferdata.vertexarrayid);
	glDrawElements(GL_TRIANGLES, index.size() * sizeof(index[0]), GL_UNSIGNED_INT, (void*)0);
}

void Mesh::InitializeBuffer()
{
	// Create vertex array 
	glGenVertexArrays(1, &bufferdata.vertexarrayid);
	glBindVertexArray(bufferdata.vertexarrayid);

	// Create vertex buffer 
	glGenBuffers(1, &bufferdata.vertexbufferid);
	glBindBuffer(GL_ARRAY_BUFFER, bufferdata.vertexbufferid);

	// Create index buffer
	glGenBuffers(1, &bufferdata.indexbufferid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferdata.indexbufferid);

	// Set vertex data to vertex buffer
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data[0]), data.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(index[0]), index.data(), GL_STATIC_DRAW);

	// Enable vertex attribute
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(data[0]), (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(data[0]), (void*)(sizeof(cy::Point3f)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(data[0]), (void*)(2 * sizeof(cy::Point3f)));
}
