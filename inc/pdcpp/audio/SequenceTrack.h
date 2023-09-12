/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/29/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <pd_api.h>
#include "Synthesizer.h"
#include "ControlSignal.h"


namespace pdcpp
{
    class SequenceTrackContainer
    {
    public:
        /**
         * Creates a new container for a PlaydateCAPI::SequenceTrack*, which
         * comprises a PDSynthInstrument, a sequence of notes to play on that
         * instrument, and any number of ControlSignal objects to control
         * parameter changes.
         *
         * These are generally owned by `SoundSequence` objects, but can be
         * created stand-alone with the SequenceTrack class.
         *
         * @param track the pointer to reference
         */
        explicit SequenceTrackContainer(::SequenceTrack* track);

        /**
         * Changes the instrument used by this track
         *
         * @param inst the new instrument to use
         */
        void setInstrument(const pdcpp::Synthesizer& inst);

        // Temporary API to ensure coverage
        PDSynthInstrument* getInstrument();

        /**
         * Adds a single note to the track
         *
         * @param step the step position at which to add the note
         * @param len how long the note should be played
         * @param note The note to play
         * @param velocity the volume 0-1f of the note.
         */
        void addNoteEvent(uint32_t step, uint32_t len, MIDINote note, float velocity);

        /**
         * Removes the note at the given timestep
         *
         * @param step the step at which to remove the note
         * @param note the note to remove
         */
        void removeNoteEvent(uint32_t step, MIDINote note);

        /**
         * Empties the track of all notes at all steps.
         */
        void clearNotes();

        /**
         * @returns the number of control signals on this track
         */
        int getControlSignalCount();

        /**
         * @param idx the index of the ControlSignal to access
         * @returns the signal at the index
         */
        ControlSignalContainer getControlSignal(int idx);

        /**
         * Gets a ControlSignalContainer for a given MIDI controller number,
         * optionally creating one owned by this track
         *
         * @param controller the MIDI controller number
         * @param create if the ControlSignal should be created if it's doesn't
         *     exist yet.
         * @return a ControlSignalContainer for the controller
         */
        ControlSignalContainer getSignalForController(int controller, bool create=true);

        /**
         * Removes all control signals from this track
         */
        void clearAllControlSignals();

        /**
         * @returns the maximum number of simultaneously playing notes in the
         * track. (Currently, this value is only set when the track was loaded
         * from a MIDI file. We don’t yet track polyphony for user-created
         * events.)
         */
        int getPolyphony();

        /**
         * @returns  the number of voices currently playing in the track’s
         *     instrument
         */
        int getActiveVoiceCount();

        /**
         * @param mute sets whether to mute or un-mute the track.
         */
        void setMuted(bool mute);

        /**
         * @returns the position in steps where the last note in the track ends.
         */
        uint32_t getLength(); // in steps, includes full last note

        /**
         * Internally, notes are held contiguously. This function converts from
         * a step-wise index to a note-wise index for iterating over notes in a
         * track
         *
         * @param step the step to convert to a note index
         * @returns the internal array index for the first note at the given
         *     step.
         */
        int getIndexForStep(uint32_t step);

        /**
         * Attempts to retrieve a note from a given note index (not step index.)
         * Will return 0 if the index is past the end of the note array.
         *
         * **NOTE:** this, and the previous function, are very "C", and have a
         * 1-1 relationship with the underlying API. Future versions of this
         * API will use C++ iterators, at which point these functions will be
         * deprecated.
         *
         * @param index
         * @param outStep
         * @param outLen
         * @param outNote
         * @param outVelocity
         * @return
         */
        int getNoteAtIndex(int index, uint32_t* outStep, uint32_t* outLen, MIDINote* outNote, float* outVelocity);

        /**
         * @returns if the track pointed to by this container is addressable.
         */
        bool isValid() { return p_Track != nullptr; }
        /**
         * @returns a C API-compatible pointer to the SequenceTrack
         */
        [[ nodiscard ]] operator ::SequenceTrack*() const;  // NOLINT (*-explicit-constructor)
    protected:
        ::SequenceTrack* p_Track;
    };


    class SequenceTrack
        : SequenceTrackContainer
    {
    public:
        /**
         * A SequenceTrack is a SequenceTrackContainer which owns its C API
         * pointer, and will free the memory when the destructor is called.
         */
        explicit SequenceTrack(const pdcpp::Synthesizer& instrument);

        /**
         * Frees the internal pointer.
         */
        ~SequenceTrack();

        // Move constructor
        SequenceTrack(SequenceTrack&& other) noexcept;

        // Move-assignment constructor
        pdcpp::SequenceTrack& operator=(SequenceTrack&& other) noexcept;

    private:
        PDCPP_DECLARE_NON_COPYABLE(SequenceTrack);
    };
}