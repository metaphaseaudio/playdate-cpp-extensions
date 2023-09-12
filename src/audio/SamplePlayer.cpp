/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/5/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/SamplePlayer.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::SamplePlayer::SamplePlayer()
    : p_SamplePlayer(pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->newPlayer())
{}

pdcpp::SamplePlayer::SamplePlayer(SamplePlayer&& other)
        : p_SamplePlayer(other.p_SamplePlayer)
{ other.p_SamplePlayer = nullptr; }

pdcpp::SamplePlayer& pdcpp::SamplePlayer::operator=(SamplePlayer&& other)
{
    p_SamplePlayer = other.p_SamplePlayer;
    other.p_SamplePlayer = nullptr;
    return *this;
}

pdcpp::SamplePlayer::~SamplePlayer()
    { pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->freePlayer(p_SamplePlayer); }

void pdcpp::SamplePlayer::setSample(const pdcpp::AudioSample& sample)
    { pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->setSample(p_SamplePlayer, sample); }

void pdcpp::SamplePlayer::setPlayRange(int start, int end)
    { pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->setPlayRange(p_SamplePlayer, start, end); }

void pdcpp::SamplePlayer::setPaused(bool shouldPause)
    { pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->setPaused(p_SamplePlayer, shouldPause); }

int pdcpp::SamplePlayer::play(float rate, int repeatCount)
    { return pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->play(p_SamplePlayer, repeatCount, rate); }

void pdcpp::SamplePlayer::stop()
    { pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->stop(p_SamplePlayer); }

void pdcpp::SamplePlayer::setOffset(float offset)
    { pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->setOffset(p_SamplePlayer, offset); }

float pdcpp::SamplePlayer::getOffset() const
    { return pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->getOffset(p_SamplePlayer); }

void pdcpp::SamplePlayer::setRate(float rate)
    { pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->setRate(p_SamplePlayer, rate); }

float pdcpp::SamplePlayer::getRate() const
    { return pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->getRate(p_SamplePlayer); }

float pdcpp::SamplePlayer::getLength()
    { return pdcpp::GlobalPlaydateAPI::get()->sound->sampleplayer->getLength(p_SamplePlayer); }

pdcpp::SamplePlayer::operator ::SoundSource*() const { return reinterpret_cast<::SoundSource*>(p_SamplePlayer); }
