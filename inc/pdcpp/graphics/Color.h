//
// Created by Matt on 12/8/2024.
//

#pragma once
#include <pd_api.h>
#include <array>


namespace pdcpp
{

    class Color
    {
    public:
        Color() = default;
        Color(LCDColor color); // NOLINT(*-explicit-constructor)
        explicit Color(std::array<uint8_t, 16> pattern);
        [[ nodiscard ]] pdcpp::Color inverted() const;
        [[ nodiscard ]] operator LCDColor() const { return reinterpret_cast<LCDColor>(m_Pattern.data()); };  // NOLINT(*-explicit-constructor)

    private:
        std::array<uint8_t, 16> m_Pattern;
    };

} // pdcpp
