#pragma once

#include "SendIRController.h"

class SendTest : public rtos::task<>
{
private:
    SendIRController &sendIRController;

public:
    SendTest(SendIRController &sendIRController, unsigned int priority)
        : rtos::task<>(priority, "SENDTEST_TASK"), sendIRController(sendIRController)
    {
    }

    void main()
    {
        // uint16_t message = 0xFF0F;
        uint16_t message2 = 0x1234;


        while (true)
        {
            sendIRController.sendMessage(message2);
            // sendIRController.sendMessage(message2);

            hwlib::wait_ms(2000);
        }
    }
};