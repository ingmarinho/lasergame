#pragma once

#include "MessageDecoder.h"
#include "RunGameController.h"
#include "Logger.h"

class MessageDecoder;

class ReceiveIRController : public rtos::task<>
{
private:
    rtos::channel<uint16_t, 1024> messageChannel;

    MessageDecoder messageDecoder;
    RunGameController &runGameController;

public:
    ReceiveIRController(RunGameController& runGameController, hwlib::target::pin_in &tsopSignal, hwlib::target::pin_out &led, Logger &logger, unsigned int ReceiveIRControllerPriority, unsigned int MessageDecoderPriority, unsigned int IRReceiverPriority);

    void sendMessage(const uint16_t &message);
private:
    void main();
};