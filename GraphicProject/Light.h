#pragma once
#include <glm/gtc/matrix_transform.hpp>

class Light
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
	glm::vec3 position;
};

class DirectionalLight : public Light
{
public:
	glm::vec3 direction;
};
