//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include <map>
#include <string>

namespace fw {

class Mesh;
class Material;
class Resource;
class ShaderProgram;
class SpriteSheet;
class Texture;

enum class ResourceType
{
    Material,
    Mesh,
    Shader,
    SpriteSheet,
    Texture,
};

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    void AddMesh(Mesh* pMesh);
    void AddShader(ShaderProgram* pShader);
    void AddTexture(Texture* pTexture);
    void AddMaterial(Material* pMaterial);
    void AddSpriteSheet(SpriteSheet* pSpriteSheet);

    Mesh* GetMesh(std::string name);
    ShaderProgram* GetShader(std::string name);
    Texture* GetTexture(std::string name);
    Material* GetMaterial(std::string name);
    SpriteSheet* GetSpriteSheet(std::string name);

    void Editor_DisplayResources();
    void Editor_DisplaySelectedResource();

protected:
    void AddResource(ResourceType type, Resource* pResource);
    Resource* GetResource(ResourceType type, std::string name);
    
    std::map<ResourceType, std::map<std::string, Resource*>> m_Resources;

    Resource* m_pSelectedResource = nullptr;
};

} // namespace fw
