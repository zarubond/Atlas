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
#include "mesh.h"

Mesh::Mesh()
{
    width=0.0;
    height=0.0;
    depth=0.0;

    materials=NULL;
    parts=NULL;

    material_count=0;
    vertex_data=NULL;
    index_data=NULL;
    index_count=0;
}

Mesh::~Mesh()
{
    Material * tmp_mat;
    while(materials!=NULL)
    {
        tmp_mat=materials->next;
        delete materials;
        materials=tmp_mat;
    }

    Part * tmp_mesh;
    while(parts!=NULL)
    {
        tmp_mesh=parts->next;
        delete parts;
        parts=tmp_mesh;
    }

    delete [] vertex_data;
    delete [] index_data;
}

void Mesh::addMaterial(Mesh::Material *material)
{
    material->next=materials;
    materials=material;
    material_count++;
}
void Mesh::addPart(Mesh::Part *part)
{
    part->next=parts;
    parts=part;
}

void Mesh::setData(GLfloat *vertex_data, GLuint *index_data, GLuint index_count)
{
    this->vertex_data=vertex_data;
    this->index_data=index_data;
    this->index_count=index_count;
}

const char *Mesh::getFile() const
{
    return file.c_str();
}

float Mesh::getWidth()
{
    return width;
}

float Mesh::getHeight()
{
    return height;
}

float Mesh::getDepth()
{
    return depth;
}

Mesh::Part *Mesh::getParts() const
{
    return parts;
}

Mesh::Part *Mesh::getPart(int id) const
{
    Part * part=parts;
    for(int i=0;i<id;i++)
        part=part->next;
    return part;
}

Mesh::Material *Mesh::getMaterials() const
{
    return materials;
}

Mesh::Material *Mesh::getMaterial(int id) const
{
    Material * mater=materials;
    for(int i=0;i<id;i++)
        mater=mater->next;
    return mater;
}

int Mesh::materialCount() const
{
    return material_count;
}

void Mesh::render(GLint attribute_v_coord,GLint attribute_normal,GLint attribute_texcoord) const
{
    vbo_vertices.bind();
    glVertexAttribPointer(attribute_v_coord,3, GL_FLOAT, GL_FALSE,0,0);

    vbo_normals.bind();
    glVertexAttribPointer(attribute_normal, 3,GL_FLOAT, GL_FALSE,0, 0);

    vbo_tex.bind();
    glVertexAttribPointer(attribute_texcoord,2,GL_FLOAT,GL_FALSE,0,0);

    Mesh::Part * mesh_part=parts;
    while(mesh_part!=NULL)
    {
        if(mesh_part->index_count>0)
        {
            mesh_part->material->texture.bind();
            ibo_elements.draw(GL_TRIANGLES,index_count,GL_UNSIGNED_INT,mesh_part->start_index*sizeof(GLuint));
        }

        mesh_part=mesh_part->next;
    }
    vbo_tex.unbind();
}

bool Mesh::collision(const Matrix4f &matrix, const Vertex3f &a, Vertex3f &b,Vertex3f&normal)
{
    Triangle triangle;
    Vertex4f t1,t2,t3;
    Vertex3f p1,p2,p3;
    Vertex3f len,size=a-b;
    float s=size.length();
    bool hit=false;

    for(GLuint i=0;i < index_count;i+=3)
    {
        t1.set(vertex_data[index_data[i]*3]
                ,vertex_data[index_data[i]*3+1]
                ,vertex_data[index_data[i]*3+2],1.0);
        t1=matrix*t1;

        t2.set(vertex_data[index_data[i+1]*3]
                ,vertex_data[index_data[i+1]*3+1]
                ,vertex_data[index_data[i+1]*3+2],1.0);
        t2=matrix*t2;

        t3.set(vertex_data[index_data[i+2]*3]
                ,vertex_data[index_data[i+2]*3+1]
                ,vertex_data[index_data[i+2]*3+2],1.0);
        t3=matrix*t3;

        p1.set(t1[0],t1[1],t1[2]);
        p2.set(t2[0],t2[1],t2[2]);
        p3.set(t3[0],t3[1],t3[2]);

        triangle.set(p1,p2,p3);

        Vertex3f tmp=triangle.collision(a,b);
        if(tmp!=b)
        {
            len=a-tmp;
            if(len.length()<s)
            {
                s=len.length();
                normal=triangle.normal();
                b=tmp;
                hit=true;
            }
        }
    }
    return hit;
}

GLuint Mesh::indexCount()
{
    return index_count;
}

GLfloat *Mesh::vertexData()
{
    return vertex_data;
}

GLuint *Mesh::indexData()
{
    return index_data;
}

bool Mesh::load(const char *file_name)
{
    std::string name=file_name;
    int i=name.rfind('.');
    std::string type=name.substr(i);
    if(type==".obj")
    {
        MeshObj loader;
        return loader.load(file_name,this);
    }
    else if(type==".3ds")
    {
        Mesh3ds loader;
        return loader.load(file_name,this);
    }
    else
    {
        std::cerr<<"Model format not supported! "<<name<<std::endl;
        return false;
    }
    return true;
}

std::vector<std::string> Mesh::meshFiles(const char * file_name)
{
    std::string name=file_name;
    int i=name.rfind('.');
    std::string type=name.substr(i);
    if(type==".obj")
    {
        MeshObj loader;
        return loader.meshFiles(file_name);
    }
    else if(type==".3ds")
    {
        Mesh3ds loader;
        return loader.meshFiles(file_name);
    }
    else
    {
        std::cerr<<"Model format not supported! "<<name<<std::endl;
    }

    return std::vector<std::string>();
}
