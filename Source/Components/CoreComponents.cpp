#include "CoreComponents.h"
#include "Objects/GameObject.h"
#include "Scenes/Scene.h"
#include "../Libraries/nlohmann-json/single_include/nlohmann/json.hpp"

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
        //jGameObject["MeshData"]["Mesh"] = (uint64)pMeshData->pMesh;
        //jGameObject["MeshData"]["Material"] = (uint64)pMeshData->pMaterial;
    }
}

void ComponentManager::LoadGameObjectComponentsFromJSON(GameObject* pGameObject, nlohmann::json& jGameObject)
{
}

void ComponentManager::Editor_DisplayComponentsForGameObject(GameObject* pGameObject)
{
}

} // namespace fw
