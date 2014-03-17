varying vec3 normal;
varying vec3 position;
varying float fog_factor;
uniform samplerCube texture;

const vec4 skytop = vec4(0.0f, 0.0f, 1.0f, 1.0f);
const vec4 skyhorizon = vec4(0.3294f, 0.92157f, 1.0f, 1.0f);

void main(void)
{
    vec3 coord=normalize(normal);
    float m=(coord.y+1.0)/2.0;
    m=m*m;
    gl_FragData[0] = mix(skyhorizon, skytop, m);
    gl_FragData[1] = vec4(0.0);//vec4(coord,1.0);
    gl_FragData[2] = vec4(position,1.0);
}
