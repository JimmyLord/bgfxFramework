#include "CoreHeaders.h"
#include "ComponentManager.h"
#include "CoreComponents.h"
#include "GameCore.h"
#include "Objects/GameObject.h"
#include "Scenes/Scene.h"
#include "Resources/Material.h"
#include "Resources/Mesh.h"
#include "Resources/ResourceManager.h"
#include "../Libraries/nlohmann-json/single_include/nlohmann/json.hpp"
#include "../Libraries/imgui/imgui.h"

namespace fw {

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
}

void ComponentManager::SaveGameObjectComponentsToJSON(GameObject* pGameObject, nlohmann::json& jGameObject)
{
    entt::registry& registry = pGameObject->GetScene()->GetECSRegistry();
    entt::entity id = pGameObject->GetEntityID();
    
    NameData* pNameData = registry.try_get<NameData>( id );
    if( pNameData )
    {
        jGameObject["NameData"]["Name"] = pNameData->m_Name;
    }

    TransformData* pTransformData = registry.try_get<TransformData>( id );
    if( pTransformData )
    {
        jGameObject["TransformData"]["Position"] = nlohmann::json::array();
        jGameObject["TransformData"]["Position"].push_back( pTransformData->position.x );
        jGameObject["TransformData"]["Position"].push_back( pTransformData->position.y );
        jGameObject["TransformData"]["Position"].push_back( pTransformData->position.z );

        jGameObject["TransformData"]["Rotation"] = nlohmann::json::array();
        jGameObject["TransformData"]["Rotation"].push_back( pTransformData->rotation.x );
        jGameObject["TransformData"]["Rotation"].push_back( pTransformData->rotation.y );
        jGameObject["TransformData"]["Rotation"].push_back( pTransformData->rotation.z );

        jGameObject["TransformData"]["Scale"] = nlohmann::json::array();
        jGameObject["TransformData"]["Scale"].push_back( pTransformData->scale.x );
        jGameObject["TransformData"]["Scale"].push_back( pTransformData->scale.y );
        jGameObject["TransformData"]["Scale"].push_back( pTransformData->scale.z );
    }

    MeshData* pMeshData = registry.try_get<MeshData>( id );
    if( pMeshData )
    {
        jGameObject["MeshData"]["Mesh"] = pMeshData->pMesh->GetName();
        jGameObject["MeshData"]["Material"] = pMeshData->pMaterial->GetName();
    }
}

void ComponentManager::LoadGameObjectComponentsFromJSON(GameObject* pGameObject, nlohmann::json& jGameObject)
{
    GameCore* pGameCore = pGameObject->GetScene()->GetGameCore();
    ResourceManager* pResourceManager = pGameCore->GetResourceManager();
    entt::registry& registry = pGameObject->GetScene()->GetECSRegistry();
    entt::entity id = pGameObject->GetEntityID();

    if( jGameObject.contains("NameData") )
    {
        NameData* pNameData = registry.try_get<NameData>( id );
        assert( pNameData == nullptr );

        std::string name = jGameObject["NameData"]["Name"];
        
        pNameData = &registry.emplace<NameData>( id, name.c_str() );
    }

    if( jGameObject.contains("TransformData") )
    {
        TransformData* pTransformData = registry.try_get<TransformData>( id );
        assert( pTransformData == nullptr );

        pTransformData = &registry.emplace<TransformData>( id );

        pTransformData->position.x = jGameObject["TransformData"]["Position"][0].get<float>();
        pTransformData->position.y = jGameObject["TransformData"]["Position"][1].get<float>();
        pTransformData->position.z = jGameObject["TransformData"]["Position"][2].get<float>();

        pTransformData->rotation.x = jGameObject["TransformData"]["Rotation"][0].get<float>();
        pTransformData->rotation.y = jGameObject["TransformData"]["Rotation"][1].get<float>();
        pTransformData->rotation.z = jGameObject["TransformData"]["Rotation"][2].get<float>();

        pTransformData->scale.x = jGameObject["TransformData"]["Scale"][0].get<float>();
        pTransformData->scale.y = jGameObject["TransformData"]["Scale"][1].get<float>();
        pTransformData->scale.z = jGameObject["TransformData"]["Scale"][2].get<float>();
    }

    if( jGameObject.contains("MeshData") )
    {
        MeshData* pMeshData = registry.try_get<MeshData>(id);
        assert( pMeshData == nullptr );

        pMeshData = &registry.emplace<MeshData>( id );

        pMeshData->pMesh = pResourceManager->GetMesh( jGameObject["MeshData"]["Mesh"] );
        pMeshData->pMaterial = pResourceManager->GetMaterial( jGameObject["MeshData"]["Material"] );
    }
}

void ComponentManager::Editor_DisplayComponentsForGameObject(GameObject* pGameObject)
{
    entt::registry& registry = pGameObject->GetScene()->GetECSRegistry();
    entt::entity id = pGameObject->GetEntityID();

    NameData* pNameData = registry.try_get<NameData>( id );
    if( pNameData )
    {
        if( ImGui::CollapsingHeader( "Name", ImGuiTreeNodeFlags_DefaultOpen ) )
        {
            ImGui::Text( pNameData->m_Name );
        }
    }

    ImGui::Separator();

    TransformData* pTransformData = registry.try_get<TransformData>( id );
    if( pTransformData )
    {
        if( ImGui::CollapsingHeader( "Transform", ImGuiTreeNodeFlags_DefaultOpen ) )
        {
            ImGui::DragFloat3( "Position", &pTransformData->position.x, 0.1f );
            ImGui::DragFloat3( "Rotation", &pTransformData->rotation.x, 0.1f );
            ImGui::DragFloat3( "Scale", &pTransformData->scale.x, 0.1f );
        }
    }

    ImGui::Separator();

    MeshData* pMeshData = registry.try_get<MeshData>( id );
    if( pMeshData )
    {
        if( ImGui::CollapsingHeader( "Mesh", ImGuiTreeNodeFlags_DefaultOpen ) )
        {
            ImGui::Text( "Mesh: " );
            ImGui::SameLine();
            ImGui::Text( pMeshData->pMesh->GetName() );
            ImGui::Text( "Material: " );
            ImGui::SameLine();
            ImGui::Text( pMeshData->pMaterial->GetName() );
        }
    }
}

} // namespace fw
