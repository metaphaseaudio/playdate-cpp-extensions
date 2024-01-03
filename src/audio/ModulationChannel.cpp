//
// Created by Matt on 1/2/2024.
//

#include "pdcpp/audio/ModulationChannel.h"

pdcpp::ModulationChannel::ModulationChannel()
{
    m_Voice.setCustomGenerator(m_Summer);
    m_Channel.addSource(m_Voice);
    m_Channel.setVolume(0.0f);
}

int pdcpp::ModulationChannel::DummySum::renderBlock(int32_t* leftSamps, int32_t* rightSamps, int nSamps, uint32_t rate, int32_t drate) { return nSamps; }

int pdcpp::ModulationChannel::DummySum::setParameter(int parameter, float value)
{
    m_CurrentSum += value;
    return 1;
}

bool pdcpp::ModulationChannel::DummySum::isStereo() { return false; }
float pdcpp::ModulationChannel::DummySum::getCurrentSum()
{
    auto rv = m_CurrentSum;
    m_CurrentSum = 0.0f;
    return rv;
}

void pdcpp::ModulationChannel::addModulator(pdcpp::Signal* signal)
{

    m_Modulators.push_back(signal);
    resetParameterNumbers();
    m_Summer.reset();
}


float pdcpp::ModulationChannel::step(int* iosamples, float* ifval) { return m_Summer.getCurrentSum() * m_GainFactor; }
void pdcpp::ModulationChannel::noteOn(MIDINote note, float vel, float len) { m_Voice.playMIDINote(note, vel, len); }
void pdcpp::ModulationChannel::noteOff(int stopped, int offset) { m_Voice.noteOff(); }

void pdcpp::ModulationChannel::removeModulator(pdcpp::Signal* signal)
{
    auto found = std::find(m_Modulators.begin(), m_Modulators.end(), signal);
    if (found == m_Modulators.end()) { return; }

    m_Voice.clearParameterModulator(std::distance(m_Modulators.begin(), found) + 1);
    m_Modulators.erase(found);

    resetParameterNumbers();
}

void pdcpp::ModulationChannel::removeAllModulators()
{
    for (int i = m_Modulators.size(); --i >=0;)
        { m_Voice.clearParameterModulator(i); }
    m_Modulators.clear();
}

void pdcpp::ModulationChannel::setGainFactor(float factor) { m_GainFactor = factor; }

float pdcpp::ModulationChannel::getGainFactor() const { return m_GainFactor; }

void pdcpp::ModulationChannel::resetParameterNumbers()
{
    for (int i = m_Modulators.size(); --i >=0;)
    {
        m_Voice.clearParameterModulator(i);
        m_Voice.setParameterModulator(i + 1, *m_Modulators[i]);
    }
}
