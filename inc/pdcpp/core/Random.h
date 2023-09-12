/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/26/2023
 *  Original author: MrBZapp
 */

#pragma once

namespace pdcpp
{
    /**
     * A Simple Random number generator that can isolate instances of the C
     * standard library's `rand` and `srand` functions. Limited randomness.
     */
    class Random
    {
    public:
        /**
         * Make a new random number generator. Will seed from the current
         * device time.
         */
        Random();

        /**
         * Make a new random number generator with an explicit seed.
         * @param seed the new seed to use
         */
        explicit Random(unsigned int seed);

        /**
         * Generate a new random number
         * @return an unsigned random integer
         */
        unsigned int next();

        /**
         * Generates a random floating-point number between a minimum and
         * maximum value
         *
         * @param minValue the minimum value of the number
         * @param maxValue the maximum value of the number
         * @return a random float within the range
         */
        float nextFloatInRange(float minValue, float maxValue);

    private:
        unsigned int seed;
    };

}
