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

#include "EditorCamera.h"
#include "EditorCore.h"
#include "FWCore.h"
#include "GameCore.h"
#include "Components/CoreComponents.h"
#include "Scenes/Scene.h"

namespace fw {

EditorCamera::EditorCamera(EditorCore* pEditorCore, vec3 pos)
    : m_pEditorCore( pEditorCore )
    , m_Position( pos )
{
}

EditorCamera::~EditorCamera()
{
}

void EditorCamera::Update(float deltaTime)
{
    float speed = 10;
    vec3 dir(0,0,0);

    if( m_pEditorCore->GetFramework()->IsKeyDown('W') )
        dir.z += 1;
    if( m_pEditorCore->GetFramework()->IsKeyDown('S') )
        dir.z -= 1;
    if( m_pEditorCore->GetFramework()->IsKeyDown('A') )
        dir.x -= 1;
    if( m_pEditorCore->GetFramework()->IsKeyDown('D') )
        dir.x += 1;
    if( m_pEditorCore->GetFramework()->IsKeyDown('Q') )
        dir.y -= 1;
    if( m_pEditorCore->GetFramework()->IsKeyDown('E') )
        dir.y += 1;

    dir.Normalize();

    m_Position += dir * deltaTime * speed;
}

void EditorCamera::UpdateTransforms()
{
    m_ViewMatrix.CreateSRT( vec3(1,1,1), m_Rotation, m_Position );
    m_ViewMatrix.Inverse();
    
    m_ProjectionMatrix.CreatePerspectiveVFoV( 45.0f, m_AspectRatio, 0.01f, 100.0f );
}

void EditorCamera::Enable(int viewID)
{
    // Setup view and projection matrices and uniforms.
    bgfx::setViewTransform( viewID, &m_ViewMatrix.m11, &m_ProjectionMatrix.m11 );
}

} // namespace fw
