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

protected:
    fw::Texture* m_pTexture = nullptr;
    std::map<std::string, SpriteInfo> m_Sprites;
};

} // namespace fw
