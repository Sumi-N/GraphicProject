#version 330 core
in vec4 position;
void main()
{
	float s= 1.0f;
	gl_Position = s * position;
}