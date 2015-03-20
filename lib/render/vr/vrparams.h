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

#ifndef VRPARAMS_H
#define VRPARAMS_H

class VrParams
{
public:
    VrParams();

    float interpupillaryDistance() const;
    float verticalDistanceToLensCenter() const;
    float lensDiameter() const;
    float screenToLensDistance() const;
    float eyeToLensDistance() const;
    float visibleViewportSize() const;
    float fovY() const;

//lense distortion
    float distortInverse(float radius);
    float distort(float radius);
    float distortionFactor(float radius);
    float *getCoefficients();
    void setCoefficients(float *coefficients);

protected:
    float abs(float val);

    float mInterpupillaryDistance;
    float mVerticalDistanceToLensCenter;
    float mLensDiameter;
    float mScreenToLensDistance;
    float mEyeToLensDistance;
    float mVisibleViewportSize;
    float mFovY;
    float mCoefficients[2];
};

#endif // VRPARAMS_H
