//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"
#include "Framework.h"
#include "GameCore.h"
#include "Components/ComponentManager.h"
#include "Components/CoreComponents.h"
#include "EventSystem/EventManager.h"
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
    m_pImGuiManager = new ImGuiManager( &m_FWCore, 255 );

    // Create an FBO along with a texture to render to.
    // TODO: Don't limit this to a 2048x2048 texture. Have it resize if the window is resized to a larger size.
    m_Game_FBOTexture = bgfx::createTexture2D( m_Game_TextureSize.x, m_Game_TextureSize.y, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL );
    bgfx::TextureHandle gameTextures[] = { m_Game_FBOTexture };
    m_Game_FBO = bgfx::createFrameBuffer( 1, gameTextures, true );

    m_Editor_FBOTexture = bgfx::createTexture2D( m_Editor_TextureSize.x, m_Editor_TextureSize.y, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL );
    bgfx::TextureHandle editorTextures[] = { m_Editor_FBOTexture };
    m_Editor_FBO = bgfx::createFrameBuffer( 1, editorTextures, true );
}

GameCore::~GameCore()
{
    delete m_pImGuiManager;

    if( bgfx::isValid( m_Game_FBO ) )
    {
        bgfx::destroy( m_Game_FBO );
    }

    if( bgfx::isValid( m_Game_FBOTexture ) )
    {
        bgfx::destroy( m_Game_FBOTexture );
    }

    if( bgfx::isValid( m_Editor_FBO ) )
    {
        bgfx::destroy( m_Editor_FBO );
    }

    if( bgfx::isValid( m_Editor_FBOTexture ) )
    {
        bgfx::destroy( m_Editor_FBOTexture );
    }

    delete m_pEventManager;
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

    m_pActiveScene->Update( deltaTime );
}

void GameCore::Draw()
{
    int gameViewID = 0;

    if( true )
    {
        int editorViewID = 1;
        
        Editor_DrawGameView( gameViewID );
        Editor_DrawEditorView( editorViewID );
    }
    else
    {
        // Draw only the game view. Needs to disable the "Main Dock" window, it covers this render.
        m_Game_WindowSize.x = m_FWCore.GetWindowClientWidth();
        m_Game_WindowSize.y = m_FWCore.GetWindowClientHeight();
        bgfx::setViewRect( gameViewID, 0, 0, m_Game_WindowSize.x, m_Game_WindowSize.y );
        bgfx::setViewClear( gameViewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0 );
        m_pActiveScene->Draw( gameViewID );
    }
}

void GameCore::EndFrame()
{
    m_pImGuiManager->EndFrame();
}

void GameCore::OnShutdown()
{
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

void GameCore::Editor_DrawGameView(int viewID)
{
    // Render the scene into an FBO.
    bgfx::setViewFrameBuffer( viewID, m_Game_FBO );
    bgfx::setViewRect( viewID, 0, 0, m_Game_WindowSize.x, m_Game_WindowSize.y );
    bgfx::setViewClear( viewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0 );

    // Draw our main view in a window.
    if( ImGui::Begin("Game view") )
    {
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        ivec2 size = ivec2(contentMax.x, contentMax.y) - ivec2(contentMin.x, contentMin.y);
        m_Game_WindowSize = ivec2( (int)size.x, (int)size.y );
        if( m_Game_WindowSize.x > m_Game_TextureSize.x ) { m_Game_WindowSize.x = m_Game_TextureSize.x; }
        if( m_Game_WindowSize.y > m_Game_TextureSize.y ) { m_Game_WindowSize.y = m_Game_TextureSize.y; }

        m_pActiveScene->Draw( viewID );

        ImVec2 uvMax = ImVec2( (float)m_Game_WindowSize.x / m_Game_TextureSize.x, (float)m_Game_WindowSize.y / m_Game_TextureSize.y );
        ImGui::Image( fw::imguiTexture(m_Game_FBOTexture), ImVec2( (float)m_Game_WindowSize.x, (float)m_Game_WindowSize.y ), ImVec2(0,0), uvMax );
    }
    ImGui::End();
}

void GameCore::Editor_DrawEditorView(int viewID)
{
    // Render the scene into an FBO.
    bgfx::setViewFrameBuffer( viewID, m_Editor_FBO );
    bgfx::setViewRect( viewID, 0, 0, m_Editor_WindowSize.x, m_Editor_WindowSize.y );
    bgfx::setViewClear( viewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x001000ff, 1.0f, 0 );

    // Draw our main view in a window.
    if( ImGui::Begin("Editor view") )
    {
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        ivec2 size = ivec2(contentMax.x, contentMax.y) - ivec2(contentMin.x, contentMin.y);
        m_Editor_WindowSize = ivec2( (int)size.x, (int)size.y );
        if( m_Editor_WindowSize.x > m_Editor_TextureSize.x ) { m_Editor_WindowSize.x = m_Editor_TextureSize.x; }
        if( m_Editor_WindowSize.y > m_Editor_TextureSize.y ) { m_Editor_WindowSize.y = m_Editor_TextureSize.y; }

        m_pActiveScene->Draw( viewID );

        ImVec2 uvMax = ImVec2( (float)m_Editor_WindowSize.x / m_Editor_TextureSize.x, (float)m_Editor_WindowSize.y / m_Editor_TextureSize.y );
        ImGui::Image( fw::imguiTexture(m_Editor_FBOTexture), ImVec2( (float)m_Editor_WindowSize.x, (float)m_Editor_WindowSize.y ), ImVec2(0,0), uvMax );
    }
    ImGui::End();
}

} // namespace fw
