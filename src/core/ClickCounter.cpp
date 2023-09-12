/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/25/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/core/ClickCounter.h>

pdcpp::ClickCounter::ClickCounter(PDButtons toDetect, unsigned int checkWindow, unsigned int nClickMax)
    : m_ButtonsToDetect(toDetect)
    , m_Up(true)
    , m_CheckWindowMS(checkWindow)
    , m_MsChecked(0)
    , m_ClicksDetected(0)
    , m_NClickMax(nClickMax)
{}

void pdcpp::ClickCounter::reset()
{
    m_MsChecked = 0;
    m_Up = true;
}

unsigned int pdcpp::ClickCounter::detect(const PDButtons& pressed, const PDButtons& released, unsigned int checkTime)
{
    if (pressed & m_ButtonsToDetect && m_Up)
    {
        // Start condition
        if (m_MsChecked == 0)
            { m_MsChecked = checkTime; }

        m_Up = false;
        m_ClicksDetected++;
    }

    if (released & m_ButtonsToDetect && !m_Up)
        { m_Up = true; }

    // If we've maxed out our clicks, or our detection window has closed, return the click count.
    if ((m_NClickMax > 0 && m_ClicksDetected >= m_NClickMax) ||
        (m_MsChecked > 0 && checkTime - m_MsChecked > m_CheckWindowMS))
    {
        auto rv = m_ClicksDetected;
        m_ClicksDetected = 0;
        m_MsChecked = 0;
        return rv;
    }

    return 0;
}
