//
// Created by Matt on 12/28/2023.
//

#pragma once
#include "CustomLFO.h"

namespace pdcpp
{
    class ConstantSignal
        : public pdcpp::CustomSignal
    {
    public:
        explicit ConstantSignal(float value = 0.0f);

        float step(int* iosamples, float* ifval) override;

        void noteOn(MIDINote note, float vel, float len) override;

        void noteOff(int stopped, int offset) override;

        void setValue(float value);

    private:
        float m_Value;
    };
}