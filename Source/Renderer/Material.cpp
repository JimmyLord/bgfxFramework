//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "Material.h"
#include "Renderer/Uniforms.h"
#include "Renderer/Texture.h"

namespace fw {

Material::Material(ShaderProgram* pShader, Texture* pTexture, color4f color, bool hasAlpha, vec4 uvScaleOffset)
    : m_pShader( pShader )
    , m_pTexture( pTexture )
    , m_UVScaleOffset( uvScaleOffset )
    , m_Color( color )
{
    if( hasAlpha )
        m_AlphaState = BGFX_STATE_BLEND_EQUATION_ADD | BGFX_STATE_BLEND_FUNC( BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA );
    else
        m_AlphaState = 0;
}

Material::Material(ShaderProgram* pShader, Texture* pTexture, color4f color, bool hasAlpha)
    : Material( pShader, pTexture, color, hasAlpha, vec4(1,1,0,0) )
{
}

Material::~Material()
{
}

void Material::Enable(const Uniforms* pUniforms) const
{
    // Textures.
    if( m_pTexture )
    {
        bgfx::setTexture( 0, pUniforms->m_Map.at("u_TextureColor"), m_pTexture->GetHandle() );
    }

    // UV scale and offset.
    bgfx::setUniform( pUniforms->m_Map.at( "u_UVScaleOffset" ), &m_UVScaleOffset.x );

    // Vertex Colors.
    bgfx::setUniform( pUniforms->m_Map.at( "u_DiffuseColor" ), &m_Color.r );
}

} // namespace fw
