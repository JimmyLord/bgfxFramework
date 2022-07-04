//
// Copyright (c) 2012-2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

namespace fw {

#define PI 3.1415926535897932384626433832795f

const float FEQUALEPSILON = 0.00001f;

inline bool fequal(const float32 a, const float32 b, const float32 epsilon = FEQUALEPSILON)
{
    return fabs(a - b) <= epsilon;
}

inline bool fnotequal(const float32 a, const float32 b, const float32 epsilon = FEQUALEPSILON)
{
    return fabs(a - b) > epsilon;
}

inline float degreesToRads(const float32 number)
{
    return number * PI / 180.0f;
}

inline float radsToDegrees(const float32 number)
{
    return number * 180.0f / PI;
}

template <class MyType> void MySwap(MyType& v1, MyType& v2)
{
    MyType temp = v1;
    v1 = v2;
    v2 = temp;
}

template <class MyType> void MyClamp(MyType& value, MyType min, MyType max)
{
    if( value < min )
        value = min;

    if( value > max )
        value = max;
}

template <class MyType> MyType MyClamp_Return(MyType value, MyType min, MyType max)
{
    MyType temp = value;

    if( temp < min )
        temp = min;

    if( temp > max )
        temp = max;

    return temp;
}

template <class MyType> void IncreaseIfBigger(MyType& value, MyType newValue)
{
    if( newValue > value )
        value = newValue;
}

template <class MyType> void DecreaseIfLower(MyType& value, MyType newValue, bool treatZeroAsInfinite = false)
{
    if( treatZeroAsInfinite && value == 0 )
        value = newValue;

    if( newValue < value )
        value = newValue;
}

static inline bool IsPowerOfTwo(uint64 x)
{
    // If positive and the following bitwise compare: 1000 & 0111 == 0
    return (x>0) && ((x&(x-1)) == 0);
}

} // namespace fw
