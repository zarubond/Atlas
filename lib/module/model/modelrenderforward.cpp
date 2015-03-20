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

#include "modelrenderforward.h"

ModelRenderForward::ModelRenderForward()
{
    attribute_coord=0;
    attribute_normal=0;
    attribute_texcoord=0;
}

bool ModelRenderForward::init()
{
    const char * vertex_file=":lib/shader/ModelForward.vsh";
    const char * fragment_file=":lib/shader/ModelForward.fsh";

     if(!program.loadFile(vertex_file,fragment_file))
        return false;

     if((uniform_texture = program.getUniform("diffuse_texture"))==-1)
        return false;
     if((uniform_selected = program.getUniform("selected"))==-1)
        return false;
     if((uniform_diffuse = program.getUniform("diffuse"))==-1)
         return false;
     if((uniform_sun_dir = program.getUniform("sun_dir"))==-1)
        return false;
     if((uniform_ambient = program.getUniform("ambient"))==-1)
         return false;

     if((attribute_coord = program.getAttribute("coord3d"))==-1)
         return false;
     if((attribute_normal = program.getAttribute("normal"))==-1)
            return false;
     if((attribute_texcoord = program.getAttribute("texcoord"))==-1)
         return false;


    //geometry
     if((uniform_mvp = program.getUniform("mvp"))==-1)
         return false;
     if((uniform_mv = program.getUniform("mv"))==-1)
         return false;

     program.bind();
     Texture::active(Texture::TEXTURE0);
     program.uniform(uniform_texture,0);
     program.release();

     return true;
}

void ModelRenderForward::render(WorldGraphics *world, const Environment *envi, const Matrix4f &mvp, unsigned int)
{
    if(!world->model.isVisible()) return;

    Matrix4f mat=mvp;
    Matrix4f nmat;
    Matrix3f mv;

    Model * tmp;

    program.bind();
    Texture::active(Texture::TEXTURE0);
    program.uniform(uniform_diffuse, envi->sun.getDiffuse());
    program.uniform(uniform_sun_dir, envi->sun.getDirection());
    program.uniform(uniform_ambient, envi->ambient.getColor());

    int count;
    Model ** list=world->model.renderList(count);
    for(int i=0;i<count;i++)
    {
        tmp=list[i];
        mat=tmp->getMatrix()*mvp;

        nmat=tmp->getMatrix();
        nmat.inverse();

        mv[0]=nmat[0];
        mv[1]=nmat[1];
        mv[2]=nmat[2];

        mv[3]=nmat[4];
        mv[4]=nmat[5];
        mv[5]=nmat[6];

        mv[6]=nmat[8];
        mv[7]=nmat[9];
        mv[8]=nmat[10];
        mv.transpose();

        if(tmp==world->model.hightlighted())
            program.uniform(uniform_selected,true);
        else
            program.uniform(uniform_selected,false);

        program.uniformMatrix(uniform_mv,mv);
        program.uniformMatrix(uniform_mvp,mat);

        renderMesh(tmp->getMesh());
    }/*
    while(world->model.renderCount()>0)
    {
        tmp=world->model.popRender();
        mat=tmp->getMatrix()*mvp;

        nmat=tmp->getMatrix();
        nmat.inverse();

        mv[0]=nmat[0];
        mv[1]=nmat[1];
        mv[2]=nmat[2];

        mv[3]=nmat[4];
        mv[4]=nmat[5];
        mv[5]=nmat[6];

        mv[6]=nmat[8];
        mv[7]=nmat[9];
        mv[8]=nmat[10];
        mv.transpose();

        if(tmp==world->model.hightlighted())
            program.uniform(uniform_selected,true);
        else
            program.uniform(uniform_selected,false);

        program.uniformMatrix(uniform_mv,mv);
        program.uniformMatrix(uniform_mvp,mat);

        renderMesh(tmp->getMesh());
    }*/
    program.release();
}

void ModelRenderForward::renderMesh(ModelMesh * mesh)
{
    if(!mesh->vao.isCreated())
        setupRender(mesh);

    mesh->vao.bind();
    ModelMeshMaterial * material;
    for (std::vector<MeshPart*>::const_iterator it = mesh->parts.begin() ; it != mesh->parts.end(); ++it)
    {
        if((*it)->index_count>0)
        {
            material=(ModelMeshMaterial*)(*it)->material;
            material->texture.bind();
            mesh->ibo.draw(IndexBuffer::TRIANGLES, (*it)->index_count, IndexBuffer::UNSIGNED_SHORT, (*it)->start_index*sizeof(GLushort));
        }
    }
    mesh->vao.release();
}

void ModelRenderForward::setupRender(ModelMesh * mesh)
{
    mesh->vao.create();
    mesh->vao.bind();
    mesh->ibo.bind();
    mesh->vbo_vertices.bind();
    program.enableAttribute(attribute_coord);
    program.setAttribute(attribute_coord,3,GL_FLOAT);

    mesh->vbo_normals.bind();
    program.enableAttribute(attribute_normal);
    program.setAttribute(attribute_normal, 3, GL_FLOAT);

    mesh->vbo_tex.bind();
    program.enableAttribute(attribute_texcoord);
    program.setAttribute(attribute_texcoord, 2, GL_FLOAT);

    mesh->vao.release();
}
