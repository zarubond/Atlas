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

#include "model.h"
#include "modelgraphics.h"

Model::Model(ModelGraphics *graphics)
{
    mesh=NULL;
    this->graphics=graphics;
    matrix.loadIdentity();

    min_x=1000000;
    max_x=-1000000;
    min_y=1000000;
    max_y=-1000000;
    min_z=1000000;
    max_z=-1000000;
}

Model::Model(ModelGraphics *graphics, const Model *model)
{
    matrix=model->matrix;
    mesh=model->mesh;
    name=model->name;
    this->graphics=graphics;
}

Model::~Model()
{
    if(mesh!=NULL)
        mesh->removeModel(this);
}

void Model::setMesh(ModelMesh *mesh)
{
    this->mesh=mesh;
    mesh->appendModel(this);
    bbox();
}

ModelMesh *Model::getMesh()
{
    return mesh;
}

bool Model::inBox(int x, int y, int z, int w) const
{
    if(x<=min_x && x+w>=max_x && y<=min_y && y+w>=max_y && z<=min_z && z+w>=max_z)
    {
        return true;
    }
    return false;
}

void Model::collision(const Vector3f &a, Collision &b)
{
    Matrix4f m=matrix;
    m.transpose();
    if(mesh->collision(m,a,b.collision,b.normal))
        b.object=this;
}

void Model::setName(const char *name)
{
    this->name=name;
}

void Model::selectDraw(const Camera & camera)
{
    if(camera.boxInFrustum(box))
        graphics->pushRender(this);
}

void Model::selectDraw()
{
    graphics->pushRender(this);
}

const char *Model::getName() const
{
    return name.c_str();
}

void Model::bbox()
{
    matrix=makeMatrix();
    if(mesh!=NULL)
    {
        Vector4f p;
        Matrix4f m=matrix;
        m.transpose();
        min_x=1000000;
        max_x=-1000000;
        min_y=1000000;
        max_y=-1000000;
        min_z=1000000;
        max_z=-1000000;

        for(GLuint i=0;i < mesh->indexCount();i++)
        {
            p.set((mesh->vertexData())[(mesh->indexData())[i]*3]
                    ,mesh->vertexData()[mesh->indexData()[i]*3+1]
                    ,mesh->vertexData()[mesh->indexData()[i]*3+2],1.0);
            p=m*p;
            if(p[0]<min_x)  min_x=p[0];
            if(p[0]>max_x)  max_x=p[0];

            if(p[1]<min_y)  min_y=p[1];
            if(p[1]>max_y)  max_y=p[1];

            if(p[2]<min_z)  min_z=p[2];
            if(p[2]>max_z)  max_z=p[2];
        }
/*
        if(min_x<0)
        {
            m_position[0]-=min_x;
            min_x=0;
        }
        if(min_y<0)
        {
            m_position[1]-=min_y;
            min_y=0;
        }
        if(min_z<0)
        {
            m_position[2]-=min_z;
            min_z=0;
        }
*/
        box.setExtreme(min_x,min_y,min_z,max_x,max_y,max_z);
    }
}

void Model::setPosX(float x)
{
    m_position[0]=x;
    bbox();
}

void Model::setPosY(float y)
{
    m_position[1]=y;
    bbox();
}

void Model::setPosZ(float z)
{
    m_position[2]=z;
    bbox();
}

void Model::onUpdate()
{
    bbox();
}

const Matrix4f &Model::getMatrix() const
{
    return matrix;
}

int Model::type()
{
    return graphics->type();
}

Vector3f Model::center()
{
    return Vector3f((max_x-min_x)/2.0+min_x, (max_y-min_y)/2.0+min_y, (max_z-min_z)/2.0+min_z);
}

