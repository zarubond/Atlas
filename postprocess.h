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
#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "./tools/shader.h"
#include "./tools/texture.h"
#include "./tools/math/math.h"
#include "./tools/vertexbuffrer.h"
#include "world/complement/lightmanager.h"
#include <QElapsedTimer>

/**
 * @brief The PostProcess class is handling Differed shading drawing
 */
class PostProcess
{
public:
    PostProcess();
    /**
     * @brief init Initialize post processing shaders
     * @return
     */
    bool init();
    /**
     * @brief resize Resize screen.
     * @param width
     * @param height
     */
    void resize(unsigned short width, unsigned short height);
    /**
     * @brief shadow Begin drawing shadows.
     */
    void shadow();
    /**
     * @brief process Begin drawing scene.
     * @param tick
     */
    void process(int tick);
    /**
     * @brief render Render screen.
     * @param light Directional light (sun).
     * @param cam_pos Position od camera.
     * @param depthMVP Matrix used for rendering shadows.
     */
    void render(const LightManager &light, const Vertex3f &cam_pos, const Matrix4f &depthMVP);
private:
    bool buffer_switch;
    Shader shader;
    //Texture tex_position,tex_normal,tex_color;
    GLuint  tex_color,fbo_depth, fbo_normal,tex_position;
    GLuint fbo,rbo_depth,
    rbo_texture,rbo_normal,rbo_position,uniform_tex_shadow;
    GLuint fbo_shadow;
    GLuint tex_shadow;
    GLuint  attribute_v_coord_postproc,
    uniform_fbo_texture,uniform_scale,uniform_fbo_depth,uniform_fbo_normal,uniform_tex_position,
    uniform_inv,uniform_s_width,uniform_s_height;


    GLint uniform_mvp,uniform_w2c,uniform_num_lights,uniform_cam_pos,
        uniform_depthmvp;
    LightUniform uniform_light[8];
    GLuint uniform_fog_density,uniform_fog_color;
    GLuint uniform_ambient,uniform_material_ambient,uniform_material_diffuse, uniform_material_specular, uniform_shadow;


    VertexBuffer canvas_buffer;
    Matrix4f biasMatrix;

    int screen_width, screen_height;
    GLfloat scale,shadow_scale;

    int light_num;

    bool enable_shadow;

};

#endif // POSTPROCESS_H
