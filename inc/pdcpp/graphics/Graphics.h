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
        static pdcpp::Rectangle<int> getScreenBounds();

        /**
         * Draws a line from point a to point b with thickness px, and an
         * optional color. Alias for the C API that uses pdcpp::Points.
         *
         * @param a starting point
         * @param b ending point
         * @param px thickness of the line in pixels
         * @param color optional color for the line. default is black.
         */
        static void drawLine(const pdcpp::Point<int>& a, const pdcpp::Point<int>& b, int px, LCDColor color=kColorBlack);

        /**
         * Draws a line forming an ellipse within the bounds of a rectangle.
         *
         * @param rect the bounds of the ellipse.
         * @param lineThickness the thickness of the line.
         * @param startAngle the starting angle from which to draw in degrees.
         *     0 is straight up.
         * @param endAngle the ending angle to which to draw in degrees. Will
         *     draw a complete ellipse if equal to `startAngle`.
         * @param color optional color of for line. default is black.
         */
        static void drawEllipse(const pdcpp::Rectangle<int>& rect, int lineThickness, float startAngle, float endAngle, LCDColor color=kColorBlack);

        /**
         * Fills an ellipse with a color within the bounds of a rectangle.
         *
         * @param rect the bounds of the ellipse.
         * @param startAngle the starting angle from which to fill in degrees.
         *     0 is straight up.
         * @param endAngle the ending angle to which to fill in degrees. Will
         *     fill a complete ellipse if equal to `startAngle`.
         * @param color optional color of for line. default is black.
         */
        static void fillEllipse(const pdcpp::Rectangle<int>& rect, float startAngle, float endAngle, LCDColor color=kColorBlack);

        /**
         * Draws a rectangle with 1-pixel wide lines.
         *
         * @param rect the rectangle to draw.
         * @param color optional color for the lines. default is black.
         */
        static void drawRectangle(const pdcpp::Rectangle<int>& rect, LCDColor color=kColorBlack);

        /**
         * Fills a rectangular area with a color
         *
         * @param rect the rectangle to fill
         * @param color optional color to fill. default is black.
         */
        static void fillRectangle(const pdcpp::Rectangle<int>& rect, LCDColor color=kColorBlack);

        /**
         * Draws a rectangle where each of the corners is rounded over.
         *
         * @param bounds the boundaries of the rectangle
         * @param radius the radius of the roundover in pixels
         * @param linePx the thickeness of the lines in pixels
         * @param color optional color for the lines. default is black.
         */
        static void drawRoundedRectangle(const pdcpp::Rectangle<int>& bounds, int radius, int linePx, LCDColor color=kColorBlack);

        /**
         * Fills a rectangular area where each of the corners is rounded over.
         *
         * @param bounds the boundaries of the rectangle
         * @param radius the radius of the roundover in pixels
         * @param color optional color for the lines. default is black.
         */
        static void fillRoundedRectangle(const pdcpp::Rectangle<int>& bounds, int radius, LCDColor color=kColorBlack);

        /**
         * Fills a triangle with a color
         *
         * @param a first point of the triangle
         * @param b second point of the triangle
         * @param c third point of the triangle
         * @param color the color to fill
         */
        static void fillTriangle(const pdcpp::Point<int>& a, const pdcpp::Point<int>& b, const pdcpp::Point<int>& c, LCDColor color);

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

        /**
         * Sets the end cap style used by line drawing functions.
         *
         * @param endCapStyle the style to use
         */
        static void setLineCapStyle(LCDLineCapStyle endCapStyle);

        /**
         * C++ Alias for the C API so you can avoid having to import the
         * `GlobalPlaydateAPI` just to set the draw mode.
         *
         * @param drawMode the new LCDBitmapDrawMode to use
         */
        static LCDBitmapDrawMode setDrawMode(LCDBitmapDrawMode drawMode);

        /**
         * Sets the area where drawing is permitted within the current graphics
         * context.
         *
         * @param clipRect the area where drawing will be visible.
         */
        static void setClipRect(const pdcpp::Rectangle<int>& clipRect);

        /**
         * Removes any boundaries to drawing.
         */
        static void clearClipRect();
    };
}
