/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/1/2023
 *  Original author: MrBZapp
 */

#pragma once

#include <pdcpp/audio/Arpeggiator.h>
#include <pdcpp/audio/Synthesizer.h>
#include <pdcpp/audio/SynthesizerVoice.h>
#include <pdcpp/audio/SamplePlayer.h>

namespace pdcppong
{
    class Sounds
        : public pdcpp::Synthesizer
    {
    public:
        Sounds();

        [[ nodiscard ]] bool isGoalSoundPlaying() const;

        static constexpr int WALL_NOTE = 65;
        static constexpr int PLAYER_PADDLE_NOTE = 63;
        static constexpr int COMPUTER_PADDLE_NOTE = 67;
        static constexpr int PLAYER_GOAL_NOTE = 50;

        void finished() override;

        static constexpr int COMPUTER_GOAL_NOTE = 55;

        void playWhistle();

    private:
        pdcpp::SynthesizerVoice m_PlayerPaddle, m_PlayerGoal, m_ComputerPaddle, m_ComputerGoal, m_Wall;
        pdcpp::WaveformLFO m_LFO;
        pdcpp::Arpeggiator m_Arp;
        pdcpp::AudioSample m_WhistleSample;
        pdcpp::SamplePlayer m_SamplePlayer;
    };
}
