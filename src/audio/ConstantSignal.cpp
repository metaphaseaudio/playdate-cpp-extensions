//
// Created by Matt on 12/28/2023.
//

#include "pdcpp/audio/ConstantSignal.h"


pdcpp::ConstantSignal::ConstantSignal(float value)
    : pdcpp::CustomSignal()
    , m_Value(value)
{}

void pdcpp::ConstantSignal::setValue(float value)
{
    m_Value = value;
}

float pdcpp::ConstantSignal::step(int* iosamples, float* ifval)
{
    return m_Value;
}

void pdcpp::ConstantSignal::noteOn(MIDINote note, float vel, float len)
{
}

void pdcpp::ConstantSignal::noteOff(int stopped, int offset)
{
}
