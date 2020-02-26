#version 420 core

layout(location = 0) out vec4 color;

uniform sampler2D texture0;

in vec4 clipcoord;

void main()
{
	vec2 normalizedDeviceCoordinate = (clipcoord.xy/clipcoord.w)*0.5 + 0.5;
	color = texture(texture0, normalizedDeviceCoordinate);
}