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

pdcpp::Timer::Timer(unsigned int intervalMs)
    : m_Interval(intervalMs)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    m_LastMilliseconds = pd->system->getCurrentTimeMilliseconds();
}

void pdcpp::Timer::tick()
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    auto now = pd->system->getCurrentTimeMilliseconds();
    if (now - m_LastMilliseconds >= m_Interval)
    {
        timerCallback();
        m_LastMilliseconds = now;
    }
}

void pdcpp::Timer::setInterval(unsigned int intervalMs, bool reset)
{
    m_Interval = intervalMs;
    if (reset)
        { m_LastMilliseconds = pdcpp::GlobalPlaydateAPI::get()->system->getCurrentTimeMilliseconds(); }
}

pdcpp::FrameTimer::FrameTimer(unsigned int nFrames)
    : m_NFrames(nFrames)
    , m_FramesRemaining(nFrames)
{}

void pdcpp::FrameTimer::setNFrames(unsigned int nFrames, bool reset)
{
    m_NFrames = nFrames;
    if (reset)
        { m_FramesRemaining = m_NFrames; }
}

void pdcpp::FrameTimer::tick()
{
    if (--m_FramesRemaining == 0)
    {
        timerCallback();
        m_FramesRemaining = m_NFrames;
    }
}
