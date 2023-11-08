/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/2/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <pd_api.h>
#include "Signal.h"
#include "pdcpp/core/util.h"


namespace pdcpp
{
    /**
     * Base class of all Channel sound effects. Handles common controls for all
     * sound effects, and requires all sub-classes be able to implicitly convert
     * themselves to a ::SoundEffect* for use with the C API.
     */
    class SoundEffect
    {
    public:
        /**
         * Sets a modulator which will alter the wet/dry mix of this SoundEffect
         *
         * @param mod the pdcpp::Signal to use as a modulator
         */
        void setMixModulator(const pdcpp::Signal& mod);

        /**
         * Sets the wet/dry mix of this effect.
         *
         * @param mix a floating point value 0-1 where 0 is full dry, and 1 is
         *     full wet.
         */
        void setWetDryMix(float mix);

        /**
         * Pure virtual conversion.
         *
         * @returns a pointer to the effect suitable for use with the C API
         */
        [[ nodiscard ]] virtual operator ::SoundEffect*() const = 0; // NOLINT(*-explicit-constructor)
    };

    class CustomSoundEffect
        : public SoundEffect
    {
    public:
        /**
         * Creates a new, custom sound effect. Inherit from this class and
         * implement the `processBlock` method to create a new effect.
         */
        CustomSoundEffect();

        // Move constructor
        CustomSoundEffect(CustomSoundEffect&& other) noexcept ;

        // Move-assignment constructor
        CustomSoundEffect& operator=(CustomSoundEffect&& other) noexcept ;

        // Destructor
        ~CustomSoundEffect();

        /**
         * Implement this function and create an effect by modifying the samples
         * in the left and/or right buffers. buffers are in Q8.24 format, ie.
         * 24 bits of depth in a 32-bit integer.
         *
         * @param left the left channel of samples
         * @param right the right channel of samples
         * @param nSamples the number of samples in each channel
         * @param active whether or not this buffer is coming in "active," ie.
         *     if the samples have been set in either buffer by something up the
         *     processing chain.
         * @return a boolean to tell the system whether or not this effect
         *     modified any of the samples
         */
        virtual bool processBlock(int32_t* left, int32_t* right, int nSamples, bool active) = 0;

        // Implementation for custom effects
        [[ nodiscard ]] operator ::SoundEffect*() const override { return p_Effect; };  // NOLINT(*-explicit-constructor)

    private:
        ::SoundEffect* p_Effect;

        PDCPP_DECLARE_NON_COPYABLE(CustomSoundEffect);
    };
}
