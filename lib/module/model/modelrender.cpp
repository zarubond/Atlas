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

#include "modelrender.h"

ModelRender::ModelRender()
{
    attribute_coord=0;
    attribute_normal=0;
    attribute_texcoord=0;
}

bool ModelRender::init()
{
    const char * vertex_file=":lib/shader/Model.vsh";
    const char * fragment_file=":lib/shader/Model.fsh";

     if(!program.loadFile(vertex_file,fragment_file))
        return false;

     if((uniform_texture = program.getUniform("diffuse"))==-1)
        return false;
     if((uniform_selected = program.getUniform("selected"))==-1)
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
     if((uniform_model = program.getUniform("model"))==-1)
         return false;

     const char * shadow_vertex_file=":lib/shader/Shadow.vsh";
     const char * shadow_fragment_file=":lib/shader/Shadow.fsh";
     if(!program_shadow.loadFile(shadow_vertex_file,shadow_fragment_file))
        return false;

     if((uniform_shadow_mvp = program_shadow.getUniform("mvp"))==-1)
         return false;
     if((attribute_shadow_coord = program_shadow.getAttribute("coord3d"))==-1)
         return false;

     program.bind();
     Texture::active(Texture::TEXTURE0);
     program.uniform(uniform_texture,0);
     program.release();

     return true;
}

void ModelRender::render(WorldGraphics *world, const Environment *envi, const Matrix4f &mvp, unsigned int)
{
    if(!world->model.isVisible()) return;

    program.bind();
    Matrix4f mat=mvp;
    Matrix4f nmat;
    Matrix3f mv;

    Model * tmp;

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
        program.uniformMatrix(uniform_model,tmp->getMatrix());

        ModelMesh * mesh=tmp->getMesh();
        render(mesh);
    }

    program.release();
}

void ModelRender::renderShadow(WorldGraphics * world, const Environment *, const Matrix4f &mvp, unsigned int)
{
    if(!world->model.isVisible()) return;

    program_shadow.bind();
    Matrix4f mat=mvp;

    Model * tmp;
    int count;
    Model ** list=world->model.renderList(count);
    for(int i=0;i<count;i++)
    {
        tmp=list[i];
        mat=tmp->getMatrix()*mvp;

        program_shadow.uniformMatrix(uniform_shadow_mvp,mat);
        ModelMesh * mesh=tmp->getMesh();
        renderShadow(mesh);
    }
    program_shadow.release();
}

void ModelRender::setupRender(ModelMesh * mesh)
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

void ModelRender::setupShadowRender(ModelMesh * mesh)
{
    mesh->shadow_vao.create();
    mesh->shadow_vao.bind();

    mesh->ibo.bind();
    mesh->vbo_vertices.bind();
    program_shadow.enableAttribute(attribute_shadow_coord);
    program_shadow.setAttribute(attribute_shadow_coord,3,GL_FLOAT);

    mesh->shadow_vao.release();
}

void ModelRender::render(ModelMesh * mesh)
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
            mesh->ibo.draw(IndexBuffer::TRIANGLES, (*it)->index_count,
                                    IndexBuffer::UNSIGNED_SHORT, (*it)->start_index*sizeof(GLushort));
        }
    }
    mesh->vao.release();
}

void ModelRender::renderShadow(ModelMesh *mesh)
{
    if(!mesh->vao.isCreated())
        setupShadowRender(mesh);

    mesh->shadow_vao.bind();
    ModelMeshMaterial * material;
    for (std::vector<MeshPart*>::const_iterator it = mesh->parts.begin() ; it != mesh->parts.end(); ++it)
    {
        if((*it)->index_count>0)
        {
            material=(ModelMeshMaterial*)(*it)->material;
            material->texture.bind();
            mesh->ibo.draw(IndexBuffer::TRIANGLES, (*it)->index_count,
                                    IndexBuffer::UNSIGNED_SHORT, (*it)->start_index*sizeof(GLushort));
        }
    }
    mesh->shadow_vao.release();
}

/*
Model *ModelRender::selectModel(int x, int y,const Camera & camera)
{
    glClearStencil(255);

    glEnable(GL_STENCIL_TEST);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    program.bind();
    Matrix4f mat;

    glEnableVertexAttribArray(attribute_v_coord);
    glEnableVertexAttribArray(attribute_normal);
    glEnableVertexAttribArray(attribute_texcoord);

    Model * models[255];

    GLuint cnt=0;
    Model * tmp;
    while(base->renderCount()>0)
    {
        tmp=base->popRender();

        models[cnt]=tmp;
        mat=tmp->getMatrix()*camera.getLook();

        program.uniformMatrix(uniform_mvp,mat);

        glStencilFunc(GL_ALWAYS, cnt, -1);
        renderModel(tmp);
        tmp=tmp->next;
        cnt++;
    }

    glDisableVertexAttribArray(attribute_v_coord);
    glDisableVertexAttribArray(attribute_normal);
    glDisableVertexAttribArray(attribute_texcoord);

    program.unbind();
    //selecting
    GLuint index;
    glReadPixels(x, camera.height() - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

    glDisable(GL_STENCIL_TEST);

    base->clearRender();
    if(index < cnt)
    {
        return models[index];
    }
    return NULL;
}
*/
