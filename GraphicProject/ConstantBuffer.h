#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

namespace ConstantBufferFormat
{
	struct Frame
	{
		// camera view perspective
		glm::mat4 cvp;
		// camera world position
		glm::vec3 cwp;

		float padding;
	};

	struct DrawCall
	{
		// model world transformation
		glm::mat4 mwt;
		// model view perspective
		glm::mat4 mvp;
		// model inverse transpose 
		glm::mat3 mit;
	};

	struct Material
	{
		glm::vec4 diffuse;
		glm::vec4 specular;
	};


}

enum class ConstantBufferTypes : uint8_t
{
	Frame = 0,
	DrawCall = 1,
	Material = 2,
};

class ConstantBuffer
{
public:
	ConstantBuffer();
	ConstantBuffer(ConstantBufferTypes i_type);
	~ConstantBuffer();

	GLuint bufferid = 0;
	ConstantBufferTypes type;
	size_t size;

	void Bind();
	void Update(const void* const i_data);
};

