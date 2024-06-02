//
// Copyright (c) 2022-2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreComponents.h"
#include "Resources/Mesh.h"
#include "Resources/Material.h"
#include "Resources/ResourceManager.h"

namespace fw {

//==============================
// NameComponentDefinition
//==============================

void NameComponentDefinition::SaveToJSON(GameObject* pObject, nlohmann::json& jComponent, const void* pData)
{
    jComponent["Name"] = ((NameData*)pData)->name;
}

void NameComponentDefinition::LoadFromJSON(GameObject* pObject, flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager)
{
    std::string nameStr = jComponent["Name"];
    const char* name = nameStr.c_str();
    entity.set<NameData>( {name} );
}

void NameComponentDefinition::Editor_AddToInspector(flecs::entity entity)
{
    const NameData* pNameData = entity.get<NameData>();
    assert( pNameData );
    if( ImGui::CollapsingHeader( "Name", ImGuiTreeNodeFlags_DefaultOpen ) )
    {
        ImGui::Text( pNameData->name );
    }
}

//==============================
// TransformComponentDefinition
//==============================

void TransformComponentDefinition::SaveToJSON(GameObject* pObject, nlohmann::json& jComponent, const void* pData)
{
    TransformData* pTransformData = (TransformData*)pData;
    jComponent["Position"] = { pTransformData->position.x, pTransformData->position.y, pTransformData->position.z };
    jComponent["Rotation"] = { pTransformData->rotation.x, pTransformData->rotation.y, pTransformData->rotation.z };
    jComponent["Scale"] = { pTransformData->scale.x, pTransformData->scale.y, pTransformData->scale.z };
}

void TransformComponentDefinition::LoadFromJSON(GameObject* pObject, flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager)
{
    TransformData transformData;
    transformData.position = vec3( jComponent["Position"][0], jComponent["Position"][1], jComponent["Position"][2] );
    transformData.rotation = vec3( jComponent["Rotation"][0], jComponent["Rotation"][1], jComponent["Rotation"][2] );
    transformData.scale = vec3( jComponent["Scale"][0], jComponent["Scale"][1], jComponent["Scale"][2] );
    entity.set<TransformData>( transformData );
}

void TransformComponentDefinition::Editor_AddToInspector(flecs::entity entity)
{
    TransformData& transformData = entity.ensure<TransformData>();
    if( ImGui::CollapsingHeader( "Transform", ImGuiTreeNodeFlags_DefaultOpen ) )
    {
        ImGui::DragFloat3( "Position", &transformData.position.x, 0.1f );
        ImGui::DragFloat3( "Rotation", &transformData.rotation.x, 0.1f );
        ImGui::DragFloat3( "Scale", &transformData.scale.x, 0.1f );
    }
    entity.modified<TransformData>();
}

//==============================
// MeshComponentDefinition
//==============================

void MeshComponentDefinition::SaveToJSON(GameObject* pObject, nlohmann::json& jComponent, const void* pData)
{
    MeshData* pMeshData = (MeshData*)pData;
    jComponent["Mesh"] = pMeshData->pMesh->GetName();
    jComponent["Material"] = pMeshData->pMaterial->GetName();
}

void MeshComponentDefinition::LoadFromJSON(GameObject* pObject, flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager)
{
    MeshData meshData;
    meshData.pMesh = pResourceManager->GetMesh( jComponent["Mesh"] );
    meshData.pMaterial = pResourceManager->GetMaterial( jComponent["Material"] );
    entity.set<MeshData>( meshData );
}

void MeshComponentDefinition::Editor_AddToInspector(flecs::entity entity)
{
    const MeshData* pMeshData = entity.get<MeshData>();
    assert( pMeshData );
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

} // namespace fw
