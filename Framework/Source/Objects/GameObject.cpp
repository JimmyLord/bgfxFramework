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

namespace fw {

GameObject::GameObject(GameCore* pGameCore, std::string name, vec3 pos, Mesh* pMesh, Material* pMaterial)
    : m_pGameCore( pGameCore )
{
    m_EntityID = pGameCore->GetECSRegistry().create();
    pGameCore->GetECSRegistry().emplace<TransformData>( m_EntityID, pos, vec3(0), vec3(1) );
    pGameCore->GetECSRegistry().emplace<NameData>( m_EntityID, name.c_str() );
    if( pMesh != nullptr )
    {
        pGameCore->GetECSRegistry().emplace<MeshData>( m_EntityID, pMesh, pMaterial );
    }
}

GameObject::~GameObject()
{
    m_pGameCore->GetECSRegistry().destroy( m_EntityID );
}

void GameObject::Update(float deltaTime)
{
}

} // namespace fw
