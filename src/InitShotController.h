#pragma once

#include "rtos.hpp"
#include "SendIRController.h"
#include "Speeltijd.h"
#include "ButtonListener.h"

class InitShotController : public rtos::task<2000>, public ButtonListener
{

private:
    enum state_t
    {
        IDLE,
        STARTGAME,
        SENDIR,
        ZOMBIE,
        GAMEOVER

    };

    state_t state = STARTGAME;
    button<1> Trigger;
    rtos::channel<char, 1024> ButtonChannel;
    rtos::flag ZombieFlag;
    rtos::flag startgame;
    rtos::flag GameOver;
    rtos::timer Timer;
    SendIRController &sendIRController;
    uint16_t Commando;
    int Delay = 2'000'000;

public:
    InitShotController(hwlib::target::pin_in &buttontrigger, SendIRController &sendIRController, unsigned int priority, unsigned int priority1)
        : rtos::task<2000>(priority, "TRIGGER_TAAK"),
          Trigger(buttontrigger, priority1), ButtonChannel(this, "BUTTON_CHANNEL"), ZombieFlag(this, "ZombieFlag"), startgame(this, "StartGame"), GameOver(this, "GameOver"), Timer(this, "Timer"), sendIRController(sendIRController)
    {
        Trigger.addListener(this);
    }

    void ButtonPressed(int ButtonID)
    {
        ButtonChannel.write(ButtonID);
    }

    void startGame(uint16_t C, int D)
    {
        Commando = C;
        Delay = D;

        startgame.set();
    }

    void gameOver()
    {
        GameOver.set();
    }

    void zombieFlag()
    {
        ZombieFlag.set();
    }

private:
    void main()
    {
        for (;;)
        {
            switch (state)
            {
            case STARTGAME:
            {
                wait(startgame);
                state = IDLE;
                break;
            }
            case IDLE:
            {
                auto event = wait(GameOver + ZombieFlag + ButtonChannel);

                if (event == ZombieFlag)
                {
                    state = ZOMBIE;
                }
                else if (event == GameOver)
                {
                    state = GAMEOVER;
                }
                else if (event == ButtonChannel)
                {
                    state = SENDIR;
                }
                break;
            }

            case ZOMBIE:
            {
                Timer.set(Delay);
                wait(Timer);

                state = IDLE;
                break;
            }

            case SENDIR:
            {
                sendIRController.sendMessage(Commando);
                state = IDLE;
                break;
            }

            case GAMEOVER:
                hwlib::wait_ms(30000000);
                break;
            }
            hwlib::wait_ms(200);
        }
    }
};