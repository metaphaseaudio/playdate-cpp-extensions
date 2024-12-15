//
// Created by Matt on 12/8/2024.
//

#include <pdcpp/graphics/Color.h>
#include <cstring>



namespace pdcpp
{
    Color::Color(std::array<uint8_t, 16> pattern)
        : m_Pattern(pattern)
    {}

    pdcpp::Color Color::inverted() const
    {
        std::array<uint8_t, 16> rv{};
        for (int i = 0; i < 8; i++)
            { rv[i] = ~m_Pattern[i]; }
        std::copy(m_Pattern.begin() + 8, m_Pattern.end(), rv.begin() + 8);
        return pdcpp::Color(rv);
    }

    Color::Color(LCDColor color)
    {
        if (color == kColorClear) { return; }
        else if (color == kColorWhite) { std::memset(m_Pattern.data(), 0xff, m_Pattern.size()); }
        else if (color == kColorBlack) { std::memset(m_Pattern.data() + 8, 0xff, m_Pattern.size() - 8); }
        else { std::memcpy(reinterpret_cast<uint8_t*>(color), m_Pattern.data(), 16);}
    }
} // pdcpp