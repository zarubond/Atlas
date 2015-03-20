uniform sampler2D texture_atlas[4];

varying highp vec3 f_normal;
varying highp vec3 weight;
varying highp vec3 tex_coord;
varying highp vec2 material[4];

uniform lowp vec3 diffuse;
uniform lowp vec3 ambient;
uniform lowp vec3 sun_dir;

vec4 outColor(vec3 color, vec3 normal)
{
    highp vec3 diff=diffuse * max(0.0, dot(normal, sun_dir));
    return vec4(color*(diff+ambient),1.0);
}
/*
vec4 getTexel(vec2 uv, float w,float id)
{
    vec3 ab=vec3 (uv.x,uv.y,id);
    return texture2DArray(texture_atlas, ab)*w;
}
*/
void main()
{
    highp vec4 texColor=vec4(0.0);

    highp float t;
    //0
    if(material[0].x>0.1)
    {
        texColor += texture2D(texture_atlas[0],tex_coord.yz)*weight[0]*material[0].x;
        texColor += texture2D(texture_atlas[0],tex_coord.xy)*weight[2]*material[0].x;
    }

    if(material[0].y>0.1)
    {
        texColor += texture2D(texture_atlas[0],tex_coord.zx)*weight[1]*material[0].y;
    }
    //1
    if(material[1].x>0.1)
    {
        texColor += texture2D(texture_atlas[1],tex_coord.yz)*weight[0]*material[1].x;
        texColor += texture2D(texture_atlas[1],tex_coord.xy)*weight[2]*material[1].x;
    }

    if(material[1].y>0.1)
    {
        texColor += texture2D(texture_atlas[1],tex_coord.zx)*weight[1]*material[1].y;
    }
    //2
    if(material[2].x>0.1)
    {
        texColor += texture2D(texture_atlas[2],tex_coord.yz)*weight[0]*material[2].x;
        texColor += texture2D(texture_atlas[2],tex_coord.xy)*weight[2]*material[2].x;
    }

    if(material[2].y>0.1)
    {
        texColor += texture2D(texture_atlas[2],tex_coord.zx)*weight[1]*material[2].y;
    }
    //3
    if(material[3].x>0.1)
    {
        texColor += texture2D(texture_atlas[3],tex_coord.yz)*weight[0]*material[3].x;
        texColor += texture2D(texture_atlas[3],tex_coord.xy)*weight[2]*material[3].x;
    }

    if(material[3].y>0.1)
    {
        texColor += texture2D(texture_atlas[3],tex_coord.zx)*weight[1]*material[3].y;
    }
/*
    float t;
    for(int i=0;i<4;i++)
    {
        if(material[i].x>0.0)
        {
            t=material[i].x/15.0;
            texColor += getTexel(tex_coord.yz,weight[0],float(i))*t;
            texColor += getTexel(tex_coord.xy,weight[2],float(i))*t;
        }

        if(material[i].y>0.0)
        {
            t=material[i].y/15.0;
            texColor += getTexel(tex_coord.zx,weight[1],float(i))*t;
        }
    }
*/
    gl_FragColor = outColor(texColor.rgb, f_normal);
}
