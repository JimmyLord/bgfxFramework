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
#include "Components/ComponentManager.h"
#include "Components/CoreComponents.h"
#include "Resources/Mesh.h"
#include "Scenes/Scene.h"

namespace fw {

GameObject::GameObject(Scene* pScene)
    : m_pScene( pScene )
{
    m_EntityID = m_pScene->GetECSRegistry().create();
}

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
    m_pScene->GetComponentManager()->SaveGameObjectComponentsToJSON( this, jGameObject );
}

void GameObject::LoadFromJSON(nlohmann::json& jGameObject)
{
    m_pScene->GetComponentManager()->LoadGameObjectComponentsFromJSON( this, jGameObject );
}

} // namespace fw
