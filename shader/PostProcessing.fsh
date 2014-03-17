uniform sampler2D fbo_texture;
uniform sampler2D fbo_normal;
uniform sampler2D tex_position;
uniform sampler2D tex_shadow;
uniform mat4 inv;
uniform vec3 cam_pos;
uniform mat4 depthmvp;
uniform bool enable_shadow;

varying vec2 f_texcoord;

struct Light
{
    vec3 pos,dir;//dir must be normalized
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
    float radius;
    float exponent;
    float shininess;
    int type;
};


uniform Light light[10];
uniform vec3 ambient_light;
uniform int num_lights;

uniform float fog_density;
uniform vec3 fog_color;


//http://aras-p.info/texts/CompactNormalStorage.html
vec3 decode (vec2 enc)
{
    vec2 fenc = enc*4.0-2.0;
    float f = dot(fenc,fenc);
    float g = sqrt(1.0-f/4.0);
    vec3 n;
    n.xy = fenc*g;
    n.z = 1.0-f/2.0;
    return n;
}

float getFogFactor(float coord)
{/*
   float fResult = 0.0;
   if(params.iEquation == 0)
      fResult = (params.fEnd-fFogCoord)/(params.fEnd-params.fStart);
   else if(params.iEquation == 1)
      fResult = exp(-params.fDensity*fFogCoord);
   else if(params.iEquation == 2)*/
   //float fResult = exp(-pow(fog_density*coord, 2.0));
   float fResult = exp(-fog_density*coord);

   return 1.0-clamp(fResult, 0.0, 1.0);
}


vec3 fragColor(vec3 color,vec3 normal,vec3 coord)
{
    vec3 all_light=ambient_light;
    float attenuation=1.0;
    //spot light
    for(int i=0;i<num_lights;i++)
    {
        float light_dot=dot(normal, light[i].dir);
        if(light[i].type==0)//directional light
        {
            attenuation=1.0;
            all_light+=light[i].diffuse*(max(0.0, light_dot));
        }
        else
        {
            vec3 vertexToLightSource = vec3(light[i].pos - coord);
            float distance = length(vertexToLightSource);
            vec3 light_direction = normalize(vertexToLightSource);

            float clampedCosine = max(0.0, dot(-light_direction, light[i].dir));

            float attenuation = 1.0 / (light[i].attenuation.x
                                 + light[i].attenuation.y * distance
                                 + light[i].attenuation.z * distance * distance);

            if(light[i].radius > 0)//spot light
            {
                if (clampedCosine > light[i].radius) // in cone
                {
                    attenuation *=  pow(clampedCosine, light[i].exponent);
                    all_light+= light[i].diffuse*max(0.0, -dot(normal,light_direction))*attenuation;
                }
            }
            else//point light
            {
                all_light+= light[i].diffuse*max(0.0, -dot(normal,light_direction))*attenuation;
            }
        }

        //specular
        if (light_dot > 0.0) // light on the right side
        {
            vec3 view_direction = normalize(cam_pos-coord);
            all_light+=  light[i].specular *
                    pow(max(0.0, dot(reflect(light[i].dir, normal), view_direction) ),light[i].shininess)*attenuation;
        }
    }

    return color*all_light;// mix(fog.color,color*vec4(all_light,1.0),fog_factor);
}

void main(void)
{
    vec3 normal = decode(texture2D(fbo_normal,f_texcoord).xy);
    //normal=normalize(normal);//must be done in object's shader
    vec4 color = texture2D(fbo_texture, f_texcoord);
    vec3 coord = texture2D(tex_position, f_texcoord).xyz;

    if(enable_shadow)
    {
            vec4 ShadowCoord = depthmvp * vec4(coord,1.0);
            ShadowCoord.z-=0.003;

            vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;
            vec4 distanceFromLight = texture2D(tex_shadow,shadowCoordinateWdivide.st);

            if(distanceFromLight.z < shadowCoordinateWdivide.z)
            {
                vec3 diffu=vec3(max(0.0, dot(normal, light[0].dir))/2.0);
                color =vec4(color.rgb*(diffu+ambient_light)*0.7,color.a);
            }
            else
            {
                color=vec4(fragColor(color.rgb,normal,coord),color.a);
            }

            if(fog_density>0)
            {
                float fog=getFogFactor(distance(cam_pos,coord));
                gl_FragColor=mix(color,vec4(fog_color,1.0),fog);
            }
            else
                gl_FragColor=color;
    }
    else
    {
            gl_FragColor=vec4(fragColor(color.rgb,normal,coord),color.a);
    }
}
