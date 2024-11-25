/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/4/2023
 *  Original author: MrBZapp
 */

#include <cassert>
#include "pdcpp/components/Slider.h"
#include "pdcpp/core/GlobalPlaydateAPI.h"
#include "pdcpp/graphics/Graphics.h"
#include "pdcpp/graphics/LookAndFeel.h"

void pdcpp::Slider::draw()
{
    m_CachedImage.draw(getBounds().getTopLeft().toInt());
}

pdcpp::Slider::Slider(float min, float max, float startingValue, int nSteps, SliderStyle style)
    : m_Style(style)
    , m_CurrentValue(startingValue)
    , m_Min(min)
    , m_Max(max)
    , m_IncrementSize((m_Max - m_Min) / float(nSteps))
{ onChange = [](float){}; }

pdcpp::Slider::~Slider() { m_Listeners.clear(); }

void pdcpp::Slider::setValue(float newValue, bool notify)
{
    newValue = pdcpp::limit(m_Min, m_Max, newValue);
    m_CurrentValue = newValue;
    if (notify)
        { notifyListeners(); }
    redrawCachedImage();
}

void pdcpp::Slider::setMin(float minValue)
{
    assert(minValue < m_Max);
    m_Min = minValue;
    m_CurrentValue = pdcpp::limit(m_Min, m_Max, m_CurrentValue);
    notifyListeners();
    redrawCachedImage();
}

void pdcpp::Slider::setMax(float maxValue)
{
    assert(maxValue > m_Min);
    m_Max = maxValue;
    m_CurrentValue = pdcpp::limit(m_Min, m_Max, m_CurrentValue);
    notifyListeners();
    redrawCachedImage();
}

void pdcpp::Slider::setStyle(pdcpp::Slider::SliderStyle style)
{
    m_Style = style;
    redrawCachedImage();
}

float pdcpp::Slider::getValue() const { return m_CurrentValue; }
float pdcpp::Slider::getMin() const { return m_Min; }
float pdcpp::Slider::getMax() const { return m_Max; }
pdcpp::Slider::SliderStyle pdcpp::Slider::getStyle() const { return m_Style; }

void pdcpp::Slider::notifyListeners()
{
    onChange(m_CurrentValue);
    for (auto* listener : m_Listeners)
        { listener->sliderValueChanged(this); }
}

void pdcpp::Slider::redrawCachedImage()
{
    auto bounds = getBounds();
    m_CachedImage = pdcpp::Image::drawAsImage(bounds, [&](const playdate_graphics* g){
        auto laf = getLookAndFeel();
        switch (m_Style)
        {
            case Horizontal:
                laf->drawHorizontalSlider(pdcpp::GlobalPlaydateAPI::get()->graphics, this);
                break;
            case Vertical:
                break;
            case Rotary:
                laf->drawRotarySlider(pdcpp::GlobalPlaydateAPI::get()->graphics, this);
                break;
            case Numeric:
                laf->drawNumericSlider(pdcpp::GlobalPlaydateAPI::get()->graphics, this);
                break;
        }
    });
}

void pdcpp::Slider::resized(const pdcpp::Rectangle<float>& newBounds)
{
    redrawCachedImage();
}
