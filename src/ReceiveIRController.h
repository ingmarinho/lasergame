#pragma once

#include "RunGameController.h"
#include "MessageDecoder.h"

class MessageDecoder;

class ReceiveIRController : public rtos::task<>
{
private:
    rtos::channel<uint16_t, 1024> messageChannel;

    RunGameController &runGameController;
    MessageDecoder messageDecoder;

public:
    ReceiveIRController(RunGameController& runGameController, hwlib::target::pin_in &tsopSignal, hwlib::target::pin_out &led, ReceiveIRController &receiveIRController, Logger &logger, unsigned int ReceiveIRControllerPriority, unsigned int MessageDecoderPriority, unsigned int IRReceiverPriority)
        : rtos::task<>(ReceiveIRControllerPriority, "RECEIVEIRCONTROLLER_TASK"), messageChannel(this, "MESSAGE_CHANNEL"), runGameController(runGameController), messageDecoder(this, tsopSignal, led, logger, MessageDecoderPriority, IRReceiverPriority)
    {
    }

    void sendMessage(const uint16_t &message)
    {
        messageChannel.write(message);
    }

private:
    void main()
    {
        uint16_t message;
        uint16_t mask;
        int cmdType;
        int data;
        uint16_t playerID;
        uint16_t weaponID;
        uint16_t res;

        for (;;)
        {
            wait(messageChannel);
            message = messageChannel.read();

            mask = 0b0000'0000'0011'1110;

            res = message & mask;
            res >>= 1;

            if (res >= 10)
            {
                cmdType = res;
                mask = 0b0000'0111'1100'0000;
                res = message & mask;
                data = res >> 6;
                // runGameController.meldCMDReceived(cmdType, data);
            }
            else if (res >= 0 && res <= 9)
            {
                playerID = res;
                mask = 0b0000'0111'1100'0000;
                res = message & mask;
                weaponID = res >> 6;
                // runGameController.meldHitReceived(playerID, weaponID);
            }
        }
    }
};