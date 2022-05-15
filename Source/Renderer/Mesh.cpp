//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "bgfx/platform.h"

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Math/Matrix.h"
#include "Renderer/Material.h"
#include "Utility/Utility.h"

namespace fw {

Mesh::Mesh(const bgfx::VertexLayout& vertexFormat, const void* verts, uint32_t vertsSize, const void* indices, uint32_t indicesSize)
{
    Create( vertexFormat, verts, vertsSize, indices, indicesSize );
}

Mesh::~Mesh()
{
    bgfx::destroy( m_VBO );
    bgfx::destroy( m_IBO );
}

void Mesh::Create(const bgfx::VertexLayout& vertexFormat, const void* verts, uint32_t vertsSize, const void* indices, uint32_t indicesSize)
{
    m_VBO = bgfx::createVertexBuffer( bgfx::makeRef(verts, vertsSize), vertexFormat );
    m_IBO = bgfx::createIndexBuffer( bgfx::makeRef(indices, indicesSize) );
}

void Mesh::Draw(const Uniforms* pUniforms, const Material* pMaterial, const mat4* worldMat)
{
    // Set vertex and index buffer.
    bgfx::setVertexBuffer( 0, m_VBO );
    bgfx::setIndexBuffer( m_IBO );

    // Setup the material's uniforms.
    pMaterial->Enable( pUniforms );

    // Set render states.
    uint64_t state = 0
        | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CCW
        | BGFX_STATE_MSAA
        ;

    if( pMaterial->GetHasAlpha() )
    {
        state = state
            | BGFX_STATE_BLEND_EQUATION_ADD
            | BGFX_STATE_BLEND_FUNC( BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA )
            ;
    }

    bgfx::setState( state );

    if( worldMat )
    {
        bgfx::setTransform( &worldMat->m11 );
    }

    // Submit primitive for rendering to view 0.
    bgfx::submit( 0, pMaterial->GetShader()->GetProgram() );
}

} // namespace fw