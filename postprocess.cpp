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
#include "postprocess.h"


PostProcess::PostProcess()
{
    screen_height=600;
    screen_width=800;
    scale=1.0;
    shadow_scale=2.0;
    light_num=0;
    enable_shadow=false;
}

bool PostProcess::init()
{
#ifndef OPENGL_ES
    // frame buffer shadow
    glGenFramebuffers(1, &fbo_shadow);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_shadow);

    glGenTextures(1, &tex_shadow);
    glBindTexture(GL_TEXTURE_2D, tex_shadow);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, screen_width, screen_height
                 , 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   // glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           tex_shadow, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // frame buffer rendering
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenRenderbuffers(1, &rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, screen_width, screen_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);


    glGenTextures(1, &tex_color);
    glBindTexture(GL_TEXTURE_2D, tex_color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width,
                 screen_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &fbo_normal);
    glBindTexture(GL_TEXTURE_2D, fbo_normal);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen_width,
                 screen_height, 0, GL_RGBA, GL_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &tex_position);
    glBindTexture(GL_TEXTURE_2D, tex_position);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, screen_width,
                 screen_height, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);


    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, tex_color, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D, fbo_normal, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D, tex_position, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, buffers);

    GLenum status;
    //nvidia problem
    if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
        switch(status)
        {
        //case GL_FRAMEBUFFER_UNDEFINED:
          //  std::cerr<<"GL_FRAMEBUFFER_UNDEFINED"<<std::endl;
            //break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cerr<<"GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"<<std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cerr<<"GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"<<std::endl;
            break;
        default:
            std::cerr<<"glCheckFramebufferStatus: error "<<status<<std::endl;
            break;
        }

    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

////////////////////////////////////////////////////

    static const GLfloat g_quad_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
    };
/*
    glGenBuffers(1, &vbo_fbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_fbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    */

    canvas_buffer.create(sizeof(g_quad_vertex_buffer_data),g_quad_vertex_buffer_data);

    shader.loadFile(":/shader/PostProcessing.vsh",":/shader/PostProcessing.fsh");
    shader.bind();
    attribute_v_coord_postproc = shader.getAttribute("v_coord");//return false;
    uniform_fbo_texture     = shader.getUniform("fbo_texture");
    uniform_fbo_normal      = shader.getUniform("fbo_normal");
    uniform_tex_position    = shader.getUniform("tex_position");
    uniform_tex_shadow      = shader.getUniform("tex_shadow");
    uniform_depthmvp        = shader.getUniform("depthmvp");
    uniform_scale           = shader.getUniform("scale");
    uniform_shadow          = shader.getUniform("enable_shadow");

    uniform_fog_density     = shader.getUniform("fog_density");
    uniform_fog_color       = shader.getUniform("fog_color");

    uniform_cam_pos = shader.getUniform("cam_pos");
  /*  //fog
    if((uniform_fog_density = shader.getUniform("fog.density"))==-1)
        return false;
    if((uniform_fog_color = shader.getUniform("fog.color"))==-1)
        return false;
*/

    //lights
    uniform_num_lights = shader.getUniform("num_lights");
    uniform_ambient = shader.getUniform("ambient_light");

    for(int i=0;i<1;i++)
    {
        shader.getUniformLight(uniform_light[i],i);
    }

    shader.uniform(uniform_fbo_texture,0);
    shader.uniform(uniform_tex_position,1);
    shader.uniform(uniform_fbo_normal,2);
    shader.uniform(uniform_tex_shadow,3);

    Vertex3f nlight(0.4,0.4,0.4);
    shader.uniform(uniform_ambient,nlight);


    //light
    shader.uniform(uniform_num_lights,light_num);
    shader.uniform(uniform_scale,scale);

    //glUniform1f(uniform_fog_density,0.006);
    glUniform1f(uniform_fog_density,0.0);
    shader.uniform(uniform_fog_color,Vertex3f(0.8,0.8,0.8));

    shader.unbind();
#endif


    biasMatrix.set(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );

    return true;
}

void PostProcess::resize(unsigned short width, unsigned short height)
{

#ifndef OPENGL_ES
   glBindTexture(GL_TEXTURE_2D, tex_color);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
                height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

   glBindTexture(GL_TEXTURE_2D, fbo_normal);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, width,//GL_RGBA16F_ARB
    //            height, 0, GL_RGBA, GL_FLOAT, 0);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,//GL_RGBA16F_ARB
                height, 0, GL_RGBA, GL_BYTE, 0);

   glBindTexture(GL_TEXTURE_2D, tex_position);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F_ARB, width,
                height, 0, GL_RGBA, GL_FLOAT, 0);

   glBindTexture(GL_TEXTURE_2D, tex_shadow);
   glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, width*shadow_scale, height*shadow_scale
                , 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);

   glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
   glBindRenderbuffer(GL_RENDERBUFFER, 0);

   glBindFramebuffer(GL_FRAMEBUFFER, fbo);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glBindFramebuffer(GL_FRAMEBUFFER,0);

   glBindFramebuffer(GL_FRAMEBUFFER, fbo_shadow);
   glClear(GL_DEPTH_BUFFER_BIT);
   glBindFramebuffer(GL_FRAMEBUFFER,0);
#endif

   screen_height=height;
   screen_width=width;

}

void PostProcess::shadow()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_shadow);
    glClear(GL_DEPTH_BUFFER_BIT);// pointless and add +2ms
#ifndef OPENGL_ES
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
#endif
    glViewport(0,0,screen_width*shadow_scale,screen_height*shadow_scale);
    //glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);

    enable_shadow=true;
}

void PostProcess::process(int tick)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    scale-=((tick-34.0f)/34.0f)*scale*0.2;
    if(scale<0.4)
        scale=0.4;
    if(scale>2)
        scale=2;
    scale=1.0;

    //future http://www.gamerendering.com/category/lighting/
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
   // glClearDepth(1.0f);
     //   glDepthFunc(GL_LESS);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// pointless and add +2ms

   // glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(0,0,screen_width*scale,screen_height*scale);

#ifndef OPENGL_ES
    GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, buffers);
#else
    //GLenum buffers[] = { GL_COLOR_ATTACHMENT0_NV, GL_COLOR_ATTACHMENT1_NV, GL_COLOR_ATTACHMENT2_NV};
    //glDrawBuffersNV(3, buffers);
#endif

}


void PostProcess::render(const LightManager &light, const Vertex3f &cam_pos, const Matrix4f &depthMVP)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0,screen_width,screen_height);

    shader.bind();

    Texture::active(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_color);

    Texture::active(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex_position);

    Texture::active(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, fbo_normal);

    Texture::active(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, tex_shadow);


    Matrix4f bias = depthMVP*biasMatrix;

    shader.uniform(uniform_cam_pos, cam_pos);
    shader.uniformMatrix(uniform_depthmvp,bias);
    shader.uniform(uniform_shadow,enable_shadow);
    enable_shadow=false;

    //light
    light_num=1;
    shader.uniform(uniform_num_lights,light_num);

    //for(int i=0;i<light_num;i++)
    //{
        shader.uniformLight(uniform_light[0],light.sun);
    //}

    shader.uniform(uniform_ambient,light.getAmbient());


    glEnableVertexAttribArray(attribute_v_coord_postproc);

    canvas_buffer.bind();
    glVertexAttribPointer(attribute_v_coord_postproc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    canvas_buffer.draw(6);
    canvas_buffer.unbind();
    Texture::active(GL_TEXTURE0);
}
