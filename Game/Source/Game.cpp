//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "DataTypes.h"
#include "Game.h"
#include "Events/GameEvents.h"
#include "Objects/Player.h"
#include "Objects/PlayerController.h"
#include "Meshes/Shapes.h"

Game::Game(fw::FWCore& fwCore)
    : GameCore( fwCore )
{
    Init();
}

Game::~Game()
{
    for( auto& meshPair : m_pMeshes )
    {
        delete meshPair.second;
    }

    delete m_pTexture;

    for( auto& shaderPair : m_pShaders )
    {
        delete shaderPair.second;
    }

    for( fw::GameObject* pObject : m_Objects )
    {
        delete pObject;
    }
    
    delete m_pCamera;

    delete m_pPlayerController;

    delete m_pEventManager;
}

void Game::Init()
{
    // General renderer settings.
    bgfx::setViewClear( 0, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x000030ff, 1.0f, 0 );
    bgfx::setViewRect( 0, 0, 0, m_FWCore.GetWindowWidth(), m_FWCore.GetWindowHeight() );

    m_pEventManager = new fw::EventManager();

    // Create uniforms.
    m_Uniforms.CreateFrameworkUniforms();

    // Create vertex formats.
    InitTextureFormats();

    // Create some meshes.
    m_pMeshes["Triangle"] = CreateTriangleMesh();
    m_pMeshes["Square"] = CreateSquareMesh();
    m_pMeshes["Sprite"] = CreateSpriteMesh();

    // Create some shaders.
    m_pShaders["Color"] = new fw::ShaderProgram( "Data/Shaders/", "Color.vert.bin", "Color.frag.bin" );
    m_pShaders["Texture"] = new fw::ShaderProgram( "Data/Shaders/", "Texture.vert.bin", "Texture.frag.bin" );

    // Load some textures.
    m_pTexture = new fw::Texture( "Data/Textures/Sokoban.png" );

    // Create a controller.
    m_pPlayerController = new PlayerController();

    // Create some GameObjects.
    m_pCamera = new fw::Camera( this, vec3(5,5,-20) );
    m_pCamera->SetLookAtPosition( vec3(5,5,0) );
    m_pPlayer = new Player( this, m_pPlayerController, "Player", vec3(6,5,-0.1f), m_pMeshes["Sprite"], m_pShaders["Texture"] );
    m_pPlayer->SetHasAlpha( true );
    m_Objects.push_back( m_pPlayer );

    m_Objects.push_back( new fw::GameObject( this, "Enemy 1", vec3(0,0,0), m_pMeshes["Triangle"], m_pShaders["Color"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Enemy 2", vec3(10,10,0), m_pMeshes["Triangle"], m_pShaders["Color"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Enemy 3", vec3(5,5,0), m_pMeshes["Square"], m_pShaders["Color"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Enemy 4", vec3(1,1,0), m_pMeshes["Square"], m_pShaders["Color"] ) );
    m_Objects.push_back( new fw::GameObject( this, "Enemy 5", vec3(1,9,0), m_pMeshes["Square"], m_pShaders["Color"] ) );
}

void Game::StartFrame(float deltaTime)
{
    // Reset the controller.
    m_pPlayerController->StartFrame();

    // Process our events.
    m_pEventManager->DispatchAllEvents( deltaTime, this );
}

void Game::OnEvent(fw::Event* pEvent)
{
    m_pPlayerController->OnEvent( pEvent );

    if( pEvent->GetType() == RemoveFromGameEvent::GetStaticEventType() )
    {
        RemoveFromGameEvent* pRemoveFromGameEvent = static_cast<RemoveFromGameEvent*>( pEvent );
        fw::GameObject* pObject = pRemoveFromGameEvent->GetGameObject();

        auto it = std::find( m_Objects.begin(), m_Objects.end(), pObject );
        m_Objects.erase( it );

        delete pObject;
    }
}

void Game::Update(float deltaTime)
{
    for( fw::GameObject* pObject : m_Objects )
    {
        pObject->Update( deltaTime );
    }

    m_pCamera->Update( deltaTime );
}

void Game::Draw()
{
    //bgfx::touch( 0 );

    // Setup time uniforms.
    float time = (float)fw::GetSystemTimeSinceGameStart();
    bgfx::setUniform( m_Uniforms.m_Map["u_Time"], &time );

    // Hard-coded Texture and UV scale and offset for the sokoban player image.
    bgfx::setTexture( 0, m_Uniforms.m_Map["u_TextureColor"], m_pTexture->GetHandle() );
    vec4 uvScaleOffset( 64/1024.0f, 64/512.0f, 780/1024.0f, 383/512.0f );
    bgfx::setUniform( m_Uniforms.m_Map["u_UVScaleOffset"], &uvScaleOffset );

    // Program the view and proj uniforms from the camera.
    m_pCamera->Enable();

    // Draw all objects.
    for( fw::GameObject* pObject : m_Objects )
    {
        pObject->Draw();
    }

    //// Display debug stats.
    //bgfx::dbgTextClear();
    //bgfx::setDebug( BGFX_DEBUG_STATS );
}
