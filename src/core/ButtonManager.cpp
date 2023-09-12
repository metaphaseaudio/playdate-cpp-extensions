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
    { m_Listeners.emplace_back(toAdd); }

void pdcpp::ButtonManager::removeListener(pdcpp::ButtonManager::Listener* toRemove)
    { m_Listeners.erase(std::find(m_Listeners.begin(), m_Listeners.end(), toRemove)); }
