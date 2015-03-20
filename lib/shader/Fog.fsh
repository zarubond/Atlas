uniform sampler2D tex_coord;
uniform vec3 color;
uniform vec3 cam_pos;
uniform float density;
uniform float height;
varying vec2 f_texcoord;

void main(void)
{
    vec3 coord=texture2D(tex_coord,f_texcoord).xyz;

    float dist=0.0;
    float h=height;
    if(coord[1]<h)
    {
        if(cam_pos[1]>h)
        {
            vec3 pos1=mix(coord, cam_pos, (h-coord[1])/(cam_pos[1]-coord[1]));
            dist=distance(coord,pos1);
        }
        else
            dist=distance(coord,cam_pos);
    }
    else if(cam_pos[1]<h)
    {
        vec3 pos=mix(coord, cam_pos, (h-coord[1])/(cam_pos[1]-coord[1]));
        dist=distance(cam_pos,pos);
    }

    float fog_factor = 1.0-clamp(exp(-pow(density*dist, 2.0)), 0.0, 1.0);

    gl_FragData[0] = vec4(color,fog_factor);
}
