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

#ifndef MESH_H
#define MESH_H

#include "../math/math.h"

#include "mesh3ds.h"
#include "meshobj.h"
#include "meshfbx.h"

#include <vector>


/**
 * @brief The Material struct handles material od mesh part
 */
class MeshMaterial {
public:
    virtual ~MeshMaterial(){}
    virtual void setTexture(const char *){}
    virtual void setBumpMap(const char *){}

    std::string name;
    int start_index,index_count;
    Vector3f diffuse;
    Vector3f ambient;
    Vector3f specular;
};

/**
 * @brief The MeshPart struct handles parts of mesh.
 */
struct MeshPart{
    MeshMaterial * material;
    int start_index,index_count;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

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
    void addMaterial(MeshMaterial * material);

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

    bool collision(const Matrix4f & matrix, const Vector3f &a, Vector3f &b, Vector3f &normal);

    unsigned int indexCount() const;
    float *vertexData() const;
    unsigned short *indexData() const;

    friend class MeshObj;
    friend class Mesh3ds;
    friend class MeshFBX;
protected:

    virtual void setIndices(unsigned short *indices, unsigned int count);
    virtual void setVertices(float *vertices, unsigned int);
    virtual void setNormals(float *normals, unsigned int);
    virtual void setTexturesUV(float *uvs, unsigned int);

    virtual MeshMaterial *createMaterial();
    virtual MeshPart *createPart(MeshMaterial *material, int start_index, int index_count);

    /**
     * @brief file Mesh file
     */
    std::string file;

    float * vertex_data;
    unsigned short * index_data;
    unsigned int index_count;

    std::vector<MeshPart *> parts;
    std::vector<MeshMaterial *> materials;

    float width,height,depth;
};

#endif // MESH_H
