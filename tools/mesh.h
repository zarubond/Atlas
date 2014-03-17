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
#ifndef MESH_H
#define MESH_H

#include "./math/math.h"
#include "./opengl.h"

#include "mesh3ds.h"
#include "meshobj.h"
#include "meshfbx.h"

#include <QImage>


class Mesh
{
public:
    Mesh();
    ~Mesh();

    /**
     * @brief The Material struct handles material od mesh part
     */
    struct Material{
        Texture texture;
        Texture bump;
        std::string name;
        int start_index,index_count;
        Vertex3f diffuse;
        Vertex3f ambient;
        Vertex3f specular;

        Material * next;
    };

    /**
     * @brief The MeshPart struct handles parts of mesh.
     */
    struct Part{
        Material * material;
        int start_index,index_count;
        Part * next;
    };
    /**
     * @brief operator = Compare two matrices.
     * @param second
     * @return
     */
    Mesh& operator=(Mesh & second);
    /**
     * @brief addMaterial Add new model material.
     * @param material
     */
    void addMaterial(Mesh::Material * material);
    /**
     * @brief addPart Add part to the mesh
     * @param part
     */
    void addPart(Mesh::Part * part);
    /**
     * @brief getParts Get linked list of mesh parts
     * @return
     */
    Part * getParts() const;
    Part *getPart(int id) const;
    int partCount() const;
    /**
     * @brief getMaterials Get linked list of mesh matrerials.
     * @return
     */
    Material *getMaterials() const;
    Material *getMaterial(int id) const;
    int materialCount() const;

    /**
     * @brief setData Set mesh collision triangle data
     * @param vertex_data
     * @param index_data
     * @param index_count
     */
    void setData(GLfloat * vertex_data, GLuint *index_data, GLuint index_count);
    /**
     * @brief getFile Get mesh file path.
     * @return
     */
    const char* getFile() const;
    /**
     * @brief getWidth Get mesh width.
     * @return
     */
    float getWidth();
    /**
     * @brief getHeight Get mesh height.
     * @return
     */
    float getHeight();
    /**
     * @brief getDepth Get mesh depth.
     * @return
     */
    float getDepth();

    bool load(const char *file_name);

    static std::vector<std::string> meshFiles(const char *file_name);

    void render(GLint attribute_v_coord, GLint attribute_normal, GLint attribute_texcoord) const;

    bool collision(const Matrix4f & matrix, const Vertex3f &a, Vertex3f &b, Vertex3f &normal);

    GLuint indexCount();
    GLfloat *vertexData();
    GLuint *indexData();

    friend class MeshObj;
    friend class Mesh3ds;
    friend class MeshFBX;
protected:

    /**
     * @brief file Mesh file
     */
    std::string file;

    // rendering data
    VertexBuffer vbo_vertices,vbo_normals,vbo_tex;
    IndexBuffer ibo_elements;


    GLfloat * vertex_data;
    GLuint * index_data;
    GLuint index_count;

    Part * parts;
    Material * materials;
    int material_count;

    float width,height,depth;
};

#endif // MESH_H
