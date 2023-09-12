/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/4/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/Envelope.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>


pdcpp::Envelope::Envelope(PDSynthEnvelope* env)
    : p_Envelope(env)
    , m_Owned(false)
{}

pdcpp::Envelope::Envelope(float a, float d, float s, float r)
    : p_Envelope(pdcpp::GlobalPlaydateAPI::get()->sound->envelope->newEnvelope(a, d, s, r))
    , m_Owned(true)
{}

pdcpp::Envelope::~Envelope()
{
    if (m_Owned)
        { pdcpp::GlobalPlaydateAPI::get()->sound->envelope->freeEnvelope(p_Envelope); }
}

void pdcpp::Envelope::setAttack(float attack) { pdcpp::GlobalPlaydateAPI::get()->sound->envelope->setAttack(p_Envelope, attack); }
void pdcpp::Envelope::setDecay(float decay) { pdcpp::GlobalPlaydateAPI::get()->sound->envelope->setDecay(p_Envelope, decay); }
void pdcpp::Envelope::setSustain(float sustain) { pdcpp::GlobalPlaydateAPI::get()->sound->envelope->setSustain(p_Envelope, sustain); }
void pdcpp::Envelope::setRelease(float release) { pdcpp::GlobalPlaydateAPI::get()->sound->envelope->setRelease(p_Envelope, release); }
void pdcpp::Envelope::setCurvature(float amount) { pdcpp::GlobalPlaydateAPI::get()->sound->envelope->setCurvature(p_Envelope, amount); }
void pdcpp::Envelope::setVelocitySensitivity(float sensitivity) { pdcpp::GlobalPlaydateAPI::get()->sound->envelope->setVelocitySensitivity (p_Envelope, sensitivity); }
void pdcpp::Envelope::setRateScaling(float scaling, MIDINote start, MIDINote end) { pdcpp::GlobalPlaydateAPI::get()->sound->envelope->setRateScaling(p_Envelope, scaling, start, end); }
void pdcpp::Envelope::enableLegato(bool enable) { pdcpp::GlobalPlaydateAPI::get()->sound->envelope->setLegato(p_Envelope, enable); }
void pdcpp::Envelope::enableRetrigger(bool enable) { pdcpp::GlobalPlaydateAPI::get()->sound->envelope->setRetrigger(p_Envelope, enable); }

pdcpp::Envelope::operator ::PDSynthSignalValue*() const { return reinterpret_cast<::PDSynthSignalValue*>(p_Envelope); }
