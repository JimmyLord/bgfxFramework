//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math/Vector.h"

namespace fw {

class Camera;
class GameCore;
class Mesh;
class ShaderProgram;
class Texture;

class GameObject
{
public:
    GameObject(GameCore* pGameCore, std::string name, vec3 pos, Mesh* pMesh, ShaderProgram* pShader);
    virtual ~GameObject();

    virtual void Update(float deltaTime);
    virtual void Draw();

    // Getters.
    std::string GetName() { return m_Name; }
    vec3 GetPosition() { return m_Position; }

    // Setters.
    void SetPosition(vec3 pos) { m_Position = pos; }
    void SetHasAlpha(bool hasAlpha) { m_HasAlpha = hasAlpha; }

protected:
    GameCore* m_pGameCore = nullptr;

    std::string m_Name;

    vec3 m_Position;

    Mesh* m_pMesh = nullptr;
    ShaderProgram* m_pShader = nullptr;
    bool m_HasAlpha = false;
};

} // namespace fw
