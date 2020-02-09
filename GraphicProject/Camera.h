#pragma once

#include "Object.h"
#include "Constatnt.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <mutex>
extern std::mutex m;

class Camera : public Object
{
public:
	glm::mat4 view;
	glm::mat4 perspective;
	glm::mat4 orthographics;

	glm::vec3 forwardvec;
	glm::vec3 upvec;
	glm::vec3 rightvec;

	float fov;
	float nearestclip;
	float farestclip;

public:

	Camera()
	{
		forwardvec = glm::vec3(0, 0, -1);
		upvec      = glm::vec3(0, 1, 0);
		rightvec   = glm::normalize(glm::cross(forwardvec, upvec));

		fov         = 45.0f;
		nearestclip = 0.1f;
		farestclip  = 100.0f;

		// The parameters for lookAt function are position, target, up vector
		// target is equal to forwardvector + position;
		view          = glm::lookAt(pos, pos + forwardvec, upvec);
		perspective   = glm::perspective(glm::radians(fov), (float)WIDTH / HEIGHT, nearestclip, farestclip);
		orthographics = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
	}

	void RotateAround(float amount, glm::vec3 & axis)
	{
		forwardvec = glm::rotate(forwardvec, -1 * glm::radians(amount), axis);
		upvec      = glm::rotate(upvec, -1 * glm::radians(amount), axis);
		rightvec   = glm::cross(forwardvec, upvec);
	}

	void MoveCamera(glm::vec3 & dir)
	{
		vel = -1.0f * dir;
	}

	void Update(float dt)
	{
		//m.lock();

		pos += dt * vel;
		view = glm::lookAt(pos, pos + forwardvec, upvec);

		//m.unlock();
	}
};
