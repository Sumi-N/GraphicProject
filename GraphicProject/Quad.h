#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Object.h"
#include "FrameBuffer.h"

class Quad : public Object
{
public:
	glm::vec4 plane_equation;
	glm::mat4 mirror_inverse_mat;

	void Init();
	void Bind(FrameBuffer framebuffer);
	void Update(float) override;
};

inline void Quad::Init()
{
	mesh = new Mesh();
	this->SetMesh(mesh);
	mesh->Load("../Assets/Meshes/quad.obj");
	mesh->Init();

	mesh->material = new Material();
	mesh->material->LoadShader("../Assets/Shaders/quad.vert.glsl", "../Assets/Shaders/quad.frag.glsl");
}

inline void Quad::Bind(FrameBuffer framebuffer)
{
	glUseProgram(mesh->material->programid);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, framebuffer.color.textureid);
	glUniform1i(glGetUniformLocation(mesh->material->programid, "texture0"), 0);
}

inline void Quad::Update(float dt)
{
	Object::Update(dt);

	glm::vec3 quad_normal = glm::normalize(glm::mat3(mesh->model_pos_mat) * glm::vec3(0, 0, 1));
	float offset = -(quad_normal.x * pos.x + quad_normal.y * pos.y + quad_normal.z * pos.z);
	plane_equation = glm::vec4(quad_normal, offset);
	float mirror_inverse_floats[16] =
	{
		1 - 2 * plane_equation.x * plane_equation.x, -2 * plane_equation.y * plane_equation.x, -2 * plane_equation.z * plane_equation.x, 0.0,
		-2 * plane_equation.x * plane_equation.y, 1 - 2 * plane_equation.y * plane_equation.y, -2 * plane_equation.z * plane_equation.y, 0.0,
		-2 * plane_equation.x * plane_equation.z, -2 * plane_equation.y * plane_equation.z, 1 - 2 * plane_equation.z * plane_equation.z, 0.0,
		-2 * plane_equation.x * plane_equation.w, -2 * plane_equation.y * plane_equation.w, -2 * plane_equation.z * plane_equation.w, 1.0,
	};
	mirror_inverse_mat = glm::make_mat4(mirror_inverse_floats);
}
