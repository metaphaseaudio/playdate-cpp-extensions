//
// Created by Matt on 1/18/2025.
//
#include "pdcpp/components/LevelMeter.h"
#include "pdcpp/graphics/Graphics.h"
#include "pdcpp/graphics/Colors.h"

static const float kMinimumRMSdB =  pdcpp::gainToDB(std::numeric_limits<float>::min());

void pdcpp::LevelMeter::draw()
{
    auto localBounds = getBounds();
    pdcpp::Graphics::fillRectangle(localBounds.toInt(), pdcpp::Colors::white);
    int height = localBounds.height * (1.0f - (m_CurrentRMS / kMinimumRMSdB));

//    int height = localBounds.height * m_CurrentRMS;
    if (height > 0)
        { pdcpp::Graphics::fillRectangle(localBounds.removeFromBottom(height).toInt(), pdcpp::Colors::solid50GrayA); }
    pdcpp::Graphics::drawRectangle(getBounds().toInt(), pdcpp::Colors::black);
}

void pdcpp::LevelMeter::update(const int16_t* data, int len)
{
    float rms = 0;
    for (int i = len; --i >=0;)
    {
        float sample = float(data[i]) / INT16_MAX;
        rms += sample * sample;
    }
//    m_CurrentRMS = std::sqrtf(rms / len);
    m_CurrentRMS = rms > 0 ? pdcpp::gainToDB(std::sqrtf(rms / len)) : kMinimumRMSdB;
}
