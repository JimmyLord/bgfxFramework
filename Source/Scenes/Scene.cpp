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
#include "Components/ComponentManager.h"
#include "Components/CoreComponents.h"
#include "Editor/EditorCore.h"
#include "EventSystem/Events.h"
#include "EventSystem/EventManager.h"
#include "Math/Matrix.h"
#include "Objects/GameObject.h"
#include "Resources/Mesh.h"
#include "../Libraries/imgui/imgui.h"
#include "../Libraries/nlohmann-json/single_include/nlohmann/json.hpp"

namespace fw {

Scene::Scene(GameCore* pGameCore)
    : m_pGameCore( pGameCore )
{
    CreateComponentManager();
}

Scene::~Scene()
{
    for( GameObject* pObject : m_Objects )
    {
        delete pObject;
    }

    delete m_pComponentManager;
}

void Scene::CreateComponentManager()
{
    m_pComponentManager = new fw::ComponentManager();
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
    
    auto group = m_pComponentManager->GetECSRegistry().group<TransformData>( entt::get<MeshData> );
    for( auto entity : group )
    {
        auto& [transformData, meshData] = group.get<TransformData, MeshData>( entity );

        mat4 worldMat;
        worldMat.CreateSRT( transformData.scale, transformData.rotation, transformData.position );
        if( meshData.pMesh )
        {
            meshData.pMesh->Draw( viewID, pUniforms, meshData.pMaterial, &worldMat );
        }
    }
}

void Scene::SaveToJSON(nlohmann::json& jScene)
{
    nlohmann::json jGameObjectArray = nlohmann::json::array();

    for( GameObject* pObject : m_Objects )
    {
        nlohmann::json jGameObject;
        pObject->SaveToJSON( jGameObject );
        jGameObjectArray.push_back( jGameObject );
    }
    jScene["Objects"] = jGameObjectArray;
}

void Scene::LoadFromJSON(nlohmann::json& jScene)
{
    nlohmann::json jGameObjectArray = jScene["Objects"];

    for( nlohmann::json jGameObject : jGameObjectArray )
    {
        GameObject* pObject = new GameObject( this );
        pObject->LoadFromJSON( jGameObject );
        m_Objects.push_back( pObject );
    }
}

entt::registry& Scene::GetECSRegistry()
{
    return m_pComponentManager->GetECSRegistry();
}

entt::entity Scene::CreateEntity()
{
    return m_pComponentManager->GetECSRegistry().create();
}

void Scene::Editor_DisplayObjectList()
{
    EditorCore* pEditorCore = dynamic_cast<EditorCore*>( m_pGameCore );
    if( pEditorCore )
    {
        for( GameObject* pGameObject : m_Objects )
        {
            const char* name = "No Name";
        
            bool hasName = m_pComponentManager->GetECSRegistry().try_get<fw::NameData>( pGameObject->GetEntityID() );
            if( hasName )
            {
                auto& nameData = m_pComponentManager->GetECSRegistry().get<fw::NameData>( pGameObject->GetEntityID() );
                if( nameData.m_Name[0] != '\0' )
                {
                    name = nameData.m_Name;
                }
            }

            bool selected = false;
            if( pEditorCore->Editor_GetSelectedObject() == pGameObject )
                selected = true;
        
            if( ImGui::Selectable( name, &selected ) )
            {
                pEditorCore->Editor_SetSelectedObject( pGameObject );
            }

            if( ImGui::BeginPopupContextItem() )
            {
                if( ImGui::MenuItem( "Delete" ) )
                {
                    if( pEditorCore->Editor_GetSelectedObject() == pGameObject )
                    {
                        pEditorCore->Editor_SetSelectedObject( nullptr );
                    }
                    pEditorCore->GetEventManager()->AddEvent( new RemoveFromGameEvent( pGameObject ) );
                }
            
                //if( ImGui::MenuItem( "Duplicate" ) )
                //{
                //    GameObject* pNewObject = new GameObject( this );
                //    //pNewObject->CopyFrom( pGameObject );
                //    m_Objects.push_back( pNewObject );
                //}

                //if( ImGui::MenuItem( "Rename" ) )
                //{
                //    ImGui::OpenPopup( "Rename" );
                //}

                //if( ImGui::BeginPopup("Rename ")
                //{
                //    static char name[128] = "";
                //        ImGui::InputText("Name", name, 128);
                //        if( ImGui::Button("OK") )
                //        {
                //            nameData.m_Name = name;
                //                ImGui::CloseCurrentPopup();
                //        }
                //    ImGui::EndPopup();
                //}

                if( ImGui::BeginMenu( "Add Component.." ) )
                {
                    m_pComponentManager->Editor_AddComponentToGameObject( pGameObject );
                    ImGui::EndMenu();
                }

                ImGui::EndPopup();
            }
        }
    }
}

} // namespace fw