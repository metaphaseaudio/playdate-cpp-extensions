//
// Created by Matt on 1/2/2024.
//

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

        /**
         * Sets a value by which the output will be multiplied. Negatives will
         * invert the output.
         *
         * @param factor the factor by which to multiply all the modulators
         */
        void setGainFactor(float factor);

        /**
         * @returns the current gain factor of this channel
         */
        [[ nodiscard ]] float getGainFactor() const;

    private:
        class DummySum
            : public pdcpp::CustomSynthGenerator
        {
        public:
            float getCurrentSum();
            void reset() { m_CurrentSum = 0.0f; }

            bool isStereo() override;

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

        float m_GainFactor = 1.0f;
        std::vector<pdcpp::Signal*> m_Modulators;
        DummySum m_Summer;
        pdcpp::SynthesizerVoice m_Voice;
        pdcpp::Channel m_Channel;
    };
}
