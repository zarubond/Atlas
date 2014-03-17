varying vec3 f_normal;
varying vec2 f_texcoord;
varying vec4 coord;

uniform sampler2D texture;
uniform bool selected;

vec4 encode (vec3 n)
{
    float p = sqrt(n.z*8.0+8.0);
    return vec4(n.xy/p + 0.5,0.0,1.0);
}

//program
void main(void)
{
   vec4 vTexColor=texture2D(texture,f_texcoord);

   if(vTexColor.a<0.6)
       discard;
       //gl_FragDepth=1000;


   vTexColor.a=1.0;
   if(selected)
       vTexColor.r=1.0;
   vec3 nor=normalize(f_normal);

   gl_FragData[0] = vTexColor;
   gl_FragData[1] = encode(nor);
   gl_FragData[2] = coord;
}
