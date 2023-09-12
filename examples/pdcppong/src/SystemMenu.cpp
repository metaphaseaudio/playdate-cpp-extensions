/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/28/2023
 *  Original author: MrBZapp
 */

#include "SystemMenu.h"

pdcppong::SystemMenu::SystemMenu(pdcppong::Ball& ball, pdcppong::ScoreBoard& board)
    : m_SpeedSelector(ball)
    , m_Reset(board)
{}

pdcppong::SystemMenu::BallSpeedSelection::BallSpeedSelection(pdcppong::Ball& ball)
    : pdcpp::SystemMenu::OptionsItem("Ball:", {"Slow", "Regular", "Fast", "Bonkers"})
    , r_Ball(ball)
{ setSelectedIndex(1); }

void pdcppong::SystemMenu::BallSpeedSelection::callback()
{
    r_Ball.setSpeed(float(getSelectedIndex() + 1) * 5.0f);
}

pdcppong::SystemMenu::Reset::Reset(pdcppong::ScoreBoard& scoreBoard)
    : pdcpp::SystemMenu::BasicItem("Reset Score")
    , r_ScoreBoard(scoreBoard)
{}

void pdcppong::SystemMenu::Reset::callback()
{
    r_ScoreBoard.reset();
}
