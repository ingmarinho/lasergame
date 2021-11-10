#pragma once

#include "Toetsenbord4x4.hpp"
#include "RunGameController.h"



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

    rtos::channel<char, 1024> KeyChannel;
    Toetsenbord4x4<1> &keypad;
    RunGameController &runGameController;

public:
    ParametersController(Toetsenbord4x4<1> &keypad, RunGameController &runGameController, unsigned int priority) 
    : rtos::task<>(priority, "PARAMETERS_TAAK"), KeyChannel(this, "KEY_CHANNEL"), keypad(keypad), runGameController(runGameController)
    {
        keypad.addListener(this);
    }

private:
    int PlayerID;
    int WeaponID;
    bool PlayerNumber = false;
    bool WeaponNumber = false;

    void main()
    {
        char KeyID;
        for (;;)
        {
            switch (state)
            {
            case IDLE:
            {
                wait(KeyChannel);

                KeyID = KeyChannel.read();

                if (KeyID == 'A')
                {
                    state = GET_PLAYER_NUMBER;
                }
                else if (KeyID == 'B')
                {

                    state = GET_FIRE_POWER;
                }
                else if (KeyID == '*' && PlayerNumber && WeaponNumber)
                {
                    state = GAME_IS_RUNNING;
                }
                break;
            }
            case GET_FIRE_POWER:
            {
                if (KeyID >= '0' && KeyID <= '9')
                {
                    WeaponID = (int)KeyID - 48;
                    WeaponNumber = true;
                }

                break;
            }
            case GET_PLAYER_NUMBER:
            {
                if (KeyID >= '0' && KeyID <= '9')
                {
                    PlayerID = (int)KeyID - 48;
                    PlayerNumber = true;
                }

                break;
            }
            case GAME_IS_RUNNING:
            {
                RunGameController.MeldGameParameters(PlayerID, WeaponID);

                break;
            }
            
            break;
        }
    }
};