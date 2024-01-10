/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/29/2023
 *  Original author: MrBZapp
 */

#include "Game.h"
#include <cmath>
#include <pdcpp/core/GlobalPlaydateAPI.h>


pdcppong::Game::Game()
    : m_NewRound(true)
    , m_Player(SpriteTag::PLAYER, {0, 0, 10, 50})
    , m_Computer(SpriteTag::COMPUTER, {0, 0, 10, 50})
    , m_UpperBounds(SpriteTag::WALL, {0, -10, 400, 10})
    , m_LowerBounds(SpriteTag::WALL, {0, 240, 400, 10})
    , m_PlayerGoal(SpriteTag::GOAL, {400, 0, 10, 240})
    , m_ComputerGoal(SpriteTag::GOAL, {-10, 0, 10, 240})
    , m_Ball(m_Sounds, [&](const pdcpp::Sprite* x){ scoreAndReset(x); })
    , m_ComputerAI(m_Computer, m_Ball)
    , m_BallSpeedSelection("Ball:", {"Slow", "Regular", "Fast", "Bonkers"}, [&](auto, int i){ m_Ball.setSpeed(float(i + 1) * 5.0f); })
    , m_ResetGame("Reset Score", [&](){ m_ScoreBoard.reset(); })
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    pd->display->setRefreshRate(20);

    auto height = pd->display->getHeight();
    auto width = pd->display->getWidth();

    m_PlayerLane = width - 20;
    m_ComputerLane = 20;
    m_Player.moveTo(m_PlayerLane, height / 2.0f);
    m_Computer.moveTo(m_ComputerLane, height / 2.0f);
    m_Ball.moveTo(width / 2.0f, height / 2.0f);
}

int pdcppong::Game::update()
{
    m_Sounds.allNotesOff();
    auto pd = pdcpp::GlobalPlaydateAPI::get();

    if (m_NewRound)
    {
        m_LaunchReadyIndicator.setVisible(true);
        PDButtons clicked;
        pd->system->getButtonState(&clicked, nullptr, nullptr);
        if (clicked & kButtonA)
        {
            m_Sounds.playWhistle();
            m_Ball.launch(false);
            m_LaunchReadyIndicator.setVisible(false);
            m_NewRound = false;
        }
    }

    auto angle = ::fmodf(pd->system->getCrankAngle() + 180.0f, 360.0f);
    auto crankPosition = std::abs(angle - 180.0f) / 180.0f;
    auto playerBounds = m_Player.getBounds();
    auto height = float(pd->display->getHeight());
    auto rangeMin = playerBounds.height / 2.0f;
    auto rangeMax = height - playerBounds.height;

    m_Player.moveTo(m_PlayerLane, crankPosition * rangeMax + rangeMin);
    m_ComputerAI.update();
    pd->sprite->updateAndDrawSprites();
    return 1;
}

void pdcppong::Game::scoreAndReset(const pdcpp::Sprite* goal)
{
    if (goal == &m_PlayerGoal) { m_ScoreBoard.incrementComputerScore(); }
    if (goal == &m_ComputerGoal) { m_ScoreBoard.incrementPlayerScore(); }

    m_Ball.moveTo(200, 120);
    m_Ball.setVisible(true);
    m_NewRound = true;
}
