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

#ifndef GRASSMANAGER_H
#define GRASSMANAGER_H

#include "../../math/math.h"
#include "../modulebase.h"
#include "grassregion.h"
#include "../../map.h"

/**
 * @brief The GrassManager class to be done in future.
 */
class GrassBase: public ModuleBase
{
public:
    GrassBase(int type);
    ~GrassBase();

    void setMap(Map * map);

    void addTuft(const Vector3f &position);
    bool removeTuft(const Vector3f & position);
    Vector3f getTuft(unsigned id) const;

    void build(unsigned short nodes_x, unsigned short nodes_y, unsigned short nodes_z, unsigned short node_size);
    GrassRegion *getRegion(int x, int y, int z);
    unsigned short nodesX();
    unsigned short nodesY();
    unsigned short nodesZ();
protected:
    virtual void createRegion(int x,int y,int z);
    GrassRegion ** grid;
    unsigned short nodes_x,nodes_y,nodes_z,region_size;
    Map * map;
};
#endif // GRASSMANAGER_H
