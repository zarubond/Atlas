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

#include "editorgraphics.h"

EditorGraphics::EditorGraphics()
{
    position_data=NULL;
    scale_data=NULL;
    axis_switch.set(1.0,1.0,1.0);
}

EditorGraphics::~EditorGraphics()
{
    delete [] position_data;
    delete [] scale_data;
}

bool EditorGraphics::init()
{
    loadAxis();
    loadScale();
    loadBox();
    loadPointer();

    return true;
}

EditorGraphics::Axis EditorGraphics::positionCollision(const Vector3<double> &start, Vector3<double> &end)
{
    Triangle tri;
    Vector3f tmp,t=end;
    const Vector3f correction=axis_switch*axis.scale();

    for(int i=0;i<position_num;i+=9)
    {
        //012
        tri.set(Vector3d(position_data[i+0]*correction[0]+axis.position()[0], position_data[i+1]*correction[1]+axis.position()[1], position_data[i+2]*correction[2]+axis.position()[2]),
                Vector3d(position_data[i+3]*correction[0]+axis.position()[0], position_data[i+4]*correction[1]+axis.position()[1], position_data[i+5]*correction[2]+axis.position()[2]),
                Vector3d(position_data[i+6]*correction[0]+axis.position()[0], position_data[i+7]*correction[1]+axis.position()[1], position_data[i+8]*correction[2]+axis.position()[2]));
        tmp=tri.collision(start, end);
        if(tmp!=t)
        {
            end=tmp;
            return Axis::X;
        }
        //102
        tri.set(Vector3d(position_data[i+1]*correction[0]+axis.position()[0], position_data[i+0]*correction[1]+axis.position()[1], position_data[i+2]*correction[2]+axis.position()[2]),
                Vector3d(position_data[i+4]*correction[0]+axis.position()[0], position_data[i+3]*correction[1]+axis.position()[1], position_data[i+5]*correction[2]+axis.position()[2]),
                Vector3d(position_data[i+7]*correction[0]+axis.position()[0], position_data[i+6]*correction[1]+axis.position()[1], position_data[i+8]*correction[2]+axis.position()[2]));
        tmp=tri.collision(start, end);
        if(tmp!=t)
        {
            end=tmp;
            return Axis::Y;
        }

        //210
        tri.set(Vector3d(position_data[i+2]*correction[0]+axis.position()[0], position_data[i+1]*correction[1]+axis.position()[1], position_data[i+0]*correction[2]+axis.position()[2]),
                Vector3d(position_data[i+5]*correction[0]+axis.position()[0], position_data[i+4]*correction[1]+axis.position()[1], position_data[i+3]*correction[2]+axis.position()[2]),
                Vector3d(position_data[i+8]*correction[0]+axis.position()[0], position_data[i+7]*correction[1]+axis.position()[1], position_data[i+6]*correction[2]+axis.position()[2]));
        tmp=tri.collision(start, end);
        if(tmp!=t)
        {
            end=tmp;
            return Axis::Z;
        }
    }
    return Axis::NONE;
}

EditorGraphics::Axis EditorGraphics::scaleCollision(const Vector3<double> &start, Vector3<double> &end)
{
    Triangle tri;
    Vector3f tmp,t=end;
    const Vector3f correction=axis_switch*axis.scale();

    for(int i=0;i<scale_num;i+=9)
    {
        //012
        tri.set(Vector3d(scale_data[i+0]*correction[0]+axis.position()[0], scale_data[i+1]*correction[1]+axis.position()[1], scale_data[i+2]*correction[2]+axis.position()[2]),
                Vector3d(scale_data[i+3]*correction[0]+axis.position()[0], scale_data[i+4]*correction[1]+axis.position()[1], scale_data[i+5]*correction[2]+axis.position()[2]),
                Vector3d(scale_data[i+6]*correction[0]+axis.position()[0], scale_data[i+7]*correction[1]+axis.position()[1], scale_data[i+8]*correction[2]+axis.position()[2]));
        tmp=tri.collision(start, end);
        if(tmp!=t)
        {
            end=tmp;
            return Axis::X;
        }
        //102
        tri.set(Vector3d(scale_data[i+1]*correction[0]+axis.position()[0], scale_data[i+0]*correction[1]+axis.position()[1], scale_data[i+2]*correction[2]+axis.position()[2]),
                Vector3d(scale_data[i+4]*correction[0]+axis.position()[0], scale_data[i+3]*correction[1]+axis.position()[1], scale_data[i+5]*correction[2]+axis.position()[2]),
                Vector3d(scale_data[i+7]*correction[0]+axis.position()[0], scale_data[i+6]*correction[1]+axis.position()[1], scale_data[i+8]*correction[2]+axis.position()[2]));
        tmp=tri.collision(start, end);
        if(tmp!=t)
        {
            end=tmp;
            return Axis::Y;
        }

        //210
        tri.set(Vector3d(scale_data[i+2]*correction[0]+axis.position()[0], scale_data[i+1]*correction[1]+axis.position()[1], scale_data[i+0]*correction[2]+axis.position()[2]),
                Vector3d(scale_data[i+5]*correction[0]+axis.position()[0], scale_data[i+4]*correction[1]+axis.position()[1], scale_data[i+3]*correction[2]+axis.position()[2]),
                Vector3d(scale_data[i+8]*correction[0]+axis.position()[0], scale_data[i+7]*correction[1]+axis.position()[1], scale_data[i+6]*correction[2]+axis.position()[2]));
        tmp=tri.collision(start, end);
        if(tmp!=t)
        {
            end=tmp;
            return Axis::Z;
        }
    }

    return Axis::NONE;
}

EditorGraphics::Axis EditorGraphics::collision(const Vector3<double> &start, Vector3<double> &end)
{
    switch (axis.type())
    {
    case EditorPositioning::POSITION:
        return positionCollision(start,end);
        break;
    case EditorPositioning::SCALE:
        return scaleCollision(start, end);
    default:
        break;
    }
    return Axis::NONE;
}

void EditorGraphics::loadAxis()
{
    float *cone_data=NULL,*cyli_data=NULL;
    int cone_size,cyli_size;
    Cone::triangleMesh(cone_data,cone_size,10,3,1.5,Vector3f(5.0,0.0,0.0));
    Cylinder::triangleMesh(cyli_data,cyli_size,10,5.0,0.1);

    float * data=new float[(cone_size/3*7+cyli_size/3*7)*3];
    position_data=new float[cone_size+cyli_size];

    memcpy(position_data, cone_data, cone_size*sizeof(float));
    memcpy(&position_data[cone_size], cyli_data, cyli_size*sizeof(float));

    int data_pos=0;
    for(int j=0; j<3;j++)
    {
        for(int i=0;i<cyli_size;i+=3)
        {
            if(j==0)
            {
                data[data_pos++] = cyli_data[i+0];
                data[data_pos++] = cyli_data[i+1];
                data[data_pos++] = cyli_data[i+2];
            }
            else if(j==1)
            {
                data[data_pos++] = cyli_data[i+1];
                data[data_pos++] = cyli_data[i+0];
                data[data_pos++] = cyli_data[i+2];
            }
            else if(j==2)
            {
                data[data_pos++] = cyli_data[i+2];
                data[data_pos++] = cyli_data[i+1];
                data[data_pos++] = cyli_data[i+0];
            }
            data[data_pos++] = j==0;
            data[data_pos++] = j==1;
            data[data_pos++] = j==2;
            data[data_pos++] = 1.0;
        }

        for(int i=0;i<cone_size;i+=3)
        {
            if(j==0)
            {
                data[data_pos++] = cone_data[i+0];
                data[data_pos++] = cone_data[i+1];
                data[data_pos++] = cone_data[i+2];
            }
            else if(j==1)
            {
                data[data_pos++] = cone_data[i+1];
                data[data_pos++] = cone_data[i+0];
                data[data_pos++] = cone_data[i+2];
            }
            else if(j==2)
            {
                data[data_pos++] = cone_data[i+2];
                data[data_pos++] = cone_data[i+1];
                data[data_pos++] = cone_data[i+0];
            }
            data[data_pos++] = j==0;
            data[data_pos++] = j==1;
            data[data_pos++] = j==2;
            data[data_pos++] = 1.0;
        }
    }

    vbo_position.create(sizeof(GLfloat)*data_pos,data);
    position_num=cone_size+cyli_size;

    delete [] cone_data;
    delete [] cyli_data;
    delete [] data;
}

void EditorGraphics::loadBox()
{
    GLfloat * cube_verts=NULL;
    int size=0;
    Cube::createMeshContour(cube_verts,size);
    float * data=new float[size/3*7];
    for(int i=0;i<size/6;i++)
    {
        data[i*14+0]=cube_verts[i*6+0];
        data[i*14+1]=cube_verts[i*6+1];
        data[i*14+2]=cube_verts[i*6+2];
        data[i*14+3]=std::abs(sgn(cube_verts[i*6+0]-cube_verts[i*6+3]));
        data[i*14+4]=std::abs(sgn(cube_verts[i*6+1]-cube_verts[i*6+4]));
        data[i*14+5]=std::abs(sgn(cube_verts[i*6+2]-cube_verts[i*6+5]));
        data[i*14+6]=1.0;

        data[i*14+7]=cube_verts[i*6+3];
        data[i*14+8]=cube_verts[i*6+4];
        data[i*14+9]=cube_verts[i*6+5];
        data[i*14+10]=std::abs(sgn(cube_verts[i*6+0]-cube_verts[i*6+3]));
        data[i*14+11]=std::abs(sgn(cube_verts[i*6+1]-cube_verts[i*6+4]));
        data[i*14+12]=std::abs(sgn(cube_verts[i*6+2]-cube_verts[i*6+5]));
        data[i*14+13]=1.0;
    }

    vbo_box.create(sizeof(GLfloat)*size/3*7,data);
    delete [] cube_verts;
    delete [] data;
    box_num=size/3;
}

void EditorGraphics::loadScale()
{
    float *cyli_data=NULL,*cube_data=NULL;
    int cyli_size,cube_size;
    Cylinder::triangleMesh(cyli_data, cyli_size,10,5.0,0.1);
    Cube::triangleMesh(cube_data, cube_size,Vector3f(5.0,0.0,0.0),Vector3f(2.0));

    float * data=new float[(cube_size/3*7+cyli_size/3*7)*3];
    scale_data=new float[cube_size+cyli_size];

    memcpy(scale_data, cyli_data, cyli_size*sizeof(float));
    memcpy(&scale_data[cyli_size], cube_data, cube_size*sizeof(float));

    int data_pos=0;

    for(int j=0; j<3;j++)
    {
        for(int i=0;i<cyli_size;i+=3)
        {
            if(j==0)
            {
                data[data_pos++] = cyli_data[i+0];
                data[data_pos++] = cyli_data[i+1];
                data[data_pos++] = cyli_data[i+2];
            }
            else if(j==1)
            {
                data[data_pos++] = cyli_data[i+1];
                data[data_pos++] = cyli_data[i+0];
                data[data_pos++] = cyli_data[i+2];
            }
            else if(j==2)
            {
                data[data_pos++] = cyli_data[i+2];
                data[data_pos++] = cyli_data[i+1];
                data[data_pos++] = cyli_data[i+0];
            }
            data[data_pos++] = j==0;
            data[data_pos++] = j==1;
            data[data_pos++] = j==2;
            data[data_pos++] = 1.0;
        }

        for(int i=0;i<cube_size;i+=3)
        {
            if(j==0)
            {
                data[data_pos++] = cube_data[i+0];
                data[data_pos++] = cube_data[i+1];
                data[data_pos++] = cube_data[i+2];
            }
            else if(j==1)
            {
                data[data_pos++] = cube_data[i+1];
                data[data_pos++] = cube_data[i+0];
                data[data_pos++] = cube_data[i+2];
            }
            else if(j==2)
            {
                data[data_pos++] = cube_data[i+2];
                data[data_pos++] = cube_data[i+1];
                data[data_pos++] = cube_data[i+0];
            }

            data[data_pos++] = j==0;
            data[data_pos++] = j==1;
            data[data_pos++] = j==2;
            data[data_pos++] = 1.0;
        }
    }

    vbo_scale.create(sizeof(GLfloat)*data_pos,data);
    delete [] cube_data;
    delete [] cyli_data;
    delete [] data;

    scale_num=cube_size+cyli_size;
}

void EditorGraphics::loadPointer()
{
    int n=0;
    GLfloat verts[5000];
    const int space = 20;
    float radius=1.0;

    for(float b = 0; b <= 180 - space; b+=space)
    {
        for(float a = 0; a <= 360 - space; a+=space)
        {
            verts[n++] = radius * sin((a) / 180.0 * M_PI) * sin((b) / 180.0 *M_PI);
            verts[n++] = radius * cos((a) / 180.0 * M_PI) * sin((b) / 180.0 *M_PI);
            verts[n++] = radius * cos((b) / 180.0 * M_PI);
            verts[n++] = 1.0;
            verts[n++] = 0.0;
            verts[n++] = 0.0;
            verts[n++] = 1.0;

            verts[n++] = radius * sin((a) / 180.0 * M_PI) * sin((b + space) / 180.0 * M_PI);
            verts[n++] = radius * cos((a) / 180.0 * M_PI) * sin((b + space) / 180.0 * M_PI);
            verts[n++] = radius * cos((b + space) / 180.0 * M_PI);
            verts[n++] = 1.0;
            verts[n++] = 0.0;
            verts[n++] = 0.0;
            verts[n++] = 1.0;

            verts[n++] = radius * sin((a + space) / 180.0 * M_PI) * sin((b) / 180.0 * M_PI);
            verts[n++] = radius * cos((a + space) / 180.0 * M_PI) * sin((b) / 180.0 * M_PI);
            verts[n++] = radius * cos((b) / 180.0 * M_PI);
            verts[n++] = 1.0;
            verts[n++] = 0.0;
            verts[n++] = 0.0;
            verts[n++] = 1.0;

            verts[n++] = radius * sin((a + space) / 180.0 * M_PI) * sin((b + space) /180.0 * M_PI);
            verts[n++] = radius * cos((a + space) / 180.0 * M_PI) * sin((b + space) /180.0 * M_PI);
            verts[n++] = radius * cos((b + space) / 180.0 * M_PI);
            verts[n++] = 1.0;
            verts[n++] = 0.0;
            verts[n++] = 0.0;
            verts[n++] = 1.0;
        }
    }

    vbo_sphere.create(sizeof(GLfloat)*n,verts);

    sphere_num=n/7;

    //box shape
    GLfloat * cube_verts=NULL;
    int size=0;
    Cube::createMeshContour(cube_verts,size);
    for(int i=0;i<size;i++)
        cube_verts[i]*=2;
    float * data=new float[size/3*7];
    for(int i=0;i<size/6;i++)
    {
        data[i*14+0]=cube_verts[i*6+0];
        data[i*14+1]=cube_verts[i*6+1];
        data[i*14+2]=cube_verts[i*6+2];
        data[i*14+3]=1.0;
        data[i*14+4]=0.0;
        data[i*14+5]=0.0;
        data[i*14+6]=1.0;

        data[i*14+7]=cube_verts[i*6+3];
        data[i*14+8]=cube_verts[i*6+4];
        data[i*14+9]=cube_verts[i*6+5];
        data[i*14+10]=1.0;
        data[i*14+11]=0.0;
        data[i*14+12]=0.0;
        data[i*14+13]=1.0;
    }


    vbo_cube.create(sizeof(GLfloat)*size/3*7,data);
    delete [] cube_verts;
    delete [] data;
    cube_num=size/3;
}
