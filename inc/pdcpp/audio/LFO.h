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
#include <pd_api.h>
#include "Signal.h"
#include "../core/util.h"


namespace pdcpp
{
    class LFO
        : public Signal
    {
    public:
        /**
         * Creates a new LFO from a raw pointer. This object will take ownership
         * of the pointer, and free it during destruction.
         *
         * @param lfoToOwn a raw pointer to the PDSynthLFO to own.
         */
        explicit LFO(PDSynthLFO* lfoToOwn);

        // Move constructor
        LFO(LFO&& other) noexcept;

        // Move assignment constructor
        LFO& operator=(LFO&& other) noexcept;

        // Destructor. frees the owned pointer.
        virtual ~LFO();

        /**
         * Set the rate of the LFO in Hertz.
         *
         * @param rateHz the new rate to use for this LFO
         */
        void setRate(float rateHz);

        /**
         * Sets the phase (0-1) of the LFO where 0 is no shift, and 1 is the
         * maximum possible shift.
         *
         * @param phase the phase 0-1 to use
         */
        void setPhase(float phase);

        /**
         * Sets the starting phase (0-1) upon re-triggering of this LFO.
         *
         * @param phase the initial phase 0-1 to use.
         */
        void setStartPhase(float phase);

        /**
         * Sets the center point of the LFO. LFOs are typically treated as
         * oscillating between +/- depth (with some exceptions for oscillators
         * like arpeggiators,) and this adds a constant to the value, "biasing"
         * the LFO toward the value. This is applied after the scaling inherited
         * from pdcpp::Signal, so you can use this to introduce a constant value
         * by setting the scale to 0, and using a constant bias.
         *
         * @param center the new center value of the LFO
         */
        void setBias(float center);

        /**
         * Sets the depth, the max/min points, of the LFO
         *
         * @param depth The new depth to use
         */
        void setDepth(float depth);

        /**
         * Sets an initial holdoff time for the LFO where the LFO remains at its
         * center value, and a ramp time where the value increases linearly to
         * its maximum depth. Values are in seconds.
         *
         * @param holdoff the time in seconds to remain at the center/bias value
         * @param ramptime the time in seconds to increase to maximum depth.
         */
        void setDelay(float holdoff, float ramptime);

        /**
         * Enables or disables re-triggering. If enabled, the LFO’s phase is
         * reset to 0 when a synth using the LFO starts playing a note.
         *
         * @param enable enable or disable re-triggering
         */
        void setRetrigger(bool enable);

        [[ nodiscard ]] operator ::PDSynthSignalValue*() const override;  // NOLINT (*-explicit-constructor)

    protected:
        PDSynthLFO* p_LFO;

        PDCPP_DECLARE_NON_COPYABLE(LFO);
    };

    class WaveformLFO
        : public LFO
    {
    public:
        enum Waveform {
            Square = LFOType::kLFOTypeSquare,
            SawtoothUp = LFOType::kLFOTypeSawtoothUp,
            SawtoothDown = LFOType::kLFOTypeSawtoothDown,
            Triangle = LFOType::kLFOTypeTriangle,
            Sine = LFOType::kLFOTypeSine,
            SampleAndHold = LFOType::kLFOTypeSampleAndHold,
        };

        /**
         * Creates an LFO which uses one of the Playdate's built-in waveforms.
         *
         * @param lfoType the waveform to use
         */
        explicit WaveformLFO(Waveform lfoType);

        /**
         * Changes this LFO to use a new waveform.
         *
         * @param lfoType the new waveform to use
         */
        void setType(Waveform lfoType);
    };

}
