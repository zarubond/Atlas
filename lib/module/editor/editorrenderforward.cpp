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

#include "editorrenderforward.h"

EditorRenderForward::EditorRenderForward()
{
}

bool EditorRenderForward::init()
{
    loadShader();
    return true;
}

void EditorRenderForward::render(WorldGraphics *world, const Environment *envi, const Matrix4f &mvp, unsigned int elapsed)
{
    if(!world->editor.isVisible()) return;

    renderNavigator(&world->editor,world->editor.navig, envi, mvp);
    renderBox(world, envi, mvp);
    renderPositioning(&world->editor, mvp);
}

void EditorRenderForward::renderPrev(WorldGraphics *world, const Environment *envi, const Camera &camera, unsigned int elapsed)
{
    Vector3f cam_pos=camera.position();
    world->editor.axis.setScale(world->editor.axis.position().distance(camera.position())*0.02+1.0);

    Vector3f dir(1.0);
    if(cam_pos[0]<world->editor.axis.position()[0])
        dir[0]=-1.0;
    if(cam_pos[1]<world->editor.axis.position()[1])
        dir[1]=-1.0;
    if(cam_pos[2]<world->editor.axis.position()[2])
        dir[2]=-1.0;
    world->editor.axis_switch=dir;
}

void EditorRenderForward::loadBox(EditorGraphics * editor)
{
    editor->vao_box.create();
    editor->vao_box.bind();
    editor->vbo_box.bind();
    program.enableAttribute(a_coord);
    program.setAttribute(a_coord, 3, GL_FLOAT, 7*sizeof(GLfloat), 0);
    program.enableAttribute(a_color);
    program.setAttribute(a_color, 4, GL_FLOAT, 7*sizeof(GLfloat), 3*sizeof(GLfloat));
    editor->vao_box.release();
}

void EditorRenderForward::loadNavigatorSphere(EditorGraphics * editor)
{
    editor->vao_sphere.create();
    editor->vao_sphere.bind();
    editor->vbo_sphere.bind();
    program.enableAttribute(a_coord);
    program.setAttribute(a_coord, 3, GL_FLOAT, 7*sizeof(GLfloat), 0);
    program.enableAttribute(a_color);
    program.setAttribute(a_color, 4, GL_FLOAT, 7*sizeof(GLfloat), 3*sizeof(GLfloat));
    editor->vao_sphere.release();
}

void EditorRenderForward::loadNavigatorCube(EditorGraphics *editor)
{
    editor->vao_cube.create();
    editor->vao_cube.bind();
    editor->vbo_cube.bind();
    program.enableAttribute(a_coord);
    program.setAttribute(a_coord, 3, GL_FLOAT, 7*sizeof(GLfloat), 0);
    program.enableAttribute(a_color);
    program.setAttribute(a_color, 4, GL_FLOAT, 7*sizeof(GLfloat), 3*sizeof(GLfloat));
    editor->vao_cube.release();
}

void EditorRenderForward::loadPosition(EditorGraphics * editor)
{
    editor->vao_position.create();
    editor->vao_position.bind();
    editor->vbo_position.bind();
    program.enableAttribute(a_coord);
    program.setAttribute(a_coord, 3, GL_FLOAT, 7*sizeof(GLfloat), 0);
    program.enableAttribute(a_color);
    program.setAttribute(a_color, 4, GL_FLOAT, 7*sizeof(GLfloat), 3*sizeof(GLfloat));
    editor->vao_position.release();
}

void EditorRenderForward::loadScale(EditorGraphics *editor)
{
    editor->vao_scale.create();
    editor->vao_scale.bind();
    editor->vbo_scale.bind();
    program.enableAttribute(a_coord);
    program.setAttribute(a_coord, 3, GL_FLOAT, 7*sizeof(GLfloat), 0);
    program.enableAttribute(a_color);
    program.setAttribute(a_color, 4, GL_FLOAT, 7*sizeof(GLfloat), 3*sizeof(GLfloat));
    editor->vao_scale.release();
}

void EditorRenderForward::renderNavigator(EditorGraphics * editor, const Instrument *navig, const Environment *envi, const Matrix4f &mvp)
{
    if(!navig->isVisible()) return;

    if(!editor->vao_cube.isCreated())
        loadNavigatorCube(editor);

    if(!editor->vao_sphere.isCreated())
        loadNavigatorSphere(editor);

    float radius=navig->getRadius();
    program.bind();
    program.uniformMatrix(uniform_mvp,mvp);
    program.uniform(uniform_pos, navig->getPosition());
    program.uniform(uniform_dir, Vector3f(1.0));

    if(navig->getForm()==Shape::SPHERE)
    {
        program.uniform(uniform_scale,radius,radius,radius);
        editor->vao_sphere.bind();
        editor->vbo_sphere.draw(editor->sphere_num,VertexBuffer::LINE_STRIP);
        editor->vao_sphere.release();
    }
    else
    {
        float r=int(radius);
        r-=0.5;
        program.uniform(uniform_scale,r,r,r);
        editor->vao_cube.bind();
        editor->vbo_cube.draw(editor->cube_num,VertexBuffer::LINES);
        editor->vao_cube.release();
    }
    program.release();
}

void EditorRenderForward::renderBox(WorldGraphics *world, const Environment *envi, const Matrix4f &mvp)
{
    if(!world->editor.vao_box.isCreated())
        loadBox(&world->editor);

    program.bind();
    program.uniformMatrix(uniform_mvp, mvp);
    program.uniform(uniform_pos, world->map.width()/2.0f, world->map.height()/2.0f, world->map.depth()/2.0f);
    program.uniform(uniform_scale, world->map.width(), world->map.height(), world->map.depth());
    program.uniform(uniform_dir, Vector3f(1.0));

    world->editor.vao_box.bind();
    world->editor.vbo_box.draw(world->editor.box_num,VertexBuffer::LINES);
    world->editor.vao_box.release();

    program.release();
}

void EditorRenderForward::renderPositioning(EditorGraphics *editor, const Matrix4f &mvp)
{
    if(!editor->axis.visible())
        return;

    program.bind();
    program.uniformMatrix(uniform_mvp, mvp);
    program.uniform(uniform_pos, editor->axis.position());
    program.uniform(uniform_scale, Vector3f(editor->axis.scale()));
    program.uniform(uniform_dir, editor->axis_switch);

    if(editor->axis.type()==EditorPositioning::POSITION)
    {
        if(!editor->vao_position.isCreated())
            loadPosition(editor);
        editor->vao_position.bind();
        editor->vbo_position.draw(editor->position_num,VertexBuffer::TRIANGLES);
        editor->vao_position.release();
    }
    else if(editor->axis.type()==EditorPositioning::SCALE)
    {
        if(!editor->vao_scale.isCreated())
            loadScale(editor);
        editor->vao_scale.bind();
        editor->vbo_scale.draw(editor->scale_num,VertexBuffer::TRIANGLES);
        editor->vao_scale.release();
    }

    program.release();
}


bool EditorRenderForward::loadShader()
{
    setlocale(LC_NUMERIC, "C");

    const char * vertex_src =
            "attribute vec3 coord;\n"
            "attribute vec4 color;\n"
            "uniform mat4 mvp;\n"
            "uniform vec3 pos;\n"
            "uniform vec3 scale;\n"
            "uniform vec3 dir;\n"
            "varying vec4 f_color;\n"
            "void main() {\n"
            "   f_color=color;\n"
            "   gl_Position = mvp * vec4((coord*dir)*scale+pos,1.0);\n"
            "}\n";


    const char * fragment_src =
            "varying vec4 f_color;\n"
            "void main() {\n"
            "   gl_FragColor = f_color;\n"
            "}\n";

    if(!program.load(vertex_src,fragment_src))
        return false;

    if((a_coord = program.getAttribute("coord"))==-1)
        return false;

    if((a_color = program.getAttribute("color"))==-1)
        return false;

    if((uniform_mvp = program.getUniform("mvp"))==-1)
        return false;

    if((uniform_pos = program.getUniform("pos"))==-1)
        return false;

    if((uniform_scale = program.getUniform("scale"))==-1)
        return false;

    if((uniform_dir = program.getUniform("dir"))==-1)
        return false;

    setlocale(LC_ALL, "");

    return true;
}

