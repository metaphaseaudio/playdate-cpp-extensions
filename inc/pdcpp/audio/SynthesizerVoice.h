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
    class CustomSynthGenerator
    {
    public:
        static constexpr int kFloatScalar = 0x7fffff;

        /**
         * Create a custom generator for a synthesizer voice. Inherit from this,
         * devine the virtual methods and use the `setGenerator` method of a
         * pdcpp::SynthesizerVoice, to start generating data for that voice.
         */
        explicit CustomSynthGenerator(bool isStereo) : m_IsStereo(isStereo) {};

        // Destructor
        virtual ~CustomSynthGenerator() = default;

        /**
         * Provides custom waveform generator functions for the synth.
         *
         * @param leftSamps sample buffers in Q8.24 format
         * @param rightSamps sample buffers in Q8.24 format if this is a stereo
         *     generator
         * @param nSamps the number of frames to generate
         * @param rate the amount to change a (Q32) phase accumulator each
         *     sample. Can be ignored if you're just using `noteOn`, but are
         *     required for any kind of transposition or modulation function to
         *     work smoothly.
         * @param drate the amount to change `rate` each sample. Can be
         *     ignored if you're just using `noteOn`, but are required for any
         *     kind of transposition or modulation function to work smoothly.
         * @return the number of samples processed. Returning less than nSamps
         *     will inform the voice it's done playing, and this generator won't
         *     be called again until the next note-on.
         */
        virtual int renderBlock(int32_t* leftSamps, int32_t* rightSamps, int nSamps, uint32_t rate, int32_t drate) = 0;

        /**
         * @returns whether this is a stereo generator
         */
        [[ nodiscard ]] bool isStereo() const { return  m_IsStereo; };

        /**
         * Called when the SynthesizerVoice gets a Note On.
         *
         * @param note the note to start playing
         * @param velo the velocity of the note
         * @param length optionally how long to play. -1 means play until
         *     release.
         */
        virtual void noteOn(MIDINote note, float velo, float length) {};

        /**
         * SynthesizerVoice has received a note off.
         *
         * @param allowTail true if the voice isn't being stolen. false if the
         *     generator can safely produce a tail of data for a while.
         */
        virtual void release(bool allowTail) {};

        /**
         * Called by the SynthesizerVoice when someone tries to use
         * `setParameter` or when a modulator tries to update a parameter.
         * Note that convention dictates that parameters are 1-indexed!
         *
         * @param parameter the parameter index being updated
         * @param value the value of the parameter being updated
         * @return 1 if the parameter was valid, 0 if it was not.
         */
        virtual int setParameter(int parameter, float value) { return 0; };

        /**
         * @returns the number of parameters supported by this generator.
         */
        [[ nodiscard ]] virtual int getNParameters() const { return 0; }

    private:
        friend class SynthesizerVoiceShims;
        // Maybe don't use this? It's not, like, a destructor or anything.
        virtual void deallocateCalled() {};

        // Maybe don't use this? It's not, like, a copy constructor or anything.
        virtual void* copyCalled() { return (void*) this; };

        bool m_IsStereo;
        PDCPP_DECLARE_NON_COPYABLE_NON_MOVABLE(CustomSynthGenerator);
    };

    class SynthesizerVoiceContainer
        : public pdcpp::SoundSource
    {
    public:
        explicit SynthesizerVoiceContainer(PDSynth* synth);

        virtual ~SynthesizerVoiceContainer() = default;

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
         * Tell the voice to use a custom signal generator instead any of the
         * built-in signal types.
         *
         * @param generator the generator to use;
         */
        void setCustomGenerator(CustomSynthGenerator& generator);

        void clearCustomGenerator();

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

        /**
         * @returns the number of parameters for this voice
         */
        [[ nodiscard ]] int getParameterCount() const;

        /**
         * Sets the value of a voice's parameters.
         *
         * @param paramNumber the parameter number to change
         * @param value the new value of the parameter
         * @returns whether or not the parameter ID was valid
         */
        bool setParameter(int paramNumber, float value);

        /**
         * Modulate a voice's parameters. Note that the paramNumber is
         * 1-indexed! 0 is still technically valid, but is a no-op on the voice,
         * allowing one to use a voice to trigger a pdcpp::Signal modulating
         * something completely separated.
         *
         * @param paramNumber the parameter ID number to modulate
         * @param mod the modulator to use
         */
        void setParameterModulator(int paramNumber, const pdcpp::Signal& mod);

        /**
         * Removes the modulator for the given parameter number
         *
         * @param paramNumber the parameter number for which the modulation
         *     should be cleared.
         */
        void clearParameterModulator(int paramNumber);

        /**
         * @returns the amplitude envelope of this voice
         */
        [[nodiscard]] pdcpp::Envelope getEnvelope() const;

        /**
         * Starts a note on this voice.
         *
         * @param note the MIDInote to play
         * @param vel a velocity for the note (0.0f - 1.0f)
         * @param len optional length for the note. If -1, will play until note
         *     off. Default -1.
         * @param when an optional time the note should be played. 0 to play the
         *     note immediately. Default 0.
         */
        void playMIDINote(MIDINote note, float vel, float len=-1, uint32_t when=0);

        /**
         * stops the currently playing note.
         *
         * @param when an optional time at which to stop the note. 0 to stop
         *     immediately. Default 0.
         */
        void noteOff(uint32_t when=0);

        /**
         * Sets the transposition of the voice. Virtual because Synth voices
         * with custom generators can't honor the transposition (no
         * `getTranspose`) so inheritance is the best way to handle this.
         *
         * @param halfSteps how many half-steps to transpose the voice
         */
        virtual void setTranspose(float halfSteps);

        /**
         * implicit conversion to a ::PDSynth* for use with the C API
         */
        [[nodiscard]] operator ::PDSynth*() const
        { return p_Synth; } // NOLINT(*-explicit-constructor)

        // Implements base class
        [[nodiscard]] operator ::SoundSource*() const override;  // NOLINT (*-explicit-constructor)

        bool operator==(SynthesizerVoiceContainer* other) { return other->p_Synth == p_Synth; }

        // Implements abstract class.
        void finished() override {};
    protected:
        PDSynth* p_Synth;
    };


    class SynthesizerVoice
        : public SynthesizerVoiceContainer
    {
    public:
        /**
         * Creates a new SynthesizerVoiceContainer. Does not own the pointer
         */
        SynthesizerVoice();

        // Move constructor
        SynthesizerVoice(SynthesizerVoice&& other) noexcept;

        // Move-assignment constructor
        SynthesizerVoice& operator=(SynthesizerVoice&& other) noexcept;

        // Destructor
        ~SynthesizerVoice() override;

    private:
        PDCPP_DECLARE_NON_COPYABLE(SynthesizerVoice);
    };

}
