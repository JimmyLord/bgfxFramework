//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "imgui.h"
#include "nlohmann-json/single_include/nlohmann/json.hpp"

#include "SpriteSheet.h"
#include "Utility/Utility.h"

namespace fw {

SpriteSheet::SpriteSheet(const char* name, const char* filename, Texture* pTexture)
    : Resource( name )
    , m_pTexture( pTexture )
{
    const char* jsonString = fw::LoadCompleteFile( filename, nullptr );
    nlohmann::json jSpriteSheet = nlohmann::json::parse( jsonString );
    delete[] jsonString;

    int sheetWidth = jSpriteSheet["Width"];
    int sheetHeight = jSpriteSheet["Height"];

    nlohmann::json& jSpriteArray = jSpriteSheet["Sprites"];
    for( int i=0; i<jSpriteArray.size(); i++ )
    {
        nlohmann::json& jSprite = jSpriteArray[i];

        float x = jSprite["X"] + 0.5f;
        float y = jSprite["Y"] + 0.5f;
        float w = jSprite["W"] - 1.0f;
        float h = jSprite["H"] - 1.0f;
        
        std::string name = jSprite["Name"];

        m_Sprites[name.c_str()] = { vec2(w/sheetWidth, h/sheetHeight), vec2(x/sheetWidth, y/sheetHeight) };
    }
}

SpriteSheet::~SpriteSheet()
{
}

SpriteSheet::SpriteInfo SpriteSheet::GetSpriteByName(std::string name)
{
	if( m_Sprites.find( name ) != m_Sprites.end() )
	{
		return m_Sprites.at( name );
	}

	return { vec2(1,1), vec2(0,0) };
}

void SpriteSheet::Editor_DisplayProperties()
{
    ImGui::Text( "SpriteSheet: %s", m_Name );
    ImGui::Separator();

    ImGui::Text( "Sprites: %d", m_Sprites.size() );
}

} // namespace fw
