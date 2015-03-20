attribute vec3 coord;
varying vec3 normal;
uniform mat4 mvp;

void main()
{
    normal=coord;
    gl_Position = mvp * vec4(coord,0.0);
}
