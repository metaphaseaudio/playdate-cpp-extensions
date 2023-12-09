//
// Created by Matt on 12/4/2023.
//

#include <cassert>
#include "pdcpp/components/Slider.h"
#include "pdcpp/core/GlobalPlaydateAPI.h"

void pdcpp::Slider::draw()
{
    auto laf = getLookAndFeel();
    laf->drawSlider(pdcpp::GlobalPlaydateAPI::get()->graphics, getBounds(), m_Min, m_Max, m_CurrentValue);
}

pdcpp::Slider::Slider(float min, float max, float startingValue, int nSteps)
    : m_Min(min)
    , m_Max(max)
    , m_CurrentValue(startingValue)
    , m_IncrementSize((m_Max - m_Min) / float(nSteps))
{ onChange = [](float){}; }

void pdcpp::Slider::setValue(float newValue)
{
    newValue = pdcpp::limit(m_Min, m_Max, newValue);
    m_CurrentValue = newValue;
    notifyListeners();
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

pdcpp::Slider::~Slider()
    { m_Listeners.clear(); }
