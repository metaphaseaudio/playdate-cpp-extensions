/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/5/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <pd_api.h>
#include <pdcpp/core/util.h>
#include "SoundSource.h"
#include "AudioSample.h"


namespace pdcpp
{
    class SamplePlayer
        : public SoundSource
    {
    public:
        /**
         * Creates a new SamplePlayer which can play one pdcpp::AudioSample at a
         * time at various speeds, including backwards.
         */
        SamplePlayer();

        // Move constructor
        SamplePlayer(SamplePlayer&& other);

        // Move assignment constructor
        SamplePlayer& operator=(SamplePlayer&& other);

        // Destructor. Frees the internal sample player
        ~SamplePlayer();

        /**
         * Sets the current sample to play. Will any previously set sample.
         *
         * @param sample The new sample to play
         */
        void setSample(const pdcpp::AudioSample& sample);

        /**
         * Sets the start and end position in frames (samples) to play from the
         * player's current pdcpp::AudioSample. This is always "left-to-right"
         * regardless of play rate which may be reversed.
         *
         * @param start the starting
         * @param end
         */
        void setPlayRange(int start, int end);

        /**
         * Pause the SamplePlayer
         * @param shouldPause true for paused, false for 'play'
         */
        void setPaused(bool shouldPause);

        /**
         * Start playback at a given rate.
         *
         * @param rate the rate to use (eg 1.0f = normal speed, 2.0f twice speed
         *    -0.5f half-speed backwards, etc.)
         *
         * @param repeatCount
         *     1 or greater for N play-throughs of the sample
         *     0 for infinite looping of the sample
         *     -1 for ping-pong playback.
         *
         * @return Unknown, undocumented in the C API.
         */
        int play(float rate=1.0f, int repeatCount=1);

        /**
         * Stops playing the sample.
         */
        void stop();

        /**
         * Sets the current playback offset of the SamplePlayer, in seconds.
         * This value is not adjusted for rate.
         *
         * @param offset the new play head position in seconds
         */
        void setOffset(float offset);

        /**
         * @returns The current play head position.
         */
        [[ nodiscard ]] float getOffset() const;

        /**
         * Sets the playback rate. (eg 1.0f = normal speed, 2.0f twice speed
         *    -0.5f half-speed backwards, etc.)
         *
         * @param rate the new rate to use.
         */
        void setRate(float rate);

        /**
         * @returns the current rate of this SamplePlayer
         */
        [[ nodiscard ]] float getRate() const;

        /**
         * @returns Returns the length, in seconds, of the sample assigned to
         *     player.
         */
        [[ nodiscard ]] float getLength();

        /**
         * Implicit conversion to a raw playdate ::SoundSource* to allow for
         * direct use with the playdate C API.
         */
        [[ nodiscard ]] operator ::SoundSource*() const override;  // NOLINT(*-explicit-constructor)

    private:
        ::SamplePlayer* p_SamplePlayer;
    };
}