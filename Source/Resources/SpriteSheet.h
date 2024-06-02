//
// Copyright (c) 2022-2023 Jimmy Lord
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

class Texture;

class SpriteSheet : public Resource
{
public:
    struct SpriteInfo
    {
        // Needs to stay 4 contiguous floats to pass to shader as a vec4.
        vec2 uvScale;
        vec2 uvOffset;

        vec4& asVec4() { return *(vec4*)&uvScale.x; }
    };

public:
    SpriteSheet(const char* name, const char* filename, Texture* pTexture);
    virtual ~SpriteSheet();

    // Getters.
    Texture* GetTexture() { return m_pTexture; }
    SpriteInfo GetSpriteByName(std::string name);    

    // Editor.
    virtual void Editor_DisplayProperties() override;
    
protected:
    fw::Texture* m_pTexture = nullptr;
    std::map<std::string, SpriteInfo> m_Sprites;
};

} // namespace fw
