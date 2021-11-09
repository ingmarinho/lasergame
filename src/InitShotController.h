#pragma once

class button : public rtos::task<> 
{ 
    hwlib::target::pin_in Trigger;

    InitShotController(hwlib::target::pin_in buttontrigger, unsigned int priority) : 
    rtos::task<>(1, "TRIGGER_TAAK"), Trigger(buttontrigger)

}

class ButtonListener {
public:
	virtual void ButtonPressed ();
};

template <unsigned int maxNumberOfListeners>
class InitShotController  : public rtos::task<>{

private:
	
	std::array< ButtonListener*, maxNumberOfListeners> ButtonListeners;
	unsigned int currentNumberOfListeners = 0;
	state_t state = STARTGAME;

    rtos::channel<char, 1024> ButtonChannel;
    rtos::timer Timer;
    rtos::flag ZombieFlag;
    rtos::flag StartGame;
    rtos::flag GameOver;
    uint16_t Commando;
    int Delay;
    button Trigger; 

public:
	InitShotController(hwlib::target::pin_in buttontrigger, unsigned int priority) : 
    rtos::task<>(1, "TRIGGER_TAAK"), Trigger(buttontrigger)
	{
		unsigned int i=0;
		for(i=0;i<maxNumberOfListeners;i++)
		{
			ButtonListeners[i]=nullptr;
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
	
private:
	void main(){
		for(;;){
			char c = button.getc();
			hwlib::cout << c << hwlib::endl;
			
			unsigned int i=0;
			for(i=0;i < maxNumberOfListeners;i++)
			{
				auto listener = ButtonListeners[i];
				if(listener!=nullptr)
				{
				   listener->ButtonPressed(c);	
				}
			}
		}
	}
};




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

public:
    InitGameController(Button &Button, unsigned int priority) :
     rtos::task<>(priority, "BUTTON_TAAK"), ButtonChannel(this, "BUTTON_CHANNEL"), button(button),  
    {
        button.addListener(this);
    }


    void ButtonPressed(bool ButtonID)
    {
        ButtonChannel.write(ButtonID);
    }

    void StartGame(uint16_t Command, int Delay)
    {   
        Command = 
        Delay = Delay;

        
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
            auto event = wait(GameOver + ZombieFlag + ButtonChannel)

            if(event = ZombieFlag)
            {
                state = ZOMBIE;
            }

                break;

            case ZOMBIE:

            {
                hwlib::wait_ms(5000);
            }
                
                break;

            case SENDIR:
            
            sendIRcontroller.sendMessage()
                
                break;

            case GAMEOVER:
            }
        }
    }
};