/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/4/2023
 *  Original author: MrBZapp
 */
#pragma once
#include <pd_api.h>

namespace pdcpp
{
    class Colors
    {
    public:
        static LCDColor transparent50GrayA;
        static LCDColor transparent50GrayB;
        static LCDColor solid50GrayA;
        static LCDColor solid50GrayB;
        static LCDColor sparseCheckerA;

        static LCDColor steppedDither0;
        static LCDColor steppedDither1;
        static LCDColor steppedDither2;
        static LCDColor steppedDither3;
        static LCDColor steppedDither4;
        static LCDColor steppedDither5;
        static LCDColor steppedDither6;

        static LCDColor gradient[9];
    };
}
