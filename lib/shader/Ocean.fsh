uniform float time;
uniform sampler2D noise;
uniform vec4 color;
varying vec3 f_vertex;

//http://threejs.org/examples/#webgl_shaders_ocean

vec4 encode (vec3 n)
{
    float p = sqrt(n.z*8.0+8.0);
    return vec4(n.xy/p + 0.5,0.0,1.0);
}

vec4 encodeBasic (vec3 n)
{
    return vec4(n.xyz*0.5+0.5,1.0);
}

vec4 getNoise( vec2 uv )
{
        vec2 uv0 = ( uv / 103.0 ) + vec2(time / 17.0, time / 29.0);
        vec2 uv1 = uv / 107.0-vec2( time / -19.0, time / 31.0 );
        vec2 uv2 = uv / vec2( 8907.0, 9803.0 ) + vec2( time / 101.0, time / 97.0 );
        vec2 uv3 = uv / vec2( 1091.0, 1027.0 ) - vec2( time / 109.0, time / -113.0 );
        vec4 normal = ( texture2D( noise, uv0 ) ) +
                ( texture2D( noise, uv1 ) ) +
                ( texture2D( noise, uv2 ) ) +
                ( texture2D( noise, uv3 ) );
        return normal * 0.5 - 1.0;
}

void main(void)
{
    vec3 normal = getNoise( f_vertex.xz*0.1 ).xyz;
    //vec3 normal = texture2D( noise, f_vertex.xz*0.01*time ).xyz;//*0.5-vec3(1.0);//-vec3(0.5);//normalize( n.xyz );
    //normal.y=abs(normal.y);
    normal=normalize(normal);

    gl_FragData[0] = color;
    gl_FragData[1] = encode(normal);
    gl_FragData[2] = vec4(f_vertex,1.0);
}
