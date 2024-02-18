/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/17/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/core/GlobalPlaydateAPI.h>
#include <pdcpp/core/CrankManager.h>

void pdcpp::CrankManager::checkStateAndNotify()
{
    auto delta = pdcpp::GlobalPlaydateAPI::get()->system->getCrankChange();
    if (delta != 0)
    {
        auto absolute = pdcpp::GlobalPlaydateAPI::get()->system->getCrankAngle();
        for (auto* l: m_Listeners)
            { l->crankStateChanged(absolute, delta); }
    }
}

void pdcpp::CrankManager::addListener(pdcpp::CrankManager::Listener* toAdd)
{
    if (toAdd == nullptr){ return; }
    m_Listeners.emplace_back(toAdd);
}

void pdcpp::CrankManager::removeListener(pdcpp::CrankManager::Listener* toRemove)
{
    if (toRemove == nullptr) { return; }
    m_Listeners.erase(std::find(m_Listeners.begin(), m_Listeners.end(), toRemove));
}

pdcpp::GraduatedCrankHandler::GraduatedCrankHandler(int granularity)
    : m_ClickDegrees(360.0f / granularity)
{}

int pdcpp::GraduatedCrankHandler::operator()(float crankChangeDelta)
{
    auto clickCount = 0;

    m_DegSinceClick += crankChangeDelta;
    if (m_DegSinceClick > m_ClickDegrees)
    {
        while (m_DegSinceClick > m_ClickDegrees)
        {
            clickCount++;
            m_DegSinceClick -= m_ClickDegrees;
        }
        m_DegSinceClick = 0;
    }
    else if (m_DegSinceClick < -m_ClickDegrees)
    {
        while (m_DegSinceClick < -m_ClickDegrees)
        {
            clickCount--;
            m_DegSinceClick += m_ClickDegrees;
        }
        m_DegSinceClick = 0;
    }

    return clickCount;
}

