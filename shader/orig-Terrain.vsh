attribute vec3 coord3d;
attribute float tex_select;
attribute vec3 normal;

uniform mat4 mvp;

varying vec3 weight;
varying vec3 coord;
varying vec3 f_normal;
varying float[8] texture_switch;
varying float fog_factor,dist;

//fog
const float LOG2 = 1.442695;

struct Fog
{
    float density;
    vec4 color;
};

uniform Fog fog;

float fogFactor(vec4 pos)
{
    float fog_len=length(pos);
    float factor=exp2(-fog.density * fog.density * fog_len * fog_len * LOG2);
    factor = clamp(factor, 0.0, 1.0);
    return factor;
}

void main(void)
{
    //selecting by index is slow
    for(int i=0;i<8;i++)
    {
        if(tex_select==i)
           texture_switch[i]=1.0;
        else
           texture_switch[i]=0.0;
    }

//triplanar projection
    weight=abs(normalize(normal));
    weight=(weight-0.2)*7;
    weight=max(weight,0);
    weight/=(weight.x+weight.y+weight.z);

    vec4 pos=mvp * vec4(coord3d, 1.0);
//send varying
    coord=coord3d;
    f_normal=normalize(normal);

//fog
    if(fog.density>0)
        fog_factor=fogFactor(pos);
    else
        fog_factor=1.0;

    gl_Position = pos;
}
