uniform sampler2D texture;
uniform vec3 color;
varying mediump vec3 v_normal;

void main()
{
    vec3 N = normalize(v_normal);
    gl_FragColor = vec4(
                        color.r*(N.x + N.z + N.y + 3.0) / 6.0,
                        color.g*(N.x + N.z + N.y + 3.0) / 6.0,
                        color.b*(N.x + N.z + N.y + 3.0) / 6.0,
                        1.0
                   );
}
