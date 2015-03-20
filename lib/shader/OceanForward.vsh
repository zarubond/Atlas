attribute vec3 vertex;

uniform mat4 mvp;
uniform float water_height;
uniform vec3 size;

varying vec3 f_vertex;

void main()
{
    f_vertex=vertex*size;
    f_vertex.y=water_height;
    gl_Position = mvp * vec4(f_vertex,1.0);
}
