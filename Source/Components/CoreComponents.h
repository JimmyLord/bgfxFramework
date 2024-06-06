//
// Copyright (c) 2022-2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math/Vector.h"
#include "Math/Matrix.h"

namespace fw {

class GameObject;
class Mesh;
class Material;
class ResourceManager;
class Scene;

//==============================
// BaseComponentDefinition
//==============================

class BaseComponentDefinition
{
public:
    virtual const char* GetName() = 0;
    virtual void SaveToJSON(GameObject* pObject, nlohmann::json& jComponent, const void* pData) = 0;
    virtual void LoadFromJSON(GameObject* pObject, flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager) = 0;
    virtual void Editor_AddToInspector(flecs::entity entity) = 0;
};

//====================
// NameComponent
//====================

struct NameData
{
    static const int8 c_MaxNameLength = 32;
    char name[c_MaxNameLength];

    NameData() { name[0] = '\0'; }
    NameData(const char* newName) { strncpy_s( name, c_MaxNameLength, newName, c_MaxNameLength ); }
};

class NameComponentDefinition : public BaseComponentDefinition
{
public:
    virtual const char* GetName() override { return "NameData"; }
    virtual void SaveToJSON(GameObject* pObject, nlohmann::json& jComponent, const void* pData) override;
    virtual void LoadFromJSON(GameObject* pObject, flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager) override;
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
    virtual void SaveToJSON(GameObject* pObject, nlohmann::json& jComponent, const void* pData) override;
    virtual void LoadFromJSON(GameObject* pObject, flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager) override;
    virtual void Editor_AddToInspector(flecs::entity entity) override;
};

//====================
// TransformMatrixData
//====================

struct TransformMatrixData
{
    mat4 transform;
};

class TransformMatrixComponentDefinition : public BaseComponentDefinition
{
public:
    virtual const char* GetName() override { return "TransformData"; }
    virtual void SaveToJSON(GameObject* pObject, nlohmann::json& jComponent, const void* pData) override {}
    virtual void LoadFromJSON(GameObject* pObject, flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager) override {}
    virtual void Editor_AddToInspector(flecs::entity entity) override {}
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
    virtual void SaveToJSON(GameObject* pObject, nlohmann::json& jComponent, const void* pData) override;
    virtual void LoadFromJSON(GameObject* pObject, flecs::entity entity, nlohmann::json& jComponent, ResourceManager* pResourceManager) override;
    virtual void Editor_AddToInspector(flecs::entity entity) override;
};

} // namespace fw
