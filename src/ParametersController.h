#pragma once

#include "Toetsenbord4x4.hpp"
#include "RunGameController.h"

/// initialize player number and weapon type and communicate to rungamecontroller
class ParametersController : public rtos::task<2000>, public KeyPadListener
{

    enum state_t
    {
        IDLE,
        GET_FIRE_POWER,
        GET_PLAYER_NUMBER,
        GAME_IS_RUNNING,

    };

private:
    state_t state = IDLE;

    rtos::channel<char, 1024> KeyChannel; /// channel for the keys that have been pressed
    Toetsenbord4x4<1> &keypad;
    RunGameController &runGameController;

public:
    ParametersController(Toetsenbord4x4<1> &keypad, RunGameController &runGameController, unsigned int priority)
        : rtos::task<2000>(priority, "PARAMETERS_TAAK"), KeyChannel(this, "KEY_CHANNEL"), keypad(keypad), runGameController(runGameController)
    {
        keypad.addListener(this);
    }

    void KeyPressed(char KeyID) override
    {
        KeyChannel.write(KeyID);
    }

private:
    int PlayerID; /// player number
    int WeaponID; /// weapon type
    bool PlayerNumber = false; /// bool to check if player number has been specified yet
    bool WeaponNumber = false; /// bool to check if weapon type has been specified yet

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
                runGameController.MeldGameParameters(PlayerID, WeaponID);

                hwlib::wait_ms(300000000);
                break;
            }

            break;
            }
            hwlib::wait_ms(200);
        }
    }
};