/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/5/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <vector>

#include "LFO.h"

namespace pdcpp
{
    class Arpeggiator
        : public pdcpp::LFO
    {
    public:
        /**
         * Creates a new arpeggiator object. Provide an initializer list or
         * vector of floats  where the given values are in half-steps from the
         * center note. For example, the sequence (0, 4, 7, 12) plays the notes
         * of a major chord.
         *
         * @param steps the vector of steps to play
         */
        explicit Arpeggiator(const std::vector<float>& steps);

        /**
         * Sets new steps for the arpeggiator.
         * @param steps  the new steps to use.
         */
        void setSteps(const std::vector<float>& steps);
    };
} // pdcpp
