/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/26/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/core/Random.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::Random::Random()
    : seed(pdcpp::GlobalPlaydateAPI::get()->system->getCurrentTimeMilliseconds())
{}

pdcpp::Random::Random(unsigned int user_seed)
    : seed(user_seed)
{}

unsigned int pdcpp::Random::next()
{
    srand(seed);
    seed = rand();
    return seed;
}

float pdcpp::Random::nextFloatInRange(float minValue, float maxValue)
{
    auto range = maxValue - minValue;
    return (float(next()) / float(RAND_MAX)) * range + minValue;
}
