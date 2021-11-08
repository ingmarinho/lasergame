#pragma once

#include "SendIRController.h"
#include "SpeakerController.h"

class SendTest : public rtos::task<>
{
private:
    SendIRController &sendIRController;

    // testing sound
    SpeakerController &speakerController;

public:
    SendTest(SendIRController &sendIRController, SpeakerController& speakerController, unsigned int priority)
        : rtos::task<>(priority, "SENDTEST_TASK"), sendIRController(sendIRController), speakerController(speakerController)
    {
    }

    void main()
    {
        uint16_t message = 0xFF0F;
        uint16_t message2 = 0x1234;
        uint16_t message3 = 0x734A;
        uint16_t message4 = 0x5602;


        while (true)
        {
            sendIRController.sendMessage(message);
            sendIRController.sendMessage(message2);
            sendIRController.sendMessage(message3);
            sendIRController.sendMessage(message4);

            // speakerController.addSound(HITSOUND);

            hwlib::wait_ms(2000);
        }
    }
};