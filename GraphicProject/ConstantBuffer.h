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
	};

	struct DrawCall
	{
		// model world transformation
		glm::mat4 mwt;
		// model camera perspective
		glm::mat4 mvp;
		// model inverse transpose 
		glm::mat3 mit;
	};
}

enum class ConstantBufferTypes : uint8_t
{
	Frame = 0,
	DrawCall = 1,
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

