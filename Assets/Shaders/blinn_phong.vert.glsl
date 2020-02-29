#version 420 core

layout (location = 0) in vec3 model_position;
layout (location = 1) in vec3 model_normal;
layout (location = 2) in vec2 model_texcoord;

layout (std140, binding = 0) uniform const_camera
{
	mat4 view_matrix;
	mat4 perspective_matrix;
	vec3 camera_position_vector;
	float camera_padding;
};

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
	vec4 light_padding;
	mat4 light_view_perspective_matrix;
};

// Normal vector of the object at world coordinate
out vec3 world_normal;
// Point light direction vector at world coordinate
out vec3 world_pointlight_direction;
// Object direction vector at world coordinate
out vec3 world_object_direction;
// Texture coordinate
out vec2 texcoord;
// The depth value at light space
out vec4 light_space_position_depth;

void main()
{
	// Send position data at perspective coordinate
	gl_Position                = model_view_perspective_matrix * vec4(model_position, 1.0);
	// Get normal vector at world coordinate
	world_normal               = normalize(mat3(model_inverse_transpose_matrix) * model_normal);

	world_pointlight_direction = normalize(vec3(light_point_position) - vec3(model_position_matrix * vec4(model_position, 1)));

	world_object_direction     = normalize(camera_position_vector -  vec3(model_position_matrix * vec4(model_position, 1)));

	texcoord                   = model_texcoord;

	light_space_position_depth = light_view_perspective_matrix * model_position_matrix * vec4(model_position, 1.0);
}