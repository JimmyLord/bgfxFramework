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
#include "../Libraries/imgui/imgui.h"

namespace fw {

const char* ResourceTypeName[] = {
    "Mesh",
    "Shader",
    "Texture",
    "Material",
    "SpriteSheet",
};

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
    for( auto& mapPair : m_Resources )
    {
        for( auto& resourcePair : mapPair.second )
        {
            delete resourcePair.second;
        }
    }
}

void ResourceManager::AddMesh(Mesh* pMesh)                      { AddResource( ResourceType::Mesh, pMesh ); }
void ResourceManager::AddShader(ShaderProgram* pShader)         { AddResource( ResourceType::Shader, pShader ); }
void ResourceManager::AddTexture(Texture* pTexture)             { AddResource( ResourceType::Texture, pTexture ); }
void ResourceManager::AddMaterial(Material* pMaterial)          { AddResource( ResourceType::Material, pMaterial ); }
void ResourceManager::AddSpriteSheet(SpriteSheet* pSpriteSheet) { AddResource( ResourceType::SpriteSheet, pSpriteSheet ); }

Mesh* ResourceManager::GetMesh(std::string name)                { return static_cast<Mesh*>( GetResource( ResourceType::Mesh, name ) ); }
ShaderProgram* ResourceManager::GetShader(std::string name)     { return static_cast<ShaderProgram*>( GetResource( ResourceType::Shader, name ) ); }
Texture* ResourceManager::GetTexture(std::string name)          { return static_cast<Texture*>( GetResource( ResourceType::Texture, name ) ); }
Material* ResourceManager::GetMaterial(std::string name)        { return static_cast<Material*>( GetResource( ResourceType::Material, name ) ); }
SpriteSheet* ResourceManager::GetSpriteSheet(std::string name)  { return static_cast<SpriteSheet*>( GetResource( ResourceType::SpriteSheet, name ) ); }

void ResourceManager::AddResource(ResourceType type, Resource* pResource)
{
    std::map<std::string, Resource*>& list = m_Resources[type];

    const char* name = pResource->GetName();
    if( list.find(name) == list.end() )
    {
        list[pResource->GetName()] = pResource;
    }
}

Resource* ResourceManager::GetResource(ResourceType type, std::string name)
{
    std::map<std::string, Resource*>& list = m_Resources[type];

    if( list.find(name) != list.end() )
    {
        return list[name];
    }

    return nullptr;    
}

void ResourceManager::Editor_DisplayResources()
{
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if( ImGui::BeginTabBar( "ResourceTabBar", tab_bar_flags ) )
    {
        for( auto& mapPair : m_Resources )
        {
            if( ImGui::BeginTabItem( ResourceTypeName[(int)mapPair.first] ) )
            {
                for( auto& resourcePair : mapPair.second )
                {
                    ImGui::Text( "%s", resourcePair.first.c_str() );
                }
                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();
    }
}

} // namespace fw
