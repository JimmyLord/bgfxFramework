//
// Copyright (c) 2012-2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "Matrix.h"

namespace fw {

void mat4::SetIdentity()
{
    m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
    m11 = m22 = m33 = m44 = 1;
}

void mat4::SetAxesView(const vec3& right, const vec3& up, const vec3& at, const vec3& pos)
{
    m11 = right.x; m21 = right.y; m31 = right.z; m41 = pos.x;
    m12 = up.x;    m22 = up.y;    m32 = up.z;    m42 = pos.y;
    m13 = at.x;    m23 = at.y;    m33 = at.z;    m43 = pos.z;
    m14 = 0;       m24 = 0;       m34 = 0;       m44 = 1;
}

void mat4::SetAxesWorld(const vec3& right, const vec3& up, const vec3& at, const vec3& pos)
{
    m11 = right.x; m21 = up.x; m31 = at.x; m41 = pos.x;
    m12 = right.y; m22 = up.y; m32 = at.y; m42 = pos.y;
    m13 = right.z; m23 = up.z; m33 = at.z; m43 = pos.z;
    m14 = 0;       m24 = 0;    m34 = 0;    m44 = 1;
}

void mat4::SetTranslation(vec3 pos)
{
    m41 = pos.x;
    m42 = pos.y;
    m43 = pos.z;
}

void mat4::SetTranslation(float x, float y, float z)
{
    m41 = x;
    m42 = y;
    m43 = z;
}

void mat4::CreateScale(float scale)
{
    m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
    m11 = m22 = m33 = scale;
    m44 = 1;
}

void mat4::CreateScale(float x, float y, float z)
{
    m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
    m11 = x;
    m22 = y;
    m33 = z;
    m44 = 1;
}

void mat4::CreateScale(vec3 scale)
{
    m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0;
    m11 = scale.x;
    m22 = scale.y;
    m33 = scale.z;
    m44 = 1;
}

void mat4::CreateRotation(vec3 eulerdegrees)
{
    SetIdentity();
    Rotate(eulerdegrees.z, 0, 0, 1); // roll
    Rotate(eulerdegrees.x, 1, 0, 0); // pitch
    Rotate(eulerdegrees.y, 0, 1, 0); // yaw
}

void mat4::CreateTranslation(float x, float y, float z)
{
    m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = 0;
    m11 = m22 = m33 = m44 = 1;
    m41 = x;
    m42 = y;
    m43 = z;
}

void mat4::CreateTranslation(vec3 pos)
{
    m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = 0;
    m11 = m22 = m33 = m44 = 1;
    m41 = pos.x;
    m42 = pos.y;
    m43 = pos.z;
}

void mat4::CreateSRT(float scale, vec3 rot, vec3 pos)
{
    SetIdentity();
    Scale(scale);
    Rotate(rot.z, 0, 0, 1); // roll
    Rotate(rot.x, 1, 0, 0); // pitch
    Rotate(rot.y, 0, 1, 0); // yaw
    Translate(pos.x, pos.y, pos.z);
}

void mat4::CreateSRT(vec3 scale, vec3 rot, vec3 pos)
{
    CreateScale(scale.x, scale.y, scale.z);
    Rotate(rot.z, 0, 0, 1); // roll
    Rotate(rot.x, 1, 0, 0); // pitch
    Rotate(rot.y, 0, 1, 0); // yaw
    Translate(pos.x, pos.y, pos.z);
}

void mat4::Scale(float scale)
{
    m11 *= scale; m21 *= scale; m31 *= scale; m41 *= scale;
    m12 *= scale; m22 *= scale; m32 *= scale; m42 *= scale;
    m13 *= scale; m23 *= scale; m33 *= scale; m43 *= scale;
}

void mat4::Scale(float sx, float sy, float sz)
{
    m11 *= sx; m21 *= sx; m31 *= sx; m41 *= sx;
    m12 *= sy; m22 *= sy; m32 *= sy; m42 *= sy;
    m13 *= sz; m32 *= sz; m33 *= sz; m43 *= sz;
}

void mat4::Scale(vec3 scale)
{
    m11 *= scale.x; m21 *= scale.x; m31 *= scale.x; m41 *= scale.x;
    m12 *= scale.y; m22 *= scale.y; m32 *= scale.y; m42 *= scale.y;
    m13 *= scale.z; m32 *= scale.z; m33 *= scale.z; m43 *= scale.z;
}

void mat4::Rotate(float angle, float x, float y, float z)
{
    float sinAngle, cosAngle;
    float mag = sqrtf(x * x + y * y + z * z);

    sinAngle = sinf(angle * PI / 180.0f);
    cosAngle = cosf(angle * PI / 180.0f);
    if( mag > 0.0f )
    {
        float xx, yy, zz, xy, yz, zx, xs, ys, zs;
        float oneMinusCos;

        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0f - cosAngle;

        mat4 rotMat;
        rotMat.m11 = (oneMinusCos * xx) + cosAngle;
        rotMat.m12 = (oneMinusCos * xy) - zs;
        rotMat.m13 = (oneMinusCos * zx) + ys;
        rotMat.m14 = 0.0f;

        rotMat.m21 = (oneMinusCos * xy) + zs;
        rotMat.m22 = (oneMinusCos * yy) + cosAngle;
        rotMat.m23 = (oneMinusCos * yz) - xs;
        rotMat.m24 = 0.0f;

        rotMat.m31 = (oneMinusCos * zx) - ys;
        rotMat.m32 = (oneMinusCos * yz) + xs;
        rotMat.m33 = (oneMinusCos * zz) + cosAngle;
        rotMat.m34 = 0.0f;

        rotMat.m41 = 0.0f;
        rotMat.m42 = 0.0f;
        rotMat.m43 = 0.0f;
        rotMat.m44 = 1.0f;

        *this = rotMat * *this;
    }
}

void mat4::TranslatePreRotScale(vec3 translate)
{
    m41 += m11 * translate.x + m21 * translate.y + m31 * translate.z;
    m42 += m12 * translate.x + m22 * translate.y + m32 * translate.z;
    m43 += m13 * translate.x + m23 * translate.y + m33 * translate.z;
    m44 += m14 * translate.x + m24 * translate.y + m34 * translate.z;
}

void mat4::TranslatePreRotScale(float tx, float ty, float tz)
{
    m41 += m11 * tx + m21 * ty + m31 * tz;
    m42 += m12 * tx + m22 * ty + m32 * tz;
    m43 += m13 * tx + m23 * ty + m33 * tz;
    m44 += m14 * tx + m24 * ty + m34 * tz;
}

void mat4::Translate(vec3 pos)
{
    m41 += pos.x;
    m42 += pos.y;
    m43 += pos.z;
}

void mat4::Translate(float x, float y, float z)
{
    m41 += x;
    m42 += y;
    m43 += z;
}

void mat4::CreateFrustum(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;

    assert(nearZ > 0.0f && farZ > 0.0f && deltaX > 0.0f && deltaY > 0.0f && deltaZ > 0.0f);

    m11 = 2.0f * nearZ / deltaX;
    m12 = m13 = m14 = 0.0f;

    m22 = 2.0f * nearZ / deltaY;
    m21 = m23 = m24 = 0.0f;

    m31 = (right + left) / deltaX;
    m32 = (top + bottom) / deltaY;
#if MYFW_RIGHTHANDED
    m33 = -(nearZ + farZ) / deltaZ;
    m34 = -1.0f;
#else
    m33 = (nearZ + farZ) / deltaZ;
    m34 = 1.0f;
#endif

    m43 = -2.0f * nearZ * farZ / deltaZ;
    m41 = m42 = m44 = 0.0f;
}

void mat4::CreatePerspectiveVFoV(float vertfovdegrees, float aspect, float nearZ, float farZ)
{
    float frustumRight, frustumTop;

    frustumTop = tanf(vertfovdegrees / 2 * PI / 180.0f) * nearZ;
    frustumRight = frustumTop * aspect;

    CreateFrustum(-frustumRight, frustumRight, -frustumTop, frustumTop, nearZ, farZ);
}

void mat4::CreatePerspectiveHFoV(float horfovdegrees, float aspect, float nearZ, float farZ)
{
    float frustumRight, frustumTop;

    frustumRight = tanf(horfovdegrees / 2 * PI / 180.0f) * nearZ;
    frustumTop = frustumRight / aspect;

    CreateFrustum(-frustumRight, frustumRight, -frustumTop, frustumTop, nearZ, farZ);
}

void mat4::CreateOrtho(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float deltaX = (right - left);
    float deltaY = (top - bottom);
    float deltaZ = (farZ - nearZ);

    assert((deltaX != 0.0f) && (deltaY != 0.0f) && (deltaZ != 0.0f));

    m11 = 2.0f / deltaX;
    m12 = m13 = m14 = 0;

    m22 = 2.0f / deltaY;
    m21 = m23 = m24 = 0;

#if MYFW_RIGHTHANDED
    m33 = -2.0f / deltaZ;
#else
    m33 = 2.0f / deltaZ;
#endif
    m31 = m32 = m34 = 0;

    m41 = -(right + left) / deltaX;
    m42 = -(top + bottom) / deltaY;
    m43 = -(farZ + nearZ) / deltaZ;
    m44 = 1;
}

void mat4::CreateLookAtView(const vec3& eye, const vec3& up, const vec3& at)
{
#if MYFW_RIGHTHANDED
    vec3 zaxis = (eye - at).Normalize();
#else
    vec3 zaxis = (at - eye).Normalize();
#endif
    vec3 xaxis = (up.Cross(zaxis)).Normalize();
    vec3 yaxis = zaxis.Cross(xaxis);

    vec3 pos = vec3(-xaxis.Dot(eye), -yaxis.Dot(eye), -zaxis.Dot(eye));

    SetAxesView(xaxis, yaxis, zaxis, pos);
}

void mat4::CreateLookAtWorld(const vec3& objpos, const vec3& up, const vec3& at)
{
    vec3 zaxis = (at - objpos).Normalize();
    vec3 xaxis = (up.Cross(zaxis)).Normalize();
    vec3 yaxis = zaxis.Cross(xaxis);

    SetAxesWorld(xaxis, yaxis, zaxis, objpos);
}

vec3 mat4::GetEulerAngles()
{
    // from http://www.geometrictools.com/Documentation/EulerAngles.pdf and adapted to fit

    //if( m32 < +1 )
    //{
    //    if( m32 > -1 )
    //    {
    //        float x = asin( m32 );
    //        float y = atan2( -m31, m33 );
    //        float z = atan2( -m12, m22 );
    //        return vec3( x, y, z );
    //    }
    //    else // m32 = -1
    //    {
    //        // not a unique solution: thetaz - thetay = atan2(-m21,m11)
    //        float x = pi/2;
    //        float y = atan2( m21, m11 );
    //        float z = 0;
    //        return vec3( x, y, z );
    //    }
    //}
    //else // m32 = +1
    //{
    //    // not a unique solution: thetaz + thetay = atan2(-m21,m11)
    //    float x = -pi/2;
    //    float y = -atan2( m21, m11 );
    //    float z = 0;
    //    return vec3( x, y, z );
    //}

    // rearranged from above and using FEQUALEPSILON to give special cases more chance of hitting
    if( m32 > 1.0f - FEQUALEPSILON ) // Not a unique solution: thetaZ - thetaY = atan2( -m21, m11 )
    {
        float x = PI / 2;
        float y = atan2f(m21, m11);
        float z = 0.0f;
        return vec3(x, y, z) * 180.0f / PI;
    }
    else if( m32 < -1.0f + FEQUALEPSILON ) // Not a unique solution: thetaZ + thetaY = atan2( -m21, m11 )
    {
        float x = -PI / 2;
        float y = -atan2f(m21, m11);
        float z = 0.0f;
        return vec3(x, y, z) * 180.0f / PI;
    }
    else
    {
        float x = asinf(m32);
        float y = atan2f(-m31, m33);
        float z = atan2f(-m12, m22);
        return vec3(x, y, z) * 180.0f / PI;
    }
}

vec3 mat4::GetScale()
{
    vec3 scale;
    scale.x = vec3(m11, m12, m13).Length();
    scale.y = vec3(m21, m22, m23).Length();
    scale.z = vec3(m31, m32, m33).Length();
    return scale;
}

vec3 mat4::GetUp()
{
    return vec3(m21, m22, m23);
}

vec3 mat4::GetRight()
{
    return vec3(m11, m12, m13);
}

vec3 mat4::GetAt()
{
    return vec3(m31, m32, m33);
}

} // namespace fw
