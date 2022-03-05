//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "bgfx/platform.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace fw {

class ShaderProgram;

class Mesh
{
public:
    Mesh(const bgfx::VertexLayout& vertexFormat, const void* verts, uint32_t vertsSize, const void* indices, uint32_t indicesSize);
    virtual ~Mesh();

    void Create(const bgfx::VertexLayout& vertexFormat, const void* verts, uint32_t vertsSize, const void* indices, uint32_t indicesSize);

    void Draw(const ShaderProgram* pShader, const mat4* worldMat, bool blend);

protected:
    bgfx::VertexBufferHandle m_VBO;
    bgfx::IndexBufferHandle m_IBO;
    int m_NumVerts = 0;
    int m_NumIndices = 0;
};

} // namespace fw
