uniform sampler2DArray texture_atlas;
uniform sampler2DArray texture_normal;

varying vec4 coord;
varying vec3 f_normal;
varying vec3 weight;
varying vec3 tex_coord;
varying vec2 material[4];


vec4 encode(vec3 n)
{
    float p = sqrt(n.z*8.0+8.0);
    return vec4(n.xy/p + 0.5,0.0,1.0);
}

vec4 encodeBasic (vec3 n)
{
    return vec4(n.xyz*0.5+0.5,1.0);
}

vec4 getTexel(vec2 uv, float w,float id)
{
    vec3 ab=vec3 (uv.x,uv.y,id);
    //return texture2DArray(texture_atlas, ab)*w;
    return texture(texture_atlas, ab)*w;
}

vec3 getNormal(vec2 uv, float id)
{
    vec3 ab=vec3 (uv.x,uv.y,id);
    //return texture2DArray(texture_normal, ab).xyz-vec3(0.5);
    return texture(texture_normal, ab).xyz-vec3(0.5);
}

float saturate(float var)
{
    return  clamp(var, 0.0, 1.0);
}


void main(void)
{
    vec3 norm=f_normal;

    vec4 vTexColor=vec4(0.0);
    vec3 nor1=vec3(0.0),nor2=vec3(0.0),nor3=vec3(0.0);

    /*
    Does not work on adreno + android
    float t;
    for(int i=0;i<4;i++)
    {
        if(material[i].x>0.0)
        {
            t=material[i].x/15.0;
            vTexColor += getTexel(tex_coord.yz,weight[0],float(i))*t;
            vTexColor += getTexel(tex_coord.xy,weight[2],float(i))*t;
            nor1 += getNormal(tex_coord.yz,float(i))*t;
            nor3 += getNormal(tex_coord.xy,float(i))*t;
        }

        if(material[i].y>0.0)
        {
            t=material[i].y/15.0;
            vTexColor += getTexel(tex_coord.zx,weight[1],float(i))*t;
            nor2 += getNormal(tex_coord.zx,float(i))*t;
        }
    }*/

    if(material[0].x>0.1)
    {
        vTexColor += getTexel(tex_coord.yz,weight[0],0.0)*material[0].x;
        vTexColor += getTexel(tex_coord.xy,weight[2],0.0)*material[0].x;
        nor1 += getNormal(tex_coord.yz,0.0)*material[0].x;
        nor3 += getNormal(tex_coord.xy,0.0)*material[0].x;
    }

    if(material[0].y>0.1)
    {
        vTexColor += getTexel(tex_coord.zx,weight[1], 0.0)*material[0].y;
        nor2 += getNormal(tex_coord.zx, 0.0)*material[0].y;
    }

    if(material[1].x>0.1)
    {
        vTexColor += getTexel(tex_coord.yz,weight[0],1.0)*material[1].x;
        vTexColor += getTexel(tex_coord.xy,weight[2],1.0)*material[1].x;
        nor1 += getNormal(tex_coord.yz,1.0)*material[1].x;
        nor3 += getNormal(tex_coord.xy,1.0)*material[1].x;
    }

    if(material[1].y>0.1)
    {
        vTexColor += getTexel(tex_coord.zx,weight[1], 1.0)*material[1].y;
        nor2 += getNormal(tex_coord.zx, 1.0)*material[1].y;
    }

    if(material[2].x>0.1)
    {
        vTexColor += getTexel(tex_coord.yz,weight[0], 2.0)*material[2].x;
        vTexColor += getTexel(tex_coord.xy,weight[2], 2.0)*material[2].x;
        nor1 += getNormal(tex_coord.yz,2.0)*material[2].x;
        nor3 += getNormal(tex_coord.xy,2.0)*material[2].x;
    }

    if(material[2].y>0.1)
    {
        vTexColor += getTexel(tex_coord.zx,weight[1], 2.0)*material[2].y;
        nor2 += getNormal(tex_coord.zx, 2.0)*material[2].y;
    }

    if(material[3].x>0.1)
    {
        vTexColor += getTexel(tex_coord.yz,weight[0],3.0)*material[3].x;
        vTexColor += getTexel(tex_coord.xy,weight[2],3.0)*material[3].x;
        nor1 += getNormal(tex_coord.yz,3.0)*material[3].x;
        nor3 += getNormal(tex_coord.xy,3.0)*material[3].x;
    }

    if(material[3].y>0.1)
    {
        vTexColor += getTexel(tex_coord.zx,weight[1], 3.0)*material[3].y;
        nor2 += getNormal(tex_coord.zx, 3.0)*material[3].y;
    }

    vec3 w=weight*4.0;
    vec3 bump1 = vec3(0.0, nor1.x, nor1.y)*w.x;
    vec3 bump2 = vec3(nor2.y, 0.0, nor2.x)*w.y;
    vec3 bump3 = vec3(nor3.x, nor3.y, 0.0)*w.z;

    vec3 normal = bump1 + bump2 + bump3 + norm;
    normal=normalize(normal);

    vTexColor.a=1.0;

    gl_FragData[0] = vTexColor;
    gl_FragData[1] = encode(normal);
    gl_FragData[2] = coord;
}
