//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "bimg/bimg.h"
#include "bgfx/platform.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "FWCore.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Math/Matrix.h"
#include "Utility/Utility.h"
#include "Texture.h"

namespace fw {

Texture::Texture()
{
}

Texture::Texture(const char* filename)
{
    // Load the file contents.
    uint32 length;
    char* fileContents = LoadCompleteFile( filename, &length );

    // Have stb_image decompress png from memory into a raw color array.
    int width;
    int height;
    int channels;
    stbi_set_flip_vertically_on_load( true );
    unsigned char* pixels = stbi_load_from_memory( (unsigned char*)fileContents, length, &width, &height, &channels, 4 );
    assert( pixels != nullptr );

    // Create the texture.
    Rebuild( width, height, fw::Texture::Format::RGBA8, pixels );

    delete[] fileContents;
    stbi_image_free( pixels );
}

Texture::~Texture()
{
    bgfx::destroy( m_TextureHandle );
}

void Texture::Rebuild(uint32 width, uint32 height, Format format, void* pixels)
{
    bgfx::TextureFormat::Enum bgfxFormat = bgfx::TextureFormat::Unknown;
    unsigned int bufferSize = 0;

    switch( format )
    {
    case Format::RGB8:
        {
            bgfxFormat = bgfx::TextureFormat::RGB8;
            bufferSize = width*height*3;
        }
        break;
    case Format::RGBA8:
        {
            bgfxFormat = bgfx::TextureFormat::RGBA8;
            bufferSize = width*height*4;
        }
        break;
    }

    assert( bgfxFormat != bgfx::TextureFormat::Unknown );

    if( bgfx::isValid( m_TextureHandle ) )
    {
        if( m_Format != bgfxFormat )
        {
            bgfx::destroy( m_TextureHandle );
        }
    }

    if( m_Mutable && bgfx::isValid( m_TextureHandle ) )
    {
        if( m_Size != vec2(width,height) )
        {
            bgfx::destroy( m_TextureHandle );
            m_TextureHandle = bgfx::createTexture2D( width, height, false, 1, bgfxFormat, 0, nullptr );
        }

        bgfx::updateTexture2D( m_TextureHandle, 0, 0, 0, 0, width, height, bgfx::copy(pixels, bufferSize) );
    }
    else
    {
        if( bgfx::isValid( m_TextureHandle ) )
            bgfx::destroy( m_TextureHandle );

        if( pixels == 0 )
        {
            m_TextureHandle = bgfx::createTexture2D( width, height, false, 1, bgfxFormat, 0, nullptr );
            m_Mutable = true;
        }
        else
        {
            m_TextureHandle = bgfx::createTexture2D( width, height, false, 1, bgfxFormat, 0, bgfx::copy(pixels, bufferSize) );
            m_Mutable = false;
        }
    }

    m_Format = bgfxFormat;
    m_Size.Set( width, height );
    m_HasMips = false;
    m_NumLayers = 1;
    m_Flags = 0;
}

} // namespace fw
