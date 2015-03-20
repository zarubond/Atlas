attribute highp vec3 coord3d;
attribute highp vec3 normal;
attribute highp vec2 texcoord;

uniform highp mat4 mvp;
uniform highp mat3 mv;
uniform highp mat4 model;

varying highp vec3 f_normal;
varying highp vec2 f_texcoord;
varying highp vec4 coord;


void main()
{
   f_normal=mv*normal;
   f_texcoord=texcoord;
   coord=mvp * vec4(coord3d,1.0);

   gl_Position = coord;
}
