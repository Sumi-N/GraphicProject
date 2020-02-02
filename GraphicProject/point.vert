#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 mvp;
uniform mat3 mtranspose;

out vec3 normalcolor;

void main()
{
	float s = 1.0f;
	gl_Position = mvp * vec4(position, 1);
	normalcolor = mtranspose * normal;
}