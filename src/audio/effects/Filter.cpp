/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/21/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/effects/Filter.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>


pdcpp::OnePoleFilter::operator ::SoundEffect*() const { return reinterpret_cast<::SoundEffect*>(p_Filter); }

pdcpp::OnePoleFilter::OnePoleFilter(float cutoff)
    : p_Filter(pdcpp::GlobalPlaydateAPI::get()->sound->effect->onepolefilter->newFilter())
{
    setCutoff(cutoff);
}

pdcpp::OnePoleFilter::~OnePoleFilter()
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->onepolefilter->freeFilter(p_Filter); }

void pdcpp::OnePoleFilter::setCutoff(float cutoff)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->onepolefilter->setParameter(p_Filter, cutoff); }

void pdcpp::OnePoleFilter::setCutoffModulator(const pdcpp::Signal& mod)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->onepolefilter->setParameterModulator(p_Filter, mod); }

pdcpp::SignalContainer pdcpp::OnePoleFilter::getCutoffModulator() const
    { return pdcpp::SignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->effect->onepolefilter->getParameterModulator(p_Filter)); }


pdcpp::TwoPoleFilter::operator ::SoundEffect*() const { return reinterpret_cast<::SoundEffect*>(p_Filter); }

pdcpp::TwoPoleFilter::TwoPoleFilter()
    : p_Filter(pdcpp::GlobalPlaydateAPI::get()->sound->effect->twopolefilter->newFilter())
{}

pdcpp::TwoPoleFilter::TwoPoleFilter(TwoPoleFilterType filterType, float cutoff, float resonance, float gain)
    : p_Filter(pdcpp::GlobalPlaydateAPI::get()->sound->effect->twopolefilter->newFilter())
{
    setType(filterType);
    setFrequency(cutoff);
    setResonance(resonance);
    setGain(gain);
}

pdcpp::TwoPoleFilter::TwoPoleFilter(TwoPoleFilter&& other)
    : p_Filter(other.p_Filter)
{ other.p_Filter = nullptr; }

pdcpp::TwoPoleFilter& pdcpp::TwoPoleFilter::operator=(TwoPoleFilter&& other)
{
    p_Filter = other.p_Filter;
    other.p_Filter = nullptr;
    return *this;
}

pdcpp::TwoPoleFilter::~TwoPoleFilter()
{
    if (p_Filter != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->effect->twopolefilter->freeFilter(p_Filter); }
}

void pdcpp::TwoPoleFilter::setType(TwoPoleFilterType type) { pdcpp::GlobalPlaydateAPI::get()->sound->effect->twopolefilter->setType(p_Filter, type); }
void pdcpp::TwoPoleFilter::setFrequency(float frequency) { pdcpp::GlobalPlaydateAPI::get()->sound->effect->twopolefilter->setFrequency(p_Filter, frequency); }

void pdcpp::TwoPoleFilter::setFrequencyModulator(const pdcpp::Signal& signal)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->twopolefilter->setFrequencyModulator(p_Filter, signal); }

pdcpp::SignalContainer pdcpp::TwoPoleFilter::getFrequencyModulator() const
    { return pdcpp::SignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->effect->twopolefilter->getFrequencyModulator(p_Filter)); }

void pdcpp::TwoPoleFilter::setGain(float gain)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->twopolefilter->setGain(p_Filter, gain); }

void pdcpp::TwoPoleFilter::setResonance(float resonance)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->twopolefilter->setResonance(p_Filter, resonance); }

void pdcpp::TwoPoleFilter::setResonanceModulator(const pdcpp::Signal& mod)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->twopolefilter->setResonanceModulator(p_Filter, mod); }

pdcpp::SignalContainer pdcpp::TwoPoleFilter::getResonanceModulator() const
    { return pdcpp::SignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->effect->twopolefilter->getResonanceModulator(p_Filter)); }
