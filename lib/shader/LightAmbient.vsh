attribute vec3 coord;
varying vec2 f_texcoord;

void main(void)
{
    f_texcoord = (coord.xy+vec2(1.0,1.0))/2.0;
    gl_Position =  vec4(coord, 1.0);
}
