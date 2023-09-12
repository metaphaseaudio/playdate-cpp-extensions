/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/28/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <pd_api.h>

namespace pdcpp
{
    class RectHelpers
    {
    public:
        /**
         * @param x, y the two PDRects from which to calculate an overlap
         * @returns a PDRect of the overlapping area.
         */
        static PDRect getOverlappingRect(const PDRect& x, const PDRect& y);
    };
}
