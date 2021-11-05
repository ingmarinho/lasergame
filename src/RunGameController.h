#pragma once

class RunGameController : public rtos::task<>
{
    enum state_t
	{
		IDLE,
        REGISTER_GAME_PARAMETERS,
        HIT_RECEIVED,
        CMD_RECEIVER,
        MELDSPEELTIJD,
        WAIT_COUNTDOWN,
        DISPLAY_COUNTDOWN,
        START_SIGNAL,
        ZOMBIE
	};
private:


public:


private:

    void main()
    {
        for (;;)
        {
            switch (state)
            {
            case IDLE:

                break;

            case HIT_RECEIVED:

                break;

            case CMD_RECEIVER:

                break;

            case MELDSPEELTIJD:

                break;

            case WAIT_COUNTDOWN:

                break;
            case DISPLAY_COUNTDOWN:

                break;

            case START_SIGNAL:

                break;

            case ZOMBIE:

                break;
            }
            break;
        }
    }
};