//
// Created by Matt on 11/25/2024.
//

#pragma once
#include <tuple>
#include <pdcpp/graphics/Rectangle.h>


namespace pdcpp
{
    template <typename T>
    struct Border
    {
        T top, bottom, left, right;

        Rectangle<T> subtractFrom (const Rectangle<T>& original) const noexcept
        {
            return
            {
                original.x + left,
                original.y + top,
                original.width - (left + right),
                original.height - (top + bottom)
            };
        }
    };
}
