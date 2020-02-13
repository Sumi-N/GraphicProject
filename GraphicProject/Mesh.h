#pragma once

#include <GL/glew.h>
#include <cyCodeBase/cyTriMesh.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Material.h"

struct MeshFace
{
	unsigned int v[3];
};

struct MeshData
{
	cy::Point3f vertex;
	cy::Point3f normal;
	cy::Point2f uv;
};

struct MeshBufferData
{
	GLuint vertexbufferid = 0;
	GLuint indexbufferid = 0;
	GLuint vertexarrayid = 0;
};

class Mesh
{
public:

	Mesh();

	// Components
	class Object * owner;
	Texture * texture;
	Material * material;

	cy::TriMesh tmpdata;
	std::vector<MeshFace> index;
	std::vector<MeshData> data;

	glm::mat4 translation_mat;
	glm::mat4 scale_mat;
	glm::mat4 rotation_mat;

	glm::mat4 model_pos_mat;
	glm::mat3 model_vec_mat;

	void Load(const char * filename);
	void Init();
	void SetMaterial(Material * material);
	void Update();
	void Draw();


	// Functions for openGL
public:
	MeshBufferData bufferdata;
	void InitializeBuffer();
};

