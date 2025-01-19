//
// Created by Matt on 1/18/2025.
//
#include "pdcpp/components/LevelMeter.h"
#include "pdcpp/graphics/Graphics.h"
#include "pdcpp/graphics/Colors.h"
#include "pdcpp/core/GlobalPlaydateAPI.h"


void pdcpp::LevelMeter::draw()
{
    auto localBounds = getBounds();
    pdcpp::Graphics::fillRectangle(localBounds.toInt(), pdcpp::Colors::white);
    int height = localBounds.height * (1.0f - (m_CurrentRMS / m_LowerBoundDecibel));

    if (height > 0)
        { pdcpp::Graphics::fillRectangle(localBounds.removeFromBottom(height).toInt(), pdcpp::Colors::solid50GrayA); }
    pdcpp::Graphics::drawRectangle(getBounds().toInt(), pdcpp::Colors::black);
}

void pdcpp::LevelMeter::update(const int16_t* data, int len)
{
    for (int i = len; --i >=0;)
    {
        float sample = float(data[i]) / INT16_MAX;
        m_Sum += sample * sample;
    }
    m_LenOfSum += len;
    if (m_LenOfSum >= kAudioHardwareSampleRate / m_RefreshRate)
    {
        float rms = std::sqrtf(m_Sum / m_LenOfSum);
        m_CurrentRMS = rms > m_LowerBound ? pdcpp::gainToDB(rms) : m_LowerBoundDecibel;
        m_Sum = 0;
        m_LenOfSum = 0;
    }
}

pdcpp::LevelMeter::LevelMeter(int refreshRate, float lowerBound)
    : m_RefreshRate(refreshRate)
    , m_LowerBound(lowerBound)
    , m_LowerBoundDecibel(pdcpp::gainToDB(m_LowerBound))
{}

