//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"
#include "FWCore.h"
#include "GameCore.h"
#include "Scene.h"
#include "Components/CoreComponents.h"
#include "EventSystem/Events.h"
#include "Math/Matrix.h"
#include "Objects/GameObject.h"
#include "Renderer/Mesh.h"
#include "../Libraries/imgui/imgui.h"

namespace fw {

Scene::Scene(GameCore* pGameCore)
    : m_pGameCore( pGameCore )
{
}

Scene::~Scene()
{
    for( GameObject* pObject : m_Objects )
    {
        delete pObject;
    }
}

void Scene::Init()
{
}

void Scene::OnShutdown()
{
}

void Scene::StartFrame(float deltaTime)
{
}

void Scene::OnEvent(Event* pEvent)
{
    if( pEvent->GetType() == RemoveFromGameEvent::GetStaticEventType() )
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>( pEvent );
        GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        auto it = std::find( m_Objects.begin(), m_Objects.end(), pObject );
        m_Objects.erase( it );

        delete pObject;
    }
}

void Scene::Update(float deltaTime)
{
    for( GameObject* pObject : m_Objects )
    {
        pObject->Update( deltaTime );
    }
}

void Scene::Draw(int viewID)
{
    DrawIntoView( viewID );
}

void Scene::DrawIntoView(int viewID)
{
    Uniforms* pUniforms = m_pGameCore->GetUniforms();
    
    auto group = m_ECSRegistry.group<TransformData>( entt::get<MeshData> );
    for( auto entity : group )
    {
        auto& [transformData, meshData] = group.get<TransformData, MeshData>( entity );

        mat4 worldMat;
        worldMat.CreateSRT( transformData.scale, transformData.rotation, transformData.position );
        meshData.pMesh->Draw( viewID, pUniforms, meshData.pMaterial, &worldMat );
    }
}

entt::entity Scene::CreateEntity()
{
    return m_ECSRegistry.create();
}

void Scene::Editor_DisplayObjectList()
{
    auto view = m_ECSRegistry.view<fw::NameData>();
    for( auto entity : view )
    {
        auto& nameData = view.get<fw::NameData>( entity );

        ImGui::Text( "%s", nameData.m_Name );
    }
}

} // namespace fw