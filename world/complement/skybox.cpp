/**
 *  Atlas - Volumetric terrain editor
 *  Copyright (C) 2012-2013  Ondřej Záruba
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
#include "skybox.h"


SkyBox::SkyBox()
{
    vbo_cube_vertices=0;
    ibo_cube_elements=0;
    x=0;
    y=0;
    z=0;
}

SkyBox::~SkyBox()
{
    if(vbo_cube_vertices!=0)
        glDeleteBuffers(1, &vbo_cube_vertices);
    if(ibo_cube_elements!=0)
        glDeleteBuffers(1, &ibo_cube_elements);
}

bool SkyBox::load(GLfloat scale)
{
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

    glGenBuffers(1, &vbo_cube_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

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

    glGenBuffers(1, &ibo_cube_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

    texture.init();
    /*
    texture.loadBack("project/skybox/Back.bmp");
    texture.loadBottom("project/skybox/Bottom.bmp");
    texture.loadLeft("project/skybox/Left.bmp");
    texture.loadRight("project/skybox/Right.bmp");
    texture.loadFront("project/skybox/Front.bmp");
    texture.loadTop("project/skybox/Top.bmp");
*/
    return loadShader();
}

bool SkyBox::loadShader()
{
    if(shader.loadFile(":/shader/Skybox.vsh",":/shader/Skybox.fsh")==false)
        return false;

    if((attribute_coord3d = shader.getAttribute("coord3d"))==-1)
        return false;

    if((uniform_mvp = shader.getUniform("mvp"))==-1)
        return false;

    if((uniform_texture = shader.getUniform("texture"))==-1)
        return false;
    return true;
}

bool load(const char * top,const char * bottom,
          const char * front,const char * back,
          const char * right, const char * left)
{
    return false;
}

void SkyBox::draw(const Matrix4f &mvp)
{
    shader.bind();
    shader.uniformMatrix(uniform_mvp,mvp);

    Texture::active(GL_TEXTURE0);
    texture.bind();
    shader.uniform(uniform_texture,0);

    glEnableVertexAttribArray(attribute_coord3d);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(attribute_coord3d);

    shader.unbind();
}
