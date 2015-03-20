attribute vec3 coord;
varying vec3 normal;
varying vec3 f_coord;
uniform mat4 mvp;

void main(void) {

    normal=coord;
    f_coord=coord*1000.0;
    gl_Position = mvp * vec4(coord,0.0);
}
