//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "bgfx/platform.h"

#include "Camera.h"
#include "GameCore.h"
#include "Components/CoreComponents.h"

namespace fw {

Camera::Camera(GameCore* pGameCore, vec3 pos)
    : GameObject( pGameCore, "Camera", pos, nullptr, nullptr )
{
}

Camera::~Camera()
{
}

void Camera::Update(float32 deltaTime)
{
    if( m_pObjectWeAreLookingAt )
    {
        entt::entity entityID = m_pObjectWeAreLookingAt->GetEntityID();
        const TransformData& transformData = m_pGameCore->GetECSRegistry().get<TransformData>( entityID );
        m_LookAtPosition = transformData.position;
    }

    const TransformData& transformData = m_pGameCore->GetECSRegistry().get<TransformData>( m_EntityID );
    m_ViewMatrix.CreateLookAtView( transformData.position, vec3(0,1,0), m_LookAtPosition );
    m_ProjectionMatrix.CreatePerspectiveVFoV( 45.0f, m_AspectRatio, 0.01f, 100.0f );
}

void Camera::Enable(int viewID)
{
    // Setup view and projection matrices and uniforms.
    bgfx::setViewTransform( viewID, &m_ViewMatrix.m11, &m_ProjectionMatrix.m11 );
}

} // namespace fw
