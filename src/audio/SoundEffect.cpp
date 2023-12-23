/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/2/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/SoundEffect.h>
#include "pdcpp/core/GlobalPlaydateAPI.h"

void pdcpp::SoundEffect::setMixModulator(const pdcpp::Signal& mod)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->setMixModulator(*this, mod); }
void pdcpp::SoundEffect::setWetDryMix(float mix)
    { pdcpp::GlobalPlaydateAPI::get()->sound->effect->setMix(*this, mix); }

int effectProcBridge(SoundEffect* e, int32_t* left, int32_t* right, int nsamples, int bufactive)
{
    auto thisPtr = static_cast<pdcpp::CustomSoundEffect*>(pdcpp::GlobalPlaydateAPI::get()->sound->effect->getUserdata(e));
    return thisPtr->processBlock(left, right, nsamples, bufactive);
}

pdcpp::CustomSoundEffect::CustomSoundEffect()
    : p_Effect(pdcpp::GlobalPlaydateAPI::get()->sound->effect->newEffect(effectProcBridge, this))
{}

pdcpp::CustomSoundEffect::~CustomSoundEffect()
{
    if (p_Effect != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->effect->freeEffect(p_Effect); }
}

pdcpp::CustomSoundEffect::CustomSoundEffect(pdcpp::CustomSoundEffect&& other) noexcept
    : p_Effect(other.p_Effect)
{
    other.p_Effect = nullptr;
    pdcpp::GlobalPlaydateAPI::get()->sound->effect->setUserdata(p_Effect, this);
}

pdcpp::CustomSoundEffect& pdcpp::CustomSoundEffect::operator=(pdcpp::CustomSoundEffect&& other) noexcept
{
    p_Effect = other.p_Effect;
    other.p_Effect = nullptr;
    pdcpp::GlobalPlaydateAPI::get()->sound->effect->setUserdata(p_Effect, this);
    return *this;
}
