//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "../Libraries/entt/src/entt/entt.hpp"
#include "../Libraries/nlohmann-json/single_include/nlohmann/json_fwd.hpp"

namespace fw {

class ComponentManager;
class Event;
class GameCore;
class GameObject;

class Scene
{
public:
    Scene(GameCore* pGameCore);
    virtual ~Scene();

    // Customization.
    virtual void CreateComponentManager();

    virtual void Init();
    virtual void OnShutdown();
    virtual void StartFrame(float deltaTime);
    virtual void OnEvent(Event* pEvent);
    virtual void Update(float deltaTime);
    virtual void Draw(int viewID);

    void DrawIntoView(int viewID);

    // Getters.
    GameCore* GetGameCore() { return m_pGameCore; }
    virtual Camera* GetCamera() { return nullptr; }

    // Setters.
    void SetName(std::string name) { m_Name = name; }

    // Save/Load.
    virtual void SaveToJSON(nlohmann::json& jScene);
    virtual void LoadFromJSON(nlohmann::json& jScene);

    // ECS.
    ComponentManager* GetComponentManager() { return m_pComponentManager; }
    entt::registry& GetECSRegistry();
    entt::entity CreateEntity();

    // Editor.
    void Editor_DisplayObjectList();
    std::string Editor_GetFilename() { return m_Editor_Filename; }
    void Editor_SetFilename(std::string filename) { m_Editor_Filename = filename; }

protected:
    // Members.
    GameCore* m_pGameCore = nullptr;
    std::string m_Name;

    // ECS.
    ComponentManager* m_pComponentManager = nullptr;

    // GameObjects.
    std::vector<GameObject*> m_Objects;

    // Editor.
    std::string m_Editor_Filename;
};

} // namespace fw