#pragma once

class InitShotController : public rtos::task<>
{
    enum state_t
    {
        IDLE,
        STARTGAME,
        SENDIR,
        ZOMBIE,
        GAMEOVER

    };

private:
	state_t state = IDLE;
    rtos::timer zombie;
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

            case STARTGAME:

                break;

            case SENDIR:

                break;

            case ZOMBIE:

                break;

            case GAMEOVER:
            }
        }
    }
};