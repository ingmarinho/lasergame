#pragma once

#include "Speeltijd.h"
#include "OledDisplay.h"
#include "RunGameController.h"

/// timer to countdown the remaining gametime
class Timer : public rtos::task<1500>
{

    enum state_t
    {
        STARTGAME,
        IDLE,
        GAMEOVER
    };

private:
    state_t state = STARTGAME;

    rtos::flag StartFlag; /// flag to start the game
    rtos::flag StopFlag; /// flag to stop the game
    rtos::timer delay; /// timer
    OledDisplay &oledDisplay;
    Speeltijd & speeltijd;
    int gametime; /// remaining gametime

public:
    Timer(OledDisplay& oledDisplay, Speeltijd &speeltijd, unsigned int priority) 
    : rtos::task<1500>(priority, "TIMER_TAAK"), StartFlag(this, "START_FLAG"), StopFlag(this, "STOP_FLAG"), delay(this, "DELAY"), oledDisplay(oledDisplay), speeltijd(speeltijd)
    {
        gametime = speeltijd.GetGameTime();
    }

    /// set the startflag
    void startTimer()
    {
    	StartFlag.set();
    }

    /// set the stopflag
    void stopFlag()
    {
        StopFlag.set();
    }

private:

    void main()
    {
        for (;;)
        {

            switch (state)
            {
                case STARTGAME:
                    wait(StartFlag);
                    state = IDLE;
                    break;

                case IDLE:
                {
                    oledDisplay.updateTimer(gametime);
                    delay.set(1000'000);

                    auto evt = wait(delay + StopFlag);

                    if(evt == StopFlag)
                    {
                        state = GAMEOVER;
                    }

                    gametime--;

                    break;
                }
                case GAMEOVER:
                    {
                        break;
                    }
            }
            break;
        }
    }
};