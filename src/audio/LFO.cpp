/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/4/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/LFO.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::LFO::LFO(PDSynthLFO* lfo)
    : p_LFO(lfo)
{}

pdcpp::LFO::LFO(pdcpp::LFO&& other) noexcept
    : p_LFO(other.p_LFO)
{ other.p_LFO = nullptr; }

pdcpp::LFO& pdcpp::LFO::operator=(pdcpp::LFO&& other) noexcept
{
    p_LFO = other.p_LFO ;
    other.p_LFO = nullptr;
    return *this;
}

pdcpp::LFO::~LFO()
{
    if (p_LFO != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->lfo->freeLFO(p_LFO); }
}

void pdcpp::LFO::setRate(float rate) { pdcpp::GlobalPlaydateAPI::get()->sound->lfo->setRate(p_LFO, rate); }
pdcpp::LFO::operator ::PDSynthSignalValue*() const { return reinterpret_cast<PDSynthSignalValue*>(p_LFO); }
void pdcpp::LFO::setPhase(float phase) { pdcpp::GlobalPlaydateAPI::get()->sound->lfo->setPhase(p_LFO, phase); }
void pdcpp::LFO::setBias(float center) { pdcpp::GlobalPlaydateAPI::get()->sound->lfo->setCenter(p_LFO, center); }
void pdcpp::LFO::setDepth(float depth) { pdcpp::GlobalPlaydateAPI::get()->sound->lfo->setDepth(p_LFO, depth); }
void pdcpp::LFO::setDelay(float holdoff, float ramptime) { pdcpp::GlobalPlaydateAPI::get()->sound->lfo->setDelay(p_LFO, holdoff, ramptime); }
void pdcpp::LFO::setRetrigger(bool enable) { pdcpp::GlobalPlaydateAPI::get()->sound->lfo->setRetrigger(p_LFO, enable); }

pdcpp::WaveformLFO::WaveformLFO(pdcpp::WaveformLFO::Waveform lfoType)
    : pdcpp::LFO(pdcpp::GlobalPlaydateAPI::get()->sound->lfo->newLFO(static_cast<LFOType>(lfoType)))
{}

void pdcpp::WaveformLFO::setType(WaveformLFO::Waveform lfoType) { pdcpp::GlobalPlaydateAPI::get()->sound->lfo->setType(p_LFO, static_cast<LFOType>(lfoType)); }
