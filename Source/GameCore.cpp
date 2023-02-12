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
#include "Components/ComponentManager.h"
#include "Components/CoreComponents.h"
#include "Imgui/ImGuiManager.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Resources/Material.h"
#include "Resources/Mesh.h"
#include "Scenes/Scene.h"
#include "Utility/Utility.h"
#include "../Libraries/imgui/imgui.h"
#include "../Libraries/nlohmann-json/single_include/nlohmann/json.hpp"

namespace fw {

GameCore::GameCore(FWCore& fwCore)
    : m_FWCore( fwCore )
{
    m_pImGuiManager = new ImGuiManager( &m_FWCore, 1 );
}

GameCore::~GameCore()
{
    delete m_pImGuiManager;
}

void GameCore::StartFrame(float deltaTime)
{
    m_pImGuiManager->StartFrame( deltaTime );

    Editor_CreateMainFrame();
    Editor_DisplayMainMenu();
}

void GameCore::Update(float deltaTime)
{
    Editor_DisplayObjectList();
    Editor_ShowInspector();
}

void GameCore::EndFrame()
{
    m_pImGuiManager->EndFrame();
}

void GameCore::Editor_CreateMainFrame()
{
    // Setup a main window with no frame and a dockspace that covers the entire viewport.
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar
                           | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
                           | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos( viewport->WorkPos );
    ImGui::SetNextWindowSize( viewport->WorkSize );

    if( ImGui::Begin( "Main Dock", nullptr, flags ) )
    {
        ImGuiID dockspaceID = ImGui::GetID( "My Dockspace" );
        ImGui::DockSpace( dockspaceID );
    }
    ImGui::End();
}

void GameCore::Editor_DisplayMainMenu()
{
    // Add the main menu bar.
    ImGui::BeginMainMenuBar();

    if( ImGui::BeginMenu( "File" ) )
    {
        // Show bgfx debug stats.
        if( ImGui::MenuItem( "Save Scene", "" ) )
        {
            nlohmann::json jScene;
            m_pActiveScene->SaveToJSON( jScene );
            std::string jsonString = jScene.dump( 4 );
            SaveCompleteFile( "Data/Scenes/TestScene.scene", jsonString.c_str(), (int32)jsonString.length() );
        }

        if( ImGui::MenuItem( "Load Scene", "" ) )
        {
            delete m_pActiveScene;
            m_pActiveScene = new Scene( this );

            const char* jsonString = LoadCompleteFile( "Data/Scenes/TestScene.scene", nullptr );
            nlohmann::json jScene = nlohmann::json::parse( jsonString );
            delete[] jsonString;

            m_pActiveScene->LoadFromJSON( jScene );
        }

        ImGui::EndMenu();
    }

    if( ImGui::BeginMenu( "Debug" ) )
    {
        // Show bgfx debug stats.
        ImGui::MenuItem( "Show Debug Stats", "", &m_ShowDebugStats );
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void GameCore::Editor_DisplayObjectList()
{
    ImGui::Begin( "Object List" );

    m_pActiveScene->Editor_DisplayObjectList();

    ImGui::End(); // "Object List"
}

void GameCore::Editor_ShowInspector()
{
    if( ImGui::Begin( "Inspector" ) )
    {
        if( m_pSelectedObject )
        {
            m_pActiveScene->GetComponentManager()->Editor_DisplayComponentsForGameObject( m_pSelectedObject );
        }
    }
    ImGui::End();
}

} // namespace fw
