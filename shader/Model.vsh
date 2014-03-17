attribute vec3 coord3d;
attribute vec3 normal;
attribute vec2 texcoord;

uniform mat4 mvp;
uniform mat3 mv;
uniform mat4 model;

varying vec3 f_normal;
varying vec2 f_texcoord;
varying vec4 coord;


void main(void)
{
   f_normal=mv*normal;
   vec4 c=vec4(coord3d,1.0);

   f_texcoord=texcoord;
   coord=model *c;
   vec4 mpos=mvp * c;

   gl_Position = mpos;
}
