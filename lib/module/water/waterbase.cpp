/**
 *  Atlas - Volumetric terrain editor
 *  Copyright (C) 2012-2015  Ondřej Záruba
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#include "waterbase.h"

Water::Water(int type):ModuleBase(type)
{
    water_height=25;
    visible=true;
}

bool Water::init()
{
    const char * vertex_file=":lib/shader/Water.vert";
    const char * fragment_file=":lib/shader/Water.frag";

    shader.loadFile(vertex_file,fragment_file);

    uniform_time = shader.getUniform("time");
    uniform_mvp = shader.getUniform("mvp");
    uniform_map = shader.getUniform("map");
    uniform_water_height = shader.getUniform("water_height");

    attribute_vertex = shader.getAttribute("vertex");

    //normal_map.load("./project/normal_map.png",GL_LINEAR_MIPMAP_LINEAR);
    normal_map.loadFile("/mnt/disk/Projects/Atlas-v2/Maps/Island/water.png",GL_LINEAR_MIPMAP_LINEAR);

    GLfloat coordverts[]={
        1024.0,20.0,1024.0,
        1024.0,20.0,0.0,
        0.0,20.0,0.0,

        0.0,20.0,0.0,
        0.0,20.0,1024.0,

        1024.0,20.0,1024.0
    };

   // for(int i=0;i<24;i++)
       // coordverts[i]*=50;


    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordverts), coordverts, GL_STATIC_DRAW);
    return true;
}

void Water::render(const Matrix4f &mvp,GLfloat time)
{
    shader.bind();
    Texture::active(Texture::TEXTURE0);

    shader.uniform(uniform_map,0);
    normal_map.bind();

    time/=10;
    shader.uniform(uniform_time,time);
    shader.uniformMatrix(uniform_mvp,mvp);
    shader.uniform(uniform_water_height,water_height);

    glEnableVertexAttribArray(attribute_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glVertexAttribPointer(attribute_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //glDrawArrays(GL_TRIANGLES,0,6);
}
