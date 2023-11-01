/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/1/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <pd_api.h>
#include <pdcpp/core/util.h>

#include "Signal.h"
#include "Envelope.h"
#include "AudioSample.h"
#include "SoundSource.h"

namespace pdcpp
{
    class SynthesizerVoice
        : public pdcpp::SoundSource
    {
    public:
        /**
         * Creates a new SynthesizerVoice
         */
        SynthesizerVoice();

        // Move constructor
        SynthesizerVoice(SynthesizerVoice&& other) noexcept;

        // Move-assignment constructor
        SynthesizerVoice& operator=(SynthesizerVoice&& other) noexcept;

        // Destructor
        ~SynthesizerVoice();

        /**
         * Sets the waveform to be used by this voice
         *
         * @param waveform a PD C API ::SoundWaveform constant
         */
        void setWaveform(SoundWaveform waveform);

        /**
         * Sets a sample to play as this synthesizer's voice
         *
         * @param sample the pdcpp::AudioSample to use
         * @param sustainStart the start of the range to loop when sustaining in
         *     samples
         * @param sustainEnd the end of the range to loop when sustaining in
         *     samples
         */
        void setSample(const pdcpp::AudioSample& sample, uint32_t sustainStart, uint32_t sustainEnd);

        /**
         * Sets a wavetable for the synth to play. Sample data must be 16-bit
         * mono uncompressed.
         *
         * When there's a wavetable set, there are four parameters which can
         * control the x/y position of the wavetable. Manipulate these with
         * `setParameter` and `setParameterModulator`
         *
         * Parameters are:
         *     1: x position, values are from 0 to the table width
         *     2: x position, values are normalized from 0 to 1
         *     3: y position, values are from 0 to the table height
         *     4: y position, values are from normalized from 0 to 1
         *
         * @param sample the sample to use as a wavetable
         * @param log2size the base 2 logarithm of the number of samples in each
         *     waveform "cell" in the table
         * @param nColumns the number of cells horizontally across the wavetable
         * @param nRows the number of cells vertically across the wavetable
         */
        void setWavetable(const pdcpp::AudioSample& sample, int log2size, int nColumns, int nRows);

        /**
         * Set the attack (rise to maximum) time of the synthesizer voice's
         * envelope
         *
         * @param attack time in seconds
         */
        void setAttackTime(float attack);

        /**
         * Set the decay (fall to sustain) time of the synthesizer voice's
         * envelope
         *
         * @param decay time in seconds
         */
        void setDecayTime(float decay);

        /**
         * Set the sustain level of the synthesizer voice's envelope
         *
         * @param sustain gain coefficient 0.0f to 1.0f
         */
        void setSustainLevel(float sustain);

        /**
         * Set the release (fall to 0) time of the synthesizer voice's
         * envelope
         *
         * @param release time in seconds
         */
        void setReleaseTime(float release);

        /**
         * Adds a modulator to this voice's frequency
         *
         * @param mod the modulator to use
         */
        void setFrequencyModulator(const pdcpp::Signal& mod);

        /**
          * @returns a handle to the envelope of this SynthesizerVoice.
          */
        void setAmplitudeModulator(const pdcpp::Signal& mod);

        [[ nodiscard ]] pdcpp::Envelope getEnvelope() const;

        /**
         * implicit conversion to a ::PDSynth* for use with the C API
         */
        [[ nodiscard ]] operator ::PDSynth* () const { return p_Synth; } // NOLINT(*-explicit-constructor)

        // Implements base class
        [[ nodiscard ]] operator ::SoundSource*() const override;  // NOLINT (*-explicit-constructor)
    private:
        PDSynth* p_Synth;
        PDCPP_DECLARE_NON_COPYABLE(SynthesizerVoice);
    };
}
