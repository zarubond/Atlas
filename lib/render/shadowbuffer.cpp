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

#include "shadowbuffer.h"

ShadowBuffer::ShadowBuffer()
{
    m_width=256;
    m_height=256;
}

bool ShadowBuffer::init()
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);

    // glDrawBuffer(GL_NONE); NOT IN ES 3

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
            cerr<<"Shadow buffer: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"<<endl;
            ok=false;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            cerr<<"Shadow buffer: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"<<endl;
            ok=false;
            break;
        default:
            cerr<<"Shadow buffer: glCheckFramebufferStatus: error "<<status<<endl;
            ok=false;
            break;
        }

    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return ok;
}

void ShadowBuffer::resize(int width, int height)
{
    m_width=std::max(width,4096);
    m_height=std::max(height,4096);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, 0);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void ShadowBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void ShadowBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowBuffer::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

int ShadowBuffer::width() const
{
    return m_width;
}

int ShadowBuffer::height() const
{
    return m_height;
}
