#pragma once

#include <array>

template <unsigned int maxNumberOfWeapons>
class damagelist {
private:
	std::array<int, maxNumberOfWeapons> firepowers;
public:
	damagelist(){
		int damage = 10;
		for(unsigned int i = 0 ; i < maxNumberOfWeapons ; i++){
		
		}
	}
};

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
    state_t state = IDLE;
    ParametersController &parameterscontroller


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

            case REGISTER_GAME_PARAMETERS:
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