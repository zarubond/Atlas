attribute vec3 coord3d;
attribute vec3 normal;
attribute vec4 texture;

uniform mat4 mvp;

varying vec3 weight;
varying vec3 tex_coord;
varying vec4 coord;
varying vec3 f_normal;
varying vec2 material[4];//x-vertical y-horizontal

//http://ogldev.atspace.co.uk/www/tutorial37/tutorial37.html
void main(void)
{
    //send varying
    coord=vec4(coord3d,1.0);
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
    weight = (weight-0.5)*2.0;//*7;//(weight - 0.267900);
    weight = normalize(max( weight, vec3( 0.0)));
    weight /= vec3(weight.x  + weight.y + weight.z);

    vec4 pos=mvp * coord;
    gl_Position = pos;
}
