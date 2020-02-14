#pragma once

#include "ConstantBuffer.h"
#include <stdio.h>



ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::ConstantBuffer(ConstantBufferTypes i_type)
{
	this->type = i_type;
}


ConstantBuffer::~ConstantBuffer()
{
}

void ConstantBuffer::Bind()
{
	glGenBuffers(1, &bufferid);
	glBindBuffer(GL_UNIFORM_BUFFER, bufferid);

	if (type == ConstantBufferTypes::Frame)
	{
		size = sizeof(glm::mat4) + sizeof(glm::mat3) + sizeof(float);
		//size = 100;
	}
	else if (type == ConstantBufferTypes::DrawCall)
	{
		size = 2 * sizeof(glm::mat4) + sizeof(glm::mat3);
		//size = 146;
	}
	else if (type == ConstantBufferTypes::Material)
	{
		size = sizeof(glm::vec4) + sizeof(glm::vec4);
		//size = 32;
	}
	else if (type == ConstantBufferTypes::Light)
	{
		size = 3 * sizeof(glm::vec4);
	}
	else
	{
		printf("You don't have such frame buffer type");
	}

	glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(size), nullptr, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, static_cast<GLuint>(type), bufferid);
}

void ConstantBuffer::Update(const void* const i_data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, bufferid);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, i_data);
}
