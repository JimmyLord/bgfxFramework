#pragma once

#include "Math/Vector.h"
#include "../Libraries/nlohmann-json/single_include/nlohmann/json_fwd.hpp"

namespace fw {

class GameObject;
class Mesh;
class Material;

class ComponentManager
{
public:
    ComponentManager();
    ~ComponentManager();

    virtual void SaveGameObjectComponentsToJSON(GameObject* pGameObject, nlohmann::json& jGameObject);
    virtual void LoadGameObjectComponentsFromJSON(GameObject* pGameObject, nlohmann::json& jGameObject);

    virtual void Editor_DisplayComponentsForGameObject(GameObject* pGameObject);
};

struct NameData
{
    static const int8 c_MaxNameLength = 32;
    char m_Name[c_MaxNameLength];

    NameData(const char* name)
    {
        strncpy_s( m_Name, c_MaxNameLength, name, c_MaxNameLength );
    }
    //NameData(const NameData& other)
    //{
    //    strncpy_s( m_Name, c_MaxNameLength, other.m_Name, c_MaxNameLength );
    //}
    //NameData(NameData&& other)
    //{
    //    strncpy_s( m_Name, c_MaxNameLength, other.m_Name, c_MaxNameLength );
    //}
    //NameData& operator=(const NameData& other)
    //{
    //    strncpy_s( m_Name, c_MaxNameLength, other.m_Name, c_MaxNameLength );
    //    return *this;
    //}
    //NameData& operator=(const NameData&& other)
    //{
    //    strncpy_s( m_Name, c_MaxNameLength, other.m_Name, c_MaxNameLength );
    //    return *this;
    //}
};

struct TransformData
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
};

struct MeshData
{
    Mesh* pMesh;
    Material* pMaterial;
};

} // namespace fw