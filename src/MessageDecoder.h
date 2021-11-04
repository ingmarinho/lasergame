#pragma once

#include "IRReceiver.h"
#include "Logger.h"

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
    // receiveIRController &receiveIRctrl;
    IRReceiver<1> irReceiver;
    Logger &logger;

public:
    MessageDecoder(hwlib::target::pin_in &tsopSignal, hwlib::target::pin_out &led, Logger &logger, unsigned int MessageDecoderPriority, unsigned int IRReceiverPriority)
        : rtos::task<>(MessageDecoderPriority, "MESSAGEDECODER_TASK"), pausesChannel(this, "PAUSE_CHANNEL"), irReceiver(tsopSignal, led, IRReceiverPriority), logger(logger)
    {
        irReceiver.addListener(this);
    }

    void pauseDetected(int pause) override
    {
        pausesChannel.write(pause);
    }

private:
    bool check(const uint16_t &message)
    {
        unsigned int checkBitOne = 1;
        unsigned int checkBitTwo = 6;

        for (unsigned int checkBitThree = 11; checkBitThree < 15; checkBitThree++)
        {
            if (((message >> checkBitThree) & 1) == (((message >> checkBitOne) & 1) ^ ((message >> checkBitTwo) & 1)))
            {
                checkBitOne++;
                checkBitTwo++;
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    void main()
    {
        int pause;
        uint16_t message = 0x0;

        for (;;)
        {
            switch (state)
            {
            case IDLE:

                wait(pausesChannel);

                pause = pausesChannel.read();
                // logger.addLog(pause);

                if (pause > 2500 && pause < 5000)
                {
                    state = MESSAGE;
                    logger.addLog(1001);
                }

                break;

            case MESSAGE:
                message = 0x0;
                pause = pausesChannel.read();

                // hwlib::cout << pause << hwlib::endl;

                for (unsigned int i = 0; i < 16; i++)
                {
                    message <<= 1;

                    if (pause > 200 && pause < 2000)
                    {
                        message |= (pause > 1000) ? 0 : 1;
                        pause = pausesChannel.read();
                        // hwlib::cout << pause << hwlib::endl;
                    }
                    else if (pause > 2500 && pause < 5000)
                    {
                        hwlib::cout << "error\n";
                        state = IDLE;
                        break;
                    }
                }

                // logger.addLog(message == 0xFF0F ? "Correct" : "Incorrect");
                logger.addLog(message);
                // logger.addLog(message);

                state = IDLE;

                break;
            }
        }
    }
};