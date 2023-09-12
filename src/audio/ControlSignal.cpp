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
#include <pdcpp/audio/ControlSignal.h>

pdcpp::ControlSignalContainer::ControlSignalContainer(::ControlSignal* signal)
    : p_Signal(signal)
{}

void pdcpp::ControlSignalContainer::clearEvents()
{
    pdcpp::GlobalPlaydateAPI::get()->sound->controlsignal->clearEvents(p_Signal);
}

void pdcpp::ControlSignalContainer::addEvent(int step, float value, bool interpolate)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->controlsignal->addEvent(p_Signal, step, value, interpolate);
}

void pdcpp::ControlSignalContainer::removeEvent(int step)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->controlsignal->removeEvent(p_Signal, step);
}

int pdcpp::ControlSignalContainer::getMIDIControllerNumber()
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->controlsignal->getMIDIControllerNumber(p_Signal);
}

pdcpp::ControlSignalContainer::operator ::PDSynthSignalValue*() const
{
    return reinterpret_cast<::PDSynthSignalValue*>(p_Signal);
}

pdcpp::ControlSignal::ControlSignal()
    : pdcpp::ControlSignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->controlsignal->newSignal())
{}

pdcpp::ControlSignal::~ControlSignal()
{
    if (p_Signal != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->controlsignal->freeSignal(p_Signal); }
}

pdcpp::ControlSignal::ControlSignal(ControlSignal&& other) noexcept
    : pdcpp::ControlSignalContainer(other.p_Signal)
{
    other.p_Signal = nullptr;
}

pdcpp::ControlSignal& pdcpp::ControlSignal::operator=(ControlSignal&& other) noexcept
{
    p_Signal = other.p_Signal;
    other.p_Signal = nullptr;
    return *this;
}
