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
#include <pdcpp/audio/SequenceTrack.h>

pdcpp::SequenceTrackContainer::SequenceTrackContainer(::SequenceTrack* track)
    : p_Track(track)
{}

void pdcpp::SequenceTrackContainer::setInstrument(const pdcpp::Synthesizer& inst)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->track->setInstrument(p_Track, inst);
}

PDSynthInstrument* pdcpp::SequenceTrackContainer::getInstrument()
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->track->getInstrument(p_Track);
}

void pdcpp::SequenceTrackContainer::addNoteEvent(uint32_t step, uint32_t len, MIDINote note, float velocity)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->track->addNoteEvent(p_Track, step, len, note, velocity);
}

void pdcpp::SequenceTrackContainer::removeNoteEvent(uint32_t step, MIDINote note)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->track->removeNoteEvent(p_Track, step, note);
}

void pdcpp::SequenceTrackContainer::clearNotes()
{
    pdcpp::GlobalPlaydateAPI::get()->sound->track->clearNotes(p_Track);
}

int pdcpp::SequenceTrackContainer::getControlSignalCount()
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->track->getControlSignalCount(p_Track);
}

pdcpp::ControlSignalContainer pdcpp::SequenceTrackContainer::getControlSignal(int idx)
{
    return pdcpp::ControlSignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->track->getControlSignal(p_Track, idx));
}

pdcpp::ControlSignalContainer pdcpp::SequenceTrackContainer::getSignalForController(int controller, bool create)
{
    return pdcpp::ControlSignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->track->getSignalForController(p_Track, controller, create));
}

void pdcpp::SequenceTrackContainer::clearAllControlSignals()
{
    pdcpp::GlobalPlaydateAPI::get()->sound->track->clearControlEvents(p_Track);
}

int pdcpp::SequenceTrackContainer::getPolyphony()
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->track->getPolyphony(p_Track);
}

int pdcpp::SequenceTrackContainer::getActiveVoiceCount()
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->track->activeVoiceCount(p_Track);
}

void pdcpp::SequenceTrackContainer::setMuted(bool mute)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->track->setMuted(p_Track, mute);
}

uint32_t pdcpp::SequenceTrackContainer::getLength()
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->track->getLength(p_Track);
}

int pdcpp::SequenceTrackContainer::getIndexForStep(uint32_t step)
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->track->getIndexForStep(p_Track, step);
}

int pdcpp::SequenceTrackContainer::getNoteAtIndex(int index, uint32_t* outStep, uint32_t* outLen, MIDINote* outNote, float* outVelocity)
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->track->getNoteAtIndex(p_Track, index, outStep, outLen, outNote, outVelocity);
}

pdcpp::SequenceTrackContainer::operator ::SequenceTrack*() const { return p_Track; }

pdcpp::SequenceTrack::SequenceTrack(const pdcpp::Synthesizer& instrument)
    : pdcpp::SequenceTrackContainer(pdcpp::GlobalPlaydateAPI::get()->sound->track->newTrack())
{ setInstrument(instrument); }

pdcpp::SequenceTrack::~SequenceTrack()
{
    if (p_Track != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->track->freeTrack(p_Track); }
}

pdcpp::SequenceTrack::SequenceTrack(SequenceTrack&& other) noexcept
    : pdcpp::SequenceTrackContainer(other.p_Track)
{
    other.p_Track = nullptr;
}

pdcpp::SequenceTrack& pdcpp::SequenceTrack::operator=(SequenceTrack&& other) noexcept
{
    p_Track = other.p_Track;
    other.p_Track = nullptr;
    return *this;
}
