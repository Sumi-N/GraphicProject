#pragma once
#include <glm/gtc/matrix_transform.hpp>

#include "Object.h"

class Light : public Object 
{
public:
	glm::vec3 intensity;
};

class AmbientLight : public Light
{

};

class PointLight : public Light
{
public:
};

class DirectionalLight : public Light
{
public:
	glm::vec3 direction;
};
