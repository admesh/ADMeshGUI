uniform sampler2D texture;
uniform vec3 color;
uniform vec3 badColor;
varying mediump vec3 v_normal;

void main()
{
    vec3 N = normalize(v_normal);
    float factor = (N.x + N.z + N.y + 3.0) / 6.0;

    if (gl_FrontFacing){
        gl_FragColor = vec4(color*factor, 1.0);
     } else {
        gl_FragColor = vec4(badColor*factor, 1.0);
     }
}
