attribute vec3 v_coord;
varying vec2 f_texcoord;
uniform float scale;

void main(void)
{
    f_texcoord = (v_coord.xy+vec2(1.0,1.0))/2.0;
    f_texcoord *=scale;
    gl_Position =  vec4(v_coord, 1.0);
}
