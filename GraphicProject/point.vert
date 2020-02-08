#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 mvp;
uniform mat4 modelmatrix;
uniform mat4 cameramatrix;
uniform mat3 mtranspose;
uniform vec3 ambientintensity;
uniform vec3 pointintensity;
uniform vec3 pointposition;
uniform vec3 diffuse;
uniform vec4 specular;

out vec3 normalvetor;
out vec3 ambientintensityout;
out vec3 pointlightdirectioncout;
out vec3 pointlightintensityout;
out vec3 diffuseout;
out vec4 specularout;

void main()
{
	gl_Position = mvp * vec4(position, 1);
	//normalvetor = abs(normalize(mtranspose * normal));
	normalvetor = normalize(mtranspose * normal);
	ambientintensityout = ambientintensity;
	pointlightdirectioncout = normalize(pointposition - vec3(modelmatrix * vec4(position,1)));
	pointlightintensityout = pointintensity;
	diffuseout = diffuse;
	specularout = specular;
}