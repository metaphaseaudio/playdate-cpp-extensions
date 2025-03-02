//
// Created by Matt on 1/20/2025.
//

#pragma once

#include <ranges>
#include <variant>
#include "Component.h"
#include <pdcpp/core/util.h>
#include <pdcpp/audio/AudioSample.h>

namespace pdcpp
{
    class WaveformViewComponent
        : public Component
    {
    public:
        void setData(pdcpp::RangeOf<std::variant<int16_t, int8_t, float>> auto& data)
        {
            const auto size = std::ranges::size(data);
            const auto bounds = getBounds();
            const auto itrMax = std::min<int>(size, bounds.width);
            const auto nSampsPerPixel = float(size) / itrMax;

            m_Heights.clear();
            m_Heights.reserve(itrMax);

            for (int i = 0; i < size; i += int(nSampsPerPixel))
            {
                auto endI = std::min<int>(size - i, nSampsPerPixel);
                auto begin = std::begin(data) + i;
                auto end = begin + endI;
                float peak = 0;
                for (std::variant<int16_t, int8_t, float> d : std::ranges::subrange(begin, end))
                {
                    peak = std::max(peak, std::abs(std::visit( pdcpp::Overload{
                        [](int16_t x)
                            { return float(x) / INT16_MAX; },
                        [](int8_t x)
                            { return float(x) / INT8_MAX; },
                        [](float x)
                            { return x; }
                    }, d)));
                }
                m_Heights.emplace_back(peak);
            }
        }

    protected:
        void draw() override;

    private:
        std::vector<float> m_Heights{};
    };
}
