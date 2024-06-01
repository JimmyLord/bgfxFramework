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
#include "../Libraries/bgfx/include/bgfx/bgfx.h"
#include "../Libraries/imgui/imgui.h"
#include "../Libraries/ImGuizmo/ImGuizmo.h"

namespace fw {

class Camera;
class Event;
class EventManager;
class FWCore;
class GameObject;
class ImGuiManager;
class ResourceManager;
class Scene;
class Uniforms;

class GameCore
{
public:
    GameCore(FWCore& fwCore);
    virtual ~GameCore();

    virtual void StartFrame(float deltaTime);
    virtual bool OnEvent(Event* pEvent);
    virtual void Update(float deltaTime);
    void HandleKeyboardShortcuts();
    virtual void Draw();
    virtual void EndFrame();
    virtual void OnShutdown();

    // Getters.
    FWCore* GetFramework() { return &m_FWCore; }
    ResourceManager* GetResourceManager() { return m_pResources; }
    Uniforms* GetUniforms() { return m_pUniforms; }
    EventManager* GetEventManager() { return m_pEventManager; }

protected:
    FWCore& m_FWCore;

    // Interface.
    ImGuiManager* m_pImGuiManager = nullptr;

    // Resources.
    Uniforms* m_pUniforms = nullptr;
    ResourceManager* m_pResources = nullptr;

    // Events.
    EventManager* m_pEventManager = nullptr;

    // Scene.
    Scene* m_pActiveScene = nullptr;

    // Other.
    bool m_ShowDebugStats = false;
};

} // namespace fw
