/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/21/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/effects/Overdrive.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::Overdrive::Overdrive()
    : p_Effect(pdcpp::GlobalPlaydateAPI::get()->sound->effect->overdrive->newOverdrive())
{}

pdcpp::Overdrive::Overdrive(Overdrive&& other)
        : p_Effect(other.p_Effect)
{ other.p_Effect = nullptr; }

pdcpp::Overdrive& pdcpp::Overdrive::operator=(Overdrive&& other)
{
    p_Effect = other.p_Effect;
    other.p_Effect = nullptr;
    return *this;
}

pdcpp::Overdrive::~Overdrive()
{
    if (p_Effect != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->effect->overdrive->freeOverdrive(p_Effect); }
}

void pdcpp::Overdrive::setGain(float gain) { pdcpp::GlobalPlaydateAPI::get()->sound->effect->overdrive->setGain(p_Effect, gain); }
void pdcpp::Overdrive::setLimit(float limit) { pdcpp::GlobalPlaydateAPI::get()->sound->effect->overdrive->setLimit(p_Effect, limit); }
void pdcpp::Overdrive::setBias(float bias) { pdcpp::GlobalPlaydateAPI::get()->sound->effect->overdrive->setOffset(p_Effect, bias); }

void pdcpp::Overdrive::setLimitModulator(const pdcpp::Signal& mod)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->overdrive->setLimitModulator(p_Effect, mod); }

void pdcpp::Overdrive::setBiasModulator(const pdcpp::Signal& mod)
    {  pdcpp::GlobalPlaydateAPI::get()->sound->effect->overdrive->setOffsetModulator(p_Effect, mod); }

pdcpp::SignalContainer pdcpp::Overdrive::getLimitModulator() const
    { return pdcpp::SignalContainer( pdcpp::GlobalPlaydateAPI::get()->sound->effect->overdrive->getLimitModulator(p_Effect)); }

pdcpp::SignalContainer pdcpp::Overdrive::getBiasModulator() const
    { return pdcpp::SignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->effect->overdrive->getOffsetModulator(p_Effect)); }

pdcpp::Overdrive::operator ::SoundEffect*() const { return reinterpret_cast<::SoundEffect*>(p_Effect); }
