#pragma once
#include <vector>
#include <tuple>
#include <string>

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

namespace ConstantData
{
	struct Camera 
	{
		glm::mat4 view_perspective_matrix;
		glm::vec3 camera_position_vector;

		float padding;
	};

	struct Object 
	{
		glm::mat4 model_position_matrix;
		glm::mat4 model_view_perspective_matrix;
		glm::mat3 model_inverse_transpose_matrix;
	};

	struct Material 
	{
		glm::vec4 diffuse;
		glm::vec4 specular;
	};

	struct Light 
	{
		glm::vec4 light_ambient_intensity;
		glm::vec4 light_point_intensity;
		glm::vec4 pointposition;
	};

	/////////////////// Don't forget to add index and size data after addding a new uniform data //////////////

	enum class Index : uint8_t
	{
		Camera   = 0,
		Object   = 1,
		Material = 2,
		Light    = 3,
	};

	enum class Size : uint8_t
	{
		Camera = sizeof(ConstantData::Camera),
		Object   = sizeof(ConstantData::Object),
		Material = sizeof(ConstantData::Material),
		Light    = sizeof(ConstantData::Light),
	};
}

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	GLuint bufferid = 0;
	GLuint index;
	GLsizeiptr size;

	void Init(ConstantData::Index, ConstantData::Size);
	void Update(const void* const);
};

