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
#include "SoundSource.h"
#include "SoundEffect.h"
#include "Signal.h"


namespace pdcpp
{
    class Channel
    {
    public:
        /**
         * Creates a new audio Channel. Channels allow for groupwise control of
         * pdcpp::SoundSource's effects and volumes.
         *
         * @param startAdded add the channel to the sound engine as part of
         *     construction
         */
        explicit Channel(bool startAdded=true);

        // Move constructor
        Channel(Channel&& other) noexcept;

        // Move-assignment constructor
        Channel& operator=(Channel&& other) noexcept;

        /**
         * Destructor. Cleans up the channel object, not any of the sources.
         */
        virtual ~Channel();

        /**
         * Adds the channel to the sound engine.
         */
        void addToSoundEngine();

        /*
         * Removes the channel from the sound engine.
         */
        void removeFromSoundEngine();

        /**
         * Adds a source to this channel.
         *
         * @param source the pdcpp::SoundSource to add
         */
        void addSource(const pdcpp::SoundSource& source);

        /**
         * Removes a source from this channel
         *
         * @param source the pdcpp::SoundSource to remove
         */
        void removeSource(const pdcpp::SoundSource& source);

        /**
         * Adds an effect to this channel
         *
         * @param effect the effect to add
         */
        void addEffect(const pdcpp::SoundEffect& effect);

        /**
         * Removes an effect from this channel
         *
         * @param effect the effect to remove
         */
        void removeEffect(const pdcpp::SoundEffect& effect);

        /**
         * Sets the volume of this channel
         *
         * @param vol the volume, range 0-1 with 0 being muted, and 1 being full
         *    volume.
         */
        void setVolume(float vol);

        /**
         * @returns the volume of this channel.
         */
        [[ nodiscard ]] float getVolume() const;

        /**
         * Sets the pan of this channel
         *
         * @param pan the pan, range -1 to 1, left to right, 0 as center.
         */
        void setPan(float pan);

        /**
         * Set a modulator to automatically adjust the volume of the channel
         *
         * @param mod a pdcpp::SignalBase object to use as the modulator
         */
        void setVolumeModulator(const pdcpp::Signal& mod);

        /**
         * @returns a C API-compatible pointer to the volume modulator
         */
        [[ nodiscard ]] pdcpp::SignalContainer getVolumeModulator() const;

        /**
         * Set a modulator to automatically adjust the pan of the channel
         *
         * @param mod a pdcpp::SignalBase object to use as the modulator
         */
        void setPanModulator(const pdcpp::Signal& mod);

        /**
         * @returns a C API-compatible pointer to the pan modulator
         */
        [[ nodiscard ]] pdcpp::SignalContainer getPanModulator() const;

        SoundChannel* get() { return p_Chan; }

    protected:
        SoundChannel* p_Chan;
    };

    class DefaultChannel
        : public Channel
    {
    public:
        /**
         * Provides pdcpp::Channel-style access to the global default channel.
         */
        DefaultChannel();

        // Destructor. doesn't do anything.
        ~DefaultChannel() override;
    };
}
