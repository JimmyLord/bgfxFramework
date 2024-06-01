#include "CoreHeaders.h"
#include "ComponentManager.h"
#include "CoreComponents.h"
#include "GameCore.h"
#include "Objects/GameObject.h"
#include "Scenes/Scene.h"
#include "Resources/Material.h"
#include "Resources/Mesh.h"
#include "Resources/ResourceManager.h"
#include "../Libraries/imgui/imgui.h"

namespace fw {

ComponentManager::ComponentManager()
{
    flecs::id_t nameID = m_FlecsWorld.component<NameData>();
    flecs::id_t transformID = m_FlecsWorld.component<TransformData>();
    flecs::id_t meshID = m_FlecsWorld.component<MeshData>();

    m_ComponentDefinitions[nameID] = new NameComponentDefinition();
    m_ComponentDefinitions[transformID] = new TransformComponentDefinition();
    m_ComponentDefinitions[meshID] = new MeshComponentDefinition();
}

ComponentManager::~ComponentManager()
{
    for( auto& pair : m_ComponentDefinitions )
    {
        delete pair.second;
    }
}

void ComponentManager::Editor_AddComponentToGameObject(GameObject* pObject)
{
    bool hasIt;
    
    // Grey out the components the object already has.
    hasIt = pObject->GetEntity().has<NameData>();
    if( ImGui::MenuItem( "Name", "", nullptr, !hasIt ) ) { pObject->GetEntity().add<NameData>(); }

    hasIt = pObject->GetEntity().has<TransformData>();
    if( ImGui::MenuItem( "Transform", "", nullptr, !hasIt ) ) { pObject->GetEntity().add<TransformData>(); }

    hasIt = pObject->GetEntity().has<MeshData>();
    if( ImGui::MenuItem( "Mesh", "", nullptr, !hasIt ) ) { pObject->GetEntity().add<MeshData>(); }
}

void ComponentManager::SaveGameObjectComponentsToJSON(GameObject* pGameObject, nlohmann::json& jGameObject)
{
    flecs::entity entity = pGameObject->GetEntity();

    // iterate over all components and save them to the JSON object
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

    const NameData* pNameData = entity.get<NameData>();
    if( pNameData )
    {
        if( ImGui::CollapsingHeader( "Name", ImGuiTreeNodeFlags_DefaultOpen ) )
        {
            ImGui::Text( pNameData->m_Name );
        }
    }

    ImGui::Separator();

    TransformData* pTransformData = &entity.ensure<TransformData>();
    if( pTransformData )
    {
        if( ImGui::CollapsingHeader( "Transform", ImGuiTreeNodeFlags_DefaultOpen ) )
        {
            ImGui::DragFloat3( "Position", &pTransformData->position.x, 0.1f );
            ImGui::DragFloat3( "Rotation", &pTransformData->rotation.x, 0.1f );
            ImGui::DragFloat3( "Scale", &pTransformData->scale.x, 0.1f );
            entity.modified<TransformData>();
        }
    }

    ImGui::Separator();

    const MeshData* pMeshData = entity.get<MeshData>();
    if( pMeshData )
    {
        if( ImGui::CollapsingHeader( "Mesh", ImGuiTreeNodeFlags_DefaultOpen ) )
        {
            ImGui::Text( "Mesh: " );
            ImGui::SameLine();
            if( pMeshData->pMesh )
            {
                ImGui::Text( pMeshData->pMesh->GetName() );
            }
            else
            {
                ImGui::Text( "None" );
            }
            ImGui::Text( "Material: " );
            ImGui::SameLine();
            if( pMeshData->pMaterial )
            {
                ImGui::Text( pMeshData->pMaterial->GetName() );
            }
            else
            {
                ImGui::Text( "None" );
            }
        }
    }
}

} // namespace fw
