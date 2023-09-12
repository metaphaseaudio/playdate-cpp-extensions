/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/3/2023
 *  Original author: MrBZapp
 */
#include <pdcpp/audio/SoundSource.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>
#include <algorithm>


bool pdcpp::SoundSource::isPlaying() const { return pdcpp::GlobalPlaydateAPI::get()->sound->source->isPlaying(*this); }
void pdcpp::SoundSource::setVolume(float lvol, float rvol) { pdcpp::GlobalPlaydateAPI::get()->sound->source->setVolume(*this, lvol, rvol); }
std::pair<float, float> pdcpp::SoundSource::getVolume() const
{
    float lvol, rvol;
    pdcpp::GlobalPlaydateAPI::get()->sound->source->getVolume(*this, &lvol, &rvol);
    return {lvol, rvol};
}

void pdcpp::SoundSource::setFinishCallback(sndCallbackProc* func) { pdcpp::GlobalPlaydateAPI::get()->sound->source->setFinishCallback(*this, func); }
