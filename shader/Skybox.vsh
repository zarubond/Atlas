attribute vec3 coord3d;
varying vec3 normal;
varying vec3 position;
uniform mat4 mvp;
uniform vec3 pos;

void main(void) {

    normal=coord3d;
    position=coord3d+pos;
    gl_Position = mvp * vec4(position,1.0);
}
