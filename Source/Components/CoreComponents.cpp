#include "CoreComponents.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Resources/ResourceManager.h"

namespace fw {

void NameComponentDefinition::SaveToJSON(nlohmann::json& jComponent, const void* pData)
{
    jComponent["Name"] = ((NameData*)pData)->m_Name;
}

void NameComponentDefinition::LoadFromJSON(flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager)
{
    std::string nameStr = jComponent["Name"];
    const char* name = nameStr.c_str();
    entity.set<NameData>( {name} );
}

void TransformComponentDefinition::SaveToJSON(nlohmann::json& jComponent, const void* pData)
{
    TransformData* pTransformData = (TransformData*)pData;
    jComponent["Position"] = { pTransformData->position.x, pTransformData->position.y, pTransformData->position.z };
    jComponent["Rotation"] = { pTransformData->rotation.x, pTransformData->rotation.y, pTransformData->rotation.z };
    jComponent["Scale"] = { pTransformData->scale.x, pTransformData->scale.y, pTransformData->scale.z };
}

void TransformComponentDefinition::LoadFromJSON(flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager)
{
    TransformData transformData;
    transformData.position = vec3( jComponent["Position"][0], jComponent["Position"][1], jComponent["Position"][2] );
    transformData.rotation = vec3( jComponent["Rotation"][0], jComponent["Rotation"][1], jComponent["Rotation"][2] );
    transformData.scale = vec3( jComponent["Scale"][0], jComponent["Scale"][1], jComponent["Scale"][2] );
    entity.set<TransformData>( transformData );
}

void MeshComponentDefinition::SaveToJSON(nlohmann::json& jComponent, const void* pData)
{
    MeshData* pMeshData = (MeshData*)pData;
    jComponent["Mesh"] = pMeshData->pMesh->GetName();
    jComponent["Material"] = pMeshData->pMaterial->GetName();
}

void MeshComponentDefinition::LoadFromJSON(flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager)
{
    MeshData meshData;
    meshData.pMesh = pResourceManager->GetMesh( jComponent["Mesh"] );
    meshData.pMaterial = pResourceManager->GetMaterial( jComponent["Material"] );
    entity.set<MeshData>( meshData );
}

} // namespace fw
