#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 TexCoord;

layout (std140, binding = 0) uniform constant_frame
{
	mat4 cvp;
	vec3 cwp;
	float padding;
};

layout (std140, binding = 1) uniform const_drawcall
{
	mat4 mwt;
	mat4 mvp;
	mat3 mit;
};

layout (std140, binding = 3) uniform const_light
{
	vec4 ambientintensity;
	vec4 pointintensity;
	vec4 pointposition;
};

out vec3 normalvetor;
out vec3 pointlightdirectioncout;
out vec3 seeangle;

out vec2 TexCoord0;

void main()
{
	gl_Position = mvp * vec4(position, 1);
	normalvetor = normalize(mit * normal);
	seeangle = normalize(cwp -  vec3(mwt * vec4(position,1)));
	pointlightdirectioncout = normalize(vec3(pointposition) - vec3(mwt * vec4(position,1)));

	TexCoord0 = TexCoord;
}