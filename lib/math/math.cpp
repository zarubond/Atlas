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

#include "math.h"

Math::Math()
{
}

float fabs(float val)
{
    if(val<0)
        return -val;
    return val;
}
//http://my.safaribooksonline.com/book/programming/opengl/9780321563835/gl-half-float-oes/app01lev1sec2

// -15 stored using a single precision bias of 127
const unsigned int  HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP = 0x38000000;
// max exponent value in single precision that will be converted
// to Inf or Nan when stored as a half-float
const unsigned int  HALF_FLOAT_MAX_BIASED_EXP_AS_SINGLE_FP_EXP = 0x47800000;

// 255 is the max exponent biased value
const unsigned int  FLOAT_MAX_BIASED_EXP = (0xFF << 23);

const unsigned int  HALF_FLOAT_MAX_BIASED_EXP = (0x1F << 10);


hfloat Math::makeHFloat(float f)
{
    //unsigned int    x = *(unsigned int *)f;
    unsigned int    x = *(unsigned int *)&f;
    /*unsigned int x;
    char *a =(char*)&x, *b=(char*)&f;
    a[0]=b[0];
    a[1]=b[1];
    a[2]=b[2];
    a[3]=b[3];*/

    unsigned int    sign = (unsigned short)(x >> 31);
    unsigned int    mantissa;
    unsigned int    exp;
    hfloat          hf;

    // get mantissa
    mantissa = x & ((1 << 23) - 1);
    // get exponent bits
    exp = x & FLOAT_MAX_BIASED_EXP;
    if (exp >= HALF_FLOAT_MAX_BIASED_EXP_AS_SINGLE_FP_EXP)
    {
        // check if the original single precision float number is a NaN
        if (mantissa && (exp == FLOAT_MAX_BIASED_EXP))
        {
            // we have a single precision NaN
            mantissa = (1 << 23) - 1;
        }
        else
        {
            // 16-bit half-float representation stores number as Inf
            mantissa = 0;
        }
        hf = (((hfloat)sign) << 15) | (hfloat)(HALF_FLOAT_MAX_BIASED_EXP) |
              (hfloat)(mantissa >> 13);
    }
    // check if exponent is <= -15
    else if (exp <= HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP)
    {

        // store a denorm half-float value or zero
        exp = (HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP - exp) >> 23;
        mantissa >>= (14 + exp);

        hf = (((hfloat)sign) << 15) | (hfloat)(mantissa);
    }
    else
    {
        hf = (((hfloat)sign) << 15) |
              (hfloat)((exp - HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP) >> 13) |
              (hfloat)(mantissa >> 13);
    }

    return hf;
}

float Math::makeFloat(hfloat hf)
{
    unsigned int    sign = (unsigned int)(hf >> 15);
    unsigned int    mantissa = (unsigned int)(hf & ((1 << 10) - 1));
    unsigned int    exp = (unsigned int)(hf & HALF_FLOAT_MAX_BIASED_EXP);
    unsigned int    f;

    if (exp == HALF_FLOAT_MAX_BIASED_EXP)
    {
        // we have a half-float NaN or Inf
        // half-float NaNs will be converted to a single precision NaN
        // half-float Infs will be converted to a single precision Inf
        exp = FLOAT_MAX_BIASED_EXP;
        if (mantissa)
            mantissa = (1 << 23) - 1;    // set all bits to indicate a NaN
    }
    else if (exp == 0x0)
    {
        // convert half-float zero/denorm to single precision value
        if (mantissa)
        {
           mantissa <<= 1;
           exp = HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP;
           // check for leading 1 in denorm mantissa
           while ((mantissa & (1 << 10)) == 0)
           {
               // for every leading 0, decrement single precision exponent by 1
               // and shift half-float mantissa value to the left
               mantissa <<= 1;
               exp -= (1 << 23);
            }
            // clamp the mantissa to 10-bits
            mantissa &= ((1 << 10) - 1);
            // shift left to generate single-precision mantissa of 23-bits
            mantissa <<= 13;
        }
    }
    else
    {
        // shift left to generate single-precision mantissa of 23-bits
        mantissa <<= 13;
        // generate single precision biased exponent value
        exp = (exp << 13) + HALF_FLOAT_MIN_BIASED_EXP_AS_SINGLE_FP_EXP;
    }

    f = (sign << 31) | exp | mantissa;
    return *((float *)&f);
}





/*
void printGLM(const glm::mat4 & m)
{
    std::cout<<m[0][0]<<" "<<m[0][1]<<" "<<m[0][2]<<" "<<m[0][3]<<std::endl;
    std::cout<<m[1][0]<<" "<<m[1][1]<<" "<<m[1][2]<<" "<<m[1][3]<<std::endl;
    std::cout<<m[2][0]<<" "<<m[2][1]<<" "<<m[2][2]<<" "<<m[2][3]<<std::endl;
    std::cout<<m[3][0]<<" "<<m[3][1]<<" "<<m[3][2]<<" "<<m[3][3]<<std::endl;
}

glm::mat4 matrixGLM(const Matrix4f & mat)
{
    glm::mat4 m;
    m[0][0]=mat[0];
    m[0][1]=mat[1];
    m[0][2]=mat[2];
    m[0][3]=mat[3];

    m[1][0]=mat[4];
    m[1][1]=mat[5];
    m[1][2]=mat[6];
    m[1][3]=mat[7];

    m[2][0]=mat[8];
    m[2][1]=mat[9];
    m[2][2]=mat[10];
    m[2][3]=mat[11];

    m[3][0]=mat[12];
    m[3][1]=mat[13];
    m[3][2]=mat[14];
    m[3][3]=mat[15];
    return m;
}

void printGLM4(const glm::vec4 & v)
{
    std::cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<" "<<v[3]<<std::endl;
}

void printGLM3(const glm::vec3 & v)
{
    std::cout<<v[0]<<" "<<v[1]<<" "<<v[2]<<std::endl;
}
*/
