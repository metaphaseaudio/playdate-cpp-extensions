/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/24/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <pd_api.h>
#include "util.h"

namespace pdcpp
{
    /**
     * This class allows for global access to the underlying Playdate C API
     * without having to pass it around to every single object. This class
     * must be initialized in the event handler's init section in order to use
     * most of the classes in this library.
     */
    class GlobalPlaydateAPI
    {
    public:
        /**
         * Returns the current instance, creating a new instance if one doesn't
         * already exist. This should be called in the init section of the
         * `eventShim` and nowhere else.
         *
         * @param pd_api A pointer to the `PlaydateAPI` object
         * @return
         */
        static GlobalPlaydateAPI* initialize(PlaydateAPI* pd_api);

        /**
         * This is the method that you'll probably call the most from this
         * class. Use it whenever you need the C API
         *
         * @return a pointer to the underlying PlaydateAPI object from the C
         *         API.
         */
        inline static PlaydateAPI* get() { return instance->pd; };

        /**
         * Destroys the underlying instance. Must be called in the terminate
         * section of of the `eventShim` and nowhere else.
         */
        static void destroyInstance();

    protected:
        /**
         * The Singleton constructor
         * @param pd_api a pointer to the Playdate C API struct.
         */
        explicit GlobalPlaydateAPI(PlaydateAPI* pd_api);

        static GlobalPlaydateAPI* instance;
        PlaydateAPI* pd;

        PDCPP_DECLARE_NON_COPYABLE_NON_MOVABLE(GlobalPlaydateAPI);
    };
}
