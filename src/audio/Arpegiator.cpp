/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/5/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/Arpeggiator.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::Arpeggiator::Arpeggiator(const std::vector<float>& steps)
    : pdcpp::LFO(pdcpp::GlobalPlaydateAPI::get()->sound->lfo->newLFO(LFOType::kLFOTypeArpeggiator))
{
    pdcpp::GlobalPlaydateAPI::get()->sound->lfo->setType(p_LFO, LFOType::kLFOTypeArpeggiator);
    setSteps(steps);
}

void pdcpp::Arpeggiator::setSteps(const std::vector<float>& steps)
    { pdcpp::GlobalPlaydateAPI::get()->sound->lfo->setArpeggiation(p_LFO, steps.size(), const_cast<float*>(steps.data())); }

