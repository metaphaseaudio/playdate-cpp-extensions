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
#include <cmath>

namespace pdcpp
{
    template <typename T>
    class Point
    {
    public:
        /**
         * Represents a point in two-dimensional space for a given type
         * @param xIn: the x value of the point
         * @param yIn: the y value of the point
         */
        Point(T xIn, T yIn) : x(xIn), y(yIn) {}

        /**
         * calculates the absolute distance between this point and another.
         *
         * @param other: the other point from which distance will be measured
         * @return the absolute distance between this point and the `other`.
         */
        [[ nodiscard ]] float distance(const Point<T>& other) const
        {
            auto a2 = ::powf(::abs(other.x - x), 2);
            auto b2 = ::powf(::abs(other.y - y), 2);
            return ::sqrtf(a2 + b2);
        }

        /**
         * Rotates a point around a center.
         *
         * @param center: the center point of rotation
         * @param radians: how many radians (clockwise) this point will rotate
         * @return A new point, rotated.
         */
        [[ nodiscard ]] Point<T> rotated(const Point<T>& center, float radians) const;

        [[ nodiscard ]] Point<T> withX(T newX) const;
        [[ nodiscard ]] Point<T> withY(T newY) const;

        [[ nodiscard ]] Point<float> toFloat() const { return {float(x), float(y)}; }
        [[ nodiscard ]] Point<int> toInt() const { return {int(x), int(y)}; }

        [[ nodiscard ]] Point<T> abs() const;

        [[ nodiscard ]] Point<T> operator- (Point<T> other) const { return {x - other.x, y - other.y}; }
        [[ nodiscard ]] Point<T> operator+ (Point<T> other) const { return {x + other.x, y + other.y}; }
        [[ nodiscard ]] Point<T> operator* (T value) const { return {x * value, y * value}; }
        [[ nodiscard ]] bool operator== (const Point<T>& other) const { return x == other.x && y == other.y; }
        [[ nodiscard ]] bool operator!= (const Point<T>& other) const { return x != other.x || y != other.y; }

        T x, y;
    };

    template<typename T>
    Point<T> Point<T>::abs() const
    { return Point<T>(T(std::fabs(x)), T(std::fabs(y))); }

    template<typename T>
    Point<T> Point<T>::withX(T newX) const
    {
        return Point<T>(newX, y);
    }

    template<typename T>
    Point<T> Point<T>::withY(T newY) const
    {
        return Point<T>(x, newY);
    }

    template<typename T>
    Point<T> Point<T>::rotated(const Point<T>& center, float radians) const
    {
        float c = ::cosf(radians);
        float s = ::sinf(radians);
        auto p = *this - center;

        return Point<T>(p.x * c - p.y * s, p.x * s + p.y * c) + center;
    }

}