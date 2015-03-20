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
#ifndef TERRAINUNDO_H
#define TERRAINUNDO_H

#include "../../undocommand.h"
#include "../../../lib/module/terrain/terrainbase.h"
/**
 * @brief The TerrainUndo class
 */
class TerrainUndo : public UndoCommand
{
public:
    TerrainUndo(TerrainBase *terrain);
    ~TerrainUndo();
    bool backup(int x, int y, int z, int size);

    signed char value(unsigned x, unsigned y, unsigned z) const;
    TerrainVoxel *voxel(unsigned x,unsigned y,unsigned z) const;
    void undo();
    void redo();
private:
    unsigned int backup_size;
    unsigned backup_x,backup_y,backup_z;

    TerrainVoxel * backup_buffer;
    TerrainBase * const terrain;
    bool first;//hack to surpase fist redo call
};

#endif // TERRAINUNDO_H
