#version 130

in vec4 a_position;
in vec3 a_normal;
uniform mat4 mvp_matrix;

void main()
{
    gl_Position = mvp_matrix * a_position;
}
