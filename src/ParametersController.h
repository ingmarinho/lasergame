#pragma once

class ParametersController : public rtos::task<>
{

    enum state_t
    {
        IDLE,
        GET_FIRE_POWER,
        GET_PLAYER_NUMBER,
        GAME_IS_RUNNING
    };

private:
    state_t state = IDLE;

    rtos::channel<int, 1024> KeyChannel;
    hwlib::keypad::keypad;

public:
    ParametersController(hwlib::keypad<16> &keypad)
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

            case GET_FIRE_POWER:

                break;

            case GET_PLAYER_NUMBER:

                break;

            case GAME_IS_RUNNING:

                break;
            }
            break;
        }
    }
};