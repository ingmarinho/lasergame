#pragma once

#include "rtos.hpp"
#include "SendIRController.h"
#include "Speeltijd.h"

class ButtonListener
{
public:
    virtual void ButtonPressed(int buttonid);
};

template <unsigned int maxNumberOfListeners>
class button : public rtos::task<>
{

private:
    std::array<ButtonListener *, maxNumberOfListeners> ButtonListeners;
    hwlib::target::pin_in &Trigger;
    unsigned int currentNumberOfListeners = 0;

public:
    button(hwlib::target::pin_in &button, unsigned int priority) : rtos::task<>(priority, "BUTTON_TAAK"), Trigger(button)
    {
        for (unsigned int i = 0; i < maxNumberOfListeners; i++)
        {
            ButtonListeners[i] = nullptr;
        }
    }

    void addListener(ButtonListener *listener)
    {
        if (currentNumberOfListeners < maxNumberOfListeners)
        {
            ButtonListeners[currentNumberOfListeners] = listener;
            currentNumberOfListeners++;
        }
    }

    void main()
    {
        for (;;)
        {
            bool t = false;

            while (!t)
            {
                t = Trigger.read();
            }

            for (unsigned int i = 0; i < maxNumberOfListeners; i++)
            {
                auto listener = ButtonListeners[i];
                if (listener != nullptr)
                {
                    listener->ButtonPressed(0);
                }
            }
        }
    }
};

class InitShotController : public rtos::task<>, public ButtonListener
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
    InitShotController(hwlib::target::pin_in buttontrigger, SendIRController &sendIRController, unsigned int priority, unsigned int priority1) : rtos::task<>(priority, "TRIGGER_TAAK"),
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
                wait(startgame);
                state = IDLE;
                break;

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
            }

            break;

            case SENDIR:
            {
                sendIRController.sendMessage(Commando);
                state = IDLE;
            }

            break;

            case GAMEOVER:

                break;
            }
        }
    }
};