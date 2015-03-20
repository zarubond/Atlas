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

#ifndef MODEL_H
#define MODEL_H

#include "../../math/math.h"
#include "../../tools/moveable.h"
#include "../../mapobject.h"
#include "./modelmesh.h"

class ModelGraphics;
/**
 * @brief The Model class is representing model on map.
 */
class Model:  public MapObject,public Moveable
{
public:
    Model(ModelGraphics * graphics);
    Model(ModelGraphics * graphics, const Model *model);
    ~Model();

    /**
     * @brief Mesh representing the model.
     * @param mesh
     */
    void setMesh(ModelMesh *mesh);
    ModelMesh * getMesh();
    /**
     * @brief inBox Is Model in box.
     * @param x
     * @param y
     * @param z
     * @param w
     * @return
     */
    bool inBox(int x, int y, int z, int w)const;
    void collision(const Vector3f &a, Collision &b);
    /**
     * @brief setName Set model name.
     * @param name
     */
    void setName(const char * name);

    /**
     * @brief setPosition in space
     * @param x Position x in space.
     * @param y Position y in space.
     * @param z Position z in space.
     */

    void setPosX(float x);
    void setPosY(float y);
    void setPosZ(float z);

    void setAngle(const Vector3f &value);

    void setOffsetPosition(const Vector3f & m_position);
    void setOffsetAngle(const Vector3f & angle);

    /**
     * @brief selectDraw Select if the model will be drawn.
     */
    void selectDraw(const Camera &camera);
    void selectDraw();
    /**
     * @brief getName Get model's name.
     * @return
     */
    const char * getName()const;

    void onUpdate();

    const Matrix4f & getMatrix() const;

    int type();
    Vector3f center();
    /**
     * @brief next For list in model Manager.
     */
   // Model * next;
private:
    Matrix4f matrix;

    ModelMesh * mesh;
    void bbox();
    Box box;

    ModelGraphics * graphics;
    std::string name;

    float min_x,max_x,min_y,max_y,min_z,max_z;
};

#endif // MODEL_H
