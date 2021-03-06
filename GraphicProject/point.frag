#version 330 core

out vec4 fragment;

in vec3 normalvetor;
in vec3 ambientintensityout;
in vec3 pointlightdirectioncout;
in vec3 pointlightintensityout;
in vec3 seeangle;
in vec3 diffuseout;
in vec4 specularout;

in vec2 TexCoord0;
uniform sampler2D gSampler;
uniform sampler2D gSampler2;

void main()
{
	fragment = vec4(ambientintensityout.x * diffuseout.x, ambientintensityout.y * diffuseout.y, ambientintensityout.z * diffuseout.z, 1.0);
	
	float costheta = dot(normalvetor, pointlightdirectioncout);
	
	if (costheta > 0)
	{
		fragment += costheta * texture2D(gSampler, TexCoord0.st) * vec4(diffuseout, 1.0) *  vec4(pointlightintensityout, 1.0);
	
		vec3 h = normalize(seeangle + pointlightdirectioncout);
		if (dot(h, normalvetor) > 0)
		{
			fragment += texture2D(gSampler2, TexCoord0.st) * vec4(vec3(specularout) * pow(dot(h, normalvetor), specularout.w), 1.0);
		}
	}

	//fragment += texture2D(gSampler, TexCoord0.st);
}