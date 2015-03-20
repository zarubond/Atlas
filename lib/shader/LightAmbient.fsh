uniform sampler2D tex_color;
uniform vec3 color;
varying vec2 f_texcoord;

void main(void)
{
    gl_FragData[0] = vec4(color*texture2D(tex_color,f_texcoord).xyz,1.0);
}
