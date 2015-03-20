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

#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "../../graphics/graphics.h"
#include "../../math/math.h"
#include "../../tools/camera.h"
#include "model.h"
#include "../modulebase.h"


/**
 * @brief The ModelManager class handling models rendering.
 */
class ModelBase: public ModuleBase
{
public:
    ModelBase(int type);
    ~ModelBase();

    void addMesh(ModelMesh * mesh);
    /**
     * @brief deleteMesh Delete mesh and all models.
     * @param mesh
     * @return
     */
    bool removeMesh(ModelMesh * mesh);
    /**
     * @brief getMeshes Get mesh list.
     * @return
     */
    const std::set<ModelMesh *> &getMeshes() const;

protected:

    std::set<ModelMesh *> meshes;
};

#endif // MODELMANAGER_H
