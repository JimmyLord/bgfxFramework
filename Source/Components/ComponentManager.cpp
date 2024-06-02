#include "CoreHeaders.h"
#include "ComponentManager.h"
#include "CoreComponents.h"
#include "GameCore.h"
#include "Objects/GameObject.h"
#include "Scenes/Scene.h"
#include "Resources/ResourceManager.h"
#include "../Libraries/imgui/imgui.h"

namespace fw {

ComponentManager::ComponentManager()
{
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
            pComponentDef->SaveToJSON( jGameObject[pComponentDef->GetName()], pData );
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
            pair.second->LoadFromJSON( entity, jGameObject[pair.second->GetName()], pResourceManager );
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
