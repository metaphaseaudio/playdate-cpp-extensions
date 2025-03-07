//
// Created by Matt on 1/11/2025.
//

#pragma once

#include <pd_api.h>

namespace pdcpp
{
    class Microphone
    {
    public:
        virtual void dataReceived(int16_t* data, int len) = 0;
        virtual ~Microphone();

        void startRecording(MicSource micSource);
        void stopRecording();

        [[ nodiscard ]] bool isRecording() { return m_IsRecording; }
    private:
        bool m_IsRecording{false};
    };
}
