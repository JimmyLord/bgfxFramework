//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math/Matrix.h"
#include "Objects/GameObject.h"

namespace fw {

class Camera : public GameObject
{
public:
    Camera(GameCore* pGameCore, vec3 pos);
    virtual ~Camera();

    virtual void Update(float32 deltaTime) override;

    void Enable();

    // Getters.
    mat4& GetViewMatrix() { return m_ViewMatrix; }
    mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }

    // Setters.
    void SetAspectRatio(float32 aspectRatio) { m_AspectRatio = aspectRatio; }
    void SetLookAtPosition(vec3 pos) { m_LookAtPosition = pos; }
    void SetObjectWeAreLookingAt(GameObject* pObj) { m_pObjectWeAreLookingAt = pObj; }

protected:
    mat4 m_ViewMatrix;
    mat4 m_ProjectionMatrix;

    float32 m_AspectRatio = 1.0f;

    vec3 m_LookAtPosition = vec3(0,0,0);
    GameObject* m_pObjectWeAreLookingAt = nullptr;
};

} // namespace fw
