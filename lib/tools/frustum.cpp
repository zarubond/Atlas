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

#include "frustum.h"

Frustum::Frustum()
{
    hnear=0;
    wnear=0;
    hfar=0;
    wfar=0;
    near=0;
    far=0;
}

void Frustum::setup(float fov, float width, float height, float near, float far)
{
    float ratio=width/height;
    hnear = 2 * tan(fov / 2) * near;
    wnear = hnear * ratio;

    hfar = 2 * tan(fov / 2) * far;
    wfar = hfar * ratio;

    this->near=near;
    this->far=far;
}

void Frustum::lookAt(Vector3f &eye, Vector3f &center, Vector3f &up)
{
    Vector3f nc,fc,x,y,z;

    z = eye - center;
    x = up * z;

    x.normalize();
    z.normalize();

    y = z * x;

    nc = eye - z * near;
    fc = eye - z * far;

    ntl = nc + y * hnear - x * wnear;
    ntr = nc + y * hnear + x * wnear;
    nbl = nc - y * hnear - x * wnear;
    nbr = nc - y * hnear + x * wnear;

    ftl = fc + y * hfar - x * wfar;
    ftr = fc + y * hfar + x * wfar;
    fbl = fc - y * hfar - x * wfar;
    fbr = fc - y * hfar + x * wfar;

    plane[TOP].set3Points(ntr,ntl,ftl);
    plane[BOTTOM].set3Points(nbl,nbr,fbr);
    plane[LEFT].set3Points(ntl,nbl,fbl);
    plane[RIGHT].set3Points(nbr,ntr,fbr);
    plane[NEAR].set3Points(ntl,ntr,nbr);
    plane[FAR].set3Points(ftr,ftl,fbl);
}


bool Frustum::pointInFrustum(const Vector3f &p) const {

    bool result = true;

    for(int i=0; i < 6; i++) {
        if (plane[i].distance(p) < 0)
            return false;
    }
    return result;
}

bool Frustum::sphereInFrustum(const Sphere &sphere) const
{
    float distance;
    for(int i=0; i < 6; i++)
    {
        distance = plane[i].distance(sphere.getPosition());
        if (distance < -sphere.getRadius())
            return false;
    }
    return true;
}

bool Frustum::boxInFrustum(const Box &b) const
{
    for(int i=0; i < 6; i++)
    {
        // is the positive vertex outside?
        if (plane[i].distance(b.getVertexP(plane[i].getNormal())) < 0)
            return false;
    }
    return true;
 }

bool Frustum::cubeInFrustum(const Cube &box) const
{
    return false;
}

Vector3f Frustum::centroid()
{
    Vector3f cent(ntl);
    cent+=ntr;
    cent+=nbl;
    cent+=nbr;
    cent+=ftl;
    cent+=ftr;
    cent+=fbl;
    cent+=fbr;
    cent/=8;

    return cent;
}
