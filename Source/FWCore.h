//
// Copyright (c) 2016-2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "bgfx/platform.h"

namespace fw {

class GameCore;

class FWCore
{
public:
    FWCore(uint32 width, uint32 height, bgfx::RendererType::Enum renderType);
    virtual ~FWCore();

    bool Init(uint32 width, uint32 height);
    uint32 Run(GameCore& game);
    void Shutdown();

    void SetWindowSize(uint32 width, uint32 height);
    void SetClientPositionAndSize(int32 x, int32 y, uint32 width, uint32 height, bool maximized);
    void SetWindowPositionAndSize(int32 x, int32 y, uint32 width, uint32 height, bool maximized);

    bool IsKeyDown(uint32 value);
    bool IsMouseButtonDown(uint32 id);
    void GetMouseCoordinates(int32* mx, int32* my);
    float GetMouseWheel();

    HWND GetWindowHandle() { return m_hWnd; }
    uint32 GetWindowClientWidth() { return m_WindowClientWidth; }
    uint32 GetWindowClientHeight() { return m_WindowClientHeight; }

    void SetEscapeKeyWillQuit(bool value) { m_EscapeKeyWillQuit = value; }

    uint32 GetFrameCount() { return m_FrameCount; }

protected:
    void ResizeWindow(uint32 width, uint32 height);
    bool CreateRenderWindow(char* title, uint32 width, uint32 height, uint8 colorBits, bool fullscreenflag);
    bool FailAndCleanup(const char* pMessage);
    void DestroyRenderWindow(bool destroyInstance);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    GameCore* m_pGame = nullptr;

    bool m_EscapeKeyWillQuit = true;

    uint32 m_WindowClientWidth = 0;
    uint32 m_WindowClientHeight = 0;

    HWND m_hWnd = nullptr;
    HINSTANCE m_hInstance = nullptr;

    bool m_KeyStates[256] = {};
    bool m_MouseButtonStates[3] = {};
    float m_MouseWheel = 0;
    bool m_OldKeyStates[256] = {};
    bool m_OldMouseButtonStates[3] = {};
    bool m_WindowIsActive = false;
    bool m_FullscreenMode = false;

    int32 m_FrameCount = 0;
};

} // namespace fw
