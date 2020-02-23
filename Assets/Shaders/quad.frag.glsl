#version 420 core

layout(location = 0) out vec4 color;

uniform sampler2D texture0;

in vec2 texcoord;

void main()
{
	color = texture(texture0, texcoord.st) + vec4(0.1, 0.1, 0.1, 0);
}