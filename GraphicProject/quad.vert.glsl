#version 420 core

layout (location = 0) in vec3 model_position;
layout (location = 1) in vec3 model_normal;
layout (location = 2) in vec2 model_texcoord;

layout (std140, binding = 1) uniform const_drawcall
{
	mat4 model_position_matrix;
	mat4 model_view_perspective_matrix;
	mat3 model_inverse_transpose_matrix;
};

out vec2 texcoord;

void main()
{
	texcoord                   = model_texcoord;

	// Send position data at perspective coordinate
	gl_Position                = model_view_perspective_matrix * vec4(model_position, 1);
}