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

    class Overdrive
        : public pdcpp::SoundEffect
    {
    public:
        /**
         * Creates a new Overdrive sound effect
         */
        Overdrive();

        // Move constructor
        Overdrive(Overdrive&& other);

        // Move-assignment constructor
        Overdrive& operator=(Overdrive&& other);

        // Destructor
        ~Overdrive();

        /**
         * Sets the gain applied to the incoming signal before clipping
         *
         * @param gain the gain coefficient to apply
         */
        void setGain(float gain);

        /**
         * Sets the level at which the amplified input clips
         *
         * @param limit the new limit level
         */
        void setLimit(float limit);

        /**
         * Adds an offset to the upper and lower limits to create an asymmetric
         * clipping.
         *
         * @param bias the offset to add
         */
        void setBias(float bias);

        /**
         * Sets a modulator for the limit
         *
         * @param mod the signal to use as a modulator
         */
        void setLimitModulator(const pdcpp::Signal& mod);

        /**
         * Sets a modulator for the bias
         *
         * @param mod the signal to use as a modulator
         */
        void setBiasModulator(const pdcpp::Signal& mod);

        /**
         * @returns a SignalContainer for the limit modulator
         */
        [[ nodiscard ]] pdcpp::SignalContainer getLimitModulator() const;

        /**
         * @returns a SignalContainer for the Bias modulator
         */
        [[ nodiscard ]] pdcpp::SignalContainer getBiasModulator() const;

        [[ nodiscard ]] operator ::SoundEffect*() const override;  // NOLINT(*-explicit-constructor)
    private:
        ::Overdrive* p_Effect;
        PDCPP_DECLARE_NON_COPYABLE(Overdrive);
    };

} // pdcpp
