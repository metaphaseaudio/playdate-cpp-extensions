/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/9/2023
 *  Original author: MrBZapp
 */
#pragma once

#include <vector>
#include "Rectangle.h"

namespace pdcpp
{
    class Graphics
    {
    public:
        /**
         * @returns the screen bounds as a pdcpp::Rectangle<float>
         */
        static pdcpp::Rectangle<float> getScreenBounds();

        static void drawLine(const pdcpp::Point<int>& a, const pdcpp::Point<int>& b, int px, LCDColor color=kColorBlack);

        static void drawEllipse(const pdcpp::Rectangle<int>& rect, int lineThickness, float startAngle, float endAngle, LCDColor color=kColorBlack);
        static void fillEllipse(const pdcpp::Rectangle<int>& rect, float startAngle, float endAngle, LCDColor color=kColorBlack);

        static void drawRectangle(const pdcpp::Rectangle<int>& rect, LCDColor color=kColorBlack);
        static void fillRectangle(const pdcpp::Rectangle<int>& rect, LCDColor color=kColorBlack);

        static void drawRoundedRectangle(const pdcpp::Rectangle<int>& bounds, int radius, int linePx, LCDColor color=kColorBlack);
        static void fillRoundedRectangle(const pdcpp::Rectangle<int>& bounds, int radius, LCDColor color=kColorBlack);

        struct Dash
        {
            float length;
            LCDColor color;
        };

        /**
         * Draws a dashed line. This is best used when dashes can't be expressed
         * using LCDColors/Patterns, otherwise they can look a bit clunky.
         *
         * @param start The starting point of the line
         * @param end The ending point of the line
         * @param dashes a vector of Dash objects
         * @param weightPx how wide in px to draw the line
         * @param startDashIndex which dash index to start from
         */
        static void drawDashedLine
            (const Point<int>& start, const Point<int>& end, const std::vector<Dash>& dashes, int weightPx, int startDashIndex=0);

        /**
         * Helper wrapper around the internal `fillPolygon` function. Do
         * remember to duplicate the first point as the last point when drawing
         * a closed polygon.
         *
         * @param points the points of the polygon to draw
         * @param color the color to fill the polygon
         * @param fillRule the fill rule
         */
        static void fillPolygon(const std::vector<pdcpp::Point<float>>& points, LCDColor color, LCDPolygonFillRule fillRule);

        /**
         * Draws the polygon as a series of lines between points.
         *
         * @param points the points of the polygon to draw
         * @param thickness the thickness of the lines
         * @param color the color of the lines
         */
        static void drawPolygon(const std::vector<pdcpp::Point<float>>& points, int thickness, LCDColor color);
    };
}
