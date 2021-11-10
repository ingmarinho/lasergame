#pragma once

#include <array>

template <unsigned int maxNumberOfWeapons>

class DamageList {
private:
	
	struct info{
		string WeaponName;
		int damage;
		int delay;
		info(string WeaponName, int damage, int delay) : WeaponName(WeaponName), damage(damage), delay(delay){}
	};
	
	std::array<info, maxNumberOfWeapons> Weapons;
	
public:
	DamageList(){
		
		info Weapon0("Knife", 10, 1'500'000);
		Weapons[0] = Weapon0;
		info Weapon1("Eagle", 15, 2'000'000);
		Weapons[1] = Weapon1;
		info Weapon2("Machine Gun", 5, 700'000);
		Weapons[2] = Weapon2;
		info Weapon3("AUG", 5, 500'000);
		Weapons[3] = Weapon3;
		info Weapon4("AK-47", 25, 3'500'000);
		Weapons[4] = Weapon4;
		info Weapon5("M4A4", 20, 3'000'000);
		Weapons[5] = Weapon5;
		info Weapon6("AWP", 40, 5'500'000);
		Weapons[6] = Weapon6;
		info Weapon7("M249", 3, 500'000);
		Weapons[7] = Weapon7;
		info Weapon8("Shotgun", 30, 4'000'000);
		Weapons[8] = Weapon8;
		info Weapon9("rbg", 50, 6'500'000);
		Weapons[9] = Weapon9;
	}
	
	const char * GetName(int i){
		if(i < maxNumberOfWeapons){
			return Weapons[i].WeaponName;
		}
	}
	
	int GetDamage(int i){
		if(i < maxNumberOfWeapons){
			return Weapons[i].damage;
		}
	}
	
	int GetDelay(int i){
		if(i < maxNumberOfWeapons){
			return Weapons[i].delay;
		}
	}
};

class Speeltijd
{
private:
	int speeltijd = 0;
public:
	
	SetGameTime(int time)
	{
		speeltijd = time;
	}
	
	int GetGameTime()
	{
		return speeltijd;
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
        ZOMBIE,
        GAMEOVER
	};
private:
    state_t state = REGISTER_GAME_PARAMETERS;
    ParametersController &parameterscontroller
    Speeltijd &speeltijd;
    DamageList<10> damagelist;
	InitShotController &initshotcontroller;
	Led green;
	ReceiveIRController &receiveircontroller;
	Timer &countdown;
	Hitog &hitlog;
	rtos::channel<std::array<int, 2>, 1024> HitChannel;
	rtos::channel<std::array<int, 2>, 1024> CmdChannel;
	rtos::channel<std::array<int, 2>, 1024> ParametersChannel;
	rtos::flag gameover;
	rtos::timer delay;


public:
	RunGameController(ParametersController &parameterscontroller, Speeltijd &speeltijd, InitShotController &initshotcontroller,
	                  hwlib::target::pin_out &led, ReceiveIRController &receiveircontroller, Timer &countdown, HitLog &hitLog, unsigned int priority)
	                  : rtos::task<>(priority, "RunGameController"), parameterscontroller(parameterscontroller), speeltijd(speeltijd), initshotcontroller(initshotcontroller),
	                  green(led), receiveircontroller(receiveircontroller), countdown(countdown), hitlog(hitLog), HitChannel(this, "HitChannel"), CmdChannel(this, "CmdChannel"),
	                  ParametersChannel(this, "ParametersChannel"), gameover(this, "gameover"), delay(this, "delay")
	                  {
							
	                  }
	                  
	void MeldHitReceived(int PlayerID, int WeaponID)
	{
		std::array<int, 2> hit;
		hit[0] = PlayerID;
		hit[1] = WeaponID;
		HitChannel.write(hit);
	}
	
	void MeldCMDReceived(int CmdType, int Data)
	{
		std::array<int, 2> cmd;
		cmd[0] = CmdType;
		cmd[1] = Data;
		CmdChannel.write(cmd);
	}
	
	void MeldGameParameters(int PlayerID, int WeaponID)
	{
		std::array<int, 2> parameters;
		parameters[0] = PlayerID;
		parameters[1] = WeaponID;
		ParametersChannel.writ(parameters);
	}
	
	void GameOver()
	{
		gameover.set();
	}
	
private:

    void main()
    {
		int PlayerID;
		int WeaponID;
		int enemyID;
		int enemyWeapon;
		int cmd;
		int data;
		
        for (;;)
        {
            switch (state)
            {
            case REGISTER_GAME_PARAMETERS:
				
            	wait(ParametersChannel);
            	std::array<int, 2> parameters = ParametersChannel.read();
                PlayerID = parameters[0];
                WeaponID = parameters[1];
                state = IDLE;
                break;

            case IDLE:
            	auto evt = wait(HitChannel + CmdChannel + gameover);
            	
            	if(evt == gameover){
            		state = GAMEOVER;
            		
            	}else if(evt == HitChannel){
            		std::array<int, 2> hit = HitChannel.read();
            		enemyID = hit[0];
            		enemyWeapon = hit[1];
            		state = HIT_RECEIVED;
            		
            	}else if(evt == CmdChannel){
            		std::array<int, 2> commando = CmdChannel.read();
            		cmd = commando[0];
            		data = commando[1];
            		state = CMD_RECEIVER;
            	}
            	
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
            
            case GAMEOVER:
            	
            	break;
            }
            break;
        }
    }
};