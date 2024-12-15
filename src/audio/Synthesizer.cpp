/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/1/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/Synthesizer.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::Synthesizer::Synthesizer()
    : p_Instrument(pdcpp::GlobalPlaydateAPI::get()->sound->instrument->newInstrument())
{}

pdcpp::Synthesizer::Synthesizer(pdcpp::Synthesizer&& other) noexcept
    : p_Instrument(other.p_Instrument)
{ other.p_Instrument = nullptr; }

pdcpp::Synthesizer& pdcpp::Synthesizer::operator=(pdcpp::Synthesizer&& other) noexcept
{
    p_Instrument = other.p_Instrument;
    other.p_Instrument = nullptr;
    return *this;
}

pdcpp::Synthesizer::~Synthesizer()
{
    if (p_Instrument != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->instrument->freeInstrument(p_Instrument); }
}

void pdcpp::Synthesizer::addVoice(const pdcpp::SynthesizerVoice& voice, MIDINote lowNote, MIDINote hiNote, float transpose)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    auto err = pd->sound->instrument->addVoice(p_Instrument, voice, lowNote, hiNote, transpose);
    if (err == 0)
        { pd->system->error("Failed to add voice to instrument"); }
}
pdcpp::SynthesizerVoiceContainer pdcpp::Synthesizer::noteOn(MIDINote note, float vel, float len, uint32_t when)
    { return pdcpp::SynthesizerVoiceContainer(pdcpp::GlobalPlaydateAPI::get()->sound->instrument->playMIDINote(p_Instrument, note, vel, len, when)); }

pdcpp::SynthesizerVoiceContainer pdcpp::Synthesizer::noteOn(MIDINote note, float vel, uint32_t when)
    { return noteOn(note, vel, -1, when); }

void pdcpp::Synthesizer::noteOff(MIDINote note, uint32_t when)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    // This is a bug. if `when` is 0, it should turn off *right now* but it
    // doesn't, so we'll fake it for the caller.
    if (when == 0)
        { when = pd->sound->getCurrentTime(); }
    pd->sound->instrument->noteOff(p_Instrument, note, when);
}

void pdcpp::Synthesizer::allNotesOff(uint32_t when)
    { pdcpp::GlobalPlaydateAPI::get()->sound->instrument->allNotesOff(p_Instrument, when); }

void pdcpp::Synthesizer::setPitchBend(float bend)
    { pdcpp::GlobalPlaydateAPI::get()->sound->instrument->setPitchBend(p_Instrument, bend); }

void pdcpp::Synthesizer::setPitchBendRange(float halfSteps)
    { pdcpp::GlobalPlaydateAPI::get()->sound->instrument->setPitchBendRange(p_Instrument, halfSteps); }

void pdcpp::Synthesizer::setTranspose(float halfSteps)
    { pdcpp::GlobalPlaydateAPI::get()->sound->instrument->setTranspose(p_Instrument, halfSteps); }

int pdcpp::Synthesizer::getActiveVoiceCount() const
    { return pdcpp::GlobalPlaydateAPI::get()->sound->instrument->activeVoiceCount(p_Instrument); }

pdcpp::Synthesizer::operator ::PDSynthInstrument*() const { return p_Instrument; }
pdcpp::Synthesizer::operator ::SoundSource*() const{ return reinterpret_cast<::SoundSource*>(p_Instrument); }
