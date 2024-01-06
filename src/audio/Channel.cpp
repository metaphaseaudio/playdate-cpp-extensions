/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/1/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/Channel.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>


pdcpp::Channel::Channel(bool startAdded)
    : p_Chan(pdcpp::GlobalPlaydateAPI::get()->sound->channel->newChannel())
{
    if (!startAdded)
        { pdcpp::GlobalPlaydateAPI::get()->sound->removeChannel(p_Chan); }
}

pdcpp::Channel::~Channel()
{
    if (p_Chan != nullptr)
    {
        auto pd = pdcpp::GlobalPlaydateAPI::get();
        pd->sound->removeChannel(p_Chan);
        pd->sound->channel->freeChannel(p_Chan);
    }
}

void pdcpp::Channel::addSource(const pdcpp::SoundSource& source)
    { pdcpp::GlobalPlaydateAPI::get()->sound->channel->addSource(p_Chan, source); }

void pdcpp::Channel::removeSource(const pdcpp::SoundSource& source)
    { pdcpp::GlobalPlaydateAPI::get()->sound->channel->removeSource(p_Chan, source); }

void pdcpp::Channel::addEffect(const SoundEffect& effect)
    { pdcpp::GlobalPlaydateAPI::get()->sound->channel->addEffect(p_Chan, effect); }

void pdcpp::Channel::removeEffect(const SoundEffect& effect)
    { pdcpp::GlobalPlaydateAPI::get()->sound->channel->removeEffect(p_Chan, effect); }

void pdcpp::Channel::setVolume(float vol) { pdcpp::GlobalPlaydateAPI::get()->sound->channel->setVolume(p_Chan, vol); }
float pdcpp::Channel::getVolume() const { return pdcpp::GlobalPlaydateAPI::get()->sound->channel->getVolume(p_Chan); }
void pdcpp::Channel::setPan(float pan) { pdcpp::GlobalPlaydateAPI::get()->sound->channel->setPan(p_Chan, pan); }

void pdcpp::Channel::setVolumeModulator(const pdcpp::Signal& mod) { pdcpp::GlobalPlaydateAPI::get()->sound->channel->setVolumeModulator(p_Chan, mod); }

pdcpp::SignalContainer pdcpp::Channel::getVolumeModulator() const
    { return  pdcpp::SignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->channel->getVolumeModulator (p_Chan)); }
void pdcpp::Channel::setPanModulator(const pdcpp::Signal& mod) { pdcpp::GlobalPlaydateAPI::get()->sound->channel->setPanModulator(p_Chan, mod); }

pdcpp::SignalContainer pdcpp::Channel::getPanModulator() const
    { return  pdcpp::SignalContainer(pdcpp::GlobalPlaydateAPI::get()->sound->channel->getPanModulator(p_Chan)); }

pdcpp::Channel::Channel(pdcpp::Channel&& other) noexcept
    : p_Chan(other.p_Chan)
{ other.p_Chan = nullptr; }

pdcpp::Channel& pdcpp::Channel::operator=(pdcpp::Channel&& other) noexcept
{
    p_Chan = other.p_Chan;
    other.p_Chan = nullptr;
    return *this;
}

void pdcpp::Channel::addToSoundEngine()
{
    pdcpp::GlobalPlaydateAPI::get()->sound->addChannel(p_Chan);
}

void pdcpp::Channel::removeFromSoundEngine()
{
    pdcpp::GlobalPlaydateAPI::get()->sound->removeChannel(p_Chan);
}


pdcpp::DefaultChannel::DefaultChannel() { p_Chan = pdcpp::GlobalPlaydateAPI::get()->sound->getDefaultChannel(); }
pdcpp::DefaultChannel::~DefaultChannel() { p_Chan = nullptr; }
