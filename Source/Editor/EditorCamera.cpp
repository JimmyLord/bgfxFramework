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
    FWCore* pFramework = m_pEditorCore->GetFramework();
    
    float speed = 10;
    float rotSpeed = 5;
    
    vec3 dir(0,0,0);

    if( pFramework->IsKeyDown('W') )
        dir.z += 1;
    if( pFramework->IsKeyDown('S') )
        dir.z -= 1;
    if( pFramework->IsKeyDown('A') )
        dir.x -= 1;
    if( pFramework->IsKeyDown('D') )
        dir.x += 1;
    if( pFramework->IsKeyDown('Q') )
        dir.y += 1;
    if( pFramework->IsKeyDown('E') )
        dir.y -= 1;
    if( pFramework->IsKeyDown('Z') )
        dir.y -= 1;

    dir.Normalize();

    bool mouseDown = pFramework->IsMouseButtonDown(1);
    vec2 mouseDir = pFramework->GetMouseDir();
    
    if( mouseDown )
    {
        m_Rotation.x += mouseDir.y * deltaTime * rotSpeed;
        m_Rotation.y += mouseDir.x * deltaTime * rotSpeed;
    }
    
    // Rotate dir by camera rotation.
    mat4 rot;
    rot.SetIdentity();
    rot.Rotate( m_Rotation.x, 1, 0, 0 ); // pitch
    rot.Rotate( m_Rotation.y, 0, 1, 0 ); // yaw
    
    vec3 at = rot.GetAt();
    vec3 up = rot.GetUp();
    vec3 right = rot.GetRight();
    
    vec3 newdir = dir.x * right + dir.y * up + dir.z * at;

    m_Position += newdir * deltaTime * speed;
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
