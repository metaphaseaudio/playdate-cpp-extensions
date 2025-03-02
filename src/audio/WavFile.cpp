//
// Created by Matt on 1/21/2025.
//

#include "pdcpp/audio/WavFile.h"
#include "pdcpp/core/File.h"

struct WAVHeader {
    char riff[4] = {'R', 'I', 'F', 'F'};
    uint32_t chunkSize;
    char wave[4] = {'W', 'A', 'V', 'E'};
    char fmt[4] = {'f', 'm', 't', ' '};
    uint32_t fmtChunkSize;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
    char data[4] = {'d', 'a', 't', 'a'};
    uint32_t dataSize;
};


std::optional<std::unique_ptr<pdcpp::AudioSample>> pdcpp::WavFile::loadFromFile(const std::string& filename)
{
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

    std::vector<uint8_t> data(header.dataSize, 0);

    handle.read(data.data(), header.dataSize);

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

void
pdcpp::WavFile::saveToFile(const std::string& filename, uint8_t* data, uint32_t nBytes, SoundFormat fmt, uint32_t
sampleRate)
{
    WAVHeader header{
        .chunkSize=nBytes + 36,
        .fmtChunkSize=16,
        .audioFormat=1,
        .numChannels=uint16_t(SoundFormatIsStereo(fmt) ? 2 : 1),
        .sampleRate=uint16_t(sampleRate),
        .byteRate=uint16_t(SoundFormat_bytesPerFrame(fmt) * sampleRate),
        .blockAlign=uint16_t(SoundFormat_bytesPerFrame(fmt)),
        .bitsPerSample=uint16_t(SoundFormatIs16bit(fmt) ? 16 : 8),
        .dataSize=nBytes
    };

    auto handle = pdcpp::FileHandle(filename, FileOptions::kFileWrite);

    handle.write(&header, sizeof(WAVHeader));
    handle.write(data, nBytes);
}

void pdcpp::WavFile::saveToFile(const std::string& name, const AudioSample& sample)
{
    uint8_t* data;
    SoundFormat fmt;
    uint32_t sampleRate;
    uint32_t byteCount;
    sample.getInfo(&data, &fmt, &sampleRate, &byteCount);
    saveToFile(name, data, byteCount, fmt, sampleRate);
}
