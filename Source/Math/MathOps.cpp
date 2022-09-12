//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "MathOps.h"
#include "Vector.h"

namespace fw {

// From: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect.
// Based on an algorithm in Andre LeMothe's "Tricks of the Windows Game Programming Gurus".
//bool DoLineSegmentsIntersect(vec2 p0, vec2 p1, vec2 p2, vec2 p3, vec2 *out)
//{
//    vec2 s1, s2;
//    s1.x = p1.x - p0.x;     s1.y = p1.y - p0.y;
//    s2.x = p3.x - p2.x;     s2.y = p3.y - p2.y;
//
//    float s, t;
//    s = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / (-s2.x * s1.y + s1.x * s2.y);
//    t = ( s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / (-s2.x * s1.y + s1.x * s2.y);
//
//    if( s >= 0 && s <= 1 && t >= 0 && t <= 1 )
//    {
//        // Collision detected.
//        if( out )
//        {
//            out->x = p0.x + (t * s1.x);
//            out->y = p0.y + (t * s1.y);
//        }
//        return true;
//    }
//
//    return false; // No collision.
//}

// From: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect.
// Delayed division.
bool DoLineSegmentsIntersect(vec2 p0, vec2 p1, vec2 p2, vec2 p3, vec2 *out)
{
    vec2 s02, s10, s32;
    float s_numer, t_numer, denom, t;
    s10.x = p1.x - p0.x;
    s10.y = p1.y - p0.y;
    s32.x = p3.x - p2.x;
    s32.y = p3.y - p2.y;

    denom = s10.x * s32.y - s32.x * s10.y;
    if( denom == 0 )
        return false; // Collinear.
    bool denomPositive = denom > 0;

    s02.x = p0.x - p2.x;
    s02.y = p0.y - p2.y;
    s_numer = s10.x * s02.y - s10.y * s02.x;
    if( (s_numer < 0) == denomPositive )
        return false; // No collision.

    t_numer = s32.x * s02.y - s32.y * s02.x;
    if( (t_numer < 0) == denomPositive )
        return false; // No collision.

    if( ((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive) )
        return false; // No collision.
    
    // Collision detected.
    t = t_numer / denom;
    if( out != nullptr )
    {
        out->x = p0.x + (t * s10.x);
        out->y = p0.y + (t * s10.y);
    }

    return true;
}

} // namespace fw
