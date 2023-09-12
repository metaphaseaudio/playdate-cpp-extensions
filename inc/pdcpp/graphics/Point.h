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
         Point(T xIn, T yIn) : x(xIn), y(yIn) {}
         [[ nodiscard ]] T getX() const { return x; }
         [[ nodiscard ]] T getY() const { return y; }

    private:
        T x, y;
    };
}