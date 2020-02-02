#pragma once

#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	glm::mat4 mvp;
	glm::mat4 view;
	glm::mat4 perspective;
	glm::mat4 orthographics;

	glm::vec3 position;
	glm::vec3 forwardvector;
	glm::vec3 upvector;
	glm::vec3 rightvector;
	glm::vec3 lookatvector;

	Camera()
	{
		position      = glm::vec3(0, 0, 0);
		forwardvector = glm::vec3(0, 0, -1);
		upvector      = glm::vec3(0, 1, 0);
		rightvector   = glm::normalize(glm::cross(forwardvector, upvector));
		lookatvector  = forwardvector - position;

		perspective   = glm::perspective(glm::radians(45.0f), (float)WIDTH/HEIGHT, 0.1f, 100.0f);
		orthographics = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
		view          = glm::lookAt(position, lookatvector, upvector);
	}

	void rotate(float amount, glm::vec3 & axis)
	{
		if (axis == upvector)
		{
			rightvector   = (float)glm::cos(amount * glm::radians(1.0f)) * rightvector - (float)glm::sin(amount * glm::radians(1.0f)) * forwardvector;
			forwardvector = (float)glm::sin(amount * glm::radians(1.0f)) * rightvector + (float)glm::cos(amount * glm::radians(1.0f)) * forwardvector;

		}
		else if (axis == rightvector)
		{
			forwardvector = (float)glm::cos(amount * glm::radians(1.0f)) * forwardvector - (float)glm::sin(amount * glm::radians(1.0f)) * upvector;
			upvector      = (float)glm::sin(amount * glm::radians(1.0f)) * forwardvector + (float)glm::cos(amount * glm::radians(1.0f)) * upvector;
		}
		update();
	}

	void translate(float amount, glm::vec3 & axis)
	{
		position += amount * axis;
		update();
	}

	void update()
	{
		lookatvector = forwardvector + position;
		view = glm::lookAt(position, lookatvector, upvector);
	}

	void updatemvp(Object & obj)
	{
		obj.update();
		mvp = perspective * view * obj.modelcoordinate;
	}
};
