/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 1/2/2024
 *  Original author: MrBZapp
 */
#pragma once
#include "Signal.h"
#include "SynthesizerVoice.h"
#include "Channel.h"

namespace pdcpp
{
    class ModulationChannel
        : public pdcpp::CustomSignal
    {
    public:
        /**
         * Creates a channel for mixing the pdcpp::Signals typically used for
         * modulation.
         */
        ModulationChannel();

        /**
         * Add a modulator to the channel
         *
         * @param signal the signal to add
         */
        void addModulator(pdcpp::Signal* signal);

        /**
         * Removes a modulator from the channel
         *
         * @param signal the signal to remove. does nothing if not found.
         */
        void removeModulator(pdcpp::Signal* signal);

        /**
         * Clears the channel of all modulators
         */
        void removeAllModulators();

    private:
        class DummySum
            : public pdcpp::CustomSynthGenerator
        {
        public:
            DummySum();
            float getCurrentSum();
            void reset() { m_CurrentSum = 0.0f; }
        private:
            // These two functions make up our "summing" function.
            int renderBlock(int32_t* leftSamps, int32_t* rightSamps, int nSamps, uint32_t rate, int32_t drate) override;
            int setParameter(int parameter, float value) override;
            float m_CurrentSum;
        };

        float step(int* iosamples, float* ifval) override;
        void noteOn(MIDINote note, float vel, float len) override;
        void noteOff(int stopped, int offset) override;

        void resetParameterNumbers();

        std::vector<pdcpp::Signal*> m_Modulators;
        DummySum m_Summer;
        pdcpp::SynthesizerVoice m_Voice;
        pdcpp::Channel m_Channel;
    };
}
