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

namespace fw {

class ShaderProgram
{
public:
    ShaderProgram();
    ShaderProgram(const char* shaderFolder, const char* vertFilename, const char* fragFilename);
    virtual ~ShaderProgram();

    // Getters.
    const bgfx::ProgramHandle& GetProgram() const { return m_Program; }

protected:
    void Cleanup();

    bool Init(const char* shaderFolder, const char* vertFilename, const char* fragFilename);
    bool Reload();

protected:
    char* m_VertShaderString = nullptr;
    char* m_FragShaderString = nullptr;
    uint32 m_VertShaderStringLength = 0;
    uint32 m_FragShaderStringLength = 0;

    bgfx::ShaderHandle m_VertShader = BGFX_INVALID_HANDLE;
    bgfx::ShaderHandle m_FragShader = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle m_Program = BGFX_INVALID_HANDLE;
};

} // namespace fw
