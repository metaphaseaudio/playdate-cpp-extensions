/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/4/2023
 *  Original author: MrBZapp
 */

#include <memory>
#include "pdcpp/graphics/LookAndFeel.h"
#include "pdcpp/graphics/Colors.h"
#include "pdcpp/graphics/Graphics.h"
#include "pdcpp/components/Slider.h"

pdcpp::LookAndFeel* pdcpp::LookAndFeel::defaultLookAndFeel = nullptr;
std::map<std::string, pdcpp::Font> pdcpp::LookAndFeel::g_Fonts = {};

pdcpp::LookAndFeel::LookAndFeel()
    : m_DefaultFont("/System/Fonts/Asheville-Sans-14-Bold.pft")
{}

pdcpp::LookAndFeel* pdcpp::LookAndFeel::getDefaultLookAndFeel()
{
    if (defaultLookAndFeel == nullptr)
        { defaultLookAndFeel = new LookAndFeel(); }
    return defaultLookAndFeel;
}

void pdcpp::LookAndFeel::setDefaultLookAndFeel(pdcpp::LookAndFeel* newLAF)
{
    delete defaultLookAndFeel;
    defaultLookAndFeel = newLAF;
}

void pdcpp::LookAndFeel::setDefaultFont(pdcpp::Font newFont) { m_DefaultFont = newFont; }

pdcpp::Font& pdcpp::LookAndFeel::getDefaultFont() { return m_DefaultFont; }
const pdcpp::Font& pdcpp::LookAndFeel::getDefaultFont() const { return m_DefaultFont; }

void pdcpp::LookAndFeel::drawHorizontalSlider(const playdate_graphics* g, const pdcpp::Slider* slider) const
{
    const auto range = slider->getMax() - slider->getMin();
    const auto ratio = (slider->getValue() - slider->getMin()) / range;
    auto localBounds = slider->getLocalBounds();

    const auto startMarker = localBounds.removeFromLeft(2);
    const auto endMarker = localBounds.removeFromRight(2);
    const auto center = localBounds.getCenter();
    const auto middleLine = pdcpp::Rectangle<float>(localBounds.x, center.y - 1, localBounds.width, 2);

    g->fillRect(startMarker.x, startMarker.y, startMarker.width, startMarker.height, kColorBlack);
    g->fillRect(endMarker.x, endMarker.y, endMarker.width, endMarker.height, kColorBlack);
    g->fillRect(middleLine.x, middleLine.y, middleLine.width, middleLine.height, pdcpp::Colors::solid50GrayA);

    // draw the Slider
    int sliderPosition = ratio * localBounds.width + localBounds.x;
    int polyPoints[10] = {
        sliderPosition - 3, int(center.y),
        sliderPosition, int(localBounds.y),
        sliderPosition + 3, int(center.y),
        sliderPosition, int(localBounds.y + localBounds.height),
        sliderPosition - 3, int(center.y),
    };
    g->fillPolygon(5, polyPoints, kColorBlack, kPolygonFillNonZero);
}

void pdcpp::LookAndFeel::drawRotarySlider(const playdate_graphics* g, const pdcpp::Slider* slider) const
{
    auto bounds = slider->getLocalBounds();
    const auto range = slider->getMax() - slider->getMin();
    const auto ratio = (slider->getValue() - slider->getMin()) / range;
    pdcpp::Graphics::fillEllipse(bounds.toInt(), 0, 0, kColorBlack);

    // Draw the indicator pip
    auto pipBounds = pdcpp::Rectangle<int>(0,0, bounds.width * 0.2f, bounds.width * 0.2f);
    const auto center = bounds.getCenter();
    const auto pipPointAtHalf = pdcpp::Point<float>(center.x, center.y - bounds.reduced(4).height / 2.0f);
    pipBounds = pipBounds.withCenter(pipPointAtHalf.rotated(center, pdcpp::degToRad(288 * ratio - 143)).toInt());
    pdcpp::Graphics::fillEllipse(pipBounds, 0, 0, kColorWhite);
}

pdcpp::Font* pdcpp::LookAndFeel::getFont(const std::string& fontName)
{
    if (!g_Fonts.contains(fontName))
    {
        g_Fonts.emplace(std::piecewise_construct, std::forward_as_tuple(fontName), std::forward_as_tuple(fontName));
    }
    return &g_Fonts.at(fontName);
}

void pdcpp::LookAndFeel::drawNumericSlider(const playdate_graphics* g, const pdcpp::Slider* slider) const
{
    auto bounds = slider->getLocalBounds();
    pdcpp::Graphics::fillRectangle(bounds.toInt(), kColorWhite);
    pdcpp::Graphics::drawRectangle(bounds.toInt(), kColorBlack);

    auto font = getDefaultFont();
    auto yOffset = (bounds.height - font.getFontHeight()) / 2.0f;
    auto value = pdcpp::to_string_with_precision(slider->getValue(), 1);
    font.drawWrappedText(value, bounds.withOrigin({0, yOffset}), pdcpp::Font::Justification::Right);
}
