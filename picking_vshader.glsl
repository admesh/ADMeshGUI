#version 120

attribute vec4 a_position;
attribute vec3 a_normal;
uniform mat4 mvp_matrix;

void main()
{
    gl_Position = mvp_matrix * a_position;
}
