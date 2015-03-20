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

#ifndef TERRAINVOXEL_H
#define TERRAINVOXEL_H

/**
 * @brief The TerrainVoxel struct is basic type fo voxel.
 */
class TerrainVoxel{
public:
    TerrainVoxel();
    TerrainVoxel(signed char value);
    /**
     * @brief setTextureV Set intesity of vertical texture;
     * @param id Id of texture between 0 and 3
     * @param value Texture intensity between 0 and 15
     */
    void setTextureRawV(int id, unsigned char value);
    /**
     * @brief setTextureH
     * @param id
     * @param value
     */
    void setTextureRawH(int id,unsigned char value);
    /**
     * @brief setMaterial
     * @param material
     */
    void setMaterial(const unsigned char * material);

    void clearMaterial();

    unsigned char textureV(int id);
    unsigned char textureH(int id);

    void setTextureV(unsigned char id, unsigned char value);
    void setTextureH(unsigned char id, unsigned char value);

    /**
     * @brief value Isovalue of voxel.
     */
    signed char value;
    //material upper is vertival lower is horizontal;
    unsigned char material[4];
};


#endif // TERRAINVOXEL_H
