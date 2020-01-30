#pragma once

#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	glm::mat4 mvp;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 perspective;
	glm::mat4 orthographics;

	glm::vec3 position;
	glm::vec3 forwardvector;
	glm::vec3 upvector;
	glm::vec3 rightvector;

	Camera()
	{
		position      = glm::vec3(0, 0, 0);
		forwardvector = glm::vec3(0, 0, -1);
		upvector      = glm::vec3(0, 1, 0);
		rightvector   = glm::cross(forwardvector, upvector);

		perspective   = glm::perspective(glm::radians(45.0f), (float)WIDTH/HEIGHT, 0.1f, 100.0f);
		orthographics = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
		view          = glm::lookAt(position, forwardvector + position, upvector + position);
		model         = glm::translate(glm::mat4(1.0), glm::vec3(0,0,-50));
		mvp           = perspective * view * model;
	}

	void rotate(float amount, glm::vec3 & axis)
	{
		if (axis == upvector)
		{
			rightvector = (float)glm::cos(amount * glm::radians(1.0f)) * rightvector - (float)glm::sin(amount * glm::radians(1.0f)) * forwardvector;
			forwardvector = (float)glm::sin(amount * glm::radians(1.0f)) * rightvector + (float)glm::cos(amount * glm::radians(1.0f)) * forwardvector;

			//rightvector = glm::rotate(glm::mat4(), amount * glm::radians(0.01f), upvector) * glm::vec4(rightvector, 1);
			//forwardvector = glm::rotate(glm::mat4(), amount * glm::radians(0.01f), upvector) * glm::vec4(forwardvector, 1);
		}
		else if (axis == rightvector)
		{
			forwardvector = (float)glm::cos(amount * glm::radians(1.0f)) * forwardvector - (float)glm::sin(amount * glm::radians(1.0f)) * upvector;
			upvector = (float)glm::sin(amount * glm::radians(1.0f)) * forwardvector + (float)glm::cos(amount * glm::radians(1.0f)) * upvector;
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
		view = glm::lookAt(position, forwardvector, upvector);
		mvp = perspective * view * model;
	}
};
