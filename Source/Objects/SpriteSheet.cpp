#include "CoreHeaders.h"
#include "SpriteSheet.h"
#include "Utility/Utility.h"

#include "nlohmann-json/single_include/nlohmann/json.hpp"

namespace fw {

SpriteSheet::SpriteSheet(const char* filename, Texture* pTexture)
	: m_pTexture( pTexture )
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

} // namespace fw
