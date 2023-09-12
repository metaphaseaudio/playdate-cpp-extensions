/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/28/2023
 *  Original author: MrBZapp
 */

#pragma once
#include "Ball.h"
#include "ScoreBoard.h"
#include <pdcpp/core/SystemMenu.h>

namespace pdcppong
{
    class SystemMenu
    {
    public:
        class BallSpeedSelection
            : public pdcpp::SystemMenu::OptionsItem
        {
        public:
            explicit BallSpeedSelection(pdcppong::Ball& ball);
            void callback() override;

        private:
            pdcppong::Ball& r_Ball;
        };

        class Reset
            : public pdcpp::SystemMenu::BasicItem
        {
        public:
            explicit Reset(pdcppong::ScoreBoard& scoreBoard);
            void callback() override;
        private:
            pdcppong::ScoreBoard& r_ScoreBoard;
        };

        explicit SystemMenu(pdcppong::Ball& ball, pdcppong::ScoreBoard& board);

    private:
        BallSpeedSelection m_SpeedSelector;
        Reset m_Reset;
    };
}
