//
// Created by Matt on 1/18/2025.
//

#pragma once
#include <cstdint>
#include <pdcpp/components/Component.h>

namespace pdcpp
{
    class LevelMeter
        : public Component
    {
    public:
        explicit LevelMeter(int refreshRate, float lowerBound=0.002f);
        void update(const int16_t* data, int len);

    protected:
        void draw() override;

        float m_RefreshRate, m_LowerBound, m_LowerBoundDecibel, m_Sum{0},
            m_LenOfSum{0}, m_CurrentRMS{0}, m_CurrentPeak{0}, m_RunningPeak{0};
    };
}
