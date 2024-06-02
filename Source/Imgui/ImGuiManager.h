//
// Copyright (c) 2022-2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Resources/Texture.h"
#include "../Libraries/imgui/imgui.h"

struct ImDrawData;

namespace fw {

class FWCore;
class Texture;

inline ImTextureID imguiTexture(bgfx::TextureHandle handle, uint8_t flags = 0, uint8_t mip = 0)
{
    union
    {
        struct
        {
            bgfx::TextureHandle handle;
            uint8_t flags;
            uint8_t mip;
        } s;
        ImTextureID id;
    } tex;

    tex.s.handle = handle;
    tex.s.flags  = flags;
    tex.s.mip    = mip;
    return tex.id;
}

inline ImTextureID imguiTexture(fw::Texture* pTexture, uint8_t flags = 0, uint8_t mip = 0)
{
    return imguiTexture( pTexture->GetHandle(), flags, mip );
}

class ImGuiManager
{
public:
    ImGuiManager(FWCore* pFramework, int viewID);
    virtual ~ImGuiManager();

    void Init(int viewID);
    void Shutdown();

    bool WantsMouse();
    bool WantsKeyboard();
    bool WantsTextInput();

    void AddInputCharacter(unsigned int value);

    void OnFocusLost();

    void StartFrame(float deltaTime);
    void EndFrame();

protected:
    void ClearInput();

    void RenderDrawLists(ImDrawData* draw_data);

    bool CreateDeviceObjects();
    void InvalidateDeviceObjects();

protected:
    FWCore* m_pFramework = nullptr;
};

} // namespace fw
