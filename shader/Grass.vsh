uniform mat4 mvp;
uniform vec3 pos;
uniform float scale;
uniform float time;

attribute vec3 coord3d;
attribute vec2 texcoord;
attribute vec3 normal;

varying vec2 tex;
varying vec3 f_normal;
varying vec3 coord;

void main(void)
{
   tex=texcoord;
   f_normal=normal;
   coord=coord3d;

   float a=sin(coord.y)*0.3;
   a*=a;
   coord.z+=a*sin(time+pos.z);
   coord.x+=a*sin(time+pos.x);
   coord.y+=a*sin(time);

   coord*=scale;
   coord+=pos;
   gl_Position = mvp * vec4(coord, 1.0);
}
