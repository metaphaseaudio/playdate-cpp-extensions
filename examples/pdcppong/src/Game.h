/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/29/2023
 *  Original author: MrBZapp
 */

#pragma once
#include "BlockSprite.h"
#include "Ball.h"
#include "ScoreBoard.h"
#include "LaunchReadyIndicator.h"
#include "ComputerAI.h"
#include "Sounds.h"
#include <pdcpp/core/SystemMenu.h>


namespace pdcppong
{
    class Game
    {
    public:
        Game();
        int update();
    private:
        void scoreAndReset(const pdcpp::Sprite*);
        bool m_NewRound;
        float m_PlayerLane, m_ComputerLane;
        BlockSprite m_Player, m_Computer, m_UpperBounds, m_LowerBounds, m_PlayerGoal, m_ComputerGoal;
        Sounds m_Sounds;
        Ball m_Ball;
        ScoreBoard m_ScoreBoard;
        LaunchReadyIndicator m_LaunchReadyIndicator;
        ComputerAI m_ComputerAI;

        // System Menu
        pdcpp::SystemMenu::OptionsItem m_BallSpeedSelection;
        pdcpp::SystemMenu::BasicItem m_ResetGame;
    };
}
