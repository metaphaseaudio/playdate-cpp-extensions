/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/15/2023
 *  Original author: MrBZapp
 */

#pragma once
#include "pd_api.h"
#include <pdcpp/core/util.h>

namespace pdcpp
{
    /**
     * Represents anything that can be used as a modulator. Requires that all
     * sub-classes (CustomSignal, LFO, and Envelope) be able to implicitly
     * convert to a PDSynthSignalValue to allow each class to be used directly
     * with modulator methods in the underlying C API.
     */
    class Signal
    {
    public:
        /**
         * @returns the current value of the Signal
         */
        [[ nodiscard ]] float getValue() const;

        /**
         * Offsets the signal’s output by the given amount. This will come after
         * the scaling factor
         *
         * @param bias the value to add to the signal;
         */
        void setBias(float bias);

        /**
         * Scales the signal’s output by the given factor. The scale is applied
         * before the offset
         *
         * @param scale the factor to apply
         */
        void setScale(float scale);

        /**
         * Abstract. Implementations must provide an implicit conversion to a
         * raw playdate ::PDSynthSignalValue* to allow for direct use with the
         * playdate C API.
         */
        [[ nodiscard ]] virtual operator ::PDSynthSignalValue*() const = 0;  // NOLINT (*-explicit-constructor)
    };

    class SignalContainer
        : public Signal
    {
    public:
        /**
         * A class that contains, but does not own a ::PDSynthSignalValue*.
         * returned by all the API's `getModulator`-style methods.
         *
         * @param signal the signal pointer to use
         */
        explicit SignalContainer(PDSynthSignalValue* signal): p_Signal(signal) {}

        /**
         * @returns if this container actually points to a valid
         *     ::PDSynthSignalValue.
         */
        [[ nodiscard ]] bool isValid() const { return p_Signal != nullptr; }

        [[ nodiscard ]] operator ::PDSynthSignalValue*() const override { return p_Signal; };  // NOLINT (*-explicit-constructor)
    private:
        PDSynthSignalValue* p_Signal;
    };

    class CustomSignal
        : public Signal
    {
    public:
        /**
         * Create a custom Synth Signal. Inherit from this class and implement
         * the step method to make an object that can be used as a modulator for
         * channels and effects, and synths. Optionally implement the noteOn/Off
         * methods to take advantage of signals when being used on Synth objects
         */
        CustomSignal();

         // Destructor. Cleans up the custom signal.
        virtual ~CustomSignal();

        /**
         * Will be called per-frame on the audio thread to provide a new value.
         * Return the new value to use.
         *
         * @param iosamples contains the number of samples until the end of the
         *     frame. If the signal needs to provide a value in the middle of
         *     the frame (e.g. an LFO that needs to be sample-accurate) set
         *     iosamples to the sample offset required.
         * @param ifval
         * @return
         */
        virtual float step(int* iosamples, float* ifval) = 0;

        /**
         * Gets called when used in a Synthesizer object and it gets a note-on.
         *
         * @param note The number of the note that was turned on
         * @param vel The velocity of the note 0.0f to 1.0f
         * @param len Indicates how long the note will stay on, but will be -1
         *     if it's indefinite.
         */
        virtual void noteOn(MIDINote note, float vel, float len) {};

        /**
         * This, theoretically, gets called when a note of the assigned synth
         * is turned off. However, the function itself isn't documented, and
         * what is written doesn't seem to align with any of the parameter
         * names. Here's what's written:
         *     ended = 0 for note release, = 1 when note stops playing
         *
         * @param stopped Is this supposed to be `ended`?
         * @param offset What does this mean?
         */
        virtual void noteOff(int stopped, int offset) {};

        [[ nodiscard ]] operator ::PDSynthSignalValue*() const override;  // NOLINT (*-explicit-constructor)


    private:
        PDSynthSignal* p_Signal;
        PDCPP_DECLARE_NON_COPYABLE_NON_MOVABLE(CustomSignal);
    };
}