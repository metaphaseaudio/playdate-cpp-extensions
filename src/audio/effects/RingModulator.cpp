/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/21/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/effects/RingModulator.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::RingModulator::RingModulator()
    : p_Effect(pdcpp::GlobalPlaydateAPI::get()->sound->effect->ringmodulator->newRingmod())
{}

pdcpp::RingModulator::RingModulator(RingModulator&& other)
    : p_Effect(other.p_Effect)
{ other.p_Effect = nullptr; }

pdcpp::RingModulator& pdcpp::RingModulator::operator=(RingModulator&& other)
{
    p_Effect = other.p_Effect;
    other.p_Effect = nullptr;
    return *this;
}

pdcpp::RingModulator::~RingModulator()
{
    if (p_Effect != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->effect->ringmodulator->freeRingmod(p_Effect); }
}

void pdcpp::RingModulator::setFrequency(float freq) { pdcpp::GlobalPlaydateAPI::get()->sound->effect->ringmodulator->setFrequency(p_Effect, freq); }

void pdcpp::RingModulator::setFrequencyModulator(const pdcpp::Signal& mod)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->ringmodulator->setFrequencyModulator(p_Effect, mod); }

pdcpp::SignalContainer pdcpp::RingModulator::getFrequencyModulator() const
    { return pdcpp::SignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->effect->ringmodulator->getFrequencyModulator(p_Effect)); }

pdcpp::RingModulator::operator ::SoundEffect*() const { return reinterpret_cast<::SoundEffect*>(p_Effect); }
