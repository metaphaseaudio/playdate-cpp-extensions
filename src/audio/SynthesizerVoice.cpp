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
            if (obj == nullptr) { return 0; }
            auto thisPtr = static_cast<pdcpp::CustomSynthGenerator*>(obj);
            return thisPtr->renderBlock(left, right, nsamples, rate, drate);
        };

        static void noteOn(void* obj, MIDINote note, float velocity, float len)
        {
            if (obj == nullptr) { return; }
            auto thisPtr = static_cast<pdcpp::CustomSynthGenerator*>(obj);
            thisPtr->noteOn(note, velocity, len);
        };

        static void release(void* obj, int ended)
        {
            if (obj == nullptr) { return; }
            auto thisPtr = static_cast<pdcpp::CustomSynthGenerator*>(obj);
            thisPtr->release(!bool(ended));
        }

        static int setParameter(void* obj, int parameter, float value)
        {
            if (obj == nullptr) { return 0; }
            auto thisPtr = static_cast<pdcpp::CustomSynthGenerator*>(obj);
            return thisPtr->setParameter(parameter, value);
        }

        static void dealloc(void* obj)
        {
            if (obj == nullptr) { return; }
            auto thisPtr = static_cast<pdcpp::CustomSynthGenerator*>(obj);
            thisPtr->deallocateCalled();
        }

        static void* copy(void* obj)
        {
            if (obj == nullptr) { return nullptr; }
            auto thisPtr = static_cast<pdcpp::CustomSynthGenerator*>(obj);
            return thisPtr->copyCalled();
        }
    };
}

void pdcpp:: SynthesizerVoiceContainer::setWaveform(SoundWaveform waveform) { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setWaveform(p_Synth, waveform); }
void pdcpp:: SynthesizerVoiceContainer::setAttackTime(float attack) { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setAttackTime(p_Synth, attack); }
void pdcpp:: SynthesizerVoiceContainer::setDecayTime(float decay) { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setDecayTime(p_Synth, decay); }
void pdcpp:: SynthesizerVoiceContainer::setSustainLevel(float sustain) { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setSustainLevel(p_Synth, sustain); }
void pdcpp:: SynthesizerVoiceContainer::setReleaseTime(float release) { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setReleaseTime(p_Synth, release); }

void pdcpp:: SynthesizerVoiceContainer::setFrequencyModulator(const pdcpp::Signal& lfo)
    { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setFrequencyModulator(p_Synth, lfo); }

void pdcpp:: SynthesizerVoiceContainer::setAmplitudeModulator(const pdcpp::Signal& lfo)
    { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setAmplitudeModulator(p_Synth, lfo); }

pdcpp::Envelope pdcpp:: SynthesizerVoiceContainer::getEnvelope() const
    { return pdcpp::Envelope(pdcpp::GlobalPlaydateAPI::get()->sound->synth->getEnvelope(p_Synth)); }

void pdcpp:: SynthesizerVoiceContainer::setSample(const AudioSample& sample, uint32_t sustainStart, uint32_t sustainEnd)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->synth->setSample(p_Synth, sample, sustainStart, sustainEnd); }

void pdcpp:: SynthesizerVoiceContainer::setWavetable(const AudioSample& sample, int log2size, int nColumns, int nRows)
{
    auto err = pdcpp::GlobalPlaydateAPI::get()->sound->synth->setWavetable(p_Synth, sample, log2size, nColumns, nRows);

    if (err == 0)
    {
        auto errMsg = "Failed to set wavetable, dimensions don't match the sample size.";
        pdcpp::GlobalPlaydateAPI::get()->system->error(errMsg);
    }
}

pdcpp:: SynthesizerVoiceContainer::operator ::SoundSource*() const
{
    return reinterpret_cast<::SoundSource*>(p_Synth);
}

void pdcpp:: SynthesizerVoiceContainer::setCustomGenerator(pdcpp::CustomSynthGenerator& generator)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->synth->setGenerator(
        p_Synth,
        generator.isStereo(),
        SynthesizerVoiceShims::render,
        SynthesizerVoiceShims::noteOn,
        SynthesizerVoiceShims::release,
        SynthesizerVoiceShims::setParameter,
        SynthesizerVoiceShims::dealloc,
        SynthesizerVoiceShims::copy,
        &generator
    );
}

void pdcpp::SynthesizerVoiceContainer::clearCustomGenerator()
{
    pdcpp::GlobalPlaydateAPI::get()->sound->synth->setGenerator(
            p_Synth,
            true,
            SynthesizerVoiceShims::render,
            SynthesizerVoiceShims::noteOn,
            SynthesizerVoiceShims::release,
            SynthesizerVoiceShims::setParameter,
            SynthesizerVoiceShims::dealloc,
            SynthesizerVoiceShims::copy,
            nullptr
    );
}

pdcpp::SynthesizerVoiceContainer::SynthesizerVoiceContainer(PDSynth* synth)
    : p_Synth(synth)
{
    enableFinishedCallback();
}

void pdcpp::SynthesizerVoiceContainer::playMIDINote(MIDINote note, float vel, float len, uint32_t when)
    { pdcpp::GlobalPlaydateAPI::get()->sound->synth->playMIDINote(p_Synth, note, vel, len, when); }

void pdcpp::SynthesizerVoiceContainer::noteOff(uint32_t when)
    { pdcpp::GlobalPlaydateAPI::get()->sound->synth->noteOff(p_Synth, when); }

void pdcpp::SynthesizerVoiceContainer::setTranspose(float halfSteps)
    { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setTranspose(p_Synth, halfSteps); }

void pdcpp::SynthesizerVoiceContainer::setParameterModulator(int paramNumber, const pdcpp::Signal& mod)
    { pdcpp::GlobalPlaydateAPI::get()->sound->synth->setParameterModulator(p_Synth, paramNumber, mod); }

bool pdcpp::SynthesizerVoiceContainer::setParameter(int paramNumber, float value)
    { return pdcpp::GlobalPlaydateAPI::get()->sound->synth->setParameter(p_Synth, paramNumber, value) != 0; }

void pdcpp::SynthesizerVoiceContainer::clearParameterModulator(int paramNumber)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->synth->setParameterModulator(p_Synth, paramNumber, nullptr);
}

int pdcpp::SynthesizerVoiceContainer::getParameterCount() const { return pdcpp::GlobalPlaydateAPI::get()->sound->synth->getParameterCount(p_Synth); }

pdcpp::SynthesizerVoice::SynthesizerVoice()
    : SynthesizerVoiceContainer(pdcpp::GlobalPlaydateAPI::get()->sound->synth->newSynth())
{}

pdcpp::SynthesizerVoice::SynthesizerVoice(pdcpp::SynthesizerVoice&& other) noexcept
    : SynthesizerVoiceContainer(other.p_Synth)
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
    {
        for (int i = getParameterCount(); --i >= 0;)
            { clearParameterModulator(i); }
        pdcpp::GlobalPlaydateAPI::get()->sound->synth->freeSynth(p_Synth);
    }
}