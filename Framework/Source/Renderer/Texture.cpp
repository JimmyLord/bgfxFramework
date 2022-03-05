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

void Texture::ReleasePixelData(void* _ptr, void* _userData)
{
    Texture* pTexture = static_cast<Texture*>( _userData );
	
    stbi_image_free( pTexture->m_pPixels );
    pTexture->m_pPixels = nullptr;

    imageFree( pTexture->m_pImageContainer );
    pTexture->m_pImageContainer = nullptr;
}

Texture::Texture(const char* filename)
{
    // Load the file contents.
    long length;
    char* fileContents = LoadCompleteFile( filename, &length );

    // Have stb_image decompress png from memory into a raw color array.
    int width;
    int height;
    int channels;
    stbi_set_flip_vertically_on_load( true );
    m_pPixels = stbi_load_from_memory( (unsigned char*)fileContents, length, &width, &height, &channels, 4 );
    assert( m_pPixels != nullptr );

    delete[] fileContents;

    // Create an image container and load the texture.
    // TODO: Is this copy necessary?
    bimg::ImageContainer* image = bimg::imageAlloc( g_pBGFXAllocator, bimg::TextureFormat::RGBA8, width, height, 0, 1, false, false, m_pPixels );

    m_pImageContainer = image;
	uint64_t flags = BGFX_TEXTURE_NONE|BGFX_SAMPLER_NONE;
	bgfx::TextureFormat::Enum format = bgfx::TextureFormat::Enum( image->m_format );

    // Create the texture.
    bool valid = bgfx::isTextureValid( 0, false, image->m_numLayers, format, flags );
    assert( valid );
	if( valid )
	{
        // Create a bgfx memory block, which will free the memory via the ReleaseSTBPixels callback.
        const bgfx::Memory* mem = bgfx::makeRef( image->m_data, image->m_size, ReleasePixelData, this );
		m_TextureHandle = bgfx::createTexture2D( image->m_width, image->m_height, image->m_numMips > 1, image->m_numLayers, format, flags, mem );
    }
}

Texture::~Texture()
{
    bgfx::destroy( m_TextureHandle );
}

} // namespace fw
