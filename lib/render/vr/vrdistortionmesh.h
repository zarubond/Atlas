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

#ifndef VRDISTORTIONMESH_H
#define VRDISTORTIONMESH_H

#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <cmath>
#include <QDebug>

#include "vrparams.h"

class VrDistortionMesh
{
public:
    VrDistortionMesh();

    void create(VrParams * distortion, float screenWidthM, float screenHeightM, float xEyeOffsetMScreen, float yEyeOffsetMScreen,
                float textureWidthM, float textureHeightM, float xEyeOffsetMTexture, float yEyeOffsetMTexture,
                float viewportXMTexture, float viewportYMTexture, float viewportWidthMTexture, float viewportHeightMTexture);
    QOpenGLBuffer arrayBuffer;
    QOpenGLBuffer elementBuffer;
    int nIndices;
private:
    float clamp(float val, float min, float max);

};

#endif // VRDISTORTIONMESH_H
