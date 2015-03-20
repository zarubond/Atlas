attribute highp vec3 coord3d;
attribute lowp vec3 normal;
attribute highp vec4 texture;

uniform highp mat4 mvp;

varying highp vec3 weight;
varying highp vec3 tex_coord;
varying highp vec3 f_normal;
varying highp vec2 material[4];//x-vertical y-horizontal

void main()
{
    //send varying
    tex_coord=coord3d*0.05;
    f_normal=normalize(normal);
/*
    for(int i=0;i<4;i++)
    {
        material[i].x=floor(texture[i]/16.0);
        material[i].y=mod(texture[i],16.0);
    }*/

    material[0].x=floor(texture.x/16.0)/15.0;
    material[0].y=mod(  texture.x,16.0)/15.0;
    material[1].x=floor(texture.y/16.0)/15.0;
    material[1].y=mod(  texture.y,16.0)/15.0;
    material[2].x=floor(texture.z/16.0)/15.0;
    material[2].y=mod(  texture.z,16.0)/15.0;
    material[3].x=floor(texture.w/16.0)/15.0;
    material[3].y=mod(  texture.w,16.0)/15.0;

    //triplanar projection
    weight = abs(f_normal);
    weight = (weight-0.5)*7.0;//*7;//(weight - 0.267900);
    weight = normalize(max( weight, vec3( 0.0)));
    weight /= vec3(weight.x  + weight.y + weight.z);

    vec4 pos=mvp * vec4(coord3d,1.0);
    gl_Position = pos;
}
