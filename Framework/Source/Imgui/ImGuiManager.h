#pragma once

struct ImDrawData;

namespace fw {

class FWCore;

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
