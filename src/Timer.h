#pragma once

#include "OledDisplay.h"
#include "RunGameController.h"

class Timer : public rtos::task<>
{

    enum state_t
    {
        STARTGAME,
        IDLE,
        GAMEOVER
    };

private:
    state_t state = STARTGAME;

    rtos::flag StartFlag;
    rtos::flag StopFlag;
    rtos::timer delay;
    OledDisplay &oledDisplay;
    Speeltijd & speeltijd;
    int gametime;

public:
    Timer(OledDisplay& oledDisplay, Speeltijd &speeltijd, unsigned int priority) : rtos::task<>(priority, "TIMER_TAAK"), StartFlag(this, "START_FLAG"), StopFlag(this, "STOP_FLAG"), delay(this, "DELAY"), oledDisplay(oledDisplay), speeltijd(speeltijd)
    {

    }
    
    void startTimer()
    {
    	StartFlag.set();
    }
    
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
                    display.showTime(gametime);
                    delay.set(1000'000);

                    auto evt = wait(delay + StopFlag);

                    if(evt = StopFlag)
                    {
                        state = GAMEOVER;
                    }

                    gametime--;

                    break;

                case GAMEOVER:

                    break;
            }
            break;
        }
    }
};