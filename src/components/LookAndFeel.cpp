//
// Created by Matt on 12/4/2023.
//
#include <memory>
#include "pdcpp/components/LookAndFeel.h"
#include "pdcpp/components/Colours.h"


static std::unique_ptr<pdcpp::LookAndFeel> defaultLookAndFeel;
//pdcpp::LookAndFeel* pdcpp::LookAndFeel::defaultLookAndFeel = nullptr;

pdcpp::LookAndFeel::LookAndFeel()
    : m_DefaultFont("/System/Fonts/Asheville-Sans-14-Bold.pft")
{}

pdcpp::Font& pdcpp::LookAndFeel::getDefaultFont()
{
    return m_DefaultFont;
}


void pdcpp::LookAndFeel::drawSlider(const playdate_graphics* g, const pdcpp::Rectangle<float>& bounds, float min, float max, float value)
{
    const auto range = max - min;
    const auto ratio = (value - min) / range;
    auto localBounds = bounds;

    const auto startMarker = localBounds.removeFromLeft(2);
    const auto endMarker = localBounds.removeFromRight(2);
    const auto center = localBounds.getCenter();
    const auto middleLine = pdcpp::Rectangle<float>(localBounds.x, center.y - 1, localBounds.width, 2);

    g->fillRect(startMarker.x, startMarker.y, startMarker.width, startMarker.height, kColorBlack);
    g->fillRect(endMarker.x, endMarker.y, endMarker.width, endMarker.height, kColorBlack);
    g->fillRect(middleLine.x, middleLine.y, middleLine.width, middleLine.height, pdcpp::Colours::solid50GrayA);

    // draw the Slider
    int sliderPosition = ratio * localBounds.width;
    int polyPoints[10] = {
        sliderPosition - 2, int(center.y),
        sliderPosition, int(localBounds.y),
        sliderPosition + 2, int(center.y),
        sliderPosition, int(localBounds.y + localBounds.height),
        sliderPosition - 2, int(center.y),
    };
    g->fillPolygon(5, polyPoints, kColorBlack, kPolygonFillNonZero);
}

pdcpp::LookAndFeel* pdcpp::LookAndFeel::getDefaultLookAndFeel()
{
    if (defaultLookAndFeel == nullptr)
        { defaultLookAndFeel = std::make_unique<LookAndFeel>(); }
    return defaultLookAndFeel.get();
}

void pdcpp::LookAndFeel::setDefaultLookAndFeel(pdcpp::LookAndFeel* newLAF)
{
    defaultLookAndFeel.reset(newLAF);
}

void pdcpp::LookAndFeel::setDefaultFont(pdcpp::Font newFont) { m_DefaultFont = newFont; }

