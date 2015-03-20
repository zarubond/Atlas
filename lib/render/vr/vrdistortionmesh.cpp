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

#include "vrdistortionmesh.h"

VrDistortionMesh::VrDistortionMesh():
    arrayBuffer(QOpenGLBuffer::VertexBuffer), elementBuffer(QOpenGLBuffer::IndexBuffer)
{
}

void VrDistortionMesh::create(VrParams * distortion, float screenWidthM, float screenHeightM, float xEyeOffsetMScreen,
                              float yEyeOffsetMScreen, float textureWidthM, float textureHeightM, float xEyeOffsetMTexture,
                              float yEyeOffsetMTexture, float viewportXMTexture, float viewportYMTexture, float viewportWidthMTexture,
                              float viewportHeightMTexture)
{
    float mPerUScreen = screenWidthM;
    float mPerVScreen = screenHeightM;
    float mPerUTexture = textureWidthM;
    float mPerVTexture = textureHeightM;

    float * vertexData = new float[40*40*5];
    int vertexLength = 0;
    for (int row = 0; row < 40; row++) {
        for (int col = 0; col < 40; col++) {
            float uTexture = float(col) / 39.0f * (viewportWidthMTexture / textureWidthM) + viewportXMTexture / textureWidthM;
            float vTexture = float(row) / 39.0f * (viewportHeightMTexture / textureHeightM) + viewportYMTexture / textureHeightM;

            float xTexture = uTexture * mPerUTexture;
            float yTexture = vTexture * mPerVTexture;
            float xTextureEye = xTexture - xEyeOffsetMTexture;
            float yTextureEye = yTexture - yEyeOffsetMTexture;
            float rTexture = (float) sqrt(xTextureEye * xTextureEye + yTextureEye * yTextureEye);

            float textureToScreen = rTexture > 0.0F ? distortion->distortInverse(rTexture) / rTexture : 1.0F;

            float xScreen = xTextureEye * textureToScreen + xEyeOffsetMScreen;
            float yScreen = yTextureEye * textureToScreen + yEyeOffsetMScreen;
            float uScreen = xScreen / mPerUScreen;
            float vScreen = yScreen / mPerVScreen;
            float vignetteSizeMTexture = 0.0020F / textureToScreen;

            float dxTexture = xTexture - clamp(xTexture, viewportXMTexture + vignetteSizeMTexture,
                                               viewportXMTexture + viewportWidthMTexture
                                               - vignetteSizeMTexture);

            float dyTexture = yTexture - clamp(yTexture, viewportYMTexture + vignetteSizeMTexture,
                                               viewportYMTexture + viewportHeightMTexture
                                               - vignetteSizeMTexture);

            float drTexture = (float) sqrt(dxTexture * dxTexture + dyTexture * dyTexture);
            float vignette = 1.0F - clamp(drTexture / vignetteSizeMTexture, 0.0F, 1.0F);

            vertexData[(vertexLength + 0)] = (2.0F * uScreen - 1.0F);
            vertexData[(vertexLength + 1)] = (2.0F * vScreen - 1.0F);
            vertexData[(vertexLength + 2)] = vignette;
            vertexData[(vertexLength + 3)] = uTexture;
            vertexData[(vertexLength + 4)] = vTexture;

            vertexLength += 5;
        }
    }

    nIndices = 3158;
    unsigned short * indexData = new unsigned short[nIndices];
    int indexOffset = 0;
    int vertexOffset = 0;
    for (int row = 0; row < 39; row++) {
        if (row > 0) {
            indexData[indexOffset] = indexData[(indexOffset - 1)];
            indexOffset++;
        }
        for (int col = 0; col < 40; col++) {
            if (col > 0) {
                if (row % 2 == 0) {
                    vertexOffset++;
                } else {
                    vertexOffset--;
                }
            }
            indexData[(indexOffset++)] = vertexOffset;
            indexData[(indexOffset++)] = (vertexOffset + 40);
        }
        vertexOffset += 40;
    }

    arrayBuffer.create();
    arrayBuffer.bind();
    arrayBuffer.allocate(vertexLength* sizeof(float));
    arrayBuffer.write(0, vertexData, vertexLength* sizeof(float));
    arrayBuffer.release();

    elementBuffer.create();
    elementBuffer.bind();
    elementBuffer.allocate(indexOffset * sizeof(unsigned short));
    elementBuffer.write(0, indexData, indexOffset * sizeof(unsigned short));
    elementBuffer.release();


    delete [] vertexData;
    delete [] indexData;

}

float VrDistortionMesh::clamp(float val, float min, float max) {
    return std::max(min, std::min(max, val));
}
