attribute vec3 coord3d;
attribute vec3 normal;
attribute float tex_select;

uniform mat4 mvp;

varying vec3 weight;
varying vec3 tex_coord;
varying vec4 coord;
varying vec3 f_normal;
varying vec3 tex_switch;
varying vec2 texture_switch[8];

void main(void)
{
    //send varying
    coord=vec4(coord3d,1.0);
    tex_coord=coord3d*0.05;
    f_normal=normal;

    int tex1=int(floor(tex_select/16.0));
    int tex2=int(mod(tex_select,16.0));

    for(int i=0;i<8;i++)
    {
        if(tex1==i+1)
            texture_switch[i].x=1.0;
        else
            texture_switch[i].x=0.0;

        if(tex2==i+1)
            texture_switch[i].y=1.0;
        else
            texture_switch[i].y=0.0;
    }

    //triplanar projection
    weight = abs(normalize(normal));
    weight = (weight-0.5)*7;//*7;//(weight - 0.267900);
    weight = max( weight, vec3( 0.0));
    weight /= vec3(weight.x  + weight.y + weight.z);

    vec4 pos=mvp * coord;
    vec3 tmp_switch=vec3(tex_select-1.0);

    gl_Position = pos;
}
