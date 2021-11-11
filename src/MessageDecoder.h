#pragma once

#include "IRReceiver.h"
#include "ReceiveIRController.h"

class ReceiveIRController;
///
class MessageDecoder : public rtos::task<2000>, public IRReceiverListener
{

    enum state_t
    {
        IDLE,
        MESSAGE
    };

private:
    state_t state = IDLE;

    rtos::channel<int, 1024> pausesChannel; /// channel for pauses in IR
    IRReceiver<1> &irReceiver;
    ReceiveIRController &receiveIRController;

public:
    MessageDecoder(IRReceiver<1> &irReceiver, ReceiveIRController &receiveIRController, unsigned int priority);
    void pauseDetected(int pause) override;
private:
    bool createMessage(uint16_t &message, int &pause);
    bool isValidCheckSum(const uint16_t &message);
    void main();
};