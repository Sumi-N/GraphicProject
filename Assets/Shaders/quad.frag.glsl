#version 420 core

layout(location = 0) out vec4 color;

uniform sampler2D texture0;
uniform samplerCube skybox;

in vec4 clipcoord;
in vec2 texcoord;

void main()
{
	//vec2 normalizedDeviceCoordinate = (clipcoord.xy/clipcoord.w)*0.5 + 0.5;
	//color = texture(texture0, normalizedDeviceCoordinate);

	float depthvalue = texture(texture0, texcoord).x;
	color = vec4(vec3(depthvalue), 1.0);
}