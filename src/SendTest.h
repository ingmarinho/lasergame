#pragma once

#include "SendIRController.h"

class SendTest : public rtos::task<>
{
private:
    SendIRController &sendIRController;

public:
    SendTest(SendIRController &sendIRController)
        : rtos::task<>(4, "SENDTEST_TASK"), sendIRController(sendIRController)
    {
    }

    void main()
    {
        uint16_t message = 0xFF0F;

        while (true)
        {
            sendIRController.sendMessage(message);
            hwlib::wait_ms(500);
        }
    }
};