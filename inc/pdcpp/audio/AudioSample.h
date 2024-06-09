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
#include <string>
#include <pd_api.h>


namespace pdcpp
{
    class AudioSample
    {
    public:
        /**
         * Creates an AudioSample by loading the data from a file at the given
         * path (will automatically account for most PCM *.wav format headers.)
         * If the path doesn't exist, an error will be logged.
         *
         * @param filepath the path to the file to load.
         */
        explicit AudioSample(const std::string& filepath);

        /**
         * Creates an AudioSample from a pointer to a series of bytes. This
         * object can optionally own the data passed to it.
         *
         * @param data the data as a series of bytes to use for this sample. The
         *      data must not be free'd before this object.
         * @param format the format of the byte array
         * @param sampleRate the sample rate of the data
         * @param byteCount how many bytes are in the array
         * @param shouldOwnData optionally take ownership of the data supplied.
         *      if `true`, the provided data will be deleted at the end of this
         *      object's lifetime. If `false`, the data must be free'd by the
         *      caller separately. Default=false
         */
        AudioSample(uint8_t* data, SoundFormat format, uint32_t sampleRate, int byteCount, bool shouldOwnData=false);

        // Move constructor
        AudioSample(AudioSample&& other) noexcept;

        // Move-assignment constructor
        AudioSample& operator=(AudioSample&& other) noexcept;

        // Destructor
        ~AudioSample();

        /**
         * @returns a pointer to the underlying data in bytes of this
         *     AudioSample.
         */
        [[ nodiscard ]] uint8_t* getBytes() const;

        /**
         * @returns the SampleFormat of this AudioSample.
         */
        [[ nodiscard ]] SoundFormat getFormat() const;

        /**
         * @returns the sample rate of this AudioSample
         */
        [[ nodiscard ]] uint32_t getSampleRate() const;

        /**
         * @returns the length in bytes of this AudioSample.
         */
        [[ nodiscard ]] uint32_t getLengthInBytes() const;

        /**
         * @returns the length of this samples of this AudioSample.
         */
        [[ nodiscard ]] uint32_t getLengthInSamples() const;

        /**
         * @returns returns the length in seconds of this AudioSample
         */
        [[ nodiscard ]] float getLengthInSeconds() const;

        /**
         * Direct shim method to the C API to retrieve information about this
         * sample. Prefer this method over the individual property getters when
         * more than one of the property getters will be used within a given
         * scope.
         *
         * @param data a pointer to be initialized with the pointer to the
         *     underlying data of this AudioSample
         * @param format a pointer to be initialized with the SampleFormat of
         *     this AudioSample
         * @param sampleRate a pointer to be initialized with the sample rate of
         *     this AudioSample
         * @param byteCount a pointer to be initialized with the byte count of
         *     this AudioSample
         */
        void getInfo(uint8_t** data, SoundFormat* format, uint32_t* sampleRate, uint32_t* byteCount) const;

        /**
         * Implicit conversion to a raw playdate ::AudioSample to allow for
         * direct use with the playdate C API.
         */
        [[ nodiscard ]] operator ::AudioSample* () const { return p_Sample; } // NOLINT (*-explicit-constructor)

    public:
        ::AudioSample* p_Sample;

        // This object is non-copyable, *particularly* when owning the provided
        // data.
        AudioSample(const AudioSample&) = delete;
        AudioSample& operator= (AudioSample&) = delete;
    };
}
