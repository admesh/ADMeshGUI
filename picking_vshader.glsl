attribute mediump vec4 a_position;
attribute mediump vec3 a_normal;
uniform mediump mat4 mvp_matrix;

void main()
{
    gl_Position = mvp_matrix * a_position;
}
