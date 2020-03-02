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
	vec4 light_padding;
	mat4 light_view_perspective_matrix;
};

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform samplerCube skybox;
uniform sampler2D shadowmap;

// Normal vector of the object at world coordinate
in vec3 world_normal;
// Point light direction vector at world coordinate
in vec3 world_pointlight_direction;
// Object direction vector at world coordinate
in vec3 world_object_direction;
// Texture coordinate
in vec2 texcoord;
// The depth value at light space
in vec4 light_space_position_depth;

bool ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowmap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    return  currentDepth - 0.005 > closestDepth  ? true : false;
} 

void main()
{
	// Ambient light
	//color = texture2D(texture0, texcoord.st) * diffuse * light_ambient_intensity;
	color = diffuse * light_ambient_intensity;

	if(ShadowCalculation(light_space_position_depth))
	{
		return;
	}

	float cos_theta_1 = dot(world_normal, world_pointlight_direction);
	
	if (cos_theta_1 > 0)
	{
		color += texture2D(texture0, texcoord.st)  * cos_theta_1 * diffuse * light_point_intensity;
	
		vec3 h = normalize(world_object_direction + world_pointlight_direction);

		if (dot(h, world_normal) > 0)
		{
			vec3 reflection = -1 * world_object_direction + 2 * dot(world_object_direction, world_normal) * world_normal;

			color +=  (texture2D(texture1, texcoord.st) + texture(skybox, reflection)) * vec4(vec3(light_point_intensity) * vec3(specular) * pow(dot(h, world_normal), specular.w), 1.0);
		}
	}
}