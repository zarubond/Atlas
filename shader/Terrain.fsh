#extension GL_EXT_texture_array : enable

varying vec4 coord;
varying vec3 f_normal;
varying vec3 weight;
varying vec3 tex_coord;

varying vec2 texture_switch[8];

//#if GL_EXT_texture_array == 1
//uniform sampler2D texture_atlas;
//uniform sampler2D texture_normal;
//#else
uniform sampler2DArray texture_atlas;
uniform sampler2DArray texture_normal;
//#endif

varying vec3 tex_switch;

//http://aras-p.info/texts/CompactNormalStorage.html
vec4 encode (vec3 n)
{
    float p = sqrt(n.z*8.0+8.0);
    return vec4(n.xy/p + 0.5,0.0,1.0);
}

//https://github.com/ehsan/ogre/blob/master/Samples/Media/RTShaderLib/SGXLib_TextureAtlas.glsl

vec4 getTexel(vec2 uv, int tex,float id)
{
    vec3 ab=vec3 (uv.x,uv.y,id);
    return texture2DArray(texture_atlas, ab)*weight[tex];
}

vec3 getNormal(vec2 uv, int tex,float id)
{
    vec3 ab=vec3 (uv.x,uv.y,id);
    return texture2DArray(texture_normal, ab).xyz-vec3(0.5);
}


float saturate(float var)
{
    return  clamp(var, 0.0, 1.0);
}


void main(void)
{
    vec3 norm=f_normal;

    vec4 vTexColor=vec4(0);
    vec3 nor1=vec3(0.0),nor2=vec3(0.0),nor3=vec3(0.0);

    for(int i=0;i<8;i++)
    {
       if(texture_switch[i].x>0.0)
       {
           vTexColor+=getTexel(tex_coord.yz,0,i)*texture_switch[i].x;
           vTexColor+=getTexel(tex_coord.xy,2,i)*texture_switch[i].x;

           nor1 += getNormal(tex_coord.yz,0,i)*texture_switch[i].x;
           nor3 += getNormal(tex_coord.xy,2,i)*texture_switch[i].x;
       }
       if(texture_switch[i].y>0.0)
       {
            vTexColor+=getTexel(tex_coord.zx,1,i)*texture_switch[i].y;
            nor2 += getNormal(tex_coord.zx,1,i)*texture_switch[i].y;
       }
    }

/*
        vTexColor=getTexel(tex_coord.yz,0);
        normal=getNormal(tex_coord.yz,0);

        vTexColor+=getTexel(tex_coord.zx,1);
        normal=+getNormal(tex_coord.zx,1);

        vTexColor+=getTexel(tex_coord.xy,2);
        normal+=getNormal(tex_coord.xy,2);*/

    vec3 w=weight*4.0;
    vec3 bump1 = vec3(0.0, nor1.x, nor1.y)*w.x;
    vec3 bump2 = vec3(nor2.y, 0.0, nor2.x)*w.y;
    vec3 bump3 = vec3(nor3.x, nor3.y, 0.0)*w.z;

    vec3 normal = bump1 + bump2 + bump3 + norm;
    normal=normalize(normal);

    vTexColor.a=1.0;

#if __VERSION__  >= 120

    gl_FragData[0] = vTexColor;
    gl_FragData[1] = encode(normal);
    gl_FragData[2] = coord;

#else
/////////////////////////////////////////////////////////FOR TEGRA 2
    vTexColor.a=1.0;
    gl_FragColor = vTexColor;
#endif
}
