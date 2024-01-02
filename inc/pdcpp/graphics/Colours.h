//
// Created by Matt on 12/4/2023.
//

#pragma once
#include <pd_api.h>

namespace pdcpp
{
    class Colours
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
