attribute vec3 coord3d;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 mvp;
uniform mat4 model;
uniform mat3 mv;

varying vec3 f_normal;
varying vec2 f_texcoord;
varying vec4 coord;


void main(void)
{
    f_normal=mv*normal;
    f_texcoord=texcoord;
    coord = model * vec4(coord3d,1.0);
    gl_Position = mvp * vec4(coord3d,1.0);
}
