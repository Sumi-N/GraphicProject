#pragma once

#include <cyCodeBase/cyTriMesh.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Material.h"

// Forward declaration
class Object;

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

class Mesh
{
public:

	// Components
	class Object * owner;
	Texture * texture;

	cy::TriMesh tmpdata;
	std::vector<MeshFace> index;
	std::vector<MeshData> data;

	glm::mat4 translation_mat;
	glm::mat4 scale_mat;
	glm::mat4 rotation_mat;

	glm::mat4 model_pos_mat;
	glm::mat3 model_vec_mat;

	Mesh();

	void Load(const char * filename);
	void Init();
	void Update();
};

