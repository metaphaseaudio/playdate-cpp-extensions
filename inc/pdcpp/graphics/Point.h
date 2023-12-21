/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/16/2023
 *  Original author: MrBZapp
 */

#pragma once

namespace pdcpp
{
    template <typename T>
    class Point
    {
    public:
        [[ nodiscard ]] float distance(const Point<T>& other) const
        {
            auto a2 = ::powf(::abs(other.x - x), 2);
            auto b2 = ::powf(::abs(other.y - y), 2);
            return ::sqrtf(a2 + b2);
        }

        [[ nodiscard ]] Point<float> toFloat() const { return {float(x), float(y)}; }
        [[ nodiscard ]] Point<int> toInt() const { return {int(x), int(y)}; }

        [[ nodiscard ]] Point<T> operator- (Point<T> other) const { return {x - other.x, y - other.y}; }
        [[ nodiscard ]] Point<T> operator+ (Point<T> other) const { return {x + other.x, y + other.y}; }

        [[ nodiscard ]] Point<T> operator* (T value) const
        {
            return {x * value, y * value};
        }

        T x, y;
    };
}