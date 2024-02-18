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
#include "pdcpp/audio/SoundSource.h"
#include "pdcpp/core/util.h"

namespace pdcpp
{
    class Delay
        : public pdcpp::SoundEffect
    {
    public:
        class Tap
            : public pdcpp::SoundSource
        {
        public:
            /**
             * Takes ownership of a ::DelayLineTap pointer and provides the C++
             * interface for memory management. This is only ever used in this
             * library by the DelayLine class, but can be used to take ownership
             * over taps in C-based construction as well.
             *
             * Note that a Tap is considered a pdcpp::SoundSource, following
             * that API, and allowing routing to any channel.
             *
             * @param tap the tap over which to take ownership
             */
            explicit Tap(::DelayLineTap* tap);

            // Move constructor
            Tap(Tap&& other);

            // Move-assignment constructor
            Tap& operator=(Tap&& other);

            // Destructor. Removes and frees the tap.
            ~Tap();

            /**
             * If the delay line is stereo and flip is set, the tap outputs the
             * delay line’s left channel to its right output and vice versa
             *
             * @param shouldFlip enable/disable the flip
             */
            void setChannelFlip(bool shouldFlip);

            /**
             * Sets the position of the tap on the DelayLine, in samples, up to
             * the DelayLine’s length.
             *
             * @param nFrames the number of samples to delay for both mono and
             *     stereo signals
             */
            void setDelay(int nFrames);

            /**
             * Sets a modulator for the delay length of this tap
             *
             * @param mod the signal to use as a modulator
             */
            void setDelayModulator(const pdcpp::Signal& mod);

            /**
             * @returns a signal container for the tap's modulator
             */
            [[ nodiscard ]] pdcpp::SignalContainer getDelayModulator() const;

            [[ nodiscard ]] operator ::SoundSource*() const override;  // NOLINT(*-explicit-constructor)

            // Implements abstract method. Does nothing.
            void finished() override {};
        protected:
            ::DelayLineTap* p_Tap;
            PDCPP_DECLARE_NON_COPYABLE(Tap);
        };

        /**
         * Creates a Delay Line. This is an SoundEffect which can also route
         * data to other channels via various taps.
         *
         * @param length the maximum length of the delay line in samples
         * @param stereo whether or not this line is stereo or mono.
         */
        Delay(int length, bool stereo);

        // Move constructor
        Delay(Delay&& other);

        // Move-assignment constructor
        Delay& operator=(Delay&& other);

        // Destructor
        ~Delay();

        /**
         * Changes the length of the delay line, clearing its contents.
         *
         * @param length the new length of the line in samples
         */
        void setLength(int length);

        /**
         * Sets the feedback level of the line
         *
         * @param fb how much gain should be applied to the signal
         */
        void setFeedback(float fb);

        /**
         * Adds and returns a DelayLineTap object for this Delay Line.
         *
         * @param delay the initial delay length of the tap to be returned.
         *
         * @returns a new DelayLineTap object.
         */
        pdcpp::Delay::Tap addTap(int delay);

        [[ nodiscard ]] operator ::SoundEffect*() const override;  // NOLINT(*-explicit-constructor)

    private:
        ::DelayLine* p_Effect;
        PDCPP_DECLARE_NON_COPYABLE(Delay);
    };

}