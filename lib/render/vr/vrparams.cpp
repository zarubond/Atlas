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

#include "vrparams.h"

VrParams::VrParams()
{
    mInterpupillaryDistance = 0.06F;
    mVerticalDistanceToLensCenter = 0.035F;
    mLensDiameter = 0.025F;
    mScreenToLensDistance = 0.037F;
    mEyeToLensDistance = 0.011F;

    mVisibleViewportSize = 0.06F;
    mFovY = 65.0F;

    mCoefficients[0] = 250.0F;
    mCoefficients[1] = 50000.0F;
}

float VrParams::interpupillaryDistance() const
{
    return mInterpupillaryDistance;
}

float VrParams::verticalDistanceToLensCenter() const
{
    return mVerticalDistanceToLensCenter;
}

float VrParams::lensDiameter() const
{
    return mLensDiameter;
}

float VrParams::screenToLensDistance() const
{
    return mScreenToLensDistance;
}

float VrParams::eyeToLensDistance() const
{
    return mEyeToLensDistance;
}

float VrParams::visibleViewportSize() const
{
    return mVisibleViewportSize;
}

float VrParams::fovY() const
{
    return mFovY;
}

void VrParams::setCoefficients(float *coefficients) {
    mCoefficients[0] = coefficients[0];
    mCoefficients[1] = coefficients[1];
}

float *VrParams::getCoefficients() {
    return mCoefficients;
}

float VrParams::distortionFactor(float radius) {
    float rSq = radius * radius;
    return 1.0F + mCoefficients[0] * rSq + mCoefficients[1] * rSq * rSq;
}

float VrParams::distort(float radius) {
    return radius * distortionFactor(radius);
}

float VrParams::distortInverse(float radius) {
    float r0 = radius / 0.9F;
    float r1 = radius * 0.9F;

    float dr0 = radius - distort(r0);
    while (abs(r1 - r0) > 1.0E-4D) {
        float dr1 = radius - distort(r1);
        float r2 = r1 - dr1 * ((r1 - r0) / (dr1 - dr0));
        r0 = r1;
        r1 = r2;
        dr0 = dr1;
    }
    return r1;
}

float VrParams::abs(float val)
{
    if(val<0) return -val;
    return val;
}


