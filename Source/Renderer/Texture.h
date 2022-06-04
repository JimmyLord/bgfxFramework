//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "bimg/bimg.h"
#include "bgfx/platform.h"
#include "Math/Vector.h"

namespace fw {

class Texture
{
public:
    enum class Format
    {
        RGB8,
        RGBA8,
    };

public:
    Texture();
    Texture(const char* filename);
    virtual ~Texture();

    bgfx::TextureHandle GetHandle() { return m_TextureHandle; }
    void Rebuild(uint32 width, uint32 height, Format format, void* pixels);

protected:
    bgfx::TextureHandle m_TextureHandle = BGFX_INVALID_HANDLE;

    bool m_Mutable = false;
    bgfx::TextureFormat::Enum m_Format = bgfx::TextureFormat::Unknown;
    ivec2 m_Size = ivec2(0,0);
    bool m_HasMips = false;
    uint16 m_NumLayers = 1;
    uint64 m_Flags = 0;
};

} // namespace fw
