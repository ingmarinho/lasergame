#pragma once

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
    hwlib::target::pin_in Trigger;
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
    rtos::channel<char, 1024> ButtonChannel;
    rtos::timer Timer;
    rtos::flag ZombieFlag;
    rtos::flag StartGame;
    rtos::flag GameOver;
    uint16_t Commando;
    int Delay = 2'000'000;
    button<1> Trigger;

public:
    InitShotController(hwlib::target::pin_in buttontrigger, unsigned int priority, unsigned int priority1) : rtos::task<>(priority, "TRIGGER_TAAK"),
                                                                                                             Trigger(buttontrigger, priority1), ButtonChannel(this, "BUTTON_CHANNEL"), ZombieFlag(this, "ZombieFlag") StartGame(this, "StartGame"), GameOver(this, "GameOver"), Timer(this, "Timer")
    {
        button.addListener(this);
    }

    void ButtonPressed(int ButtonID)
    {
        ButtonChannel.write(ButtonID);
    }

    void StartGame(uint16_t C, int D)
    {
        Commando = C;
        Delay = D;

        StartGame.set();
    }

    void GameOver()
    {
        GameOver.set();
    }

    void ZombieFlag()
    {
        ZombieFlag.set()
    }

private:
    void main()
    {
        for (;;)
        {
            switch (state)
            {
            case STARTGAME:
                wait(StartGame);
                state = IDLE;
                break;

            case IDLE:
            {
                auto event = wait(GameOver + ZombieFlag + ButtonChannel)

                if (event = ZombieFlag)
                {
                    state = ZOMBIE;
                }
                else if (event = GameOver)
                {
                    state = GAMEOVER;
                }
                else if (event = ButtonChannel)
                {
                    state = SENDIR;
                }
            }
                break;

            case ZOMBIE:

            {
                timer.set(delay);
                wait(delay);  

                state = IDLE; 
            }

                break;

            case SENDIR:
            {    
                sendIRcontroller.sendMessage(Commando);
                state = IDLE;
            }
            
                break;

            case GAMEOVER:
            
            break;

           
            }
        }
    }
};