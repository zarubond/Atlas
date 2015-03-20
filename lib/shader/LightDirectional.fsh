uniform sampler2D tex_color;
uniform sampler2D tex_normal;
uniform sampler2D tex_position;
uniform sampler2DShadow tex_shadow;

uniform vec3 direction, diffuse, specular;
uniform vec3 camera;
uniform float shininess;
uniform mat4 shadowMVP;
uniform bool shadowEnable;

varying vec2 f_texcoord;


vec3 decode (vec4 enc)
{
    vec2 fenc = enc.xy*4.0-2.0;
    float f = dot(fenc,fenc);
    float g = sqrt(1.0-f/4.0);
    vec3 n;
    n.xy = fenc*g;
    n.z = 1.0-f/2.0;
    return n;
}

vec3 decodeBasic(vec4 enc)
{
    return enc.xyz*2.0-vec3(1.0);
}

vec3 colorIntensity(vec3 normal, vec3 position)
{
    vec3 fragColor = vec3(diffuse) * max(0.0, dot(normal, direction));

    if (dot(normal, direction) > 0.0)
    {
        vec3 viewDirection = normalize(camera-position);
        fragColor += vec3(specular) * pow(max(0.0, dot(reflect(direction, normal), viewDirection)), shininess);
    }

    return fragColor;
}

void main(void)
{
    vec3 normal = decode(texture2D(tex_normal,f_texcoord));
    vec3 position = texture2D(tex_position, f_texcoord).xyz;
    vec3 color = texture2D(tex_color, f_texcoord).rgb;
    vec3 fragIntensity;

    if(shadowEnable)
    {
        vec4 shadowCoord = shadowMVP * vec4(position,1.0);
        shadowCoord.z-=0.005;

        float visibility = texture(tex_shadow, vec3(shadowCoord.xy, (shadowCoord.z)/shadowCoord.w) );
        fragIntensity = colorIntensity(normal, position)*max(visibility,0.2);
    }
    else
        fragIntensity = colorIntensity(normal, position);

    gl_FragData[0] = vec4( fragIntensity*color, 0.0);
}
