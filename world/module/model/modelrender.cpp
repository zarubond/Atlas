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
#include "modelrender.h"

ModelRender::ModelRender()
{
}

bool ModelRender::init()
{
    const char * vertex_file=":/shader/Model.vsh";
    const char * fragment_file=":/shader/Model.fsh";

     if(!program.loadFile(vertex_file,fragment_file))
        return false;

     if((uniform_texture = program.getUniform("texture"))==-1)
        return false;
     if((uniform_selected = program.getUniform("selected"))==-1)
        return false;

     if((attribute_v_coord = program.getAttribute("coord3d"))==-1)
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

     if((uniform_model=program.getUniform("model"))==-1)
         return false;

     return true;
}

void ModelRender::load(ModelGraphics *base)
{
    this->base=base;
}

void ModelRender::clearRenderList()
{
    base->clearRender();
}

void ModelRender::render(const Matrix4f &mvp, unsigned int)
{
    program.bind();
    Matrix4f mat=mvp;
    Matrix4f nmat;
    Matrix3f mv;

    glEnableVertexAttribArray(attribute_v_coord);
    glEnableVertexAttribArray(attribute_normal);
    glEnableVertexAttribArray(attribute_texcoord);

    Model * tmp;
    while(base->renderCount()>0)
    {
        tmp=base->popRender();
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

        program.uniform(uniform_selected,tmp->isSelected());
        program.uniformMatrix(uniform_mvp,mat);
        program.uniformMatrix(uniform_mv,mv);

        renderModel(tmp);
        tmp=tmp->next;
    }

    glDisableVertexAttribArray(attribute_v_coord);
    glDisableVertexAttribArray(attribute_normal);
    glDisableVertexAttribArray(attribute_texcoord);

    program.unbind();
}

void ModelRender::renderShadow(const Matrix4f &mvp, unsigned int)
{
    program.bind();
    Matrix4f mat=mvp;

    glEnableVertexAttribArray(attribute_v_coord);
    glEnableVertexAttribArray(attribute_normal);
    glEnableVertexAttribArray(attribute_texcoord);

    Model * tmp;
    while(base->renderCount()>0)
    {
        tmp=base->popRender();
        mat=tmp->getMatrix()*mvp;

        program.uniformMatrix(uniform_mvp,mat);
        renderModel(tmp);
        tmp=tmp->next;
    }

    glDisableVertexAttribArray(attribute_v_coord);
    glDisableVertexAttribArray(attribute_normal);
    glDisableVertexAttribArray(attribute_texcoord);

    program.unbind();
}

void ModelRender::renderModel(Model * model)
{
    program.uniformMatrix(uniform_model,model->getMatrix());
    Texture::active(GL_TEXTURE0);
    program.uniform(uniform_texture,0);

    const Mesh * mesh=model->getMesh();
    mesh->render(attribute_v_coord,attribute_normal,attribute_texcoord);
}

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
    while(base->renderCount()>0 && cnt<255)
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
