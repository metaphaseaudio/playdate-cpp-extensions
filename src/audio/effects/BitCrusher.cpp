/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/21/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/effects/BitCrusher.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::BitCrushAndUndersample::operator ::SoundEffect*() const { return reinterpret_cast<::SoundEffect*>(p_Effect); }

pdcpp::BitCrushAndUndersample::BitCrushAndUndersample()
    : p_Effect(pdcpp::GlobalPlaydateAPI::get()->sound->effect->bitcrusher->newBitCrusher())
{}

pdcpp::BitCrushAndUndersample::BitCrushAndUndersample(pdcpp::BitCrushAndUndersample&& other)
    : p_Effect(other.p_Effect)
{ other.p_Effect = nullptr; }

pdcpp::BitCrushAndUndersample& pdcpp::BitCrushAndUndersample::operator=(pdcpp::BitCrushAndUndersample&& other)
{
    p_Effect = other.p_Effect;
    other.p_Effect = nullptr;
    return *this;
}

pdcpp::BitCrushAndUndersample::~BitCrushAndUndersample()
{
    if (p_Effect != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->effect->bitcrusher->freeBitCrusher(p_Effect); }
}

void pdcpp::BitCrushAndUndersample::setBitCrush(float amount)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->bitcrusher->setAmount(p_Effect, amount); }

void pdcpp::BitCrushAndUndersample::setUndersampling(float amount)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->bitcrusher->setUndersampling(p_Effect, amount); }

void pdcpp::BitCrushAndUndersample::setBitCrushModulator(const pdcpp::Signal& mod)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->bitcrusher->setAmountModulator(p_Effect, mod); }

pdcpp::SignalContainer pdcpp::BitCrushAndUndersample::getBitCrushModulator() const
    { return pdcpp::SignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->effect->bitcrusher->getAmountModulator(p_Effect)); }

void pdcpp::BitCrushAndUndersample::setUndersampleModulator(const pdcpp::Signal& mod)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->bitcrusher->setUndersampleModulator(p_Effect, mod); }

pdcpp::SignalContainer pdcpp::BitCrushAndUndersample::getUndersampleModulator() const
    { return pdcpp::SignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->effect->bitcrusher->getUndersampleModulator(p_Effect)); }
