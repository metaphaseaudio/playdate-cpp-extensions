/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/26/2023
 *  Original author: MrBZapp
 */

#pragma once
#include "Font.h"
#include "Sprite.h"
#include <pdcpp/graphics/Rectangle.h>
#include <pdcpp/core/Timer.h>

namespace pdcpp
{
    class AnimatedText
        : public Sprite
        , FrameTimer
    {
    public:
        /**
         * Animates text onto the screen over time. Useful for dialogue boxes.
         *
         * @param font The font to use for the text
         * @param text the text to animate
         * @param updateFrames how many frames should elapse before revealing a
         *     new character in the text.
         * @param encoding optional encoding for the text. default is ASCII.
         */
        AnimatedText(const Font& font, std::string text, unsigned int updateFrames, PDStringEncoding encoding=kASCIIEncoding);

        // Overridden virtual method
        void update() override;

        // Overridden virtual method
        void redraw(const pdcpp::Rectangle<float>& bounds, const pdcpp::Rectangle<float>& drawrect) override;

        // resets the visible text to 0
        void reset();

    private:
        // Overridden virtual method
        void timerCallback() override;

        int m_NVisible;
        const std::string m_Text;
        const Font& r_Font;
        const PDStringEncoding m_Encoding;
    };
}
