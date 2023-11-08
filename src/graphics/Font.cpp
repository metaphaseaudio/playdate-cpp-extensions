/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/1/2023
 *  Original author: MrBZapp
 */

#include <vector>
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

std::vector<std::string> pdcpp::Font::wrapText(const std::string& text, int maxWidth, PDStringEncoding encoding) const
{
    // Tokenize the string
    std::vector<std::string> words;

    size_t startPos = 0;
    size_t pos = 0;
    while ((pos = text.substr(startPos, std::string::npos).find(' ')) != std::string::npos)
    {
        std::string substr = text.substr(startPos, std::string::npos);
        words.emplace_back(text.substr(startPos, pos));
        startPos += pos + 1;
    }

    // Make the lines
    std::vector<std::string> rv;

    int currentLineWidth = 0;
    std::string recomposed = "";
    for (auto& word : words)
    {
        auto wordWidth = getTextWidth(word + ' ');

        // If adding the word would make the line too long, put it on a new line
        if (currentLineWidth + wordWidth > maxWidth)
        {
            // Avoid whitespace-only lines where sensible
            if (currentLineWidth > 0)
            {
                recomposed += "\n";
                rv.emplace_back(recomposed);
                recomposed = "";
                currentLineWidth = 0;
            }

            // TODO: Split words that are themselves too long.
        }
        recomposed += word + ' ';
        currentLineWidth += wordWidth;
    }
    return rv;
}

int pdcpp::Font::drawWrappedText(const std::string& text, const pdcpp::Rectangle<float>& bounds, PDStringEncoding encoding) const
{
    int lineNum = 0;
    for (auto& line : wrapText(text, bounds.width))
        { drawText(line, bounds.x, bounds.y + (lineNum++ * getFontHeight()), encoding); }
    return lineNum * getFontHeight();
}
