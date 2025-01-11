//
// Created by Matt on 1/11/2025.
//

#include "pdcpp/audio/Microphone.h"
#include "pdcpp/core/GlobalPlaydateAPI.h"

static int recordingShim(void *context, int16_t* data, int len)
{
    auto _this = static_cast<pdcpp::Microphone*>(context);
    _this->dataReceived(data, len);
    return _this->isRecording() ? 1 : 0;
}

void pdcpp::Microphone::startRecording(MicSource micSource)
{
    m_IsRecording = true;
    pdcpp::GlobalPlaydateAPI::get()->sound->setMicCallback(&recordingShim, this, micSource);
}

void pdcpp::Microphone::stopRecording()
{
    m_IsRecording = false;
}
