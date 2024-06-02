#pragma once

#include "../Libraries/nlohmann-json/single_include/nlohmann/json_fwd.hpp"
#include "../Libraries/flecs/flecs.h"

namespace fw {

class BaseComponentDefinition;
class GameObject;

class ComponentManager
{
public:
    ComponentManager();
    ~ComponentManager();

    void RegisterComponentDefinition(flecs::id_t componentId, BaseComponentDefinition* pComponentDefinition);

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