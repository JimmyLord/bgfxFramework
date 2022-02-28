//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "ShaderProgram.h"
#include "Utility/Utility.h"
#include "bgfx/platform.h"

namespace fw {

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::ShaderProgram(const char* vertFilename, const char* fragFilename)
{
    Init( vertFilename, fragFilename );
}

ShaderProgram::~ShaderProgram()
{
    Cleanup();
}

void ShaderProgram::Cleanup()
{
    if( m_VertShaderString )
        delete[] m_VertShaderString;
    if( m_FragShaderString )
        delete[] m_FragShaderString;

    bgfx::destroy( m_Program );
    bgfx::destroy( m_VertShader );
    bgfx::destroy( m_FragShader );

    m_VertShaderString = nullptr;
    m_FragShaderString = nullptr;
}

bool ShaderProgram::Init(const char* vertFilename, const char* fragFilename)
{
    m_VertShaderString = fw::LoadCompleteFile( vertFilename, &m_VertShaderStringLength );
    m_FragShaderString = fw::LoadCompleteFile( fragFilename, &m_FragShaderStringLength );

    assert( m_VertShaderString != nullptr && m_FragShaderString != nullptr );
    if( m_VertShaderString == nullptr || m_FragShaderString == nullptr )
        return false;

    return Reload();
}

bool ShaderProgram::Reload()
{
    assert( m_VertShaderString != nullptr );
    assert( m_FragShaderString != nullptr );

    const bgfx::Memory* vertMemory = bgfx::makeRef( m_VertShaderString, m_VertShaderStringLength );
    const bgfx::Memory* fragMemory = bgfx::makeRef( m_FragShaderString, m_FragShaderStringLength );

    m_VertShader = bgfx::createShader( vertMemory );
    m_FragShader = bgfx::createShader( fragMemory );    

    m_Program = bgfx::createProgram( m_VertShader, m_FragShader, false );

    return true;
}

} // namespace fw
