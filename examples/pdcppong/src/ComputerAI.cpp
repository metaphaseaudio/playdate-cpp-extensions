/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/30/2023
 *  Original author: MrBZapp
 */

#include "ComputerAI.h"

pdcppong::ComputerAI::ComputerAI(pdcpp::Sprite& paddle, const pdcpp::Sprite& ball)
    : r_Paddle(paddle)
    , r_Ball(ball)
{}

void pdcppong::ComputerAI::update()
{
    auto ballPosition = r_Ball.getPosition();
    auto paddlePosition = r_Paddle.getPosition();
    auto ballDiff = ballPosition.getY() - paddlePosition.getY();
    auto delta = std::max(-3.8f, std::min(3.8f, ballDiff));
    r_Paddle.moveByWithCollisions(0, delta);
}
