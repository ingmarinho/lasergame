#pragma once

#include "MessageDecoder.h"
#include "RunGameController.h"

class RunGameController;

class ReceiveIRController : public rtos::task<>
{
private:
    rtos::channel<uint16_t, 1024> messageChannel; /// channel for incoming messages

    RunGameController &runGameController;

public:
    ReceiveIRController(RunGameController& runGameController, unsigned int priority);

    void sendMessage(const uint16_t &message);
private:
    void main();
};