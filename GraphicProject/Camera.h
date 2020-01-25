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
		view          = glm::lookAt(position, forwardvector, upvector);
		model         = glm::translate(glm::mat4(1.0), glm::vec3(0,0,-50));
		mvp           = perspective * view * model;
	}

	void rotate(float amount, glm::vec3 & axis)
	{
		view = glm::rotate(view, 0.1f * amount, axis);
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
