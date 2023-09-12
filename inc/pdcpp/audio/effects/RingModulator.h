/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/21/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <pd_api.h>
#include "pdcpp/core/util.h"
#include "../SoundEffect.h"

namespace pdcpp
{
    class RingModulator
        : public pdcpp::SoundEffect
    {
    public:
        /**
         * Creates a new RingModulator sound effect
         */
        RingModulator();

        // Move constructor
        RingModulator(RingModulator&& other);

        // Move-assignment constructor
        RingModulator& operator=(RingModulator&& other);

        // Destructor
        ~RingModulator();

        /**
         * Sets the frequency of the ring modulation signal
         *
         * @param freq the new frequency to use in Hz
         */
        void setFrequency(float freq);

        /**
         * Sets a modulator for the frequency
         *
         * @param mod the signal to use as a modulator
         */
        void setFrequencyModulator(const pdcpp::Signal& mod);

        /**
         * @returns a SignalContainer for the frequency modulator
         */
        [[ nodiscard ]] pdcpp::SignalContainer getFrequencyModulator() const;

        [[ nodiscard ]] operator ::SoundEffect*() const override;  // NOLINT(*-explicit-constructor)

    private:
        ::RingModulator* p_Effect;
        PDCPP_DECLARE_NON_COPYABLE(RingModulator);
    };
}
