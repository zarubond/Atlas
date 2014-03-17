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
#include "navigator.h"

Navigator::Navigator()
{
    distance=5.0;
    visible=false;
    mouse_x=0;
    mouse_y=0;
    lock_x=false;
    lock_y=false;
    lock_z=false;
}

bool Navigator::init()
{
    setlocale(LC_NUMERIC, "C");

    const char * vertex_src =
            "uniform mat4 mvp;\n"
            "uniform vec4 pos;\n"
            "uniform float scale;\n"
            "varying vec4 normal;\n"
            "varying vec4 coord;\n"
            "attribute vec3 coord3d;\n"
            "void main(void) {\n"
            "   coord=vec4(coord3d*scale, 1.0) + pos;\n"
            "   normal=pos-coord;\n"
            "   gl_Position = mvp * coord;\n"
            "}\n";


    const char * fragment_src =
            "varying vec4 normal;\n"
            "varying vec4 coord;\n"
            "vec4 encode (vec3 n)\n"
            "{\n"
            "    float p = sqrt(n.z*8.0+8.0);\n"
            "    return vec4(n.xy/p + 0.5,0.0,1.0);\n"
            "}\n"
            "void main(void) {\n"
            "   gl_FragData[0] = vec4(1.0,0.5,0.5, 1.0);\n"
            "   gl_FragData[1] = encode(normalize(normal.xyz));\n"
            "   gl_FragData[2] = coord;\n"
            "}\n";

    if(!program.load(vertex_src,fragment_src))
        return false;

    if((uniform_mvp = program.getUniform("mvp"))==-1)
        return false;

    if((uniform_pos = program.getUniform("pos"))==-1)
        return false;

    if((uniform_scale = program.getUniform("scale"))==-1)
        return false;

    if((attribute_coord3d = program.getAttribute("coord3d"))==-1)
        return false;

    // Restore system locale
    setlocale(LC_ALL, "");

    radius=1;
    CreateSphere ();

    form=SPHERE;
    mode=POINTER;

    lock_x=false;
    lock_y=false;
    lock_z=false;

    return true;
}

void Navigator::CreateSphere ()
{
    int n=0;
    GLfloat verts[3000];
    const int space = 20;

    for(float b = 0; b <= 180 - space; b+=space)
    {
        for(float a = 0; a <= 360 - space; a+=space)
        {
            verts[n++] = radius * sin((a) / 180 * M_PI) * sin((b) / 180 *M_PI);
            verts[n++] = radius * cos((a) / 180 * M_PI) * sin((b) / 180 *M_PI);
            verts[n++] = radius * cos((b) / 180 * M_PI);
            /*
            texcoord[t++] = (2 * b) / 360;
            texcoord[t++] = (a) / 360;*/

            verts[n++] = radius * sin((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI);
            verts[n++] = radius * cos((a) / 180 * M_PI) * sin((b + space) / 180 * M_PI);
            verts[n++] = radius * cos((b + space) / 180 * M_PI);
            /*
            texcoord[t++] = (2 * (b + space)) / 360;
            texcoord[t++] = (a) / 360;*/

            verts[n++] = radius * sin((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI);
            verts[n++] = radius * cos((a + space) / 180 * M_PI) * sin((b) / 180 * M_PI);
            verts[n++] = radius * cos((b) / 180 * M_PI);
            /*
            texcoord[t++] = (2 * b) / 360;
            texcoord[t++] = (a + space) / 360;*/

            verts[n++] = radius * sin((a + space) / 180 * M_PI) * sin((b + space) /180 * M_PI);
            verts[n++] = radius * cos((a + space) / 180 * M_PI) * sin((b + space) /180 * M_PI);
            verts[n++] = radius * cos((b + space) / 180 * M_PI);
            /*
            texcoord[t++] = (2 * (b + space)) / 360;
            texcoord[t++] = (a + space) / 360;*/
        }

    }

    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*n, verts, GL_STATIC_DRAW);
    triangle_num=n/3;

    //box shape
    GLfloat * cube_verts;
    Cube::verticesLines(cube_verts,cube_num);
    for(int i=0;i<cube_num;i++)
        cube_verts[i]*=2;

    glGenBuffers(1, &vbo_cube);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*cube_num, cube_verts, GL_STATIC_DRAW);

    delete [] cube_verts;

}

void Navigator::setPosition(float x, float y, float z)
{
    if(form==BOX)
    {
        if(!lock_x)
            pos_x=(int)x;
        if(!lock_x)
            pos_y=(int)y;
        if(!lock_x)
            pos_z=(int)z;
    }
    else
    {
        if(!lock_x)
            pos_x=x;
        if(!lock_y)
            pos_y=y;
        if(!lock_z)
            pos_z=z;
    }
    updateExtreme();
}

void Navigator::setPosition(const Vertex3f &pos)
{
    setPosition(pos[0],pos[1],pos[2]);
}

void Navigator::setMouse(int x, int y)
{
    mouse_x=x;
    mouse_y=y;
}

void Navigator::render(const Matrix4f &mvp)
{
    if(!visible) return;

    program.bind();
    program.uniformMatrix(uniform_mvp,mvp);
    program.uniform(uniform_pos,getZ(),getY(),getX(),0);
    program.uniform(uniform_scale,radius);
    glEnableVertexAttribArray(attribute_coord3d);
    if(form==SPHERE)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
        glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINE_STRIP,0,triangle_num);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_cube);
        glVertexAttribPointer(attribute_coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays(GL_LINES,0,cube_num);
    }
    glDisableVertexAttribArray(attribute_coord3d);
    program.unbind();
}

void Navigator::setDistance(float distance)
{
    this->distance=distance;
}

void Navigator::lockX()
{
    lock_x=!lock_x;
}

void Navigator::lockY()
{
    lock_y=!lock_y;
}

void Navigator::lockZ()
{
    lock_z=!lock_z;
}

void Navigator::updatePosition(Camera *camera, Map * map)
{
    if(!visible) return;

    Vertex3f dir=camera->unproject(mouse_x,mouse_y,0.0);
    dir.normalize();

    switch(mode)
    {
    case POINTER:
        setPosition(camera->getX() + distance*dir[0],camera->getY() + distance*dir[1],camera->getZ() + distance*dir[2]);
    break;
    case MAGNET:
    {
        Vertex3f start(camera->getX(),camera->getY(),camera->getZ()),
                end(camera->getX() + 300*dir[0],camera->getY() + 300*dir[1],camera->getZ() + 300*dir[2]);

        Vertex3f c=map->collisionVisible(start,end, *camera).collision;
        setPosition(c);
    }
        break;
    case FLAT:
        break;
    default:
        break;
    }
}

Navigator::Mode Navigator::getMode() const
{
    return mode;
}

void Navigator::setMode(Navigator::Mode mode)
{
    this->mode=mode;
}

void Navigator::show()
{
    visible=true;
}

void Navigator::hide()
{
    visible=false;
}

bool Navigator::isVisible()
{
    return visible;
}

/*
void Navigator::distance(GLfloat dist)
{
    this->dist=dist;
}

void Navigator::angle(GLfloat x, GLfloat y)
{
    angle_x=x;
    angle_y=y;
}

void Navigator::setMode(NavigatorMode mode)
{
    this->mode=mode;
}
*/
