#pragma once

#include "Math/Vector.h"
#include "../Libraries/nlohmann-json/single_include/nlohmann/json.hpp"

namespace fw {

class GameObject;
class Mesh;
class Material;
class ResourceManager;

class BaseComponentDefinition
{
public:
    virtual const char* GetName() = 0;
    virtual void SaveToJSON(nlohmann::json& jComponent, const void* pData) = 0;
    virtual void LoadFromJSON(flecs::entity id, nlohmann::json& jComponent, ResourceManager* pResourceManager) = 0;
};

struct NameData
{
    static const int8 c_MaxNameLength = 32;
    char m_Name[c_MaxNameLength];

    NameData()
    {
        m_Name[0] = '\0';
    }
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

class NameComponentDefinition : public BaseComponentDefinition
{
public:
    const char* GetName() override { return "NameData"; }
    void SaveToJSON(nlohmann::json& jComponent, const void* pData) override;
    void LoadFromJSON(flecs::entity id, nlohmann::json& jComponent, ResourceManager* pResourceManager) override;
};

struct TransformData
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
};

class TransformComponentDefinition : public BaseComponentDefinition
{
    public:
    const char* GetName() override { return "TransformData"; }
    void SaveToJSON(nlohmann::json& jComponent, const void* pData) override;
    void LoadFromJSON(flecs::entity id, nlohmann::json& jComponent, ResourceManager* pResourceManager) override;
};

struct MeshData
{
    Mesh* pMesh;
    Material* pMaterial;
};

class MeshComponentDefinition : public BaseComponentDefinition
{
    public:
    const char* GetName() override { return "MeshData"; }
    void SaveToJSON(nlohmann::json& jComponent, const void* pData) override;
    void LoadFromJSON(flecs::entity id, nlohmann::json& jComponent, ResourceManager* pResourceManager) override;
};

} // namespace fw