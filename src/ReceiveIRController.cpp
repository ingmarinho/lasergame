#include "ReceiveIRController.h"


ReceiveIRController::ReceiveIRController(RunGameController &runGameController, unsigned int priority)
    : rtos::task<>(priority, "RECEIVEIRCONTROLLER_TASK"), messageChannel(this, "MESSAGE_CHANNEL"), runGameController(runGameController)
{
}

void ReceiveIRController::sendMessage(const uint16_t &message)
{
    messageChannel.write(message);
}

void ReceiveIRController::main()
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