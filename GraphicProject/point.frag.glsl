#version 420 core

out vec4 fragment;

in vec3 normalvetor;
in vec3 ambientintensityout;
in vec3 pointlightdirectioncout;
in vec3 pointlightintensityout;
in vec3 seeangle;

in vec2 TexCoord0;
uniform sampler2D gSampler;
uniform sampler2D gSampler2;
uniform vec3 diffuse;
uniform vec4 specular;

void main()
{
	fragment = texture2D(gSampler, TexCoord0.st) * vec4(ambientintensityout.x * diffuse.x, ambientintensityout.y * diffuse.y, ambientintensityout.z * diffuse.z, 1.0);
	float costheta = dot(normalvetor, pointlightdirectioncout);
	
	if (costheta > 0)
	{
		fragment += costheta * texture2D(gSampler, TexCoord0.st) * vec4(diffuse, 1.0) *  vec4(pointlightintensityout, 1.0);
	
		vec3 h = normalize(seeangle + pointlightdirectioncout);
		if (dot(h, normalvetor) > 0)
		{
			fragment += texture2D(gSampler2, TexCoord0.st) * vec4(vec3(specular) * pow(dot(h, normalvetor), specular.w), 1.0);
		}
	}
}