/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/5/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/audio/CustomLFO.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>


float lfoCShimFunction(PDSynthLFO*, void* userData)
{
    auto thisPtr = reinterpret_cast<pdcpp::CustomLFO*>(userData);
    return thisPtr->nextValue();
}

pdcpp::CustomLFO::CustomLFO(bool wantsInterpolation)
    : pdcpp::LFO(pdcpp::GlobalPlaydateAPI::get()->sound->lfo->newLFO(LFOType::kLFOTypeFunction))
{
    pdcpp::GlobalPlaydateAPI::get()->sound->lfo->setFunction(p_LFO, lfoCShimFunction, this, wantsInterpolation);
}
