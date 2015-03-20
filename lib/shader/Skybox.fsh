varying vec3 normal;
varying vec3 f_coord;
varying float fog_factor;
uniform vec3 sun_dir;

const vec4 skytop = vec4(0.0, 0.0, 1.0, 1.0);
const vec4 skyhorizon = vec4(0.3294, 0.92157, 1.0, 1.0);

vec4 encode (vec3 n)
{
    float p = sqrt(n.z*8.0+8.0);
    return vec4(n.xy/p + 0.5,0.0,1.0);
}

vec4 encodeBasic(vec3 n)
{
    return vec4(n.xyz*0.5+0.5,1.0);
}

void main(void)
{
    vec3 coord=normalize(normal);
    vec3 sun=normalize(sun_dir);
    float d=min(0.0, dot(sun,coord));
    float m=(coord.y+1.0)/2.0;
    //http://ikstewa.blogspot.cz/2012/07/animated-noise-cloud-shader.html

    gl_FragData[0] = mix(mix(skyhorizon, skytop, m*m),vec4(1.0,1.0,1.0,1.0),clamp(pow(d,48.0),0.0,1.0));
    gl_FragData[1] = encode(sun);
    gl_FragData[2] = vec4(f_coord,1.0);
}
