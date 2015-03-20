uniform sampler2D diffuse;
varying vec2 tex;
varying vec3 f_normal;
varying vec3 coord;

vec4 encode (vec3 n)
{
    float p = sqrt(n.z*8.0+8.0);
    return vec4(n.xy/p + 0.5,0.0,1.0);
}

void main(void)
{
    vec4 vTexColor=texture2D(diffuse,tex);
    if(vTexColor.a<0.6)
        discard;

    vec4 col1=vec4(0.1019,0.1176,0.0156,1.0);
    vec4 col2=vec4(0.3960,0.4352,0.1372,1.0);

    vec3 nor=normalize(f_normal);

    gl_FragData[0] = vTexColor;
    gl_FragData[1] = encode(nor);
    gl_FragData[2] = vec4(coord,1.0);
}
