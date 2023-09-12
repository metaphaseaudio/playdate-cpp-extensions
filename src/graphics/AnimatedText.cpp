/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/26/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/graphics/AnimatedText.h>

pdcpp::AnimatedText::AnimatedText(const pdcpp::Font& font, std::string text, unsigned int nFrames, PDStringEncoding encoding)
    : FrameTimer(nFrames)
    , m_NVisible(1)
    , m_Text(std::move(text))
    , r_Font(font)
    , m_Encoding(encoding)
{}

void pdcpp::AnimatedText::update() { tick(); }

void pdcpp::AnimatedText::redraw(PDRect bounds, PDRect drawrect)
{
    const auto newString = std::string(m_Text.begin(), m_Text.begin() + m_NVisible);
    r_Font.drawText(newString, bounds.x, bounds.y, m_Encoding);
}

void pdcpp::AnimatedText::timerCallback()
{
    m_NVisible = std::min<int>(m_NVisible + 1, m_Text.size());
    markDirty();
}

void pdcpp::AnimatedText::reset()
{
    m_NVisible = 0;
    markDirty();
}
