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
#include <pdcpp/graphics/Color.h>

namespace pdcpp
{
    class Colors
    {
    public:

        static pdcpp::Color transparent50GrayA;
        static pdcpp::Color transparent50GrayB;
        static pdcpp::Color solid50GrayA;
        static pdcpp::Color solid50GrayB;
        static pdcpp::Color sparseCheckerA;

        static pdcpp::Color thickVerticalLinesA;
        static pdcpp::Color verticalLines25A;
        static pdcpp::Color diagonalLinesRightWhiteOnBlack;
        static pdcpp::Color diagonalLinesRightBlackOnWhite;
        static pdcpp::Color black;
        static pdcpp::Color white;
        static pdcpp::Color clear;

        static pdcpp::Color steppedDither0;
        static pdcpp::Color steppedDither1;
        static pdcpp::Color steppedDither2;
        static pdcpp::Color steppedDither3;
        static pdcpp::Color steppedDither4;
        static pdcpp::Color steppedDither5;
        static pdcpp::Color steppedDither6;

        static pdcpp::Color gradient[9];

    };
}
