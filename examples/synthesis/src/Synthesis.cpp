//
// Created by Matt on 11/8/2023.
//

#include "Synthesis.h"
#include <pdcpp/core/GlobalPlaydateAPI.h>

SynthesisObject::SynthesisObject()
    : m_LFO1(pdcpp::WaveformLFO::Waveform::Triangle)
    , m_LFO2(pdcpp::WaveformLFO::Waveform::Triangle)
    , m_LFO3(pdcpp::WaveformLFO::Waveform::SampleAndHold)
    , m_Vibrato(pdcpp::WaveformLFO::Waveform::Triangle)
    , m_Filter(TwoPoleFilterType::kFilterTypeLowPass, 50, 0.5)
    , m_MainChan(true)
    , m_DistChan(true)
    , m_Delay(256, false)
{
    m_Taps.emplace_back(std::move(m_Delay.addTap(0)));
    m_Voice.setWaveform(SoundWaveform::kWaveformPOPhase);
    m_Voice2.setWaveform(SoundWaveform::kWaveformSawtooth);

    m_Voice.getEnvelope().setAttack(0.0f);
    m_Voice.getEnvelope().setDecay(1.0f);
    m_Voice.getEnvelope().setSustain(0.75f);
    m_Voice.getEnvelope().setRelease(2.0f);

    m_LFO1.setRate(1.0f);
    m_LFO1.setScale(0.4f);
    m_LFO1.setBias(0.5f);

    m_LFO2.setRate(0.1f);
    m_LFO2.setScale(0.5f);
    m_LFO2.setBias(0.5f);

    m_LFO3.setRate(10);
    m_LFO3.setScale(0.25f);
    m_LFO3.setBias(0.25f);

    m_Sum.addModulator(&m_LFO2);
    m_Sum.addModulator(&m_LFO3);
//    m_Sum.setScale(0.0f);

    m_Env.setAttack(0.25f);
    m_Env.setDecay(0.5f);
    m_Env.setSustain(0.25f);
    m_Env.setRelease(0.9f);
    m_Env.setScale(0.5f);
    m_Env.setBias(0.0f);

    m_Voice.setParameterModulator(0, m_Env);
//    m_Voice.setParameterModulator(1, m_LFO2);
    m_Voice.setParameterModulator(2, m_Sum);
    m_Filter.setFrequencyModulator(m_Env);

    m_Vibrato.setRate(5);
    m_Vibrato.setDepth(0.03f);
    m_Voice.setFrequencyModulator(m_Vibrato);

    m_MainChan.addSource(m_Voice);
    m_MainChan.addEffect(m_Delay);
    m_MainChan.setPan(-0.5f);

    m_DistChan.addSource(m_Taps.at(0));
    m_Taps.at(0).setVolume(0, 1.0f);
    m_DistChan.addEffect(m_Filter);
    m_Voice.playMIDINote(33, 1.0f);
    m_Voice.setVolume(0.5f, 0.5f);
    pdcpp::GlobalPlaydateAPI::get()->system->resetElapsedTime();

}

int SynthesisObject::update()
{
    if (pdcpp::GlobalPlaydateAPI::get()->system->getElapsedTime() > 10.0f)
    {
        m_Voice.noteOff();
    }
    return 1;
}

SynthesisObject::~SynthesisObject()
{
    m_DistChan.removeSource(m_Taps.at(0));
}
