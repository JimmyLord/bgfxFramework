//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

#include "Math/Vector.h"

namespace fw {

class GameObject;

//==========================
// Base event class
//==========================
class Event
{
    friend class EventManager;

public:
    Event() {};
    virtual ~Event() = 0 {};

    virtual const char* GetType() = 0;

protected:
    void SetDelay(float delay) { m_Delay = delay; }

    float32 m_Delay = 0.0f;
};

//==========================
// Input event class
//==========================
class InputEvent : public Event
{
public:
    enum class DeviceType
    {
        Keyboard,
        Mouse,
    };

    enum class DeviceState
    {
        Pressed,
        Released,
        Moved,
    };

    InputEvent(DeviceType deviceType, DeviceState deviceState, uint32 keyCode)
    {
        m_DeviceType = deviceType;
        m_DeviceState = deviceState;
        m_KeyCode = keyCode;
    }
    InputEvent(DeviceType deviceType, DeviceState deviceState, uint32 mouseButtonID, vec2 mouseDir)
    {
        m_DeviceType = deviceType;
        m_DeviceState = deviceState;
        m_KeyCode = mouseButtonID;
        m_MouseDir = mouseDir;
    }
    virtual ~InputEvent() {}

    static const char* GetStaticEventType() { return "InputEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    DeviceType GetDeviceType() { return m_DeviceType; }
    DeviceState GetDeviceState() { return m_DeviceState; }
    unsigned int GetKeyCode() { return m_KeyCode; }

protected:
    DeviceType m_DeviceType;
    DeviceState m_DeviceState;
    uint32 m_KeyCode;
    vec2 m_MouseDir;
};

//==========================
// OnChar event class
//==========================
class OnCharEvent : public Event
{
public:
    OnCharEvent(unsigned int value)
    {
        m_Value = value;
    }
    virtual ~OnCharEvent() {}

    static const char* GetStaticEventType() { return "OnCharEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    uint32 GetValue() { return m_Value; }

protected:
    uint32 m_Value;
};

//==========================
// WindowResize event class
//==========================
class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(int width, int height)
    {
        m_Width = width;
        m_Height = height;
    }
    virtual ~WindowResizeEvent() {}

    static const char* GetStaticEventType() { return "WindowResizeEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    uint16 GetWidth() { return m_Width; }
    uint16 GetHeight() { return m_Height; }

protected:
    uint16 m_Width;
    uint16 m_Height;
};

//==========================
// RemoveFromGame event class
//==========================
class RemoveFromGameEvent : public Event
{
public:
    RemoveFromGameEvent(GameObject* pObject)
    {
        m_pObject = pObject;
    }
    virtual ~RemoveFromGameEvent() {}

    static const char* GetStaticEventType() { return "RemoveFromGameEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    GameObject* GetGameObject() { return m_pObject; }

protected:
    GameObject* m_pObject;
};

} // namespace fw
