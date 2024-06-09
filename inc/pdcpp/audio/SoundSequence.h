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
#include <functional>
#include <string>
#include <pd_api.h>
#include "SequenceTrack.h"
#include "../core/util.h"


namespace pdcpp
{
    class SoundSequence
    {
    public:
        /**
         * Creates a new Sequence. Sequences comprise one or more tracks, and
         * provides an API to manage them as a unit. Load a MIDI file, or build
         * up your sequence from scratch.
         */
        SoundSequence();

        /**
         * Creates a new Sequence, loading a filepath as a MIDI file
         *
         * @param midiFile the file to load
         */
        explicit SoundSequence(const std::string& midiFile);

        // Destructor. Frees all the resources of this sequence
        ~SoundSequence();

        // Move constructor
        SoundSequence(SoundSequence&& other) noexcept;

        // Move-assignment constructor
        SoundSequence& operator=(SoundSequence&& other) noexcept;

        /**
         * Loads a midi file into this sequence. Will log an error and return
         * `false` if the file failed to load
         *
         * @param filepath the filepath to attempt to load
         * @returns true on successful load, false on failure
         */
        bool loadMidiFile(const std::string& filepath);

        /**
         * Plays the sequence from the current time forward. Optionally provide
         * a function to use as a callback when this sequence has completed.
         *
         * @param completeCallback the optional callback to use. Calling this
         *     without a callback will disable any callback that was previously
         *     set.
         */
        void play(std::function<void(pdcpp::SoundSequence*)> completeCallback=[](auto*){});

        /**
         * Stops/Pauses the sequence. Note that calling this function will hit
         * the callback function.
         */
        void stop();

        /**
         * @returns whether or not the sequence is currently playing.
         */
        [[ nodiscard ]] bool isPlaying() const;

        /**
         * @returns the current time of the playhead, in samples, since the
         *     beginning of the file. Note that this is dependent upon the
         *     current tempo of the sequence.
         */
        uint32_t getTime();

        /**
         * Moves the playhead to a new time, in samples, from the start of the
         * sequence. Note that this is dependent upon the tempo of the sequence.
         *
         * @param time the new time, in samples, of the playhead
         */
        void setTime(uint32_t time);

        /**
         * Sets a section of the sequence to loop, in steps.
         *
         * @param loopStart the step at which the loop should start
         * @param loopEnd the step at which the loop should return
         * @param loops optional count for looping. 0 means "loop indefinitely."
         */
        void setLoops(int loopStart, int loopEnd, int loops=0);

        /**
         * @return the tempo of the sequence in "steps per second"
         */
        float getTempo();

        /**
         * Changes the tempo of the sequence
         *
         * @param stepsPerSecond the new BPM to use
         */
        void setTempo(float stepsPerSecond);

        /**
         * @return the length of the longest track in the sequence, in steps.
         */
        uint32_t getLength();

        /**
         * @returns the current step of the playhead. Includes a fractional
         *     element for the case where the playhead is currently between two
         *     beats.
         */
        float getCurrentStep();

        /**
         * Moves the playhead to a new position within the sequence in beats
         *
         * @param beat the beat.fractional_beat to which the playhead will be
         *     moved
         * @param playNotes indicate whether any notes under the playhead should
         *     be played.
         */
        void setCurrentStep(float beat, bool playNotes);

        /**
         * Adds a new track to the sequence
         *
         * @param instrument the instrument to use for this track
         * @returns a container for accessing the newly-created track
         */
        SequenceTrackContainer addTrack(const pdcpp::Synthesizer& instrument);

        /**
         * @returns the number of tracks in this sequence
         */
        int getTrackCount();

        /**
         * Sets a track to a given index
         *
         * @param track the track to set to an index
         * @param idx the index to set
         */
        void setTrackAtIndex(const SequenceTrackContainer& track, unsigned int idx);

        /**
         * Gets a track at a given index
         *
         * @param idx the index from which to retrieve a track container
         * @return a container object for the track at the index
         */
        SequenceTrackContainer getTrackAtIndex(unsigned int idx);

        /**
         * Sends an "all notes off" command to all tracks/synths in the
         * sequence. This is useful for silencing tracks after calling `stop`
         */
        void allNotesOff();
    private:
        static void playCompleteShim(::SoundSequence*, void* usrData);
        std::function<void(pdcpp::SoundSequence*)> m_Callback;
        ::SoundSequence* p_Sequence;

        PDCPP_DECLARE_NON_COPYABLE(SoundSequence);
    };
}
