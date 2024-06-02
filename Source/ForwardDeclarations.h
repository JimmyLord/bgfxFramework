//
// Copyright (c) 2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "DataTypes.h"

namespace enki
{
    class TaskScheduler;
} // namespace enki

namespace fw
{
    // General.
    class BaseComponentDefinition;
    class Camera;
    class ComponentManager;
    class EditorCamera;
    class EditorCore;
    class FWCore;
    class GameCore;
    class GameObject;
    class ImGuiManager;
    class Material;
    class Mesh;
    class Resource;
    class ResourceManager;
    class Scene;
    class ShaderProgram;
    class SpriteSheet;
    class Texture;
    class Uniforms;

    // Math.
    class mat4;
    class vec2;
    class vec3;
    class vec4;
    class ivec2;
    class ivec3;
    class ivec4;
    class MyRect;
    class color4f;

    // Random.
    namespace Random
    {
        class Generator;
    }

    // Events.
    class EventManager;
    class Event;
    class InputEvent;
    class OnCharEvent;
    class WindowResizeEvent;
    class RemoveFromGameEvent;

    // Components.
    struct NameData;
    struct TransformData;
    struct MeshData;

    class NameComponentDefinition;
    class TransformComponentDefinition;
    class MeshComponentDefinition;

    // Enums.
    enum EditorViews;
    enum class ResourceType;
} // namespace fw
