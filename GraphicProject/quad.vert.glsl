#version 420 core

layout (location = 0) in vec3 model_position;
layout (location = 1) in vec3 model_normal;
layout (location = 2) in vec2 model_texcoord;

out vec2 texcoord;

void main()
{
	texcoord = model_texcoord;
	gl_Position = vec4(model_position, 1);
}