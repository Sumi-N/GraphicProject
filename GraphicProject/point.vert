#version 150 core
uniform float aspect;
in vec4 position;
void main()
{
 mat4 s = mat4(0.05,0,0,0,
               0,0.05,0,0,
               0,0,0.05,0,
               0,0,0,1);
 vec4 tmp = s * position;
 gl_Position = vec4(1.0/aspect, 1.0, 1.0, 1.0) * tmp;
}