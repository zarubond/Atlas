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

   float angle=pos.x+pos.y;
   mat3 rotationMatrix = mat3(cos(angle),   0.0, sin(angle),
                              0.0,          1.0, 0.0,
                              -sin(angle),  0.0, cos(angle));
   coord=rotationMatrix*coord;


   float a=sin(coord.y)*0.3;
   a*=a;
   coord.z+=a*sin(time+pos.z);
   coord.x+=a*sin(time+pos.x);
   coord.y+=a*sin(time);

   coord*=scale;
   //coord.x+=coord.y*0.5;
   //coord.z-=coord.y*0.5;
   coord.y*=clamp(cos(sin(pos.x)*sin(pos.z)),0.3,1.0);



   coord+=pos;
   gl_Position = mvp * vec4(coord, 1.0);
}
