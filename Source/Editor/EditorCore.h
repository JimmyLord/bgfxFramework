//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "GameCore.h"
#include "Math/Vector.h"
#include "../Libraries/bgfx/include/bgfx/bgfx.h"
#include "../Libraries/imgui/imgui.h"
#include "../Libraries/ImGuizmo/ImGuizmo.h"
#include "../Libraries/entt/src/entt/entt.hpp"

namespace fw {

class Camera;
class EditorCamera;
class Event;
class EventManager;
class FWCore;
class GameObject;
class ImGuiManager;
class ResourceManager;
class Scene;
class Uniforms;

enum EditorViews
{
    EditorView_Game,
    EditorView_Editor,
    EditorView_ImGui,
};

class EditorCore : public GameCore
{
public:
    EditorCore(FWCore& fwCore);
    virtual ~EditorCore();

    virtual void StartFrame(float deltaTime) override;
    virtual bool OnEvent(Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    void HandleKeyboardShortcuts();
    virtual void Draw() override;
    virtual void EndFrame() override;
    virtual void OnShutdown() override;

    void LoadScene(const char* filename);
    void SaveScene();

    // Editor.
    void Editor_CreateMainFrame();
    void Editor_DisplayMainMenu();
    void Editor_DisplayObjectList();
    void Editor_ShowInspector();
    void Editor_ShowResources();
    void Editor_SetSelectedObject(GameObject* pObject) { m_pEditor_SelectedObject = pObject; }
    void Editor_DrawGameView(int viewID);
    void Editor_DrawEditorView(int viewID);
    GameObject* Editor_GetSelectedObject() { return m_pEditor_SelectedObject; }

    ivec2 GetGameWindowSize() { return m_Game_WindowSize; }
    ivec2 GetEditorWindowSize() { return m_Editor_WindowSize; }

    ImGuizmo::OPERATION m_Editor_GizmoMode = ImGuizmo::TRANSLATE;

protected:
    void InitImFileDialog();

protected:
    // Editor variables.
    GameObject* m_pEditor_SelectedObject = nullptr;
    EditorCamera* m_pEditorCamera = nullptr;

    // Windows/Focus.
    bool m_Editor_GameViewInFocus = false;
    bool m_Editor_EditorViewInFocus = false;

    // Render to Texture
    ivec2 m_Game_WindowSize = vec2( 512, 512 );
    ivec2 m_Game_TextureSize = vec2( 2048, 2048 );
    bgfx::TextureHandle m_Game_FBOTexture = BGFX_INVALID_HANDLE;
    bgfx::FrameBufferHandle m_Game_FBO = BGFX_INVALID_HANDLE;

    ivec2 m_Editor_WindowSize = vec2( 512, 512 );
    ivec2 m_Editor_TextureSize = vec2( 2048, 2048 );
    bgfx::TextureHandle m_Editor_FBOTexture = BGFX_INVALID_HANDLE;
    bgfx::FrameBufferHandle m_Editor_FBO = BGFX_INVALID_HANDLE;
};

} // namespace fw
