/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/5/2023
 *  Original author: MrBZapp
 */
#include <pdcpp/audio/AudioSample.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::AudioSample::AudioSample(const std::string& filepath)
    : p_Sample(pdcpp::GlobalPlaydateAPI::get()->sound->sample->load(filepath.c_str()))
{
    if (p_Sample == nullptr)
    {
        auto errmsg = "Failed to load audio file at path " + filepath;
        pdcpp::GlobalPlaydateAPI::get()->system->error(errmsg.c_str());
    }
}

pdcpp::AudioSample::AudioSample(uint8_t* data, SoundFormat format, uint32_t sampleRate, int byteCount, bool shouldOwnData)
    : p_Sample(pdcpp::GlobalPlaydateAPI::get()->sound->sample->newSampleFromData(data, format, sampleRate, byteCount, shouldOwnData))
{}

pdcpp::AudioSample::AudioSample(AudioSample&& other) noexcept
    : p_Sample(other.p_Sample)
{ other.p_Sample = nullptr; }

pdcpp::AudioSample& pdcpp::AudioSample::operator=(AudioSample&& other) noexcept
{
    p_Sample = other.p_Sample;
    other.p_Sample = nullptr;
    return *this;
}

pdcpp::AudioSample::~AudioSample()
{
    if (p_Sample != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->sample->freeSample(p_Sample); }
}

void pdcpp::AudioSample::getInfo(uint8_t** data, SoundFormat* format, uint32_t* sampleRate, uint32_t* byteCount) const
    { pdcpp::GlobalPlaydateAPI::get()->sound->sample->getData(p_Sample, data, format, sampleRate, byteCount); }


uint8_t* pdcpp::AudioSample::getBytes() const
{
    uint8_t* data; SoundFormat format; uint32_t sampleRate; uint32_t byteCount;
    getInfo(&data, &format, &sampleRate, &byteCount);
    return data;
}

SoundFormat pdcpp::AudioSample::getFormat() const
{
    uint8_t* data; SoundFormat format; uint32_t sampleRate; uint32_t byteCount;
    getInfo(&data, &format, &sampleRate, &byteCount);

    return format;
}

uint32_t pdcpp::AudioSample::getSampleRate() const
{
    uint8_t* data; SoundFormat format; uint32_t sampleRate; uint32_t byteCount;
    getInfo(&data, &format, &sampleRate, &byteCount);
    return sampleRate;
}

uint32_t pdcpp::AudioSample::getLengthInBytes() const
{
    uint8_t* data; SoundFormat format; uint32_t sampleRate; uint32_t byteCount;
    getInfo(&data, &format, &sampleRate, &byteCount);
    return byteCount;
}

uint32_t pdcpp::AudioSample::getLengthInSamples() const
{
    uint8_t* data; SoundFormat format; uint32_t sampleRate; uint32_t byteCount;
    getInfo(&data, &format, &sampleRate, &byteCount);
    return byteCount / SoundFormat_bytesPerFrame(format);
}

float pdcpp::AudioSample::getLengthInSeconds() const
    { return pdcpp::GlobalPlaydateAPI::get()->sound->sample->getLength(p_Sample); }
