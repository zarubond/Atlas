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

#include "grassrender.h"

GrassRender::GrassRender()
{
}

void GrassRender::load(GrassGraphics *base)
{
    this->base=base;
}

bool GrassRender::init()
{
    const char * vertex_file=":lib/shader/Grass.vsh";
    const char * fragment_file=":lib/shader/Grass.fsh";

    if(!shader.loadFile(vertex_file,fragment_file))
        return false;

    if((uniform_mvp = shader.getUniform("mvp"))==-1)
        return false;

    if((uniform_pos = shader.getUniform("pos"))==-1)
        return false;

    if((uniform_scale = shader.getUniform("scale"))==-1)
        return false;

    if((uniform_time = shader.getUniform("time"))==-1)
        return false;

    if((attribute_coord3d = shader.getAttribute("coord3d"))==-1)
        return false;

    if((attribute_texcoord = shader.getAttribute("texcoord"))==-1)
        return false;

    if((attribute_normal = shader.getAttribute("normal"))==-1)
        return false;

    if((uniform_texture= shader.getUniform("diffuse"))==-1)
        return false;

    GLfloat coordverts[]={
        -0.25,0.0,-0.5, 0.0,0.0, 1.0,0.0,0.5,   0.25,0.0,0.5, 1.0,0.0, 1.0,0.0,0.5,  0.25,1.0,0.5, 1.0,1.0, 1.0,0.0,0.5,
        -0.25,1.0,-0.5, 0.0,1.0, 1.0,0.0,0.5, // 0.25,0.0,0.0, 0.0,0.0,   0.75,1.0,1.0, 1.0,1.0,

        -0.5,0.0,0.0, 0.0,0.0, 0.0,0.0,1.0,    0.5,0.0,0.0, 1.0,0.0, 0.0,0.0,1.0,    0.5,1.0,0.0, 1.0,1.0, 0.0,0.0,1.0,
        -0.5,1.0,0.0, 0.0,1.0, 0.0,0.0,1.0,   // 0.0,0.0,0.5, 0.0,0.0,    1.0,1.0,0.5, 1.0,1.0,

        -0.25,0.0,0.5, 0.0,0.0, 1.0,0.0,-0.5,  0.25,0.0,-0.5, 1.0,0.0, 1.0,0.0,-0.5,  0.25,1.0,-0.5, 1.0,1.0, 1.0,0.0,-0.5,
        -0.25,1.0,0.5, 0.0,1.0,  1.0,0.0,-0.5 // 0.25,0.0,1.0, 0.0,0.0,   0.75,1.0,0.0, 1.0,1.0
    };

    GLushort indicess[]={0, 1, 2, 3, 0, 2,
                        4, 5, 6, 7, 4, 6,
                        8,9,10,11,8,10};
/*
    GLfloat coordverts[]={
        -0.1,0.0,0.0, 0.0,1.0, 0.0,0.0,0.0,   0.1,0.0,0.0, 0.0,1.0, 0.0,0.0,0.0,
        -0.1,0.33,0.0, 0.33,1.0, 0.0,0.0,0.0,  0.1,0.33,0.0, 0.33,1.0, 0.0,0.0,0.0,
        -0.1,0.66,0.0, 0.66,1.0,  0.0,0.0,0.0,  0.1,0.66,0.0, 0.66,1.0, 0.0,0.0,0.0,
        0.0,1.0,0.0, 1.0,0.0, 0.0,0.0,0.0

        -0.1,0.0,0.2, 0.0,1.0, 0.0,0.0,0.0,   0.1,0.0,0.2, 0.0,1.0, 0.0,0.0,0.0,
        -0.1,0.33,0.2, 0.33,1.0, 0.0,0.0,0.0,  0.1,0.33,0.2, 0.33,1.0, 0.0,0.0,0.0,
        -0.1,0.66,0.2, 0.66,1.0,  0.0,0.0,0.0,  0.1,0.66,0.2, 0.66,1.0, 0.0,0.0,0.0,
        0.0,1.0,0.2, 1.0,0.0, 0.0,0.0,0.0

        -0.1,0.0,-0.2, 0.0,1.0, 0.0,0.0,0.0,   0.1,0.0,-0.2, 0.0,1.0, 0.0,0.0,0.0,
        -0.1,0.33,-0.2, 0.33,1.0, 0.0,0.0,0.0,  0.1,0.33,-0.2, 0.33,1.0, 0.0,0.0,0.0,
        -0.1,0.66,-0.2, 0.66,1.0,  0.0,0.0,0.0,  0.1,0.66,-0.2, 0.66,1.0, 0.0,0.0,0.0,
        0.0,1.0,-0.2, 1.0,0.0, 0.0,0.0,0.0
    };

    GLushort indicess[]={
        0,1,2,  1,2,3,
        2,3,4,  3,4,5,
        4,5,6,

        6+0,6+1,6+2,  6+1,6+2,6+3,
        6+2,6+3,6+4,  6+3,6+4,6+5,
        6+4,6+5,6+6,

        12+0,12+1,12+2,  12+1,12+2,12+3,
        12+2,12+3,12+4,  12+3,12+4,12+5,
        12+4,12+5,12+6
    };*/

    vertices.create(sizeof(coordverts),coordverts);
    indices.create(sizeof(indicess),indicess);
    triangle_num=sizeof(coordverts)/8;
/*
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordverts), coordverts, GL_STATIC_DRAW);
    triangle_num=sizeof(coordverts)/8;


    glGenBuffers(1,&ibo_indices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicess), indicess, GL_STATIC_DRAW);
    */
    shader.bind();

    GLfloat scale=2.7;
    shader.uniform(uniform_scale,scale);
    shader.uniform(uniform_texture,0);

    shader.release();
    return true;
}

void GrassRender::render(GrassGraphics *base, const Environment *envi, const Matrix4f &mvp, GLuint elapsed)
{
    if(base->getTexture()==NULL)
        return;
    float e=float(elapsed)/1000.0;

    shader.bind();

    Texture::active(Texture::TEXTURE0);
    base->getTexture()->bind();

    shader.uniformMatrix(uniform_mvp,mvp);
    shader.uniform(uniform_time,e);

    shader.enableAttribute(attribute_coord3d);
    shader.enableAttribute(attribute_texcoord);

    vertices.bind();//(GL_ARRAY_BUFFER, vbo_vertices);
    shader.setAttribute(attribute_coord3d, 8*sizeof(GLfloat), GL_FLOAT, 3);
    shader.setAttribute(attribute_texcoord,  8*sizeof(GLfloat), GL_FLOAT, 2, 3*sizeof(GLfloat));
    shader.setAttribute(attribute_normal,  8*sizeof(GLfloat), GL_FLOAT, 3, 5*sizeof(GLfloat));
    indices.bind();

    int indices_count=18;
    GrassRegion * region;
    while(base->renderCount()>0)
    {
        region=base->popRender();

        for(int i=0;i<region->tuftCount();i++)
        {
            shader.uniform(uniform_pos,region->getTuft(i));
            //glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_SHORT, 0);
        }
    }

    indices.release();
    vertices.release();

    shader.disableAttribute(attribute_coord3d);
    shader.disableAttribute(attribute_texcoord);

    shader.release();
}

void GrassRender::renderShadow(GrassGraphics *base, const Environment *envi, const Matrix4f &mvp, unsigned int elapsed)
{
    //render(mvp,elapsed);
}

void GrassRender::clearRenderList()
{
    base->clearRender();
}

