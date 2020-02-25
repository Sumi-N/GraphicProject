#version 420 core
layout (location = 0) in vec3 model_position;

out vec3 TexCoords;

uniform mat4 view_perspective_matrix;

void main()
{
    TexCoords = model_position;
    vec4 model_view_position = view_perspective_matrix * vec4(model_position, 1.0);
    gl_Position = model_view_position.xyww;
}