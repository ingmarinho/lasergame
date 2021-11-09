#pragma once

#include "IRReceiver.h"
#include "ReceiveIRController.h"
#include "Logger.h"

class ReceiveIRController;

class MessageDecoder : public rtos::task<>, public IRReceiverListener
{

    enum state_t
    {
        IDLE,
        MESSAGE
    };

private:
    state_t state = IDLE;

    rtos::channel<int, 1024> pausesChannel;
    ReceiveIRController &receiveIRController;
    IRReceiver<1> irReceiver;
    Logger &logger;

public:
    MessageDecoder(ReceiveIRController &receiveIRController, hwlib::target::pin_in &tsopSignal, hwlib::target::pin_out &led, Logger &logger, unsigned int MessageDecoderPriority, unsigned int IRReceiverPriority);
    void pauseDetected(int pause) override;
private:
    bool createMessage(uint16_t &message, int &pause);
    bool isValidCheckSum(const uint16_t &message);
    void main();
};