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
    MessageDecoder(hwlib::target::pin_in &tsopSignal, hwlib::target::pin_out &led)
        : pausesChannel(this), irReceiver(tsopSignal, led)
    {
        irReceiver.addListener(this);
    }

    void pauseDetected(int pause) override
    {
        pausesChannel.write(pause);
    }

private:
    bool isValid(const int &pause)
    {
        return (200 > pause && pause > 2000);
    }

    void main()
    {
        for (;;)
        {
            switch (state)
            {
            case IDLE:
                if (pausesChannel.read())
                {
                    state = MESSAGE;
                }
                break;

            case MESSAGE:
                int pause = pausesChannel.read();

                if (pause > 4000)
                {
                    state = IDLE;
                }
                break;
            }
        }
        // if !isValid(pause) {state = IDLE; break;}

        //    if (pause <)

        //       switch ()

        //          break;

        // case default:

        //    break;
        // }
    }
};