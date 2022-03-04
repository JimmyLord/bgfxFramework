//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "DataTypes.h"

class PlayerController;

class Player : public fw::GameObject
{
    typedef void (Player::*AIStateFunction)(float deltaTime);

public:
    Player(fw::GameCore* pGameCore, PlayerController* pPlayerController, std::string name, vec3 pos, fw::Mesh* pMesh, fw::ShaderProgram* pShader);
    virtual ~Player();

    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    void AIState_Idle(float deltaTime);
    void AIState_Shaking(float deltaTime);

protected:
    PlayerController* m_pPlayerController = nullptr;

    AIStateFunction m_pCurrentStateFunction = &Player::AIState_Idle;

    float m_IdleTimer = 0.0f;
    float m_ShakingTimer = 0.0f;

    vec2 m_ShakeOffset = vec2(0,0);
};
