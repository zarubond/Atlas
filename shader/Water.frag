uniform float time;
uniform sampler2D map;
varying vec3 f_vertex;

vec4 encode (vec3 n)
{
    float p = sqrt(n.z*8.0+8.0);
    return vec4(n.xy/p + 0.5,0.0,1.0);
}

void main(void)
{
    vec2 uv=vec2(f_vertex.x/100.0,f_vertex.z/100.0)*(time/3000.0);
    uv=mod(uv,1.0);

    vec4 samp=texture2D(map,uv);
    vec4 color=vec4(0.5,0.5,1.0,1.0)*samp.y;
    color.w=0.3;
    gl_FragData[0] = color;
    gl_FragData[1] = encode(samp.xyz);
    gl_FragData[2] = vec4(f_vertex,1.0);
}
