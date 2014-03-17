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
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "./matrix4.h"
#include "./opengl.h"
#include "./shader.h"
#include "./texture.h"

class ObjLoader
{
public:
    ObjLoader();
    ~ObjLoader();
    bool load(const char * file_name);
    void setScale(float scale);
    void setPosition(float x, float y, float z);
    void render(const Matrix4f &mvp);
private:
    void loadMTL(const std::string &file_name, const std::string &folder);
    void loadObject();
    bool loadShader();

    GLuint vbo_vertices,vbo_normals,vbo_tex,ibo_elements;
    Shader shader;
    GLint attribute_v_coord,attribute_normal,attribute_texcoord,
        uniform_scale,uniform_pos,uniform_mvp,uniform_texture,uniform_diffuse;
    std::string name;
    Texture texture;

    bool tex;

    struct Material{
        Texture texture;
        Texture bump;
        std::string name;
        Material * next;
        int start_index,index_count;
        float r,g,b;
    };

    struct Mesh{
        Material * material;
        int start_index,index_count;
        Mesh * next;
    };

    Mesh * meshes;
    Material * materials;
    float scale;
    float pos_x,pos_y,pos_z;
    int material_count;
};

#endif // OBJLOADER_H
