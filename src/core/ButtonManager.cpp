/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/24/2023
 *  Original author: MrBZapp
 */

#include <algorithm>
#include <pdcpp/core/ButtonManager.h>
#include "pdcpp/core/GlobalPlaydateAPI.h"


void pdcpp::ButtonManager::checkStateAndNotify()
{
    PDButtons current, pressed, released;
    GlobalPlaydateAPI::get()->system->getButtonState(&current, &pressed, &released);

    if (current || pressed || released)
    {
        for (auto* l: m_Listeners)
            { l->buttonStateChanged(current, pressed, released); }
    }
}

void pdcpp::ButtonManager::addListener(pdcpp::ButtonManager::Listener* toAdd)
{
    if (toAdd == nullptr){ return; }
    m_Listeners.emplace_back(toAdd);
}

void pdcpp::ButtonManager::removeListener(pdcpp::ButtonManager::Listener* toRemove)
{
    if (toRemove == nullptr) { return; }
    m_Listeners.erase(std::find(m_Listeners.begin(), m_Listeners.end(), toRemove));
}

pdcpp::KeyRepeatTimer::KeyRepeatTimer(int initialDelayMs, int repeatDelayMs)
    : pdcpp::Timer(initialDelayMs, false)
    , m_OnKeyRepeat([](){})
    , m_InitialDelay(initialDelayMs)
    , m_RepeatDelay(repeatDelayMs)
{}

void pdcpp::KeyRepeatTimer::keyPressed(std::function<void()> action)
{
    action();
    m_OnKeyRepeat = std::move(action);
    enable(true);
    setInterval(m_InitialDelay, true);
}

void pdcpp::KeyRepeatTimer::keyReleased()
{
    enable(false);
}

void pdcpp::KeyRepeatTimer::timerCallback()
{
    setInterval(m_RepeatDelay);
    m_OnKeyRepeat();
}
