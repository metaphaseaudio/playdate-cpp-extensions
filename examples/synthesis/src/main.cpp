/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/29/2023
 *  Original author: MrBZapp
 */
#include <memory>

#include <pd_api.h>
#include <pdcpp/pdnewlib.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>
#include "Synthesis.h"

std::unique_ptr<SynthesisObject> game;


static int update(void* userdata)
{
    return game->update();
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
    eventHandler_pdnewlib(pd, event, arg);
    if (event == kEventInit)
    {
        pdcpp::GlobalPlaydateAPI::initialize(pd);
        game = std::make_unique<SynthesisObject>();
        pd->system->setUpdateCallback(update, pd);
    }

    // Destroy the global state to prevent memory leaks
    if (event == kEventTerminate)
    {
        game = nullptr;
        pdcpp::GlobalPlaydateAPI::destroyInstance();
    }
    return 0;
}
#ifdef __cplusplus
}
#endif
