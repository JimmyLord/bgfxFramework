#pragma once

#include "../Libraries/nlohmann-json/single_include/nlohmann/json_fwd.hpp"
#include "../Libraries/entt/src/entt/entt.hpp"

namespace fw {

class GameObject;

class ComponentManager
{
public:
    ComponentManager();
    ~ComponentManager();

    virtual void Editor_AddComponentToGameObject(GameObject* pObject);
    
    virtual void SaveGameObjectComponentsToJSON(GameObject* pGameObject, nlohmann::json& jGameObject);
    virtual void LoadGameObjectComponentsFromJSON(GameObject* pGameObject, nlohmann::json& jGameObject);

    virtual void Editor_DisplayComponentsForGameObject(GameObject* pGameObject);

    // Getters.
    entt::registry& GetECSRegistry() { return m_ECSRegistry; }

protected:
    entt::registry m_ECSRegistry;
};

} // namespace fw