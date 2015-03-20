varying highp vec3 f_normal;
varying highp vec2 f_texcoord;
varying highp vec4 coord;

uniform lowp sampler2D diffuse_texture;
uniform bool selected;

uniform highp vec3 diffuse;
uniform highp vec3 sun_dir;
uniform highp vec3 ambient;

vec4 outColor(vec3 color, vec3 normal)
{
    vec3 diff=diffuse * max(0.0, dot(normal, sun_dir));
    return vec4(color*(diff+ambient),1.0);
}

//program
void main()
{
   vec4 vTexColor=texture2D(diffuse_texture,f_texcoord);

   if(vTexColor.a<0.6)
       discard;

    vTexColor.a=1.0;
    if(selected)
       vTexColor.r=1.0;

    gl_FragColor = outColor(vTexColor.rgb,f_normal);
}
