/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/1/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/graphics/Font.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::Font::Font(const std::string& fontPath, int tracking, int leading)
    : m_Tracking(tracking)
    , m_Leading(leading)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    const char* err;
    m_Font = pd->graphics->loadFont(fontPath.c_str(), &err);

    if (m_Font == nullptr)
        { pd->system->error("Couldn't load font %s: %s", fontPath.c_str(), err); }
}

void pdcpp::Font::drawText(const std::string& text, int x, int y, PDStringEncoding encoding) const
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    pd->graphics->setFont(m_Font);
    pd->graphics->setTextLeading(m_Leading);
    pd->graphics->setTextTracking(m_Tracking);
    pd->graphics->drawText(text.c_str(), text.length(), encoding, x, y);
}

uint8_t pdcpp::Font::getFontHeight() const
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    return pd->graphics->getFontHeight(m_Font);
}

int pdcpp::Font::getTextWidth(const std::string& toMeasure,  PDStringEncoding encoding) const
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    return pd->graphics->getTextWidth(m_Font, toMeasure.data(), toMeasure.size(), encoding, m_Tracking);
}

void pdcpp::Font::drawWrappedText(const std::string& text, PDRect bounds, PDStringEncoding encoding)
{
    const auto totalWidth = getTextWidth(text, encoding);
}
