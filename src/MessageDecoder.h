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
    ReceiveIRController *receiveIRController;
    IRReceiver<1> irReceiver;
    Logger &logger;

public:
    MessageDecoder(ReceiveIRController *receiveIRController, hwlib::target::pin_in &tsopSignal, hwlib::target::pin_out &led, Logger &logger, unsigned int MessageDecoderPriority, unsigned int IRReceiverPriority)
        : rtos::task<>(MessageDecoderPriority, "MESSAGEDECODER_TASK"), pausesChannel(this, "PAUSE_CHANNEL"), receiveIRController(receiveIRController), irReceiver(tsopSignal, led, IRReceiverPriority), logger(logger)
    {
        irReceiver.addListener(this);
    }

    void pauseDetected(int pause) override
    {
        pausesChannel.write(pause);
    }

private:
    bool createMessage(uint16_t &message, int &pause)
    {

        for (unsigned int i = 0; i < 16; i++)
        {
            pause = pausesChannel.read();

            if (pause > 200 && pause < 2000)
            {
                message |= (((pause > 1000) ? 0 : 1) << i);

            }

            else if (pause > 3500 && pause < 5000)
            {
                hwlib::cout << "error\n";
                return false;
            }
        }
        return true;
    }

    bool isValidCheckSum(const uint16_t &message)
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
        uint16_t firstMessage;
        uint16_t secondMessage;
        bool messageResult;

        for (;;)
        {
            switch (state)
            {
            case IDLE:

                wait(pausesChannel);

                pause = pausesChannel.read();

                if (pause > 3500 && pause < 5000)
                {
                    state = MESSAGE;
                }

                break;

            case MESSAGE:
                firstMessage = 0x0;
                secondMessage = 0x0;

                messageResult = createMessage(firstMessage, pause);
                // logger.logInt(firstMessage);

                // check first message
                if (!messageResult)
                {
                    state = IDLE;
                    break;
                }

                // check checksum
                if (!isValidCheckSum(firstMessage))
                {
                    state = IDLE;
                    break;
                }

                // check pausebit
                pause = pausesChannel.read();
                if (!(pause > 2500 && pause < 3500))
                {
                    // hwlib::cout << "error1\n";
                    state = IDLE;
                    break;
                }

                // check second message
                messageResult = createMessage(secondMessage, pause);
                if (!messageResult)
                {
                    // hwlib::cout << "error2\n";
                    state = IDLE;
                    break;
                }

                if (firstMessage == secondMessage)
                {
                    receiveIRController->sendMessage(firstMessage);
                }

                state = IDLE;

                break;

                // pause = pausesChannel.read();

                // for (unsigned int i = 0; i < 16; i++)
                // {
                //     // message >>= 1;

                //     if (pause > 200 && pause < 2000)
                //     {
                //         message |= (((pause > 1000) ? 0 : 1) << i);
                //         pause = pausesChannel.read();
                //     }

                //     else if (pause > 3500 && pause < 5000)
                //     {
                //         hwlib::cout << "error\n";
                //         state = IDLE;
                //         break;
                //     }
                // }

                // check pausebit
                // pause = pausesChannel.read();
                // if (!pause > 2500 && pause < 3500)
                // {
                //     hwlib::cout << "error\n";
                //     state = IDLE;
                //     break;
                // }

                // logger.addLog(message == 0xFF0F ? "Correct" : "Incorrect");
                // logger.logInt(message);
                // logger.addLog(message);
            }
        }
    }
};