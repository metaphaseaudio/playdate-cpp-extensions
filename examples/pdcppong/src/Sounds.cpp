/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/1/2023
 *  Original author: MrBZapp
 */
#include "Sounds.h"
#include <pdcpp/audio/Channel.h>


pdcppong::Sounds::Sounds()
    : m_LFO(pdcpp::WaveformLFO::Waveform::SawtoothDown)
    , m_Arp({0, 4, 7})
    , m_WhistleSample("whistle")
{
    pdcpp::DefaultChannel().addSource(*this);

    m_SamplePlayer.setSample(m_WhistleSample);
    m_Arp.setRate(10);

    m_LFO.setRate(100);
    m_LFO.setBias(1.0f);
    m_LFO.setDepth(0.2f);

    // Pretty much all the same envelope
    for (pdcpp::SynthesizerVoice* s : { &m_PlayerGoal, &m_ComputerGoal, &m_PlayerPaddle, &m_ComputerPaddle, &m_Wall })
    {
        s->setWaveform(SoundWaveform::kWaveformSine);
        s->setAttackTime(0.0f);
        s->setDecayTime(0.1f);
        s->setSustainLevel(0.0f);
    }

    m_PlayerGoal.setFrequencyModulator(m_LFO);
    m_PlayerGoal.setDecayTime(0.5f);
    m_ComputerGoal.setFrequencyModulator(m_Arp);

    m_ComputerGoal.setDecayTime(0.5f);

    addVoice(m_Wall, WALL_NOTE, WALL_NOTE, 0.0f);
    addVoice(m_ComputerPaddle, COMPUTER_PADDLE_NOTE, COMPUTER_PADDLE_NOTE + 1, 0.0f);
    addVoice(m_PlayerPaddle, PLAYER_PADDLE_NOTE, PLAYER_PADDLE_NOTE + 1, 0.0f);
    addVoice(m_ComputerGoal, COMPUTER_GOAL_NOTE, COMPUTER_GOAL_NOTE + 1, 12.0f);
    addVoice(m_PlayerGoal, PLAYER_GOAL_NOTE, PLAYER_GOAL_NOTE + 1, 12.0f);

}

void pdcppong::Sounds::playWhistle() { m_SamplePlayer.play(); }

bool pdcppong::Sounds::isGoalSoundPlaying() const
{
    return m_ComputerGoal.isPlaying() || m_PlayerGoal.isPlaying();
}

void pdcppong::Sounds::finished()
{

}
