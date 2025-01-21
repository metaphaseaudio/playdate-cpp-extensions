//
// Created by Matt on 1/18/2025.
//
#include <cmath>
#include "pdcpp/components/LevelMeter.h"
#include "pdcpp/graphics/Graphics.h"
#include "pdcpp/graphics/Colors.h"
#include "pdcpp/core/GlobalPlaydateAPI.h"


void pdcpp::LevelMeter::draw()
{
    pdcpp::Graphics::fillRectangle(getBounds().toInt(), pdcpp::Colors::white);
    int rmsHeight = getBounds().height * (1.0f - (m_CurrentRMS / m_LowerBoundDecibel));
    int peakHeight = getBounds().height * (1.0f - (m_CurrentPeak / m_LowerBoundDecibel));

    if (peakHeight > 0)
        { pdcpp::Graphics::fillRectangle(getBounds().removeFromBottom(peakHeight).toInt(), pdcpp::Colors::solid50GrayA); }
    if (rmsHeight > 0)
        { pdcpp::Graphics::fillRectangle(getBounds().removeFromBottom(rmsHeight).toInt(), pdcpp::Colors::black); }

    pdcpp::Graphics::drawRectangle(getBounds().toInt(), pdcpp::Colors::black);
}

void pdcpp::LevelMeter::update(const int16_t* data, int len)
{
    for (int i = len; --i >=0;)
    {
        float sample = float(data[i]) / INT16_MAX;
        m_RunningPeak = std::max(std::abs(sample), m_RunningPeak);
        m_Sum += sample * sample;
    }
    m_LenOfSum += len;
    if (m_LenOfSum >= kAudioHardwareSampleRate / m_RefreshRate)
    {
        float rms = sqrtf(m_Sum / m_LenOfSum);
        m_CurrentPeak = pdcpp::gainToDB(m_RunningPeak);
        m_RunningPeak = 0;
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

