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
#include <vector>
#include <utility>
#include <pd_api.h>

namespace pdcpp
{
    /**
     * Base Class for FilePlayer, SamplePlayer, PDSynth, DelayLineTap, and
     * PDSynthInstrument. Not user-creatable.
     */
    class SoundSource
    {
    public:
        /**
         * Set the volume, panned, of this source
         * @param lvol the left volume to set 0.0f to 1.0f
         * @param rvol the right volume to set 0.0f to 1.0f
         */
        void setVolume(float lvol, float rvol);

        /**
         * @returns the volume of this source, as an L/R pair.
         */
        [[ nodiscard ]] std::pair<float, float> getVolume() const;

        /**
         * @returns if the source is currently playing
         */
        [[ nodiscard ]] bool isPlaying() const;

        /**
         * To be implemented by all Custom and API sound sources. This bridges
         * the gap between the C API and C++.
         * @return the key structure pointer for the child object as a
         *     PD::SoundSource for use with internal APIs
         */
        [[ nodiscard ]] virtual operator ::SoundSource*() const  = 0;  // NOLINT(*-explicit-constructor)

        /**
         * Set a callback to hit when this sound source finishes playing.
         * TODO: Request that Panic add the ability to set user data with
         *    SoundSources, so this callback so we can use C++-style contexts.
         *
         * @param func the callback function to set.
         */
        void setFinishCallback(sndCallbackProc* func);
    };
}