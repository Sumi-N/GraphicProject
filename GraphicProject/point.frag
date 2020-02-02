#version 330 core

out vec4 fragment;

in vec3 normalcolor;
void main()
{
	fragment = vec4(normalcolor.x, normalcolor.y, normalcolor.z, 1.0);
}