/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/4/2023
 *  Original author: MrBZapp
 */

#include "pdcpp/audio/Signal.h"
#include <pdcpp/core/GlobalPlaydateAPI.h>

float pdcpp::Signal::getValue() const
{
    return pdcpp::GlobalPlaydateAPI::get()->sound->signal->getValue(reinterpret_cast<PDSynthSignal*>(this->operator ::PDSynthSignalValue *()));
}

void pdcpp::Signal::setValueOffset(float bias)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->signal->setValueOffset(reinterpret_cast<PDSynthSignal*>(this->operator ::PDSynthSignalValue *()), bias);
}

void pdcpp::Signal::setValueScale(float scale)
{
    pdcpp::GlobalPlaydateAPI::get()->sound->signal->setValueScale(reinterpret_cast<PDSynthSignal*>(this->operator ::PDSynthSignalValue *()), scale);
}

float stepShim(void* userdata, int* iosamples, float* ifval)
{
    auto thisPtr = static_cast<pdcpp::CustomSignal*>(userdata);
    return thisPtr->step(iosamples, ifval);
};

void noteOnShim(void* userdata, MIDINote note, float vel, float len)
{
    auto thisPtr = static_cast<pdcpp::CustomSignal*>(userdata);
    thisPtr->noteOn(note, vel, len);
};

void noteOffShim(void* userdata, int stopped, int offset)
{
    auto thisPtr = static_cast<pdcpp::CustomSignal*>(userdata);
    thisPtr->noteOff(stopped, offset);
};

void deallocShim(void*) { /* NO-OP, the destructor handles this */ };


pdcpp::CustomSignal::CustomSignal()
    : p_Signal(pdcpp::GlobalPlaydateAPI::get()->sound->signal->newSignal(stepShim, noteOnShim, noteOffShim, deallocShim, this))
{}

pdcpp::CustomSignal::~CustomSignal()
{
    if (p_Signal != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->sound->signal->freeSignal(p_Signal); }
}
pdcpp::CustomSignal::operator ::PDSynthSignalValue*() const
    { return reinterpret_cast<PDSynthSignalValue*>(p_Signal); }
