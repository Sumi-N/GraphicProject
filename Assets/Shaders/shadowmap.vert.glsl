#version 420 core

layout (location = 0) in vec3 model_position;

layout (std140, binding = 1) uniform const_object
{
	mat4 model_position_matrix;
	mat4 model_view_perspective_matrix;
	mat4 model_inverse_transpose_matrix;
};

layout (std140, binding = 3) uniform const_light
{
	vec4 light_ambient_intensity;
	vec4 light_point_intensity;
	vec4 light_point_position;
	vec4 padding;
	mat4 light_view_perspective_matrix;
};

void main()
{
    gl_Position = light_view_perspective_matrix * model_position_matrix * vec4(model_position, 1.0);
    //gl_Position = light_view_perspective_matrix * vec4(model_position, 1.0);
}