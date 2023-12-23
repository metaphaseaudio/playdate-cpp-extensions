/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/1/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/SynthesizerVoice.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

namespace pdcpp
{
    class SynthesizerVoiceShims
    {
    public:
        static int render(void* obj,  int32_t* left, int32_t* right, int nsamples, uint32_t rate, int32_t drate)
        {
            auto thisPtr = static_cast<pdcpp::CustomSynthSignalGenerator*>(obj);
            return thisPtr->renderBlock(left, right, nsamples, rate, drate);
        };

        static void noteOn(void* obj, MIDINote note, float velocity, float len)
        {
            auto thisPtr = static_cast<pdcpp::CustomSynthSignalGenerator*>(obj);
            thisPtr->noteOn(note, velocity, len);
        };

        static void release(void* obj, int ended)
        {
            auto thisPtr = static_cast<pdcpp::CustomSynthSignalGenerator*>(obj);
            thisPtr->release(!bool(ended));
        }

        static int setParameter(void* obj, int parameter, float value)
        {
            auto thisPtr = static_cast<pdcpp::CustomSynthSignalGenerator*>(obj);
            return thisPtr->setParameter(parameter, value);
        }

        static void dealloc(void* obj)
        {
            auto thisPtr = static_cast<pdcpp::CustomSynthSignalGenerator*>(obj);
            thisPtr->deallocateCalled();
        }
    };
}


pdcpp::SynthesizerVoice::SynthesizerVoice()
    : p_Synth(pdcpp::GlobalPlaydateAPI::get()->sound->synth->newSynth())
{}

pdcpp::SynthesizerVoice::SynthesizerVoice(pdcpp::SynthesizerVoice&& other) noexcept
    : p_Synth(other.p_Synth)
{ other.p_Synth = nullptr; }

pdcpp::SynthesizerVoice& pdcpp::SynthesizerVoice::operator=(pdcpp::SynthesizerVoice&& other) noexcept
{
    p_Synth = other.p_Synth;
    other.p_Synth = nullptr;
    return *this;
}

pdcpp::SynthesizerVoice::~SynthesizerVoice()
{
    if (p_Synth != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->synth->freeSynth(p_Synth); }
}

void pdcpp::SynthesizerVoice::setWaveform(SoundWaveform waveform) { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setWaveform(p_Synth, waveform); }
void pdcpp::SynthesizerVoice::setAttackTime(float attack) { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setAttackTime(p_Synth, attack); }
void pdcpp::SynthesizerVoice::setDecayTime(float decay) { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setDecayTime(p_Synth, decay); }
void pdcpp::SynthesizerVoice::setSustainLevel(float sustain) { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setSustainLevel(p_Synth, sustain); }
void pdcpp::SynthesizerVoice::setReleaseTime(float release) { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setReleaseTime(p_Synth, release); }

void pdcpp::SynthesizerVoice::setFrequencyModulator(const pdcpp::Signal& lfo)
    { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setFrequencyModulator(p_Synth, lfo); }

void pdcpp::SynthesizerVoice::setAmplitudeModulator(const pdcpp::Signal& lfo)
    { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setAmplitudeModulator(p_Synth, lfo); }

pdcpp::Envelope pdcpp::SynthesizerVoice::getEnvelope() const
    { return pdcpp::Envelope(pdcpp::GlobalPlaydateAPI::get()->sound->synth->getEnvelope(p_Synth)); }

void pdcpp::SynthesizerVoice::setSample(const AudioSample& sample, uint32_t sustainStart, uint32_t sustainEnd)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->synth->setSample(p_Synth, sample, sustainStart, sustainEnd); }

void pdcpp::SynthesizerVoice::setWavetable(const AudioSample& sample, int log2size, int nColumns, int nRows)
{
    auto err = pdcpp::GlobalPlaydateAPI::get()->sound->synth->setWavetable(p_Synth, sample, log2size, nColumns, nRows);

    if (err == 0)
    {
        auto errMsg = "Failed to set wavetable, dimensions don't match the sample size.";
        pdcpp::GlobalPlaydateAPI::get()->system->error(errMsg);
    }
}

pdcpp::SynthesizerVoice::operator ::SoundSource*() const
{
    return reinterpret_cast<::SoundSource*>(p_Synth);
}

void pdcpp::SynthesizerVoice::setCustomGenerator(pdcpp::CustomSynthSignalGenerator& generator)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->synth->setGenerator(
        p_Synth,
        generator.isStereo(),
        SynthesizerVoiceShims::render,
        SynthesizerVoiceShims::noteOn,
        SynthesizerVoiceShims::release,
        SynthesizerVoiceShims::setParameter,
        SynthesizerVoiceShims::dealloc,
        &generator
    );
}

