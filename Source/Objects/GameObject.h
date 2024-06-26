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
class Material;
class Mesh;
class Scene;
class ShaderProgram;
class Texture;
class Uniforms;

class GameObject
{
public:
    GameObject(Scene* pScene);
    GameObject(Scene* pScene, std::string name, vec3 pos, Mesh* pMesh, Material* pMaterial);
    virtual ~GameObject();

    virtual void Update(float deltaTime);

    // Getters.
    Scene* GetScene() { return m_pScene; }
    flecs::entity GetEntity() { return m_Entity; }

    // Save/Load.
    virtual void SaveToJSON(nlohmann::json& jGameObject);
    virtual void LoadFromJSON(nlohmann::json& jGameObject);

protected:
    Scene* m_pScene = nullptr;

    flecs::entity m_Entity;
};

} // namespace fw
