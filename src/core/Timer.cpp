/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/24/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/core/Timer.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::Timer::Timer(unsigned int intervalMs, bool startEnabled)
    : m_Interval(intervalMs)
    , m_Enabled(startEnabled)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    m_LastMilliseconds = pd->system->getCurrentTimeMilliseconds();
}

void pdcpp::Timer::setInterval(unsigned int intervalMs, bool reset)
{
    m_Interval = intervalMs;
    if (reset)
        { m_LastMilliseconds = pdcpp::GlobalPlaydateAPI::get()->system->getCurrentTimeMilliseconds(); }
}

void pdcpp::Timer::enable(bool shouldEnable) { m_Enabled = shouldEnable; }

void pdcpp::Timer::tick()
{
    if (!m_Enabled) { return; }
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    auto now = pd->system->getCurrentTimeMilliseconds();
    if (now - m_LastMilliseconds >= m_Interval)
    {
        timerCallback();
        m_LastMilliseconds = now;
    }
}

////////////////////////////////////////////////////////////////////////////////

pdcpp::FrameTimer::FrameTimer(unsigned int nFrames, bool startEnabled)
    : m_NFrames(nFrames)
    , m_FramesRemaining(nFrames)
    , m_Enabled(startEnabled)
{}

void pdcpp::FrameTimer::setNFrames(unsigned int nFrames, bool reset)
{
    m_NFrames = nFrames;
    if (reset)
        { m_FramesRemaining = m_NFrames; }
}

void pdcpp::FrameTimer::enable(bool shouldEnable) { m_FramesRemaining = shouldEnable; }

void pdcpp::FrameTimer::tick()
{
    if (!m_Enabled) { return; }
    if (--m_FramesRemaining == 0)
    {
        timerCallback();
        m_FramesRemaining = m_NFrames;
    }
}
