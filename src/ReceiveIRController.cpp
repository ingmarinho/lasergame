#include "ReceiveIRController.h"


ReceiveIRController::ReceiveIRController(RunGameController &runGameController, unsigned int priority)
    : rtos::task<>(priority, "RECEIVEIRCONTROLLER_TASK"), messageChannel(this, "MESSAGE_CHANNEL"), runGameController(runGameController)
{
}

void ReceiveIRController::sendMessage(const uint16_t &message)
{
    messageChannel.write(message);
}

/// decodes message from message decoder to command or hit and communicates to rungamecontroller
void ReceiveIRController::main()
{
    uint16_t message; /// message to be received
    uint16_t mask; /// mask to get specific bits from message
    int cmdType; /// specifies type of commando
    int data; /// specifies data of commando or weapon type
    uint16_t playerID; /// player number
    uint16_t weaponID; /// weapon type
    uint16_t res; /// result after mask has been used on the message

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
            runGameController.MeldCMDReceived(cmdType, data);
        }
        else if (res >= 0 && res <= 9)
        {
            playerID = res;
            mask = 0b0000'0111'1100'0000;
            res = message & mask;
            weaponID = res >> 6;
            runGameController.MeldHitReceived(playerID, weaponID);
        }
    }
}
