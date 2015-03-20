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

#ifndef MODELMESH_H
#define MODELMESH_H

#include "../../tools/mesh.h"
#include "../../graphics/graphics.h"

#include <set>

class ModelMeshMaterial: public MeshMaterial
{
public:
    virtual void setTexture(const char * path)
    {
        texture.loadFile(path,GL_LINEAR_MIPMAP_NEAREST,GL_REPEAT);
    }

    virtual void setBumpMap(const char *) {}

    Texture texture;
    Texture bump;
};

class Model;

class ModelMesh : public Mesh
{
public:
    ModelMesh();
    virtual ~ModelMesh();
    const std::string & getName() const;
    void setName(const std::string &value);
    void appendModel(Model * model);
    void removeModel(Model * model);

    const std::set<Model *> &getModels() const;
    float default_scale;
    void render(Attribute attribute_v_coord, Attribute attribute_normal, Attribute attribute_texcoord);
    void setupRender(Attribute attribute_v_coord, Attribute attribute_normal, Attribute attribute_texcoord);

    friend class ModelRenderForward;
    friend class ModelRender;
protected:
    void setIndices(GLushort *indices, GLuint count);
    void setNormals(GLfloat *normals, GLuint count);
    void setTexturesUV(GLfloat *uvs, GLuint count);
    void setVertices(GLfloat *vertices, GLuint count);

    MeshMaterial *createMaterial();
private:
    std::set<Model *> models;
    std::string name;

    // rendering data
    VertexBuffer vbo_vertices,vbo_normals,vbo_tex;
    IndexBuffer ibo;
    VertexArray vao, shadow_vao;
};

#endif // MODELMESH_H
