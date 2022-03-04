//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "Framework.h"

#include "Objects/Player.h"
#include "Objects/PlayerController.h"
#include "Game.h"

Player::Player(fw::GameCore* pGameCore, PlayerController* pPlayerController, std::string name, vec3 pos, fw::Mesh* pMesh, fw::ShaderProgram* pShader)
    : fw::GameObject( pGameCore, name, pos, pMesh, pShader )
    , m_pPlayerController( pPlayerController )
{
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
    float speed = 2.0f;

    vec2 dir;

    if( m_pPlayerController->IsHeld( PlayerController::Mask::Up ) )
    {
        dir.y += 1;
    }
    if( m_pPlayerController->IsHeld( PlayerController::Mask::Down ) )
    {
        dir.y += -1;
    }
    if( m_pPlayerController->IsHeld( PlayerController::Mask::Left ) )
    {
        dir.x += -1;
    }
    if( m_pPlayerController->IsHeld( PlayerController::Mask::Right ) )
    {
        dir.x += 1;
    }

    dir.Normalize();

    m_Position += dir * speed * deltaTime;

    (this->*m_pCurrentStateFunction)( deltaTime );
}

void Player::AIState_Idle(float deltaTime)
{
    m_IdleTimer += deltaTime;
    if( m_IdleTimer > 1 )
    {
        m_pCurrentStateFunction = &Player::AIState_Shaking;
        m_IdleTimer = 0;
    }
}

void Player::AIState_Shaking(float deltaTime)
{
    m_ShakeOffset.x += (((rand()%RAND_MAX)/(float)RAND_MAX) * 2 - 1) * 0.05f;
    m_ShakeOffset.y += (((rand()%RAND_MAX)/(float)RAND_MAX) * 2 - 1) * 0.05f;

    m_ShakingTimer += deltaTime;
    if( m_ShakingTimer > 1 )
    {
        m_pCurrentStateFunction = &Player::AIState_Idle;
        m_ShakingTimer = 0;
        m_ShakeOffset.Set(0,0);
    }
}

void Player::Draw()
{
    vec3 pos = m_Position + m_ShakeOffset;

    mat4 worldMat;
    worldMat.CreateSRT( vec3(1), vec3(0), pos );
    m_pMesh->Draw( m_pShader, &worldMat );
}
