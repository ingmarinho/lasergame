#pragma once

class InitGameController : public rtos::task<>, public KeypadListener
{

    enum state_t
    {
        IDLE,
        GET_SPEELTIJD,
        SEND_SPEELTIJD,
        SEND_START_CMD
    };

private:
    state_t state = IDLE;

    rtos::channel<int, 1024> KeyChannel;
    hwlib::keypad::keypad;

public:
    InitGameController(hwlib::keypad<16> &keypad)
	{
	}
    

private:
    void main()
    {
        for (;;)
        {
            switch (state)
            {
            case IDLE:

                break;

            case GET_SPEELTIJD:

                break;

            case SEND_SPEELTIJD:

                break;

            case SEND_START_CMD:

                break;
            }
            break;
        }
    }
};