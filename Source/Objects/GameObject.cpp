//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "GameCore.h"
#include "GameObject.h"
#include "Components/CoreComponents.h"
#include "Renderer/Mesh.h"
#include "Scenes/Scene.h"
#include "../Libraries/nlohmann-json/single_include/nlohmann/json.hpp"

namespace fw {

GameObject::GameObject(Scene* pScene, std::string name, vec3 pos, Mesh* pMesh, Material* pMaterial)
    : m_pScene( pScene )
{
    m_EntityID = m_pScene->GetECSRegistry().create();
    m_pScene->GetECSRegistry().emplace<TransformData>( m_EntityID, pos, vec3(0), vec3(1) );
    m_pScene->GetECSRegistry().emplace<NameData>( m_EntityID, name.c_str() );
    if( pMesh != nullptr )
    {
        m_pScene->GetECSRegistry().emplace<MeshData>( m_EntityID, pMesh, pMaterial );
    }
}

GameObject::~GameObject()
{
    m_pScene->GetECSRegistry().destroy( m_EntityID );
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::SaveToJSON(nlohmann::json& jGameObject)
{
    nlohmann::json jComponentArray = nlohmann::json::array();
    jGameObject["Components"] = jComponentArray;

    const NameData& nameData = m_pScene->GetECSRegistry().get<NameData>( m_EntityID );
    const TransformData& transformData = m_pScene->GetECSRegistry().get<TransformData>( m_EntityID );
    const MeshData& meshData = m_pScene->GetECSRegistry().get<MeshData>( m_EntityID );
    
    jGameObject["Name"] = nameData.m_Name;
    
    jGameObject["Position"] = nlohmann::json::array();
    jGameObject["Position"].push_back( transformData.position.x );
    jGameObject["Position"].push_back( transformData.position.y );
    jGameObject["Position"].push_back( transformData.position.z );
    
    jGameObject["Rotation"] = nlohmann::json::array();
    jGameObject["Rotation"].push_back( transformData.rotation.x );
    jGameObject["Rotation"].push_back( transformData.rotation.y );
    jGameObject["Rotation"].push_back( transformData.rotation.z );

    jGameObject["Scale"] = nlohmann::json::array();
    jGameObject["Scale"].push_back( transformData.scale.x );
    jGameObject["Scale"].push_back( transformData.scale.y );
    jGameObject["Scale"].push_back( transformData.scale.z );

    //jGameObject["Mesh"] = (int)meshData.pMesh;
    //jGameObject["Material"] = (int)meshData.pMaterial;
    
    //for( auto& pComponent :  )
    //{
    //    nlohmann::json jComponent;
    //    pComponent->SaveToJSON( jComponent );
    //    jComponentArray.push_back( jComponent );
    //}
}

void GameObject::LoadFromJSON(nlohmann::json& jGameObject)
{
}

} // namespace fw
