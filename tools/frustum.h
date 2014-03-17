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
#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "opengl.h"
#include "math/math.h"

class Frustum
{
public:
    /**
     * @brief The Side enum is list of frustum sides.
     */
    enum Side{
        TOP=0,
        BOTTOM,RIGHT,LEFT,NEAR,FAR
    };

   // https://docs.google.com/viewer?a=v&q=cache:52Fapzmq4UsJ:www8.cs.umu.se/kurser/5DV051/HT12/lab/plane_extraction.pdf+&hl=en&pid=bl&srcid=ADGEESirvYVIbaMaRD7Q9ZHau-nAUXmepbyDk31rcAJ0gdBl-rOCJymV3KRREmWebEU6_bPO8_U86rALZzqszOCq94DKEZhy5rjZNKfAT2RUr-tvxLf34F4he0vDEH9SZbq9Q3ZyfamX&sig=AHIEtbQ893o9RGnfoReKPeyG3u8eL_N4VA
//http://stackoverflow.com/questions/6301085/how-to-check-if-an-object-lies-outside-the-clipping-volume-in-opengl
    //http://en.wikibooks.org/wiki/OpenGL_Programming/Glescraft_5
    Frustum();
    /**
     * @brief setup Setup projection
     * @param fov
     * @param width
     * @param height
     * @param near
     * @param far
     */
    void setup(GLfloat fov,GLfloat width,GLfloat height,GLfloat near, GLfloat far);
    /**
     * @brief lookAt Calculate frustum.
     * @param eye
     * @param center
     * @param up
     */
    void lookAt(Vertex3f & eye,Vertex3f & center, Vertex3f & up);
    /**
     * @brief pointInFrustum Test if point is in frustum.
     * @param p
     * @return
     */
    bool pointInFrustum(const Vertex3f &p) const;
    /**
     * @brief sphereInFrustum Test if sphere is in frustum.
     * @param sphere
     * @return
     */
    bool sphereInFrustum(const Sphere & sphere) const;

    bool cubeInFrustum(const Cube & box) const;

    bool boxInFrustum(const Box &b)const;

    Vertex3f centroid();


    float near,far;
    Vertex3f ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
private:
    float hnear,hfar;
    float wnear,wfar;
    Plane plane[6];

};

#endif // FRUSTUM_H
