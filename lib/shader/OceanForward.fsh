uniform highp float time;
uniform lowp sampler2D noise;
uniform lowp vec4 color;
varying highp vec3 f_vertex;

uniform highp vec3 diffuse;
uniform highp vec3 sun_dir;
uniform highp vec3 ambient;

//http://threejs.org/examples/#webgl_shaders_ocean

vec4 getNoise( vec2 uv )
{
        lowp vec2 uv0 = ( uv / 103.0 ) + vec2(time / 17.0, time / 29.0);
        lowp vec2 uv1 = uv / 107.0-vec2( time / -19.0, time / 31.0 );
        lowp vec2 uv2 = uv / vec2( 8907.0, 9803.0 ) + vec2( time / 101.0, time / 97.0 );
        lowp vec2 uv3 = uv / vec2( 1091.0, 1027.0 ) - vec2( time / 109.0, time / -113.0 );
        lowp vec4 normal = ( texture2D( noise, uv0 ) ) +
                ( texture2D( noise, uv1 ) ) +
                ( texture2D( noise, uv2 ) ) +
                ( texture2D( noise, uv3 ) );
        return normal * 0.5 - 1.0;
}

vec4 outColor(vec3 color, vec3 normal)
{
    highp vec3 diff=diffuse * max(0.0, dot(normal, sun_dir));
    return vec4(color*(diff+ambient),1.0);
}

void main()
{
    vec3 normal = getNoise( f_vertex.xz*0.1 ).xyz;
    //vec3 normal = texture2D( noise, f_vertex.xz*0.01*time ).xyz;//*0.5-vec3(1.0);//-vec3(0.5);//normalize( n.xyz );
    //normal.y=abs(normal.y);
    normal=normalize(normal);

    vec4 frag = outColor(color.rgb,normal);
    frag.a=color.a;
    gl_FragColor = frag;
}
