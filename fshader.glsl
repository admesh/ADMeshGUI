uniform sampler2D texture;
varying mediump vec3 v_normal;

void main()
{
    vec3 N = normalize(v_normal);
    gl_FragColor = vec4((vec3(0.0, (N.y+N.x+N.z+3.0)/6.0, 0.0)),1.0);
}
