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
#include "EditorCore.h"
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
#include "../Libraries/ImGuizmo/ImGuizmo.h"
#include "../Libraries/nlohmann-json/single_include/nlohmann/json.hpp"

namespace fw {

EditorCore::EditorCore(FWCore& fwCore)
    : GameCore( fwCore )
{
    m_pImGuiManager = new ImGuiManager( &m_FWCore, EditorViews::EditorView_ImGui );

    bool isValid = bgfx::isTextureValid( 0, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT );
    assert( isValid );
    
    // Create an FBO along with a texture to render to.
    // TODO: Don't limit this to a 2048x2048 texture. Have it resize if the window is resized to a larger size.
    m_Game_FBOTexture = bgfx::createTexture2D( m_Game_TextureSize.x, m_Game_TextureSize.y, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT );
    bgfx::TextureHandle gameTextures[] = { m_Game_FBOTexture };
    m_Game_FBO = bgfx::createFrameBuffer( 1, gameTextures, true );

    m_Editor_FBOTexture = bgfx::createTexture2D( m_Editor_TextureSize.x, m_Editor_TextureSize.y, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT );
    bgfx::TextureHandle editorTextures[] = { m_Editor_FBOTexture };
    m_Editor_FBO = bgfx::createFrameBuffer( 1, editorTextures, true );

    // View 0: Render the game scene into an FBO.
    bgfx::setViewFrameBuffer( EditorViews::EditorView_Game, m_Game_FBO );
    bgfx::setViewClear( EditorViews::EditorView_Game, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00ff00ff, 1.0f, 0 );

    // View 1: Render the editor scene into an FBO.
    bgfx::setViewFrameBuffer( EditorViews::EditorView_Editor, m_Editor_FBO );
    bgfx::setViewClear( EditorViews::EditorView_Editor, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xff0000ff, 1.0f, 0 );

    // View 2: Render the imgui hud.
    bgfx::setViewRect( EditorViews::EditorView_ImGui, 0, 0, m_Editor_WindowSize.x, m_Editor_WindowSize.y );
    bgfx::setViewClear( EditorViews::EditorView_ImGui, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x0000ffff, 1.0f, 0 );
}

EditorCore::~EditorCore()
{
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
}

void EditorCore::StartFrame(float deltaTime)
{
    GameCore::StartFrame( deltaTime );
    ImGuizmo::BeginFrame();

    //ImGui::ShowDemoWindow();

    Editor_CreateMainFrame();
    Editor_DisplayMainMenu();
}

bool EditorCore::OnEvent(Event* pEvent)
{
    if( GameCore::OnEvent( pEvent ) )
        return true;

    return false;
}

void EditorCore::Update(float deltaTime)
{
    Editor_DisplayObjectList();
    Editor_ShowInspector();
    Editor_ShowResources();

    if( m_pImGuiManager->WantsKeyboard() == false )
    {
        HandleKeyboardShortcuts();
    }

    GameCore::Update( deltaTime );
}

void EditorCore::HandleKeyboardShortcuts()
{
    if( m_FWCore.IsKeyDown('1') )
    {
        m_Editor_GizmoMode = ImGuizmo::OPERATION::TRANSLATE;
    }
    if( m_FWCore.IsKeyDown('2') )
    {
        m_Editor_GizmoMode = ImGuizmo::OPERATION::ROTATE;
    }
    if( m_FWCore.IsKeyDown('3') )
    {
        m_Editor_GizmoMode = ImGuizmo::OPERATION::SCALE;
    }
}

void EditorCore::Draw()
{
    GameCore::Draw();
    
    if( true )
    {
        //mat4 world;
        //mat4 view;
        //mat4 proj;
        //world.SetIdentity();
        //view.CreateLookAtView( vec3(0,0,-5), vec3(0,1,0), vec3(0,0,0) );
        //proj.CreatePerspectiveVFoV( 45, 1, 0.01f, 100.0f );

        //bgfx::setViewRect( EditorViews::EditorView_Game, 0, 0, 1280, 720 );
        //bgfx::setViewTransform( EditorViews::EditorView_Game, &view, &proj );
        //m_pResources->GetMesh("Square")->Draw( EditorViews::EditorView_Game, m_pUniforms, m_pResources->GetMaterial("Red"), &world );

        //ImVec2 uvMin = ImVec2(0,0);
        //ImVec2 uvMax = ImVec2(1,1);
        ////ImGui::Image( fw::imguiTexture(m_pResources->GetTexture("Sokoban")), ImVec2(256,256), uvMin, uvMax );
        //ImGui::Image( fw::imguiTexture(m_Game_FBOTexture), ImVec2(256,256), uvMin, uvMax );

        Editor_DrawGameView( EditorViews::EditorView_Game );
        Editor_DrawEditorView( EditorViews::EditorView_Editor );
    }
    else
    {
        // Draw only the game view. Needs to disable the "Main Dock" window, it covers this render.
        m_Game_WindowSize.x = m_FWCore.GetWindowClientWidth();
        m_Game_WindowSize.y = m_FWCore.GetWindowClientHeight();
        bgfx::setViewRect( EditorViews::EditorView_Game, 0, 0, m_Game_WindowSize.x, m_Game_WindowSize.y );
        bgfx::setViewClear( EditorViews::EditorView_Game, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0 );
        m_pActiveScene->Draw( EditorViews::EditorView_Game );
    }
}

void EditorCore::EndFrame()
{
    GameCore::EndFrame();
}

void EditorCore::OnShutdown()
{
    GameCore::OnShutdown();
}

void EditorCore::Editor_CreateMainFrame()
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

void EditorCore::Editor_DisplayMainMenu()
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
            m_pEditor_SelectedObject = nullptr;
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

void EditorCore::Editor_DisplayObjectList()
{
    ImGui::Begin( "Object List" );

    m_pActiveScene->Editor_DisplayObjectList();

    ImGui::End(); // "Object List"
}

void EditorCore::Editor_ShowInspector()
{
    if( ImGui::Begin( "Inspector" ) )
    {
        if( m_pEditor_SelectedObject )
        {
            m_pActiveScene->GetComponentManager()->Editor_DisplayComponentsForGameObject( m_pEditor_SelectedObject );
        }
    }
    ImGui::End();
}

void EditorCore::Editor_ShowResources()
{
    if( ImGui::Begin( "Resources" ) )
    {
        m_pResources->Editor_DisplayResources();
    }
    ImGui::End();
}

void EditorCore::Editor_DrawGameView(int viewID)
{
    // Draw our main view in a window.
    if( ImGui::Begin("Game view") )
    {
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        ivec2 size = ivec2(contentMax.x, contentMax.y) - ivec2(contentMin.x, contentMin.y);
        m_Game_WindowSize = ivec2( (int)size.x, (int)size.y );
        if( m_Game_WindowSize.x > m_Game_TextureSize.x ) { m_Game_WindowSize.x = m_Game_TextureSize.x; }
        if( m_Game_WindowSize.y > m_Game_TextureSize.y ) { m_Game_WindowSize.y = m_Game_TextureSize.y; }
        bgfx::setViewRect( EditorViews::EditorView_Game, 0, 0, m_Game_WindowSize.x, m_Game_WindowSize.y );

        m_pActiveScene->Draw( viewID );

        ImVec2 uvMin = ImVec2(0,0);
        ImVec2 uvMax = ImVec2( (float)m_Game_WindowSize.x / m_Game_TextureSize.x, (float)m_Game_WindowSize.y / m_Game_TextureSize.y );
        if( bgfx::getRendererType() == bgfx::RendererType::OpenGL )
        {
            uvMin.y = 1.0f;
            uvMax.y = 1.0f - uvMax.y;
        }
        
        ImGui::Image( fw::imguiTexture(m_Game_FBOTexture), ImVec2( (float)m_Game_WindowSize.x, (float)m_Game_WindowSize.y ), uvMin, uvMax );
    }
    ImGui::End();
}

void EditorCore::Editor_DrawEditorView(int viewID)
{
    mat4 worldMat;
    mat4 deltaMat;

    // Draw our main view in a window.
    if( ImGui::Begin("Editor view") )
    {
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        ivec2 size = ivec2(contentMax.x, contentMax.y) - ivec2(contentMin.x, contentMin.y);
        m_Editor_WindowSize = ivec2( (int)size.x, (int)size.y );
        if( m_Editor_WindowSize.x > m_Editor_TextureSize.x ) { m_Editor_WindowSize.x = m_Editor_TextureSize.x; }
        if( m_Editor_WindowSize.y > m_Editor_TextureSize.y ) { m_Editor_WindowSize.y = m_Editor_TextureSize.y; }
        bgfx::setViewRect( EditorViews::EditorView_Editor, 0, 0, m_Editor_WindowSize.x, m_Editor_WindowSize.y );

        m_pActiveScene->Draw( viewID );

        ImVec2 uvMin = ImVec2(0,0);
        ImVec2 uvMax = ImVec2( (float)m_Editor_WindowSize.x / m_Editor_TextureSize.x, (float)m_Editor_WindowSize.y / m_Editor_TextureSize.y );
        if( bgfx::getRendererType() == bgfx::RendererType::OpenGL )
        {
            uvMin.y = 1.0f;
            uvMax.y = 1.0f - uvMax.y;
        }

        ImGui::Image( fw::imguiTexture(m_Editor_FBOTexture), ImVec2( (float)m_Editor_WindowSize.x, (float)m_Editor_WindowSize.y ), uvMin, uvMax );

        if( m_pEditor_SelectedObject )
        {
            ImVec2 pos = ImGui::GetWindowPos();
            
            Camera* pCamera = m_pActiveScene->GetCamera();
            if( pCamera )
            {
                mat4& view = pCamera->GetViewMatrix();
                mat4& proj = pCamera->GetProjectionMatrix();

                ImGuizmo::SetOrthographic( false );
                ImGuizmo::SetDrawlist();
                ImGuizmo::SetRect( pos.x + contentMin.x, pos.y + contentMin.y, size.x, size.y );

                if( m_pActiveScene->GetECSRegistry().try_get<TransformData>( m_pEditor_SelectedObject->GetEntityID() ) )
                {
                    TransformData& transform = m_pActiveScene->GetECSRegistry().get<TransformData>( m_pEditor_SelectedObject->GetEntityID() );
                    worldMat.CreateSRT( transform.scale, transform.rotation, transform.position );
            
                    if( ImGuizmo::Manipulate( &view.m11, &proj.m11, m_Editor_GizmoMode, ImGuizmo::MODE::LOCAL, &worldMat.m11, &deltaMat.m11 ) )
                    {
                        transform.position = worldMat.GetTranslation();
                        transform.scale = worldMat.GetScale();

                        // This isn't working well.
                        transform.rotation = worldMat.GetEulerAngles();
                    }
                }
            }
        }
    }
    ImGui::End();

    //ImGui::Text( "%0.1f, %0.1f, %0.1f, %0.1f", worldMat.m11, worldMat.m12, worldMat.m13, worldMat.m14 );
    //ImGui::Text( "%0.1f, %0.1f, %0.1f, %0.1f", worldMat.m21, worldMat.m22, worldMat.m23, worldMat.m24 );
    //ImGui::Text( "%0.1f, %0.1f, %0.1f, %0.1f", worldMat.m31, worldMat.m32, worldMat.m33, worldMat.m34 );
    //ImGui::Text( "%0.1f, %0.1f, %0.1f, %0.1f", worldMat.m41, worldMat.m42, worldMat.m43, worldMat.m44 );

    //ImGui::Text( "%0.1f, %0.1f, %0.1f, %0.1f", deltaMat.m11, deltaMat.m12, deltaMat.m13, deltaMat.m14 );
    //ImGui::Text( "%0.1f, %0.1f, %0.1f, %0.1f", deltaMat.m21, deltaMat.m22, deltaMat.m23, deltaMat.m24 );
    //ImGui::Text( "%0.1f, %0.1f, %0.1f, %0.1f", deltaMat.m31, deltaMat.m32, deltaMat.m33, deltaMat.m34 );
    //ImGui::Text( "%0.1f, %0.1f, %0.1f, %0.1f", deltaMat.m41, deltaMat.m42, deltaMat.m43, deltaMat.m44 );
}

} // namespace fw
