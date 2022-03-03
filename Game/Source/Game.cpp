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
#include "Meshes/Shapes.h"
#include "Meshes/VertexFormats.h"

Game::Game(fw::FWCore& fwCore)
    : GameCore( fwCore )
{
    Init();
}

Game::~Game()
{
    delete m_pMesh;
    delete m_pShader;
}

void Game::Init()
{
    // General renderer settings.
    bgfx::setViewClear( 0, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH, 0x000030ff, 1.0f, 0 );
    bgfx::setViewRect( 0, 0, 0, m_FWCore.GetWindowWidth(), m_FWCore.GetWindowHeight() );

    // Create uniforms.
    m_Uniforms.CreateFrameworkUniforms();

    // Create vertex formats.
    VF_PosColor::InitVertexLayout();

    // Create some meshes.
    m_pMesh = CreateTriangle();

    // Create some shaders.
    m_pShader = new fw::ShaderProgram( "Data/Shaders/", "Basic.vert.bin", "Basic.frag.bin" );
}

void Game::StartFrame(float deltaTime)
{
}

void Game::Update(float deltaTime)
{
    float speed = 4.0f;

    if( m_FWCore.IsKeyDown('W') )
        m_Position.y += speed * deltaTime;
    if( m_FWCore.IsKeyDown('S') )
        m_Position.y -= speed * deltaTime;
    if( m_FWCore.IsKeyDown('A') )
        m_Position.x -= speed * deltaTime;
    if( m_FWCore.IsKeyDown('D') )
        m_Position.x += speed * deltaTime;
}

void Game::Draw()
{
    //bgfx::touch( 0 );

    // Setup time uniforms.
    float time = (float)fw::GetSystemTimeSinceGameStart();
    bgfx::setUniform( m_Uniforms.m_Map["u_Time"], &time );

    // Setup view and projection matrices and uniforms.
    mat4 viewMat;
    viewMat.CreateLookAtView( vec3(0,0,-10), vec3(0,1,0), vec3(0,0,0) );
    mat4 projMat;
    float aspectRatio = m_FWCore.GetWindowWidth()/(float)m_FWCore.GetWindowHeight();
    projMat.CreatePerspectiveVFoV( 45.0f, aspectRatio, 0.01f, 100.0f );
    bgfx::setViewTransform( 0, &viewMat.m11, &projMat.m11 );

    // Draw a single mesh with a unique world transform.
    mat4 worldMat;
    worldMat.CreateSRT( vec3(1), vec3(0), m_Position );
    m_pMesh->Draw( m_pShader, &worldMat );

    //// Display debug stats.
    //bgfx::dbgTextClear();
    //bgfx::setDebug( BGFX_DEBUG_STATS );
}
