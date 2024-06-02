//
// Copyright (c) 2022-2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

namespace fw {

class BaseComponentDefinition;
class GameObject;
class Scene;

class ComponentManager
{
public:
    ComponentManager();
    ~ComponentManager();

    void RegisterComponentDefinition(flecs::id_t componentId, BaseComponentDefinition* pComponentDefinition);
    template<typename DataType, typename ComponentDefType> void RegisterComponentDefinition()
    {
        flecs::id_t componentId = m_FlecsWorld.component<DataType>();
        m_ComponentDefinitions[componentId] = new ComponentDefType;
    }

    virtual void Editor_AddComponentToGameObject(GameObject* pObject);
    
    virtual void SaveGameObjectComponentsToJSON(GameObject* pGameObject, nlohmann::json& jGameObject);
    virtual void LoadGameObjectComponentsFromJSON(GameObject* pGameObject, nlohmann::json& jGameObject);

    virtual void Editor_DisplayComponentsForGameObject(GameObject* pGameObject);

    // Getters.
    flecs::world& GetFlecsWorld() { return m_FlecsWorld; }

protected:
    flecs::world m_FlecsWorld;
    std::map<flecs::id_t, BaseComponentDefinition*> m_ComponentDefinitions;
};

} // namespace fw