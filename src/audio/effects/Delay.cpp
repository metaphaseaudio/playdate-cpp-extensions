/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/21/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/effects/Delay.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::Delay::Tap::Tap(::DelayLineTap* tap)
    : p_Tap(tap)
{
    // TODO: does this even make sense? How would a delay tap know when it's
    //  "done"?
    // enableFinishedCallback();
}

pdcpp::Delay::Tap::Tap(pdcpp::Delay::Tap&& other) noexcept
    : p_Tap(other.p_Tap)
{ other.p_Tap = nullptr; }

pdcpp::Delay::Tap& pdcpp::Delay::Tap::operator=(pdcpp::Delay::Tap&& other) noexcept
{
    p_Tap = other.p_Tap;
    other.p_Tap = nullptr;
    return *this;
}

pdcpp::Delay::Tap::~Tap()
{
    if (p_Tap != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->effect->delayline->freeTap(p_Tap); }
}

void pdcpp::Delay::Tap::setChannelFlip(bool shouldFlip)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->delayline->setTapChannelsFlipped(p_Tap, shouldFlip); }

void pdcpp::Delay::Tap::setDelay(int nFrames) { pdcpp::GlobalPlaydateAPI::get()->sound->effect->delayline->setTapDelay(p_Tap, nFrames); }

void pdcpp::Delay::Tap::setDelayModulator(const pdcpp::Signal& mod)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->delayline->setTapDelayModulator(p_Tap, mod); }

pdcpp::SignalContainer pdcpp::Delay::Tap::getDelayModulator() const
    { return pdcpp::SignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->effect->delayline->getTapDelayModulator(p_Tap)); }

pdcpp::Delay::Tap::operator ::SoundSource*() const { return reinterpret_cast<::SoundSource*>(p_Tap); }

////////////////////////////////////////////////////////////////////////////////
pdcpp::Delay::Delay(int length, bool stereo)
    : p_Effect(pdcpp::GlobalPlaydateAPI::get()->sound->effect->delayline->newDelayLine(length, stereo))
{}

pdcpp::Delay::Delay(pdcpp::Delay&& other)
    : p_Effect(other.p_Effect)
{ other.p_Effect = nullptr; }

pdcpp::Delay& pdcpp::Delay::operator=(pdcpp::Delay&& other)
{
    p_Effect = other.p_Effect;
    other.p_Effect = nullptr;
    return *this;
}

pdcpp::Delay::~Delay()
{
    if (p_Effect != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->effect->delayline->freeDelayLine(p_Effect); }
}

void pdcpp::Delay::setLength(int length) { pdcpp::GlobalPlaydateAPI::get()->sound->effect->delayline->setLength(p_Effect, length); }

void pdcpp::Delay::setFeedback(float fb)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->effect->delayline->setFeedback(p_Effect, fb);
}

pdcpp::Delay::Tap pdcpp::Delay::addTap(int delay)
    { return pdcpp::Delay::Tap(pdcpp::GlobalPlaydateAPI::get()->sound->effect->delayline->addTap(p_Effect, delay)); }

pdcpp::Delay::operator ::SoundEffect*() const { return reinterpret_cast<::SoundEffect*>(p_Effect); }
