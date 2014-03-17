varying vec3 coord;
varying vec3 f_normal;
varying vec3 weight;
varying float[8] texture_switch;
varying float fog_factor,dist;

uniform sampler2D texture[8];
uniform vec3 cam_pos;

struct Fog
{
    float density;
    vec4 color;
};

uniform Fog fog;
struct Light
{
    vec3 pos,dir;
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

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;

vec4 fragColor(vec4 color,vec3 normal)
{
    vec3 all_light=ambient_light*material.ambient;
    float attenuation=1.0;
    //spot light
    for(int i=0;i<num_lights;i++)
    {
        if(light[i].type==0)//directional light
        {
            attenuation=1.0;
            all_light+=material.diffuse*light[i].diffuse*(max(0.0, dot(normal, normalize(light[i].dir))));
        }
        else
        {
            vec3 vertexToLightSource = vec3(light[i].pos - coord);
            float distance = length(vertexToLightSource);
            vec3 light_direction = normalize(vertexToLightSource);

            float clampedCosine = max(0.0, dot(-light_direction, normalize(light[i].dir)));

            float attenuation = 1.0 / (light[i].attenuation.x
                                 + light[i].attenuation.y * distance
                                 + light[i].attenuation.z * distance * distance);

            vec3 spot=vec3(0.0,0.0,0.0);
            if(light[i].radius <= 90)//spot light
            {
                if (clampedCosine > cos(radians(light[i].radius))) // in cone
                {
                    attenuation *=  pow(clampedCosine, light[i].exponent);
                    all_light+= material.diffuse*light[i].diffuse*max(0.0, -dot(normal,light_direction))*attenuation;
                }
            }
            else//point light
            {
                all_light+= material.diffuse*light[i].diffuse*max(0.0, -dot(normal,light_direction))*attenuation;
            }
        }

        //specular
        if (dot(normal, normalize(light[i].dir)) > 0.0) // light on the right side
        {
            vec3 view_direction = normalize(cam_pos-coord);
            all_light+=  material.specular * light[i].specular*
                    pow(max(0.0, dot(reflect(normalize(light[i].dir), normal), view_direction)),light[i].shininess)*attenuation;
        }
    }

    return mix(fog.color,color*vec4(all_light,1.0),fog_factor);
}

void main(void) {
    //texture
    vec4 vTexColor=vec4(0.0);
    //for all graphic cards exept crippled ATI
/*
    for(int i=0;i<8;i++)
    {
       if(texture_switch[i]>0)
       {
           vTexColor += ((texture2D(texture[i], coord.yz*0.2))*weight.x
                         +(texture2D(texture[i],coord.zx*0.2))*weight.y
                         + (texture2D(texture[i],coord.xy*0.2))*weight.z)*texture_switch[i];
       }
    }*/

    if(texture_switch[0]>0)
    {
        vTexColor += ((texture2D(texture[0], coord.yz*0.2))*weight.x
                      +(texture2D(texture[0],coord.zx*0.2))*weight.y
                      + (texture2D(texture[0],coord.xy*0.2))*weight.z)*texture_switch[0];
    }
    if(texture_switch[1]>0)
    {
        vTexColor += ((texture2D(texture[1], coord.yz*0.2))*weight.x
                      +(texture2D(texture[1],coord.zx*0.2))*weight.y
                      + (texture2D(texture[1],coord.xy*0.2))*weight.z)*texture_switch[1];
    }
    if(texture_switch[2]>0)
    {
        vTexColor += ((texture2D(texture[2], coord.yz*0.2))*weight.x
                      +(texture2D(texture[2],coord.zx*0.2))*weight.y
                      + (texture2D(texture[2],coord.xy*0.2))*weight.z)*texture_switch[2];
    }
    if(texture_switch[3]>0)
    {
        vTexColor += ((texture2D(texture[3], coord.yz*0.2))*weight.x
                      +(texture2D(texture[3],coord.zx*0.2))*weight.y
                      + (texture2D(texture[3],coord.xy*0.2))*weight.z)*texture_switch[3];
    }
    if(texture_switch[4]>0)
    {
        vTexColor += ((texture2D(texture[4], coord.yz*0.2))*weight.x
                      +(texture2D(texture[4],coord.zx*0.2))*weight.y
                      + (texture2D(texture[4],coord.xy*0.2))*weight.z)*texture_switch[4];
    }
    if(texture_switch[5]>0)
    {
        vTexColor += ((texture2D(texture[5], coord.yz*0.2))*weight.x
                      +(texture2D(texture[5],coord.zx*0.2))*weight.y
                      + (texture2D(texture[5],coord.xy*0.2))*weight.z)*texture_switch[5];
    }
    if(texture_switch[6]>0)
    {
        vTexColor += ((texture2D(texture[6], coord.yz*0.2))*weight.x
                      +(texture2D(texture[6],coord.zx*0.2))*weight.y
                      + (texture2D(texture[6],coord.xy*0.2))*weight.z)*texture_switch[6];
    }
    if(texture_switch[7]>0)
    {
        vTexColor += ((texture2D(texture[7], coord.yz*0.2))*weight.x
                      +(texture2D(texture[7],coord.zx*0.2))*weight.y
                      + (texture2D(texture[7],coord.xy*0.2))*weight.z)*texture_switch[7];
    }

    //calculated lights
    //vec4 lights=lightCalc(vTexColor);

    //gl_FragColor = vec4(vTexColor*(lights))+vec4(fog_factor*0.001);
    //gl_FragColor = mix(fog.color,vTexColor*(lights),fog_factor);
    vec3 normal=normalize(f_normal + vec3((vTexColor.w*2.0-1.0)/3.0));;
    vTexColor.w=1.0;
    gl_FragColor = normalize(fragColor(vTexColor,normal));
}
