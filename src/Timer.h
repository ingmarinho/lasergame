#pragma once


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
    Display &display;
    rtos::timer delay;
    int gametime = speeltijd;

public:
    Timer(display(display), unsigned int priority) : rtos::task<>(priority, "TIMER_TAAK"), delay(this, 'delay')
    {
    }
    
    void StartTimer()
    {
    	StartFlag.set();
    }
    
    void StopFlag()
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