//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "VertexFormats.h"

static const VF_PosColor g_TriangleVerts[] =
{
    { vec3( 0.0f, 0.5f, 0.0f), 0x000000ff }, // Top
    { vec3(-0.5f,-0.5f, 0.0f), 0x0000ff00 }, // Bottom-left
    { vec3( 0.5f,-0.5f, 0.0f), 0x00ff0000 }, // Bottom-right
};

static const uint16 g_TriangleIndices[] =
{
    0, 2, 1,
};

fw::Mesh* CreateTriangle()
{
    return new fw::Mesh( VF_PosColor::format, g_TriangleVerts, sizeof(g_TriangleVerts), g_TriangleIndices, sizeof(g_TriangleIndices) );
}
