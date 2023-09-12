/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/30/2023
 *  Original author: MrBZapp
 */

#include "ScoreBoard.h"
#include "pdcpp/core/GlobalPlaydateAPI.h"

constexpr LCDPattern transparent50Gray = {
        // Bitmap
        0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101,
        // Mask
        0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010,
};

pdcppong::ScoreBoard::ScoreBoard()
    : m_ComputerScore(0)
    , m_PlayerScore(0)
    , m_Font("/System/Fonts/Roobert-10-Bold.pft")
{ setBounds({170, 0, 60, 240}); }

void pdcppong::ScoreBoard::redraw(PDRect bounds, PDRect drawrect)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    pd->graphics->drawLine(200, 0, 200, 240, 4, reinterpret_cast<LCDColor>(transparent50Gray));
    auto y = (bounds.height / 2.0f) - (m_Font.getFontHeight() / 2.0f);
    m_Font.drawText(std::to_string(m_ComputerScore), bounds.x, y);
    auto playerScoreString = std::to_string(m_PlayerScore) + " ";
    m_Font.drawText(playerScoreString, bounds.x + (bounds.width - m_Font.getTextWidth(playerScoreString)), y);
}

void pdcppong::ScoreBoard::incrementPlayerScore()
{
    m_PlayerScore++;
    markDirty();
}

void pdcppong::ScoreBoard::incrementComputerScore()
{
    m_ComputerScore++;
    markDirty();
}

void pdcppong::ScoreBoard::reset()
{
    m_ComputerScore = 0;
    m_PlayerScore = 0;
    markDirty();
}
