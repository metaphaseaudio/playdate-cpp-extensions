//
// Created by Matt on 11/8/2023.
//

#pragma once
#include <iostream>
#include <pdcpp/graphics/Sprite.h>
#include <pdcpp/graphics/Font.h>
#include <pdcpp/audio/Synthesizer.h>
#include "pdcpp/audio/LFO.h"
#include "pdcpp/audio/ModulationChannel.h"
#include "pdcpp/audio/effects/Filter.h"
#include "pdcpp/audio/effects/Delay.h"
#include "pdcpp/audio/effects/Overdrive.h"

class TestSynth
    : public pdcpp::Synthesizer
{

public:
    TestSynth()
    {
        enableFinishedCallback();
    }

    void finished() override
    {
        std::cout << "HIT!" << std::endl;
    }
};

class SynthesisObject
{
public:
    SynthesisObject();
    ~SynthesisObject();
    int update();
private:

    pdcpp::SynthesizerVoice m_Voice;
    pdcpp::SynthesizerVoice m_Voice2;
    pdcpp::Envelope m_Env;

    pdcpp::WaveformLFO m_Vibrato;

    pdcpp::WaveformLFO m_LFO1;
    pdcpp::WaveformLFO m_LFO2;
    pdcpp::WaveformLFO m_LFO3;

    pdcpp::ModulationChannel m_Sum;
    pdcpp::TwoPoleFilter m_Filter;
    pdcpp::Delay m_Delay;
    pdcpp::Overdrive m_Overdrive;
    std::vector<pdcpp::Delay::Tap> m_Taps;

    pdcpp::Channel m_MainChan, m_DistChan;
    TestSynth m_Synth;
};
