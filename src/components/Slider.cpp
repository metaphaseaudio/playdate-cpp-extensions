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

void pdcpp::Slider::draw()
{
    auto laf = getLookAndFeel();
    laf->drawSlider(pdcpp::GlobalPlaydateAPI::get()->graphics, getBounds(), m_Min, m_Max, m_CurrentValue);
}

pdcpp::Slider::Slider(float min, float max, float startingValue, int nSteps)
    : m_CurrentValue(startingValue)
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
}

float pdcpp::Slider::getValue() const
{
    return m_CurrentValue;
}

void pdcpp::Slider::setMin(float minValue)
{
    assert(minValue < m_Max);
    m_Min = minValue;
    m_CurrentValue = pdcpp::limit(m_Min, m_Max, m_CurrentValue);
    notifyListeners();
}

void pdcpp::Slider::setMax(float maxValue)
{
    assert(maxValue > m_Min);
    m_Max = maxValue;
    m_CurrentValue = pdcpp::limit(m_Min, m_Max, m_CurrentValue);
    notifyListeners();
}

float pdcpp::Slider::getMin() const
{
    return m_Min;
}

float pdcpp::Slider::getMax() const
{
    return m_Max;
}

void pdcpp::Slider::notifyListeners()
{
    onChange(m_CurrentValue);
    for (auto* listener : m_Listeners)
        { listener->sliderValueChanged(this); }
}
