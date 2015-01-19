attribute mediump vec4 a_position;
attribute mediump vec3 a_normal;
uniform mediump mat4 mvp_matrix;
varying mediump vec3 v_normal;

void main()
{
    gl_Position = mvp_matrix * a_position;
    v_normal = a_normal;
}
