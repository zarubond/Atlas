varying highp vec3 normal;

uniform lowp vec3 diffuse;
uniform highp vec3 sun_dir;
uniform lowp vec3 ambient;

vec4 outColor(vec3 color)
{
    vec3 diff=diffuse * max(0.0, dot(normal, sun_dir));
    return vec4(color*(diff+ambient),1.0);
}

void main()
{
    const vec4 skytop = vec4(0.0, 0.0, 1.0, 1.0);
    const vec4 skyhorizon = vec4(0.3294, 0.92157, 1.0, 1.0);

    vec3 coord=normalize(normal);
    vec3 sun=normalize(sun_dir);
    float d=dot(sun,coord);
    float m=(coord.y+1.0)/2.0;
    m=m*m;
    //http://ikstewa.blogspot.cz/2012/07/animated-noise-cloud-shader.html
    d=min(0.0,d);
    gl_FragColor = mix(mix(skyhorizon, skytop, m),vec4(1.0,1.0,1.0,1.0),clamp(pow(d,48.0),0.0,1.0));
}
