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

#include "modelmesh.h"

ModelMesh::ModelMesh()
{
    default_scale=1.0;
}

ModelMesh::~ModelMesh()
{

}

const std::string& ModelMesh::getName() const
{
    return name;
}

void ModelMesh::setName(const std::string &value)
{
    name = value;
}

void ModelMesh::appendModel(Model *model)
{
    models.insert(model);
}

void ModelMesh::removeModel(Model *model)
{
    models.erase(model);
}

const std::set<Model *> &ModelMesh::getModels() const
{
    return models;
}

void ModelMesh::setIndices(GLushort *indices, GLuint count)
{
    ibo.create(count*3*sizeof(GLushort),indices, GL_STATIC_DRAW);
    Mesh::setIndices(indices, count);
}

void ModelMesh::setNormals(GLfloat *normals, GLuint count)
{
    vbo_normals .create(count*3*sizeof(GLfloat),normals, GL_STATIC_DRAW);
    Mesh::setNormals(normals, count);
}

void ModelMesh::setTexturesUV(GLfloat *uvs, GLuint count)
{
    vbo_tex.create(count*2*sizeof(GLfloat),uvs, GL_STATIC_DRAW);
    Mesh::setTexturesUV(uvs, count);
}

void ModelMesh::setVertices(GLfloat *vertices, GLuint count)
{
    vbo_vertices.create(count*3*sizeof(GLfloat),vertices, GL_STATIC_DRAW);
    Mesh::setVertices(vertices, count);
}

MeshMaterial *ModelMesh::createMaterial()
{
    ModelMeshMaterial * material=new ModelMeshMaterial;
    materials.push_back(material);
    return material;
}
