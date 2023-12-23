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
#include <utility>
#include <pd_api.h>
#include <pdcpp/core/util.h>
#include "Channel.h"
#include "SynthesizerVoice.h"
#include "SoundSource.h"


namespace pdcpp
{
    class Synthesizer
        : public pdcpp::SoundSource
    {
    public:
        /**
         * Creates a Synthesizer object which is used to manage one or more
         * pdcpp::SynthesizerVoice objects.
         */
        Synthesizer();

        // Move constructor
        Synthesizer(Synthesizer&& other);

        // Move-assignment constructor
        Synthesizer& operator=(Synthesizer&& other);

        // Destructor. Frees internal C API pointer.
        ~Synthesizer();

        /**
         * Adds a new voice to this Synthesizer. When a note-on arrives, the
         * Synthesizer will select one of the voices which is able to play in
         * that range to play, and manage its playback lifecycle
         *
         * @param voice the voice to add
         * @param lowNote the lowest ::MIDINote this voice is allowed to play
         * @param hiNote the highest ::MIDINote this voice is allowed to play
         * @param transpose the number of half-steps by which playback of this
         *     voice should be transposed
         */
        void addVoice(const pdcpp::SynthesizerVoice& voice, MIDINote lowNote, MIDINote hiNote, float transpose);

        /**
         * Shifts the pitch of all voices up or down, ranged -1 to 1.
         *
         * @param bend the bend to apply
         */
        void setPitchBend(float bend);

        /**
         * Sets how far the pitch is shifted at max/min bend
         *
         * @param halfSteps the number of half steps, up and down, to apply at
         *     min/max bend.
         */
        void setPitchBendRange(float halfSteps);

        /**
         * Shifts the pitch of all voices up or down by the given number of
         * half-steps
         *
         * @param halfSteps the number of half steps to shift.
         */
        void setTranspose(float halfSteps);

        /**
         * @returns how many voices are currently active
         */
        [[ nodiscard ]] int getActiveVoiceCount() const;

        /**
         * Tells the synthesizer to play a given note with a velocity,
         * optionally with a specific time in milliseconds to trigger the
         * command.
         *
         * @param note the MIDINote to turn on
         * @param vel the velocity, or how loud the note should be
         * @param when the time in milliseconds to turn the note on. Default is
         *     0 or "right now"
         */
        SynthesizerVoiceContainer noteOn(MIDINote note, float vel, uint32_t when=0);

        /**
         * The same as `noteOn(note, vel, when)` but with a specific length for
         * the note to play for.
         *
         * @param note the MIDINote to turn on
         * @param vel the velocity, or how loud the note should be
         * @param len the time in seconds for which the note should be played
         * @param when the time in milliseconds to turn the note on
         */
        SynthesizerVoiceContainer noteOn(MIDINote note, float vel, float len, uint32_t when=0);

        /**
         * Sends a note off event to the synth to turn off the given note.
         *
         * @param note the note to turn off
         * @param when when to send the signal. default is 0 which sends the
         *     event immediately
         */
        void noteOff(MIDINote note, uint32_t when=0);

        /**
         * Sends an all notes off event, optionally at a given time
         * @param when the optional time to send the event. default is 0 which
         *    sends the event immediately
         */
        void allNotesOff(uint32_t when=0);

        [[ nodiscard ]] operator ::PDSynthInstrument*() const;  // NOLINT(*-explicit-constructor)
        [[ nodiscard ]] operator ::SoundSource*() const override;  // NOLINT(*-explicit-constructor)
    private:
        PDSynthInstrument* p_Instrument;
        PDCPP_DECLARE_NON_COPYABLE(Synthesizer);
    };
}
