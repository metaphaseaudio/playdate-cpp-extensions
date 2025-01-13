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
#include "pdcpp/core/File.h"


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

void pdcpp::AudioSample::setData(uint8_t* data, SoundFormat format, uint32_t sampleRate, int byteCount, bool shouldFreeData)
{
    p_Sample = pdcpp::GlobalPlaydateAPI::get()->sound->sample->newSampleFromData(data, format, sampleRate, byteCount, shouldFreeData);
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

std::optional<std::unique_ptr<pdcpp::AudioSample>> pdcpp::AudioSample::loadWavFile(const std::string& filename)
{
    struct WAVHeader {
        char riff[4]; // Should be "RIFF"
        uint32_t chunkSize;
        char wave[4]; // Should be "WAVE"
        char fmt[4]; // Should be "fmt"
        uint32_t fmtChunkSize;
        uint16_t audioFormat; // 1 for PCM
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;
        char data[4]; // Should be "data"
        uint32_t dataSize;
    };
    WAVHeader header{};
    auto handle = pdcpp::FileHandle(filename, FileOptions::kFileReadData);
    handle.read(&header, sizeof(WAVHeader));

    const auto riff = std::string(header.riff, header.riff + 4);
    const auto wave = std::string(header.wave, header.wave + 4);
    const auto fmtStr = std::string(header.fmt, header.fmt + 3);
    const auto dataStr = std::string(header.data, header.data + 4);
    if (riff != "RIFF") { return std::nullopt; }
    if (wave != "WAVE") { return std::nullopt; }
    if (fmtStr != "fmt") { return std::nullopt; }
    if (dataStr != "data") { return std::nullopt; }
    if (header.audioFormat != 1) { return std::nullopt; } // Must be PCM format
    if (!(header.bitsPerSample == 8 || header.bitsPerSample == 16)) { return std::nullopt; }
    if (header.numChannels > 2) { return std::nullopt; }

    auto toRead = header.dataSize * (header.bitsPerSample / 8);
    std::vector<uint8_t> data(toRead);

    handle.read(data.data(), toRead);

    SoundFormat fmt;

    switch(header.bitsPerSample)
    {
        case 8:
            fmt = header.numChannels == 1 ? SoundFormat::kSound8bitMono : SoundFormat::kSound8bitStereo;
            break;
        case 16:
            fmt = header.numChannels == 1 ? SoundFormat::kSound16bitMono : SoundFormat::kSound16bitStereo;
            break;
    }

    auto rv = std::make_unique<AudioSampleWithData>(data, fmt, header.sampleRate);
    return rv;
}

std::optional<std::unique_ptr<pdcpp::AudioSample>> pdcpp::AudioSample::loadSampleFromFile(const std::string& filename)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    std::optional<std::unique_ptr<pdcpp::AudioSample>> rv = std::nullopt;
    const std::string failMsg = "WARN: failed to open " + filename;
    if (filename.ends_with(".pda")) { rv = std::make_unique<AudioSample>(filename); }
    else if (filename.ends_with(".wav")) { rv =  pdcpp::AudioSample::loadWavFile(filename); }
    else { pd->system->logToConsole(failMsg.c_str()); }
    if (!rv) { pd->system->logToConsole(failMsg.c_str()); }
    return rv;
}


pdcpp::AudioSampleWithData::AudioSampleWithData(std::vector<uint8_t> data, SoundFormat format, uint32_t sampleRate)
    : m_Data(std::move(data))
{ setData(m_Data.data(), format, sampleRate, m_Data.size(), false); }
