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

#include "ShaderProgram.h"
#include "Utility/Utility.h"

namespace fw {

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::ShaderProgram(const char* shaderFolder, const char* vertFilename, const char* fragFilename)
{
    Init( shaderFolder, vertFilename, fragFilename );
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

bool ShaderProgram::Init(const char* shaderFolder, const char* vertFilename, const char* fragFilename)
{
    char vertFullPath[MAX_PATH];
    char fragFullPath[MAX_PATH];

    char* rendererPath = nullptr;

    switch( bgfx::getRendererType() )
    {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D9:  rendererPath = "dx9";   break;
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12: rendererPath = "dx11";  break;
    case bgfx::RendererType::Agc:
    case bgfx::RendererType::Gnm:        rendererPath = "pssl";  break;
    case bgfx::RendererType::Metal:      rendererPath = "metal"; break;
    case bgfx::RendererType::Nvn:        rendererPath = "nvn";   break;
    case bgfx::RendererType::OpenGL:     rendererPath = "glsl";  break;
    case bgfx::RendererType::OpenGLES:   rendererPath = "essl";  break;
    case bgfx::RendererType::Vulkan:
    case bgfx::RendererType::WebGPU:     rendererPath = "spirv"; break;
    case bgfx::RendererType::Count:
        assert( false );
        break;
    }

    sprintf_s( vertFullPath, MAX_PATH, "%s/%s/%s", shaderFolder, rendererPath, vertFilename );
    sprintf_s( fragFullPath, MAX_PATH, "%s/%s/%s", shaderFolder, rendererPath, fragFilename );

    m_VertShaderString = LoadCompleteFile( vertFullPath, &m_VertShaderStringLength );
    m_FragShaderString = LoadCompleteFile( fragFullPath, &m_FragShaderStringLength );

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
