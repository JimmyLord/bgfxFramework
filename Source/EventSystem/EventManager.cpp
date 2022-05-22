//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"

#include "EventManager.h"
#include "Events.h"
#include "GameCore.h"

namespace fw {

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
    while( m_EventQueue.empty() == false )
    {
        delete m_EventQueue.front();
        m_EventQueue.pop();
    }
}

void EventManager::AddEvent(Event* pEvent, float32 delayBeforeSending)
{
    pEvent->SetDelay( delayBeforeSending );
    m_EventQueue.push( pEvent );
}

void EventManager::DispatchAllEvents(float32 deltaTime, GameCore* pGameCore)
{
    if( m_EventQueue.empty() )
        return;

    // Keep track of the last event.
    Event* pLastEvent = m_EventQueue.back();

    while( m_EventQueue.empty() == false )
    {
        // Remove it from the queue.
        Event* pEvent = m_EventQueue.front();
        m_EventQueue.pop();

        pEvent->m_Delay -= deltaTime;
        if( pEvent->m_Delay <= 0 )
        {
            // Send it to the game.
            pGameCore->OnEvent( pEvent );

            // Delete the event.
            delete pEvent;
        }
        else
        {
            // Put the event back into the queue.
            m_EventQueue.push( pEvent );
        }

        // If we processed the last event, break out of the loop.
        if( pEvent == pLastEvent )
            break;
    }
}

} // namespace fw