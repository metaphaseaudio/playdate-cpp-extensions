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
#include "pdcpp/graphics/Image.h"

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

std::vector<std::string> pdcpp::Font::wrapText(const std::string& text, int maxWidth) const
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

    if (startPos < text.size()) { words.emplace_back(text.substr(startPos, std::string::npos)); }

    // Make the lines
    std::vector<std::string> rv;

    int currentLineWidth = 0;
    std::string recomposed = "";

    auto make_new_line = [&]()
    {
        recomposed += "\n";
        rv.emplace_back(recomposed);
        recomposed = "";
        currentLineWidth = 0;
    };

    auto process_word = [&](const std::string& word, bool includeSpace)
    {
        auto wordWidth = getTextWidth(word + (includeSpace ? " " : ""));

        if (currentLineWidth + wordWidth > maxWidth)
        {
            // Avoid whitespace-only lines where sensible
            if (currentLineWidth > 0) { make_new_line(); }

            // TODO: Split words that are themselves too long.
        }
        recomposed += word + (includeSpace ? " " : "");
        currentLineWidth += wordWidth;
    };

    for (auto& word : words)
    {
        startPos = 0;
        while ((pos = word.substr(startPos, std::string::npos).find('\n')) != std::string::npos)
        {
            auto sub_word = word.substr(startPos, pos);
            process_word(sub_word, false);
            make_new_line();
            startPos += pos + 1;
        }

        process_word(word.substr(startPos, std::string::npos), word != words.back());
    }
    if (!recomposed.empty()) { rv.emplace_back(recomposed);}
    return rv;
}

int pdcpp::Font::drawWrappedText(const std::string& text, const pdcpp::Rectangle<float>& bounds, PDStringEncoding encoding) const
{
    int lineNum = 0;
    for (auto& line : wrapText(text, bounds.width))
        { drawText(line, bounds.x, bounds.y + (lineNum++ * getFontHeight()), encoding); }
    return lineNum * getFontHeight();
}

void pdcpp::Font::drawWrappedText
(const std::string& text, pdcpp::Rectangle<float> bounds, pdcpp::Font::Justification justification, PDStringEncoding encoding) const
{
    int lineNum = 0;
    for (auto& line : wrapText(text, bounds.toInt().width))
    {
        const auto lineBounds = pdcpp::Rectangle<float>(0, 0, getTextWidth(line, encoding), getFontHeight());
        pdcpp::Point<float> point{0, 0};

        switch (justification)
        {
            case Left:
                point = bounds.getTopLeft();
                break;
            case Center:
                point = bounds.getTopLeft() + pdcpp::Point<float>((bounds.width - lineBounds.width) / 2.0f, 0);
                break;
            case Right:
                point = bounds.getTopLeft() + pdcpp::Point<float>(bounds.width - lineBounds.width, 0);
                break;
        }

        drawText(line, point.x, point.y + (lineNum++ * getFontHeight()), encoding);
    }
}

pdcpp::Image pdcpp::Font::getGlyphImage(uint32_t c)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    auto page = pd->graphics->getFontPage(m_Font, c);
    LCDBitmap* glyphImgPtr;
    int adv;
    pd->graphics->getPageGlyph(page, c, &glyphImgPtr, &adv);
    return pdcpp::Image::copyFromPointer(glyphImgPtr);
}
