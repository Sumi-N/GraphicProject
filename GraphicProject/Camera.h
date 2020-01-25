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

	Camera()
	{
		perspective   = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
		orthographics = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
		view          = glm::lookAt
		(
			glm::vec3(0, 0,  0),  // Camera is at
			glm::vec3(0, 0, -1),  // Camera's forward vector is
			glm::vec3(0, 1,  0)   // Camera's upper vector is 
		);
		model         = glm::translate(glm::mat4(1.0), glm::vec3(0,0,-50));
		mvp           = perspective * view * model;
	}
};
