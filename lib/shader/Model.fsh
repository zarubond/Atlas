varying vec3 f_normal;
varying vec2 f_texcoord;
varying vec4 coord;

uniform sampler2D diffuse;
uniform bool selected;

vec4 encode (vec3 n)
{
    float p = sqrt(n.z*8.0+8.0);
    return vec4(n.xy/p + 0.5,0.0,1.0);
}

vec4 encodeBasic (vec3 n)
{
    return vec4(n.xyz*0.5+0.5,1.0);
}


//program
void main(void)
{
   vec4 vTexColor=texture2D(diffuse,f_texcoord);

   if(vTexColor.a<0.6)
       discard;

   vTexColor.a=1.0;
   if(selected)
       vTexColor.r=1.0;

   gl_FragData[0] = vTexColor;
   gl_FragData[1] = encode(normalize(f_normal));
   gl_FragData[2] = coord;
}
