#version 420 core

out vec4 fragment;

in vec3 normalvetor;
in vec3 pointlightdirectioncout;
in vec3 seeangle;
in vec2 TexCoord0;

uniform sampler2D texture0;
uniform sampler2D texture1;

layout (std140, binding = 2) uniform const_material
{
	vec4 diffuse;
	vec4 specular;
};

layout (std140, binding = 3) uniform const_light
{
	vec4 ambientintensity;
	vec4 pointintensity;
	vec4 pointposition;
};

void main()
{
	fragment = texture2D(texture0, TexCoord0.st) * ambientintensity * diffuse;
	float costheta = dot(normalvetor, pointlightdirectioncout);
	
	if (costheta > 0)
	{
		fragment += costheta * texture2D(texture0, TexCoord0.st) * diffuse * pointintensity;
	
		vec3 h = normalize(seeangle + pointlightdirectioncout);
		if (dot(h, normalvetor) > 0)
		{
			fragment +=  texture2D(texture1, TexCoord0.st) * vec4(vec3(pointintensity) * vec3(specular) * pow(dot(h, normalvetor), specular.w), 1.0);
		}
	}
}