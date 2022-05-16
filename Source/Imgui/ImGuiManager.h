#pragma once

#include "Renderer/Texture.h"
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
    imguiTexture( pTexture->GetHandle(), flags, mip );
}

class ImGuiManager
{
public:
    ImGuiManager(FWCore* pFramework, int viewID);
    virtual ~ImGuiManager();

    void Init(int viewID);
    void Shutdown();

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
