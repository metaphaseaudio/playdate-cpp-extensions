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
        void update(const int16_t* data, int len);

    protected:
        void draw() override;
        float m_CurrentRMS;
    };
}
