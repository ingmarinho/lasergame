#include "MessageDecoder.h"


MessageDecoder::MessageDecoder(IRReceiver<1> &irReceiver, ReceiveIRController &receiveIRController, unsigned int priority)
    : rtos::task<2000>(priority, "MESSAGEDECODER_TASK"), pausesChannel(this, "PAUSE_CHANNEL"), irReceiver(irReceiver), receiveIRController(receiveIRController)
{
    irReceiver.addListener(this);
}

void MessageDecoder::pauseDetected(int pause)
{
    pausesChannel.write(pause);
}

bool MessageDecoder::createMessage(uint16_t &message, int &pause)
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

bool MessageDecoder::isValidCheckSum(const uint16_t &message)
{
    unsigned int checkBitOne = 1;
    unsigned int checkBitTwo = 6;
    unsigned int checkBitThree = 11;

    for (unsigned int i = 0; i < 5; i++)
    {
        if (((message >> checkBitThree) & 1) == (((message >> checkBitOne) & 1) ^ ((message >> checkBitTwo) & 1)))
        {
            checkBitThree++;
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

void MessageDecoder::main()
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
            hwlib::wait_ms(200);
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
                receiveIRController.sendMessage(firstMessage);
            }

            state = IDLE;

            break;
        }
    }
}
