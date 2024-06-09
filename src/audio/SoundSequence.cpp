/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/29/2023
 *  Original author: MrBZapp
 */
#include <pdcpp/core/GlobalPlaydateAPI.h>
#include <pdcpp/audio/SoundSequence.h>


void pdcpp::SoundSequence::playCompleteShim(::SoundSequence*, void* usrData)
{
    auto thisPtr = reinterpret_cast<pdcpp::SoundSequence*>(usrData);
    thisPtr->m_Callback(thisPtr);
}

pdcpp::SoundSequence::SoundSequence()
    : p_Sequence(pdcpp::GlobalPlaydateAPI::get()->sound->sequence->newSequence())
{}

pdcpp::SoundSequence::SoundSequence(const std::string& midiFile)
    : SoundSequence()
{
    loadMidiFile(midiFile);
}

pdcpp::SoundSequence::~SoundSequence()
{
    if (p_Sequence != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->sequence->freeSequence(p_Sequence); }
}

bool pdcpp::SoundSequence::loadMidiFile(const std::string& filepath)
{
    auto err = pdcpp::GlobalPlaydateAPI::get()->sound->sequence->loadMIDIFile(p_Sequence, filepath.c_str());
    if (err == 0)
    {
        auto msg = "Failed to load " + filepath + ". " + pdcpp::GlobalPlaydateAPI::get()->file->geterr();
        pdcpp::GlobalPlaydateAPI::get()->system->error(msg.c_str());
    }
    return err;
}

uint32_t pdcpp::SoundSequence::getTime()
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->sequence->getTime(p_Sequence);
}

void pdcpp::SoundSequence::setTime(uint32_t time)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->sequence->setTime(p_Sequence, time);
}

void pdcpp::SoundSequence::setLoops(int loopStart, int loopEnd, int loops)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->sequence->setLoops(p_Sequence, loopStart, loopEnd, loops);
}

float pdcpp::SoundSequence::getTempo()
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->sequence->getTempo(p_Sequence);
}

void pdcpp::SoundSequence::setTempo(float stepsPerSecond)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->sequence->setTempo(p_Sequence, stepsPerSecond);
}

int pdcpp::SoundSequence::getTrackCount()
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->sequence->getTrackCount(p_Sequence);
}

void pdcpp::SoundSequence::allNotesOff()
{
    pdcpp::GlobalPlaydateAPI::get()->sound->sequence->allNotesOff(p_Sequence);
}

bool pdcpp::SoundSequence::isPlaying() const
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->sequence->isPlaying(p_Sequence);
}

uint32_t pdcpp::SoundSequence::getLength()
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->sequence->getLength(p_Sequence);
}

void pdcpp::SoundSequence::play(std::function<void(pdcpp::SoundSequence*)> completeCallback)
{
    m_Callback = completeCallback;
    pdcpp::GlobalPlaydateAPI::get()->sound->sequence->play(p_Sequence, playCompleteShim, this);
}

void pdcpp::SoundSequence::stop()
{
    pdcpp::GlobalPlaydateAPI::get()->sound->sequence->stop(p_Sequence);
}

float pdcpp::SoundSequence::getCurrentStep()
{
    int timeOffset;
    float samplesPerBeat = float(getTempo()) / kAudioHardwareSampleRate;
    auto beat = float(pdcpp::GlobalPlaydateAPI::get()->sound->sequence->getCurrentStep(p_Sequence, &timeOffset));
    return beat + (float(timeOffset) / samplesPerBeat);
}

void pdcpp::SoundSequence::setCurrentStep(float beat, bool playNotes)
{
    float samplesPerBeat = float(getTempo()) / kAudioHardwareSampleRate;
    auto step = ::floor(beat);
    auto timeOffset = samplesPerBeat * (beat - step);
    pdcpp::GlobalPlaydateAPI::get()->sound->sequence->setCurrentStep(p_Sequence, step, timeOffset, playNotes);
}

void pdcpp::SoundSequence::setTrackAtIndex(const pdcpp::SequenceTrackContainer& track, unsigned int idx)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->sequence->setTrackAtIndex(p_Sequence, track, idx);
}

pdcpp::SequenceTrackContainer pdcpp::SoundSequence::addTrack(const pdcpp::Synthesizer& instrument)
{
    return pdcpp::SequenceTrackContainer(nullptr);
}

pdcpp::SequenceTrackContainer pdcpp::SoundSequence::getTrackAtIndex(unsigned int track)
{
    return pdcpp::SequenceTrackContainer(nullptr);
}

pdcpp::SoundSequence::SoundSequence(SoundSequence&& other) noexcept
    : m_Callback(std::move(other.m_Callback))
    , p_Sequence(other.p_Sequence)
{
    other.p_Sequence = nullptr;
}

pdcpp::SoundSequence& pdcpp::SoundSequence::operator=(SoundSequence&& other) noexcept
{
    m_Callback = std::move(other.m_Callback);
    p_Sequence = other.p_Sequence;
    other.p_Sequence = nullptr;
    return *this;
}

