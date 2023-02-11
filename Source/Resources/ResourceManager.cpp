//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "ResourceManager.h"
#include "Resources/Material.h"
#include "Resources/Mesh.h"
#include "Resources/ShaderProgram.h"
#include "Resources/SpriteSheet.h"
#include "Resources/Texture.h"

namespace fw {

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    for( auto& meshPair : m_Meshes )
    {
        delete meshPair.second;
    }

    for( auto& materialPair : m_Materials )
    {
        delete materialPair.second;
    }

    for( auto& texturePair : m_Textures )
    {
        delete texturePair.second;
    }

    for( auto& shaderPair : m_Shaders )
    {
        delete shaderPair.second;
    }

    for( auto& spriteSheetPair : m_SpriteSheets )
    {
        delete spriteSheetPair.second;
    }
}

void ResourceManager::AddMesh(Mesh* pMesh)
{
    const char* name = pMesh->GetName();
    if( m_Meshes.find(name) == m_Meshes.end() )
    {
        m_Meshes[name] = pMesh;
    }
}

void ResourceManager::AddShader(ShaderProgram* pShader)
{
    const char* name = pShader->GetName();
    if( m_Shaders.find(name) == m_Shaders.end() )
    {
        m_Shaders[name] = pShader;
    }
}

void ResourceManager::AddTexture(Texture* pTexture)
{
    const char* name = pTexture->GetName();
    if( m_Textures.find(name) == m_Textures.end() )
    {
        m_Textures[name] = pTexture;
    }
}

void ResourceManager::AddMaterial(Material* pMaterial)
{
    const char* name = pMaterial->GetName();
    if( m_Materials.find(name) == m_Materials.end() )
    {
        m_Materials[name] = pMaterial;
    }
}

void ResourceManager::AddSpriteSheet(SpriteSheet* pSpriteSheet)
{
    const char* name = pSpriteSheet->GetName();
    if( m_SpriteSheets.find(name) == m_SpriteSheets.end() )
    {
        m_SpriteSheets[name] = pSpriteSheet;
    }
}

Mesh* ResourceManager::GetMesh(std::string name)
{
    if( m_Meshes.find(name) != m_Meshes.end() )
    {
        return m_Meshes[name];
    }

    return nullptr;
}

ShaderProgram* ResourceManager::GetShader(std::string name)
{
    if(m_Shaders.find(name) != m_Shaders.end() )
    {
        return m_Shaders[name];
    }

    return nullptr;
}

Texture* ResourceManager::GetTexture(std::string name)
{
    if( m_Textures.find(name) != m_Textures.end() )
    {
        return m_Textures[name];
    }

    return nullptr;
}

Material* ResourceManager::GetMaterial(std::string name)
{
    if( m_Materials.find(name) != m_Materials.end() )
    {
        return m_Materials[name];
    }

    return nullptr;
}

SpriteSheet* ResourceManager::GetSpriteSheet(std::string name)
{
    if( m_SpriteSheets.find(name) != m_SpriteSheets.end() )
    {
        return m_SpriteSheets[name];
    }

    return nullptr;    
}

} // namespace fw
