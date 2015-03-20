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

#include "vrframebuffer.h"

VrFrameBuffer::VrFrameBuffer()
{
    m_width=256;
    m_height=256;
    fbo=0;
    texture=0;
}

bool VrFrameBuffer::init()
{
    // frame buffer rendering
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, isDepth24()?GL_DEPTH_COMPONENT24:GL_DEPTH_COMPONENT16, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    GLenum buffers[] = { GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, buffers);

    bool ok=true;
    GLenum status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //nvidia problem
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        switch(status)
        {
        //case GL_FRAMEBUFFER_UNDEFINED:
          //  std::cerr<<"GL_FRAMEBUFFER_UNDEFINED"<<std::endl;
            //break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            qDebug()<<"Distortion buffer: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
            ok=false;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            qDebug()<<"Distortion buffer: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
            ok=false;
            break;
        default:
            qDebug()<<"Distortion buffer: glCheckFramebufferStatus: error "<<status;
            ok=false;
            break;
        }

    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return ok;
}

void VrFrameBuffer::resize(int width, int height)
{
    m_width=width;
    m_height=height;
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, isDepth24()?GL_DEPTH_COMPONENT24:GL_DEPTH_COMPONENT16, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void VrFrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void VrFrameBuffer::release()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VrFrameBuffer::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}
