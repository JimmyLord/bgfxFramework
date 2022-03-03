//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

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
    bgfx::setViewRect( 0, 0, 0, 600, 600 );

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
}

void Game::Draw()
{
    //bgfx::touch( 0 );

    m_pMesh->Draw( m_pShader );

    //// Display debug stats.
    //bgfx::dbgTextClear();
    //bgfx::setDebug( BGFX_DEBUG_STATS );
}
