//
// Copyright (c) 2022-2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"
#include "GameCore.h"
#include "Components/ComponentManager.h"
#include "Components/CoreComponents.h"
#include "EventSystem/Events.h"
#include "EventSystem/EventManager.h"
#include "Imgui/ImGuiManager.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Renderer/Uniforms.h"
#include "Resources/Material.h"
#include "Resources/Mesh.h"
#include "Scenes/Scene.h"
#include "Utility/Utility.h"

namespace fw {

GameCore::GameCore(FWCore& fwCore)
    : m_FWCore( fwCore )
{
}

GameCore::~GameCore()
{
    delete m_pImGuiManager;

    delete m_pEventManager;

    delete m_pUniforms;
}

ComponentManager* GameCore::CreateComponentManager()
{
    return new ComponentManager();
}

Scene* GameCore::CreateScene()
{
    return new Scene( this );
}

void GameCore::StartFrame(float deltaTime)
{
    m_pImGuiManager->StartFrame( deltaTime );
    //ImGui::ShowDemoWindow();
}

bool GameCore::OnEvent(Event* pEvent)
{
    m_pActiveScene->OnEvent( pEvent );

    if( pEvent->GetType() == OnCharEvent::GetStaticEventType() )
    {
        OnCharEvent* pCharEvent = static_cast<OnCharEvent*>( pEvent );
        
        m_pImGuiManager->AddInputCharacter( pCharEvent->GetValue() );
    }

    if( m_pImGuiManager->WantsKeyboard() )
    {
        if( pEvent->GetType() == OnCharEvent::GetStaticEventType() )
            return true;

        if( pEvent->GetType() == InputEvent::GetStaticEventType() )
            return true;
    }

    return false;
}

void GameCore::Update(float deltaTime)
{
    m_pActiveScene->Update( deltaTime );
}

void GameCore::Draw()
{
}

void GameCore::EndFrame()
{
    m_pImGuiManager->EndFrame();
}

void GameCore::OnShutdown()
{
}

} // namespace fw
