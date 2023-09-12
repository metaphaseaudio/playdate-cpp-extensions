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
#include <pd_api.h>


namespace pdcpp
{
    class Font
    {
    public:
        /**
         * Loads a font from a given path, and provides access to basic drawing
         * functions.
         *
         * @param fontPath the path to the font to load
         * @param tracking optional tracking (pixels between letters) to use.
         *     default is 0.
         * @param tracking optional leading (pixels between lines) to use.
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
         * Draws the given text within the given bounds, wrapping the text on
         * words.
         *
         * @param text the text to draw with this font
         * @param x the x coordinate of the upper left corner of the text
         * @param y the y coordinate of the upper left corner of the text
         * @param encoding the encoding type of the string. Default is ASCII
         */
        void drawWrappedText(const std::string& text, PDRect bounds, PDStringEncoding encoding=PDStringEncoding::kASCIIEncoding);

    private:
        int m_Tracking, m_Leading;
        LCDFont* m_Font;
    };
}
