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

namespace fw {

class ShaderProgram;
class Texture;
class Uniforms;

class Material
{
public:
    Material(ShaderProgram* pShader, Texture* pTexture, color4f color, bool hasAlpha);
    Material(ShaderProgram* pShader, Texture* pTexture, color4f color, bool hasAlpha, vec4 uvScaleOffset);
    virtual ~Material();

    void Enable(const Uniforms* pUniforms) const;

    // Getters.
    ShaderProgram* GetShader() const { return m_pShader; }
    Texture* GetTexture() const { return m_pTexture; }
    vec4 GetUVScaleOffset() { return m_UVScaleOffset; }
    color4f GetColor() const { return m_Color; }
    uint64_t GetAlphaState() const { return m_AlphaState; }

    // Setters.
    void SetShader(ShaderProgram* pShader) { m_pShader = pShader; }
    void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }
    void SetUVScaleOffset(vec2 uvScale, vec2 uvOffset) { m_UVScaleOffset.Set( uvScale, uvOffset ); }
    void SetColor(color4f color) { m_Color = color; }
    void SetAlphaState(uint64_t alphaState) { m_AlphaState = alphaState; }

protected:
    ShaderProgram* m_pShader;
    Texture* m_pTexture;
    vec4 m_UVScaleOffset;
    color4f m_Color;

    uint64_t m_AlphaState = 0;
};

} // namespace fw
