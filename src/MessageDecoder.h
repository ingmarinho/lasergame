#pragma once

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

public:
    MessageDecoder(hwlib::target::pin_in &tsopSignal, hwlib::target::pin_out &led, unsigned int MessageDecoderPriority, unsigned int IRReceiverPriority)
        : rtos::task<>(MessageDecoderPriority, "MESSAGEDECODER_TASK"), pausesChannel(this, "PAUSE_CLOCK"), irReceiver(tsopSignal, led, IRReceiverPriority)
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
        for (;;)
        {
            switch (state)
            {
            case IDLE:

                wait(pausesChannel);

                // pause = pausesChannel.read()

                state = MESSAGE;

                break;

            case MESSAGE:
                int pause = pausesChannel.read();
                uint16_t message = 0;

                for (unsigned int i = 0; i < 16; i++)
                {
                    message <<= 1;

                    if (pause > 200 && pause < 2000)
                    {
                        message |= (pause > 1000) ? 1 : 0;
                        pause = pausesChannel.read();
                    }
                    else if (pause > 4000 && pause < 5000)
                    {
                        state = IDLE;
                    }
                }
                state = IDLE;

                break;
            }
        }
    }
};