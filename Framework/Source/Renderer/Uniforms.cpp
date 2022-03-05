//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "Uniforms.h"

namespace fw {

Uniforms::Uniforms()
{
    CreateFrameworkUniforms();
}

Uniforms::~Uniforms()
{
    for( auto& uniformPair : m_Map )
    {
        bgfx::destroy( uniformPair.second );
    }
}

void Uniforms::CreateFrameworkUniforms()
{
    assert( m_Map.empty() );

    m_Map["u_Time"] = bgfx::createUniform( "u_Time", bgfx::UniformType::Vec4 );
    m_Map["u_TextureColor"] = bgfx::createUniform( "u_TextureColor", bgfx::UniformType::Sampler );
    m_Map["u_UVScaleOffset"] = bgfx::createUniform( "u_UVScaleOffset", bgfx::UniformType::Vec4 );
    m_Map["u_DiffuseColor"] = bgfx::createUniform( "u_DiffuseColor", bgfx::UniformType::Vec4 );
}

} // namespace fw
