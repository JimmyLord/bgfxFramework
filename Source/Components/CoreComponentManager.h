#pragma once

#include "../Libraries/nlohmann-json/single_include/nlohmann/json_fwd.hpp"

namespace fw {

class GameObject;

class ComponentManager
{
public:
    ComponentManager();
    ~ComponentManager();

    virtual void SaveGameObjectComponentsToJSON(GameObject* pGameObject, nlohmann::json& jGameObject);
    virtual void LoadGameObjectComponentsFromJSON(GameObject* pGameObject, nlohmann::json& jGameObject);

    virtual void Editor_DisplayComponentsForGameObject(GameObject* pGameObject);
};

} // namespace fw