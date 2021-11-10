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
        WAIT_COUNTDOWN,
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
	int hp = 1000;
	
	void checksum(uint16_t &message)
	{
		int Bit1 = 1;
		int Bit2 = 6;
		int Bit3 = 11;
		
		uint16_t Mask1;
		uint16_t Mask2;
		uint16_t Mask3;
		
		uint16_t New1;
		uint16_t New2;
		uint16_t New3;
		
		for (int i = 0; i < 5; i++)
		{
			Mask1 = 0x1;
			Mask2 = 0x1;
			Mask3 = 0x1;
			
			Mask1 <<= Bit1;
			Mask2 <<= Bit2;
			Mask3 <<= Bit3;
			
			New1 = message & Mask1;
			
			// hwlib::cout << "NEW1:" << New1 << hwlib::endl;
			New2 = message & Mask2;
			// hwlib::cout << "NEW2:" << New2 << hwlib::endl;
			New3 = (New1 << 10) ^ (New2 << 5);
			// hwlib::cout << "XOR:" << New3 << hwlib::endl;
			message |= New3;
			// hwlib::cout << "OR:" << message << hwlib::endl;
			
			Bit1++;
			Bit2++;
			Bit3++;
		}
		hwlib::cout << message << hwlib::endl;
	}


public:
	
	// nog scherm en speaker controller toevoegen.
	
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
				hitlog.meldHit(enemyID, enemyWeapon);
				hp -= damagelist.GetDamage();
				initshotcontroller.Zombie();
				state = ZOMBIE;
				
                break;

            case CMD_RECEIVER:
            	if(cmd == 10){
            		speeltijd.SetGameTIme(data);
            		state = IDLE
            	}
            	else if(cmd == 11)
	            {
					state = WAIT_COUNTDOWN;
	            }
                break;
            	

            case WAIT_COUNTDOWN:
	            
            	while (data !< 0){
            		// laat countdown zien!!!!!!!!!!!!!!
					delay.set(1'000'000);
					wait(delay);
					data--;
	            }
            	int d = damagelist.GetDelay(WeaponID);
            	uint16_t c = PlayerID;
            	c <<= 1;
	            uint16_t masker = WeaponID;
	            masker <<= 6;
	            c |= masker;
	            checksum(c);
            	initshotcontroller.StartGame(c, d);
            	countdown.StartTimer();
            	state = IDLE;
                break;

            case ZOMBIE:
            
				delay.set(2'000'000);
				
				for(;;)
				{
					auto evt = wait(delay + HitChannel);
					
					if (evt == delay)
					{
						state = IDLE;
						break;
					}
					else
					{
						std::array<int, 2> dummy = HitChannel.read();
						
					}
				}
				
	            break;
            
            case GAMEOVER:
            	// Display Game Over!!!!!!!
            	break;
            }
            break;
        }
    }
};