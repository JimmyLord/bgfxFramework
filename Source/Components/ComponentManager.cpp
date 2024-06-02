//
// Copyright (c) 2022-2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"
#include "ComponentManager.h"
#include "CoreComponents.h"
#include "GameCore.h"
#include "Objects/GameObject.h"
#include "Scenes/Scene.h"
#include "Resources/ResourceManager.h"

namespace fw {

ComponentManager::ComponentManager()
{
    // Register base component definitions.
    RegisterComponentDefinition( m_FlecsWorld.component<NameData>(), new NameComponentDefinition() );
    RegisterComponentDefinition( m_FlecsWorld.component<TransformData>(), new TransformComponentDefinition() );
    RegisterComponentDefinition( m_FlecsWorld.component<MeshData>(), new MeshComponentDefinition() );
}

ComponentManager::~ComponentManager()
{
    for( auto& pair : m_ComponentDefinitions )
    {
        delete pair.second;
    }
}

void ComponentManager::RegisterComponentDefinition(flecs::id_t componentId, BaseComponentDefinition* pComponentDefinition)
{
    m_ComponentDefinitions[componentId] = pComponentDefinition;
}

void ComponentManager::Editor_AddComponentToGameObject(GameObject* pObject)
{
    flecs::entity entity = pObject->GetEntity();
    
    // Display a menu item for each registered component type.
    // Grey out the components the object already has.
    for( auto& pair : m_ComponentDefinitions )
    {
        bool hasIt = entity.has( pair.first );

        if( ImGui::MenuItem( pair.second->GetName(), "", nullptr, !hasIt ) )
        {
            entity.add( pair.first );
        }
    }
}

void ComponentManager::SaveGameObjectComponentsToJSON(GameObject* pGameObject, nlohmann::json& jGameObject)
{
    flecs::entity entity = pGameObject->GetEntity();

    // Iterate over all components and save them to the JSON object.
    entity.each(
        [&](const flecs::id_t componentId)
        {
            const void* pData = entity.get( componentId );
            BaseComponentDefinition* pComponentDef = m_ComponentDefinitions[componentId];
            pComponentDef->SaveToJSON( pGameObject, jGameObject[pComponentDef->GetName()], pData );
        }
    );
}

void ComponentManager::LoadGameObjectComponentsFromJSON(GameObject* pGameObject, nlohmann::json& jGameObject)
{
    GameCore* pGameCore = pGameObject->GetScene()->GetGameCore();
    ResourceManager* pResourceManager = pGameCore->GetResourceManager();
    flecs::entity entity = pGameObject->GetEntity();

    // Iterate over registered component types and load any found in the JSON object.
    for( auto& pair : m_ComponentDefinitions )
    {
        if( jGameObject.contains( pair.second->GetName() ) )
        {
            pair.second->LoadFromJSON( pGameObject, entity, jGameObject[pair.second->GetName()], pResourceManager );
        }
    }
}

void ComponentManager::Editor_DisplayComponentsForGameObject(GameObject* pGameObject)
{
    flecs::entity entity = pGameObject->GetEntity();

    // Iterate over all components and display them in the inspector.
    entity.each(
        [&](const flecs::id_t componentId)
        {
            BaseComponentDefinition* pComponentDef = m_ComponentDefinitions[componentId];
            pComponentDef->Editor_AddToInspector( entity );
        }
    );
}

} // namespace fw
