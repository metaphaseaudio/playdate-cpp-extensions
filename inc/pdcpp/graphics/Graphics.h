//
// Created by Matt on 12/9/2023.
//

#pragma once

#include "Rectangle.h"

namespace pdcpp
{
    class Graphics
    {
    public:
        static void drawRoundedRectangle(const pdcpp::Rectangle<int>& bounds, int radius, LCDColor color=kColorBlack);
        static void fillRoundedRectangle(const pdcpp::Rectangle<int>& bounds, int radius, LCDColor color=kColorBlack);

        struct Dash
        {
            float length;
            LCDColor color;
        };

        static void drawDashedLine
            (const Point<int>& start, const Point<int>& end, const Dash* dashes, int nDashes, int weightPx, int startDashIndex=0);

    };
} // pdcpp
