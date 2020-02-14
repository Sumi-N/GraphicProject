#version 420 core

out vec4 color;

layout (std140, binding = 2) uniform const_material
{
	vec4 diffuse;
	vec4 specular;
};

layout (std140, binding = 3) uniform const_light
{
	vec4 light_ambient_intensity;
	vec4 light_point_intensity;
	vec4 light_point_position;
};

uniform sampler2D texture0;
uniform sampler2D texture1;

// Normal vector of the object at world coordinate
in vec3 world_normal;
// Point light direction vector at world coordinate
in vec3 world_pointlight_direction;
// Object direction vector at world coordinate
in vec3 world_object_direction;
// Texture coordinate
in vec2 texcoord;

void main()
{
	// Ambient light
	color = texture2D(texture0, texcoord.st) * diffuse * light_ambient_intensity;

	float cos_theta_1 = dot(world_normal, world_pointlight_direction);
	
	if (cos_theta_1 > 0)
	{
		color += texture2D(texture0, texcoord.st) * cos_theta_1 * diffuse * light_point_intensity;
	
		vec3 h = normalize(world_object_direction + world_pointlight_direction);

		if (dot(h, world_normal) > 0)
		{
			color +=  texture2D(texture1, texcoord.st) * vec4(vec3(light_point_intensity) * vec3(specular) * pow(dot(h, world_normal), specular.w), 1.0);
		}
	}
}