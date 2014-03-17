attribute vec3 vertex;
uniform mat4 mvp;
varying vec3 f_vertex;

void main(void)
{
    f_vertex=vertex;
    gl_Position = mvp * vec4(vertex,1.0);//vec4(vertex,1.0);
}
