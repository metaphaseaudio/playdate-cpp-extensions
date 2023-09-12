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
#include "pdcpp/audio/SoundEffect.h"
#include "pdcpp/core/util.h"


namespace pdcpp
{
    class BitCrushAndUndersample
        : public pdcpp::SoundEffect
    {
    public:
        /**
         * Creates a new effect for bit-crushing and under-sampling.
         */
        BitCrushAndUndersample();

        // Move constructor
        BitCrushAndUndersample(BitCrushAndUndersample&& other);

        // Move-assignment constructor
        BitCrushAndUndersample& operator=(BitCrushAndUndersample&& other);

        // Destructor
        ~BitCrushAndUndersample();

        /**
         * Sets the amount of bit-crushing
         *
         * @param amount from 0 (no effect) to 1 (quantize output to 1 bit)
         */
        void setBitCrush(float amount);

        /**
         * Sets the amount of undersampling
         *
         * @param amount sets the number of samples to repeat where 0 has no
         *     effect, 1 repeats every other sample, etc.
         */
        void setUndersampling(float amount);

        /**
         * Sets a modulator for the bit-crushing amount
         *
         * @param mod the signal to use as a modulator
         */
        void setBitCrushModulator(const pdcpp::Signal& mod);

        /**
         * @returns a SignalContainer for the bit-crushing modulator
         */
        [[ nodiscard ]] pdcpp::SignalContainer getBitCrushModulator() const;

        /**
         * Sets a modulator for the under-sampling amount
         *
         * @param mod the signal to use as a modulator
         */
        void setUndersampleModulator(const pdcpp::Signal& mod);

        /**
         * @returns a SignalContainer for the under-sampling modulator
         */
        [[ nodiscard ]] pdcpp::SignalContainer getUndersampleModulator() const;

        [[ nodiscard ]] operator ::SoundEffect*() const override;  // NOLINT(*-explicit-constructor)

    private:
        ::BitCrusher* p_Effect;
        PDCPP_DECLARE_NON_COPYABLE(BitCrushAndUndersample);
    };
}
