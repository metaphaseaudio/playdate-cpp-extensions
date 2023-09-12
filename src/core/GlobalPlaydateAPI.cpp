/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/24/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::GlobalPlaydateAPI* pdcpp::GlobalPlaydateAPI::instance = nullptr;

pdcpp::GlobalPlaydateAPI::GlobalPlaydateAPI(PlaydateAPI* pd_api)
    : pd(pd_api)
{}

pdcpp::GlobalPlaydateAPI* pdcpp::GlobalPlaydateAPI::initialize(PlaydateAPI* pd_api)
{
    if (instance == nullptr)
        { instance = new GlobalPlaydateAPI(pd_api); }

    return instance;
}

void pdcpp::GlobalPlaydateAPI::destroyInstance() { delete instance; }

