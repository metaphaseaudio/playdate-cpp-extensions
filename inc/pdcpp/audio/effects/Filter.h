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
#include "../SoundEffect.h"
#include "pdcpp/core/util.h"


namespace pdcpp
{
    class OnePoleFilter
        : public pdcpp::SoundEffect
    {
    public:
        /**
         * Creates a one-pole filter, high or low pass.
         *
         * @param cutoff Optional frequency at which the filter should cut off,
         *     -1 to 1 where values above 0 are high-pass, values below 0 are
         *     low-pass. default to 0 -- basically bypassed.
         */
        explicit OnePoleFilter(float cutoff=0.0f);

        // Destructor
        ~OnePoleFilter();

        /**
         * changes the cutoff frequency of the filter.
         *
         * @param cutoff the new cutoff, -1 to 1, where values above 0 are
         *     high-pass, values below 0 are low-pass.
         */
        void setCutoff(float cutoff);

        /**
         * Modulate the cutoff with another signal
         *
         * @param mod the signal to use as a modulator
         */
        void setCutoffModulator(const pdcpp::Signal& mod);

        /**
         * @returns a SignalContainer of the current modulator
         */
        [[ nodiscard ]] pdcpp::SignalContainer getCutoffModulator() const;

        [[ nodiscard ]] operator ::SoundEffect*() const override;  // NOLINT(*-explicit-constructor)

    private:
        ::OnePoleFilter* p_Filter;
        PDCPP_DECLARE_NON_COPYABLE(OnePoleFilter);
    };

    class TwoPoleFilter
        : public pdcpp::SoundEffect
    {
    public:
        /**
         * Creates a new filter with all default settings (it is not currently
         * documented what those defaults are.)
         */
        TwoPoleFilter();

        /**
         * Creates a new filter with specific settings
         *
         * @param filterType the TwoPoleFilterType constant to set the filter'a
         *     type
         * @param freq the frequency/cutoff for the filter in Hz.
         * @param resonance sets the filter's resonance 0 to 1. Doesn't affect
         *     shelf filters.
         * @param gain sets the filter's gain if the the filter type is Shelving
         *     or Parametric (PEQ). Default is 0, or no gain.
         */
        TwoPoleFilter(TwoPoleFilterType filterType, float freq, float resonance, float gain=0.0f);

        // Move constructor
        TwoPoleFilter(TwoPoleFilter&& other);

        // Move-assignment constructor
        TwoPoleFilter& operator=(TwoPoleFilter&& other);

        // Destructor
        ~TwoPoleFilter();

        /**
         * Sets a new type for this filter
         *
         * @param type the new TwoPoleFilterType to use.
         */
        void setType(TwoPoleFilterType type);

        /**
         * Changes the frequency of the filter.
         *
         * @param frequency the value of the new frequency in Hz
         */
        void setFrequency(float frequency);

        /**
         * Sets a signal to modulate the frequency of the filter
         *
         * @param signal the signal to use as a modulator
         */
        void setFrequencyModulator(const pdcpp::Signal& signal);


        /**
         * Changes the resonance of the filter, if the filter is not a shelving
         * filter.
         *
         * @param resonance float 0-1 for the new resonance
         */
        void setResonance(float resonance);

        /**
         * Sets a signal to modulate the resonance of the filter
         *
         * @param signal the signal to use as a modulator
         */
        void setResonanceModulator(const pdcpp::Signal& signal);

        /**
         * Changes the gain of the filter if the filter is a Shelving or
         * Parametric (PEQ) type.
         *
         * @param gain the new gain to set
         */
        void setGain(float gain);

        /**
         * @returns a signal container for the frequency modulator
         */
        [[ nodiscard ]] pdcpp::SignalContainer getFrequencyModulator() const;

        /**
         * @returns a signal container for the resonance modulator
         */
        [[ nodiscard ]] pdcpp::SignalContainer getResonanceModulator() const;

        [[ nodiscard ]] operator ::SoundEffect*() const override;  // NOLINT(*-explicit-constructor)

    private:
        ::TwoPoleFilter* p_Filter;
        PDCPP_DECLARE_NON_COPYABLE(TwoPoleFilter);
    };

}