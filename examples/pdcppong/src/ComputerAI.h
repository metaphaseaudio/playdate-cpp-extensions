/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/30/2023
 *  Original author: MrBZapp
 */
#pragma once


#include "pdcpp/graphics/Sprite.h"

namespace pdcppong
{
    class ComputerAI
    {
    public:
        ComputerAI(pdcpp::Sprite& paddle, const pdcpp::Sprite& ball);
        void update();
    private:
        pdcpp::Sprite& r_Paddle;
        const pdcpp::Sprite& r_Ball;
    };
}
