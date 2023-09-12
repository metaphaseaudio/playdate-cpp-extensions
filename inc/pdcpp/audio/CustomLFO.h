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
#include "LFO.h"

namespace pdcpp
{
    class CustomLFO
        : public pdcpp::LFO
    {
    public:
        /**
         * Base class for a custom LFO. This is a simplified version of the
         * CustomSignal class and leverages portions of the C API specific for
         * custom LFOs.
         *
         * @param wantsInterpolation enables samplewise linear interpolation of
         *    the LFO's values
         */
        explicit CustomLFO(bool wantsInterpolation);

        /**
         * Pure virtual. must return the next value of the LFO when called
         *
         * @return the next value of this LFO
         */
        virtual float nextValue() = 0;
    };
}
