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
#include "modelbase.h"

ModelBase::ModelBase(int type): ModuleBase(type)
{
}

ModelBase::~ModelBase()
{
    for(std::set<ModelMesh *>::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
    {
        ModelMesh* mesh=(*it);
        delete mesh;
    }
}

const std::set<ModelMesh *> &ModelBase::getMeshes() const
{
    return meshes;
}

ModelMesh *ModelBase::newMesh(const char * file_base,const char *file_name)
{
    ModelMesh * mesh= new ModelMesh;

    std::string file=file_base;
    if(file[file.size()-1]=='/')
        file.erase(file.size()-1);
    if(file_name[0]!='/')
        file+="/";

    file+=file_name;
    if(mesh->load(file.c_str()))
    {
        meshes.insert(mesh);
        return mesh;
    }

    delete mesh;
    return NULL;
}

bool ModelBase::removeMesh(ModelMesh *mesh)
{
    return meshes.erase(mesh);
}
