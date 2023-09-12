/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/4/2023
 *  Original author: MrBZapp
 */

#pragma once
#include "Signal.h"


namespace pdcpp
{

    class Envelope
        : public Signal
    {
    public:
        explicit Envelope(PDSynthEnvelope* env);

        explicit Envelope(float a=0.0f, float d=0.1f, float s=0.5f, float r=0.2f);

        ~Envelope();

        void setAttack(float attack);
        void setDecay(float decay);
        void setSustain(float sustain);
        void setRelease(float release);

        void setCurvature(float amount);

        void setVelocitySensitivity(float sensitivity);
        void setRateScaling(float scaling, MIDINote start, MIDINote end);

        void enableLegato(bool enable);
        void enableRetrigger(bool enable);

        [[ nodiscard ]] operator ::PDSynthSignalValue*() const override;  // NOLINT (*-explicit-constructor)

    private:
        PDSynthEnvelope* p_Envelope;
        bool m_Owned;
    };
}
