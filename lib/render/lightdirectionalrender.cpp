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

#include "lightdirectionalrender.h"

LightDirectionalRender::LightDirectionalRender()
{
}

bool LightDirectionalRender::init()
{
    const char * directional_vsh=":lib/shader/LightDirectional.vsh";
    const char * directional_fsh=":lib/shader/LightDirectional.fsh";

     if(!directional.loadFile(directional_vsh,directional_fsh))
        return false;

     if((uniform_direction = directional.getUniform("direction"))==-1)
        return false;
     if((uniform_diffuse = directional.getUniform("diffuse"))==-1)
        return false;
     if((uniform_camera = directional.getUniform("camera"))==-1)
         return false;
     if((uniform_specular = directional.getUniform("specular"))==-1)
        return false;
     if((uniform_shininess = directional.getUniform("shininess"))==-1)
        return false;
     if((uniform_shadowenable = directional.getUniform("shadowEnable"))==-1)
         return false;
     if((uniform_shadowmvp = directional.getUniform("shadowMVP"))==-1)
         return false;
     if((attribute_coord = directional.getAttribute("coord"))==-1)
         return false;

     uniform_tex_color    = directional.getUniform("tex_color");
     uniform_tex_normal   = directional.getUniform("tex_normal");
     uniform_tex_position = directional.getUniform("tex_position");
     uniform_tex_shadow   = directional.getUniform("tex_shadow");

     directional.bind();
     directional.uniform(uniform_tex_color,0);
     directional.uniform(uniform_tex_normal,1);
     directional.uniform(uniform_tex_position,2);
     directional.uniform(uniform_tex_shadow,3);
     directional.release();

     //OTHER

     static const GLfloat vertex_buffer_data[] = {
         -1.0f, -1.0f, 0.0f,
          1.0f, -1.0f, 0.0f,
         -1.0f,  1.0f, 0.0f,
         -1.0f,  1.0f, 0.0f,
          1.0f, -1.0f, 0.0f,
          1.0f,  1.0f, 0.0f,
     };

     canvas_buffer.create(sizeof(vertex_buffer_data), vertex_buffer_data);

     biasMatrix.set(
         0.5, 0.0, 0.0, 0.0,
         0.0, 0.5, 0.0, 0.0,
         0.0, 0.0, 0.5, 0.0,
         0.5, 0.5, 0.5, 1.0
     );

     vao.create();
     vao.bind();
     directional.enableAttribute(attribute_coord);
     canvas_buffer.bind();
     directional.setAttribute(attribute_coord, 3, GL_FLOAT);
     vao.release();

     return true;
}

void LightDirectionalRender::render(const LightDirectional *light, const Camera &camera, GBuffer * buffer)
{
    directional.bind();

    Texture::active(Texture::TEXTURE0);
    buffer->bindTextureColor();

    Texture::active(Texture::TEXTURE1);
    buffer->bindTextureNormal();

    Texture::active(Texture::TEXTURE2);
    buffer->bindTexturePosition();

    directional.uniform(uniform_direction, light->getDirection());
    directional.uniform(uniform_diffuse  , light->getDiffuse());
    directional.uniform(uniform_specular , light->getSpecular());
    directional.uniform(uniform_shininess, light->getShininess());
    directional.uniform(uniform_camera   , camera.position());
    directional.uniform(uniform_shadowenable,false);

    vao.bind();
    canvas_buffer.draw(6);
    vao.release();
    Texture::active(Texture::TEXTURE0);

    directional.release();
}

void LightDirectionalRender::render(const LightDirectional *light, const Camera &camera, GBuffer *buffer, ShadowBuffer *shadow, const Matrix4f & shadowMVP)
{
    directional.bind();

    Texture::active(Texture::TEXTURE0);
    buffer->bindTextureColor();

    Texture::active(Texture::TEXTURE1);
    buffer->bindTextureNormal();

    Texture::active(Texture::TEXTURE2);
    buffer->bindTexturePosition();

    Texture::active(Texture::TEXTURE3);
    shadow->bindTexture();

    Matrix4f bias = shadowMVP*biasMatrix;

    directional.uniformMatrix(uniform_shadowmvp,bias);
    directional.uniform(uniform_shadowenable,true);

    directional.uniform(uniform_direction, light->getDirection());
    directional.uniform(uniform_diffuse  , light->getDiffuse());
    directional.uniform(uniform_specular , light->getSpecular());
    directional.uniform(uniform_shininess, light->getShininess());
    directional.uniform(uniform_camera   , camera.position());

    vao.bind();
    canvas_buffer.draw(6);
    vao.release();

    directional.release();
    Texture::active(Texture::TEXTURE0);
}
