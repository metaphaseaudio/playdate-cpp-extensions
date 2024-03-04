/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/1/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <string>
#include <vector>
#include <pd_api.h>
#include <pdcpp/graphics/Rectangle.h>
#include <pdcpp/graphics/Image.h>


namespace pdcpp
{
    class Font
    {
    public:
        enum Justification
        {
            Left,
            Center,
            Right
        };

        /**
         * Loads a font from a given path, and provides access to basic drawing
         * functions.
         *
         * @param fontPath the path to the font to load
         * @param tracking optional tracking (pixels between letters) to use.
         *     default is 0.
         */
        explicit Font(const std::string& fontPath, int tracking=0, int leading=0);

        /**
         * @return the height of the given font in pixels.
         */
        [[ nodiscard ]] uint8_t getFontHeight() const;

        /**
         * Measures how many pixels in width a given string would occupy if
         * drawn with this font. Use with `getFontHeight` to help determine the
         * full area of a string
         *
         * @param toMeasure the string to measure
         * @param encoding the encoding of the string. default is ASCII.
         * @return the width of the string in pixels
         */
        [[ nodiscard ]] int getTextWidth(const std::string& toMeasure, PDStringEncoding encoding=kASCIIEncoding) const;

        void setTextTracking(int px) {m_Tracking = px; };
        void setTextLeading(int px) { m_Leading = px; };
        [[ nodiscard ]] int getTextTracking() const { return m_Tracking; };
        [[ nodiscard ]] int getTextLeading() const { return m_Leading; }

        /**
         * Draws the given text at the specified position.
         *
         * @param text the text to draw with this font
         * @param x the x coordinate of the upper left corner of the text
         * @param y the y coordinate of the upper left corner of the text
         * @param encoding the encoding type of the string. Default is ASCII
         */
        void drawText(const std::string& text, int x, int y, PDStringEncoding encoding=PDStringEncoding::kASCIIEncoding) const;

        /**
         * Takes a piece of text as a string, and returns a vector of lines
         * which fit the maximum width. Use this to pre-wrap text and calculate
         * the required height of wrapped text block.
         *
         * @param text The text to wrap
         * @param maxWidth the maximum width in pixels to which the words should
         *     be limited and wrapped.
         * @param encoding optional encoding of the string. default ASCII
         * @return a vector of strings for each line of the wrapped text.
         */
        [[nodiscard]] std::vector<std::string> wrapText(const std::string& text, int maxWidth) const;

        /**
         * Draws the given text within the given bounds, wrapping the text on
         * words.
         *
         * @param text the text to draw with this font
         * @param bounds a rectangle which specifies the point at which the text
         *    should be drawn, and the width at which to limit each wrapped line
         * @param encoding the encoding type of the string. Default is ASCII
         * @returns the height of the resulting text block
         */
        [[nodiscard]] int drawWrappedText(const std::string& text, const Rectangle<float>& bounds, PDStringEncoding encoding=kASCIIEncoding) const;

        /**
         * overloaded version of `drawWrappedText` which allows the text to be
         * justified within the bounds
         *
         * @param text the text to draw
         * @param bounds the bounds in which to draw the text
         * @param justification justification of the text, left/center/right
         * @param encoding optional encoding for the text. default is ASCII
         */
        [[ nodiscard ]] int drawWrappedText(
                const std::string& text,
                pdcpp::Rectangle<float> bounds,
                pdcpp::Font::Justification justification,
                PDStringEncoding encoding=kASCIIEncoding) const;

        /**
         * Get an individual character as an image.
         *
         * Be careful with this one! It works, but only on fonts that have an
         * image table. Many do, but it's not a requirement, so it's going to
         * always be safer to draw the text into an image instead.
         *
         * @param c the character of which to retrieve the image
         * @return the image of the character
         */
        pdcpp::Image getGlyphImage(uint32_t c);

    private:
        int m_Tracking, m_Leading;
        LCDFont* m_Font;
    };
}
