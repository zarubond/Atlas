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
#ifndef GRASSMANAGER_H
#define GRASSMANAGER_H

#include "../../../tools/math/math.h"
#include "../partmanager.h"

/**
 * @brief The GrassManager class to be done in future.
 */
class GrassBase: public ModuleBase
{
public:
    GrassBase(int type);
    ~GrassBase();

    void addTuft(const Vertex3f &position);
    bool removeTuft(const Vertex3f & position);
    bool removeTuft(unsigned id);
    Vertex3f getTuft(unsigned id) const;
    int getTuftCount() const;
    const Vertex3f * tuftList() const;
    int tuftCount() const;

private:

    Vertex3f * tuft_list;
    unsigned tuft_count;
};
#endif // GRASSMANAGER_H
