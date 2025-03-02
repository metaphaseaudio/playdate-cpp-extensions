//
// Created by Matt on 1/21/2025.
//

#pragma once
#include <optional>
#include <memory>
#include "AudioSample.h"

namespace pdcpp
{
    class WavFile
    {
    public:
        static std::optional<std::unique_ptr<pdcpp::AudioSample>> loadFromFile(const std::string& filename);
        static void saveToFile(const std::string& filename, uint8_t* data, uint32_t nBytes, SoundFormat fmt, uint32_t
        sampleRate=44100);
        static void saveToFile(const std::string& name, const pdcpp::AudioSample& sample);
    };
}
