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

#include "mesh.h"

Mesh::Mesh()
{
    width=0.0;
    height=0.0;
    depth=0.0;

    vertex_data=NULL;
    index_data=NULL;
    index_count=0;
}

Mesh::~Mesh()
{
    for (std::vector<MeshMaterial*>::iterator it = materials.begin() ; it != materials.end(); ++it)
    {
        delete *it;
    }

    for (std::vector<MeshPart*>::iterator it = parts.begin() ; it != parts.end(); ++it)
    {
        delete *it;
    }

    delete [] vertex_data;
    delete [] index_data;
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

bool Mesh::collision(const Matrix4f &matrix, const Vector3f &a, Vector3f &b,Vector3f&normal)
{
    Triangle triangle;
    Vector4f t1,t2,t3;
    Vector3f p1,p2,p3;
    Vector3f len,size=a-b;
    float s=size.length();
    bool hit=false;

    for(unsigned int i=0;i < index_count;i+=3)
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

        Vector3f tmp=triangle.collision(a,b);
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

unsigned int Mesh::indexCount() const
{
    return index_count;
}

float *Mesh::vertexData() const
{
    return vertex_data;
}

unsigned short *Mesh::indexData() const
{
    return index_data;
}

void Mesh::setIndices(unsigned short *indices, unsigned int count)
{
    this->index_data=indices;
    this->index_count=count;
}

void Mesh::setVertices(float * vertices, unsigned int)
{
    this->vertex_data=vertices;
}

void Mesh::setNormals(float *normals, unsigned int)
{
    delete [] normals;
}

void Mesh::setTexturesUV(float *uvs, unsigned int)
{
    delete [] uvs;
}

MeshMaterial *Mesh::createMaterial()
{
    MeshMaterial * material=new MeshMaterial;
    materials.push_back(material);
    return material;
}

MeshPart *Mesh::createPart(MeshMaterial * material, int start_index, int index_count)
{
    MeshPart * part=new MeshPart;
    part->start_index=start_index;
    part->index_count=index_count;
    part->material=material;
    parts.push_back(part);
    return part;
}

bool Mesh::load(const char *file_name)
{
    std::string name=file_name;
    size_t i=name.rfind('.');
    if(i==std::string::npos)
    {
        cerr<<"Unable to determin mesh file format: "<<file_name<<endl;
        return false;
    }
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
        cerr<<"Model format not supported! "<<name<<endl;
        return false;
    }
    return true;
}

std::vector<std::string> Mesh::meshFiles(const char * file_name)
{
    std::string name=file_name;
    size_t i=name.rfind('.');
    if(i==std::string::npos)
    {
        cerr<<"Unable to determine mesh file format: "<<file_name<<endl;
        return std::vector<std::string>();
    }
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
        cerr<<"Model format not supported! "<<name<<endl;
    }

    return std::vector<std::string>();
}
