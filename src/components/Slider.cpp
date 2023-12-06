//
// Created by Matt on 12/4/2023.
//

#include "pdcpp/components/Slider.h"
#include "pdcpp/core/GlobalPlaydateAPI.h"

void pdcpp::Slider::draw()
{
    auto laf = getLookAndFeel();
    laf->drawSlider(pdcpp::GlobalPlaydateAPI::get()->graphics, getBounds(), m_Min, m_Max, m_CurrentValue);
}

pdcpp::Slider::Slider(float min, float max, float startingValue)
    : m_Min(min)
    , m_Max(max)
    , m_CurrentValue(startingValue)
{}

void pdcpp::Slider::setValue(float newValue)
{
    m_CurrentValue = newValue;
    notifyListeners();
}

float pdcpp::Slider::getValue() const
{
    return m_CurrentValue;
}

void pdcpp::Slider::setMin(float minValue)
{
    m_Min = minValue;
    notifyListeners();
}

void pdcpp::Slider::setMax(float maxValue)
{
    m_Max = maxValue;
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
    for (auto* listener : m_Listeners)
        { listener->sliderValueChanged(this); }
}

pdcpp::Slider::~Slider()
    { m_Listeners.clear(); }
