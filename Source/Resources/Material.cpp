//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "imgui.h"

#include "Material.h"
#include "Renderer/Uniforms.h"
#include "Resources/Texture.h"

namespace fw {

Material::Material(const char* name, ShaderProgram* pShader, Texture* pTextureColor, color4f color, bool hasAlpha, vec4 uvScaleOffset)
    : Resource( name )
    , m_pShader( pShader )
    , m_pTextureColor( pTextureColor )
    , m_UVScaleOffset( uvScaleOffset )
    , m_Color( color )
{
    if( hasAlpha == false )
    {
        m_ColorBlendEquation = BlendEquation::None;
    }
}

Material::Material(const char* name, ShaderProgram* pShader, Texture* pTextureColor, color4f color, bool hasAlpha)
    : Material( name, pShader, pTextureColor, color, hasAlpha, vec4(1,1,0,0) )
{
}

Material::~Material()
{
}

void Material::Enable(const Uniforms* pUniforms) const
{
    // Textures.
    if( m_pTextureColor )
    {
        bgfx::setTexture( 0, pUniforms->m_Map.at("u_TextureColor"), m_pTextureColor->GetHandle() );
    }

    if( m_pTextureNoise )
    {
        bgfx::setTexture( 1, pUniforms->m_Map.at("u_TextureNoise"), m_pTextureNoise->GetHandle() );
    }

    // UV scale and offset.
    bgfx::setUniform( pUniforms->m_Map.at( "u_UVScaleOffset" ), &m_UVScaleOffset.x );

    // Vertex Colors.
    bgfx::setUniform( pUniforms->m_Map.at( "u_DiffuseColor" ), &m_Color.r );

    bgfx::setUniform( pUniforms->m_Map.at( "u_ControlPerc" ), &m_ControlPerc.x );
}

uint64_t c_BlendEquationConversions[6] =
{
    0,
    BGFX_STATE_BLEND_EQUATION_ADD,
    BGFX_STATE_BLEND_EQUATION_SUB,
    BGFX_STATE_BLEND_EQUATION_REVSUB,
    BGFX_STATE_BLEND_EQUATION_MIN,
    BGFX_STATE_BLEND_EQUATION_MAX,
};

uint64_t c_BlendFuncConversions[11] =
{
    BGFX_STATE_BLEND_ZERO,
    BGFX_STATE_BLEND_ONE,
    BGFX_STATE_BLEND_SRC_COLOR,
    BGFX_STATE_BLEND_INV_SRC_COLOR,
    BGFX_STATE_BLEND_SRC_ALPHA,
    BGFX_STATE_BLEND_INV_SRC_ALPHA,
    BGFX_STATE_BLEND_DST_ALPHA,
    BGFX_STATE_BLEND_INV_DST_ALPHA,
    BGFX_STATE_BLEND_DST_COLOR,
    BGFX_STATE_BLEND_INV_DST_COLOR,
    BGFX_STATE_BLEND_SRC_ALPHA_SAT,
};

uint64_t Material::GetBGFXRenderState() const
{
    uint64_t bgfxRenderState = 0;
    
    if( m_RenderStateFlags & RenderStateFlag::WriteR ) bgfxRenderState |= BGFX_STATE_WRITE_R;
    if( m_RenderStateFlags & RenderStateFlag::WriteG ) bgfxRenderState |= BGFX_STATE_WRITE_G;
    if( m_RenderStateFlags & RenderStateFlag::WriteB ) bgfxRenderState |= BGFX_STATE_WRITE_B;
    if( m_RenderStateFlags & RenderStateFlag::WriteA ) bgfxRenderState |= BGFX_STATE_WRITE_A;
    if( m_RenderStateFlags & RenderStateFlag::CullCW ) bgfxRenderState |= BGFX_STATE_CULL_CW;
    if( m_RenderStateFlags & RenderStateFlag::CullCCW ) bgfxRenderState |= BGFX_STATE_CULL_CCW;

    bgfxRenderState |= (uint64)BGFX_STATE_DEPTH_TEST_LESS + (uint64)m_DepthTest;

    bgfxRenderState |= GetBGFXAlphaState();

    return bgfxRenderState;
}

uint64_t Material::GetBGFXAlphaState() const
{
    if( m_ColorBlendEquation == BlendEquation::None )
        return 0;

    uint64_t bgfxBlendState = 0;

    if( m_AlphaBlendEquation == BlendEquation::SameAsColor )
    {
        assert( m_ColorBlendEquation < BlendEquation::SameAsColor );

        uint64_t colorBlendEquation = c_BlendEquationConversions[(int)m_ColorBlendEquation];
        uint64_t srcColorBlendFunc = c_BlendFuncConversions[(int)m_SrcColorBlendFunc];
        uint64_t dstColorBlendFunc = c_BlendFuncConversions[(int)m_DstColorBlendFunc];

        bgfxBlendState = BGFX_STATE_BLEND_EQUATION( colorBlendEquation ) | BGFX_STATE_BLEND_FUNC( srcColorBlendFunc, dstColorBlendFunc );
    }
    else
    {
        assert( m_ColorBlendEquation < BlendEquation::SameAsColor );
        assert( m_AlphaBlendEquation < BlendEquation::SameAsColor );

        uint64_t colorBlendEquation = c_BlendEquationConversions[(int)m_ColorBlendEquation];
        uint64_t alphaBlendEquation = c_BlendEquationConversions[(int)m_AlphaBlendEquation];
        uint64_t srcColorBlendFunc = c_BlendFuncConversions[(int)m_SrcColorBlendFunc];
        uint64_t dstColorBlendFunc = c_BlendFuncConversions[(int)m_DstColorBlendFunc];
        uint64_t srcAlphaBlendFunc = c_BlendFuncConversions[(int)m_SrcAlphaBlendFunc];
        uint64_t dstAlphaBlendFunc = c_BlendFuncConversions[(int)m_DstAlphaBlendFunc];

        bgfxBlendState = BGFX_STATE_BLEND_EQUATION_SEPARATE( colorBlendEquation, alphaBlendEquation )
            | BGFX_STATE_BLEND_FUNC_SEPARATE( srcColorBlendFunc, srcAlphaBlendFunc, dstColorBlendFunc, dstAlphaBlendFunc );
    }

    return bgfxBlendState;
}

void Material::Editor_DisplayProperties()
{
    ImGui::Text( "Material: %s", m_Name );
    ImGui::Separator();

    ImGui::ColorEdit4( "Color", &m_Color.r );
    ImGui::SliderFloat( "Control Perc", &m_ControlPerc.x, 0.0f, 1.0f );
}

} // namespace fw
