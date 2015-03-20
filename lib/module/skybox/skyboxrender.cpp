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

#include "skyboxrender.h"


SkyBoxRender::SkyBoxRender()
{
    x=0;
    y=0;
    z=0;
}

SkyBoxRender::~SkyBoxRender()
{
}

void SkyBoxRender::renderPrev(WorldGraphics *world, const Environment *envi, const Camera &camera, unsigned int elapsed)
{
    shader.bind();
    shader.uniformMatrix(uniform_mvp, camera.infinitePerspectiveLook());
    shader.uniform(uniform_sun, envi->sun.getDirection());
    if(!world->skybox.vao.isCreated())
        renderSetup(&world->skybox);

    world->skybox.vao.bind();
    indices.draw(IndexBuffer::TRIANGLES, 36, IndexBuffer::UNSIGNED_SHORT);
    world->skybox.vao.release();
    shader.release();
}

bool SkyBoxRender::init()
{
    GLfloat scale=100.0;
    GLfloat cube_vertices[] = {
    // front
    -1.0, -1.0,  1.0,
     1.0, -1.0,  1.0,
     1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // top
    -1.0,  1.0,  1.0,
     1.0,  1.0,  1.0,
     1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
    // back
     1.0, -1.0, -1.0,
    -1.0, -1.0, -1.0,
    -1.0,  1.0, -1.0,
     1.0,  1.0, -1.0,
    // bottom
    -1.0, -1.0, -1.0,
     1.0, -1.0, -1.0,
     1.0, -1.0,  1.0,
    -1.0, -1.0,  1.0,
    // left
    -1.0, -1.0, -1.0,
    -1.0, -1.0,  1.0,
    -1.0,  1.0,  1.0,
    -1.0,  1.0, -1.0,
    // right
     1.0, -1.0,  1.0,
     1.0, -1.0, -1.0,
     1.0,  1.0, -1.0,
     1.0,  1.0,  1.0,
    };

    for(int i=0;i<72;i++)
        cube_vertices[i]*=scale;

    vertices.create(sizeof(cube_vertices),cube_vertices);

    GLushort cube_elements[] = {
        /*
        // front
        0,  1,  2,
        2,  3,  0,
        // top
        4,  5,  6,
        6,  7,  4,
        // back
        8,  9, 10,
        10, 11,  8,
        // bottom
        12, 13, 14,
        14, 15, 12,
        // left
        16, 17, 18,
        18, 19, 16,
        // right
        20, 21, 22,
        22, 23, 20*/

        // front
        2,  1,  0,
        0,  3,  2,
        // top
        6,  5,  4,
        4,  7,  6,
        // back
        10,  9, 8,
        8, 11,  10,
        // bottom
        14, 13, 12,
        12, 15, 14,
        // left
        18, 17, 16,
        16, 19, 18,
        // right
        22, 21, 20,
        20, 23, 22
    };

    indices.create(sizeof(cube_elements),cube_elements);

    return loadShader();
}

bool SkyBoxRender::loadShader()
{
    if(shader.loadFile(":lib/shader/Skybox.vsh",":lib/shader/Skybox.fsh")==false)
        return false;

    if((a_coord = shader.getAttribute("coord"))==-1)
        return false;

    if((uniform_mvp = shader.getUniform("mvp"))==-1)
        return false;

    if((uniform_sun = shader.getUniform("sun_dir"))==-1)
        return false;

    return true;
}

void SkyBoxRender::renderSetup(SkyBoxGraphics *skybox)
{
    skybox->vao.create();
    skybox->vao.bind();
    shader.enableAttribute(a_coord);
    vertices.bind();
    shader.setAttribute(a_coord, 3, GL_FLOAT);
    indices.bind();
    skybox->vao.release();
}
