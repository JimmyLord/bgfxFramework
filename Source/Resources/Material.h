//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math/Vector.h"
#include "Resources/Resource.h"

namespace fw {

class ShaderProgram;
class Texture;
class Uniforms;

class Material : public Resource
{
public:
    enum class BlendEquation // Change c_BlendEquationConversions if new values added.
    {
        None,
        Add,
        Sub,
        RevSub,
        Min,
        Max,
        SameAsColor, // Needs to stay at end of list.
    };

    enum class BlendFunc // Change c_ColorBlendFuncConversions and c_AlphaBlendFuncConversions if new values added.
    {
        Zero,
        One,
        SrcColor,
        InvSrcColor,
        SrcAlpha,
        InvSrcAlpha,
        DstAlpha,
        InvDstAlpha,
        DstColor,
        InvDstColor,
        SrcAlphaSaturate,
        SameAsColor, // Needs to stay at end of list.
    };

    enum RenderStateFlag
    {
        WriteR              = 1 << 0,
        WriteG              = 1 << 1,
        WriteB              = 1 << 2,
        WriteA              = 1 << 3,
        WriteZ              = 1 << 4,
        CullCW              = 1 << 6,
        CullCCW             = 1 << 7,
        WriteRGB            = WriteR + WriteG + WriteB,
        WriteRGBA           = WriteR + WriteG + WriteB + WriteA,
    };

    enum class DepthTest // Order must match bgfx order starting at BGFX_STATE_DEPTH_TEST_LESS.
    {
        Less,
        LEqual,
        Equal,
        GEqual,
        Greater,
        NotEqual,
        Never,
        Always,
    };

public:
    Material(const char* name, ShaderProgram* pShader, Texture* pTextureColor, color4f color, bool hasAlpha);
    Material(const char* name, ShaderProgram* pShader, Texture* pTextureColor, color4f color, bool hasAlpha, vec4 uvScaleOffset);
    virtual ~Material();

    void Enable(const Uniforms* pUniforms) const;

    // Getters.
    ShaderProgram* GetShader() const { return m_pShader; }
    Texture* GetTextureColor() const { return m_pTextureColor; }
    Texture* GetTextureNoise() const { return m_pTextureNoise; }
    vec4 GetUVScaleOffset() { return m_UVScaleOffset; }
    color4f GetColor() const { return m_Color; }
    float GetControlPerc() const { return m_ControlPerc.x; }
    uint64_t GetBGFXRenderState() const;
    uint64_t GetBGFXAlphaState() const;

    // Setters.
    void SetShader(ShaderProgram* pShader) { m_pShader = pShader; }
    void SetTextureColor(Texture* pTexture) { m_pTextureColor = pTexture; }
    void SetTextureNoise(Texture* pTexture) { m_pTextureNoise = pTexture; }
    void SetUVScaleOffset(vec2 uvScale, vec2 uvOffset) { m_UVScaleOffset.Set( uvScale, uvOffset ); }
    void SetColor(color4f color) { m_Color = color; }
    void SetControlPerc(float perc) { m_ControlPerc.x = perc; }
    void SetColorBlend(BlendEquation eq, BlendFunc srcFunc, BlendFunc dstFunc) { m_ColorBlendEquation = eq; m_SrcColorBlendFunc = srcFunc; m_DstColorBlendFunc = dstFunc; }
    void SetAlphaBlend(BlendEquation eq = BlendEquation::SameAsColor, BlendFunc srcFunc = BlendFunc::SameAsColor, BlendFunc dstFunc = BlendFunc::SameAsColor) { m_AlphaBlendEquation = eq; m_SrcAlphaBlendFunc = srcFunc; m_DstAlphaBlendFunc = dstFunc; }
    void SetRenderStateFlags(uint32 flags) { m_RenderStateFlags = flags; }
    void SetDepthTest(DepthTest setting) { m_DepthTest = setting; }

    // Editor.
    virtual void Editor_DisplayProperties() override;

protected:
    ShaderProgram* m_pShader = nullptr;
    Texture* m_pTextureColor = nullptr;
    Texture* m_pTextureNoise = nullptr;
    vec4 m_UVScaleOffset;
    color4f m_Color;
    vec4 m_ControlPerc;

    BlendEquation m_ColorBlendEquation = BlendEquation::Add;
    BlendEquation m_AlphaBlendEquation = BlendEquation::SameAsColor;
    BlendFunc m_SrcColorBlendFunc = BlendFunc::SrcAlpha;
    BlendFunc m_DstColorBlendFunc = BlendFunc::InvSrcAlpha;
    BlendFunc m_SrcAlphaBlendFunc = BlendFunc::SameAsColor;
    BlendFunc m_DstAlphaBlendFunc = BlendFunc::SameAsColor;

    uint32 m_RenderStateFlags = RenderStateFlag::WriteRGBA | RenderStateFlag::CullCCW;
    DepthTest m_DepthTest = DepthTest::Less;
};

} // namespace fw
