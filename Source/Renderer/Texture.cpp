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
    unsigned char* pixels = stbi_load_from_memory( (unsigned char*)fileContents, length, &width, &height, &channels, 4 );
    assert( pixels != nullptr );

    // Create the texture.
    m_TextureHandle = bgfx::createTexture2D( width, height, false, 1, bgfx::TextureFormat::RGBA8,
                                             0, bgfx::copy(pixels, width*height*4));

    delete[] fileContents;
    stbi_image_free( pixels );
}

Texture::~Texture()
{
    bgfx::destroy( m_TextureHandle );
}

} // namespace fw
