#pragma once

#include "Math/Vector.h"
#include "../Libraries/nlohmann-json/single_include/nlohmann/json.hpp"

namespace fw {

class GameObject;
class Mesh;
class Material;
class ResourceManager;

//==============================
// BaseComponentDefinition
//==============================

class BaseComponentDefinition
{
public:
    virtual const char* GetName() = 0;
    virtual void SaveToJSON(nlohmann::json& jComponent, const void* pData) = 0;
    virtual void LoadFromJSON(flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager) = 0;
    virtual void Editor_AddToInspector(flecs::entity entity) = 0;
};

//====================
// NameComponent
//====================

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
};

class NameComponentDefinition : public BaseComponentDefinition
{
public:
    virtual const char* GetName() override { return "NameData"; }
    virtual void SaveToJSON(nlohmann::json& jComponent, const void* pData) override;
    virtual void LoadFromJSON(flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager) override;
    virtual void Editor_AddToInspector(flecs::entity entity) override;
};

//====================
// TransformComponent
//====================

struct TransformData
{
    vec3 position = 0;
    vec3 rotation = 0;
    vec3 scale = 0;
};

class TransformComponentDefinition : public BaseComponentDefinition
{
public:
    virtual const char* GetName() override { return "TransformData"; }
    virtual void SaveToJSON(nlohmann::json& jComponent, const void* pData) override;
    virtual void LoadFromJSON(flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager) override;
    virtual void Editor_AddToInspector(flecs::entity entity) override;
};

//====================
// MeshComponent
//====================

struct MeshData
{
    Mesh* pMesh = nullptr;
    Material* pMaterial = nullptr;
};

class MeshComponentDefinition : public BaseComponentDefinition
{
public:
    virtual const char* GetName() override { return "MeshData"; }
    virtual void SaveToJSON(nlohmann::json& jComponent, const void* pData) override;
    virtual void LoadFromJSON(flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager) override;
    virtual void Editor_AddToInspector(flecs::entity entity) override;
};

} // namespace fw