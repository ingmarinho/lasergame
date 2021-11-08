#pragma once
#include "Toetsenbord4x4.hpp"

class InitGameController : public rtos::task<>, public KeyPadListener
{

    enum state_t
    {
        IDLE,
        GET_SPEELTIJD,
        SEND_SPEELTIJD,
        SEND_START_CMD
    };

private:
    state_t state = IDLE;

    rtos::channel<char, 1024> KeyChannel;
    Toetsenbord4x4<1> &keypad;
    SendIRController &sendIRController;

public:
    InitGameController(Toetsenbord4x4<1> &keypad, unsigned int priority, SendIRController &sendIRController) : 
    rtos::task<>(priority, "INITGAME_TAAK"), KeyChannel(this, "KEY_CHANNEL"), keypad(keypad), sendIRController(sendIRController)
    {
        keypad.addListener(this);
    }

    void KeyPressed(char KeyID)
    {
        KeyChannel.write(KeyID);
    }

    void checksum(uint16_t &check)
    {
        uint16_t f = 0;
        uint16_t l = 5;
        uint16_t c = 10;
        for (int i = 0; i < 5; i++)
        {
            uint16_t mf = 0x1;
            mf = mf << f;
            uint16_t ml = 0x1;
            ml = ml << l;
            uint16_t mc = 0x1;
            mc = mc << c;

            uint16_t res = check & mf;
            uint16_t res2 = check & ml;
            uint16_t res3 = res ^ res2;
            check = check | res3;

            f++;
            l++;
            c++;
        }
    }

private:
    int speeltijd = 0;
    int countdown = 20;
    uint16_t speeltijd_cmd = 0x14;
    uint16_t start_cmd = 0x16;
    void main()
    {

        for (;;)
        {
            switch (state)
            {
            case IDLE:
            {
                    
                wait(KeyChannel);
                char KeyID = KeyChannel.read();

                if (KeyID == 'C')
                {
                    state = GET_SPEELTIJD;
                }
                break;
            }
            
            case GET_SPEELTIJD:
            {
                hwlib::cout << 1 << hwlib::endl;
                wait(KeyChannel);

                char KeyID = KeyChannel.read();

                if (KeyID >= '1' && KeyID <= '9')
                {
                    if (speeltijd == 0)
                    {
                        speeltijd += (int)KeyID - 48;
                    }

                    else if (speeltijd == 1)
                    {

                        speeltijd = (10 + (int)KeyID - 48);
                    }
                    else
                    {

                        speeltijd = 0;
                        
                    }
                }
                else if (KeyID == '#')
                {

                    state = SEND_SPEELTIJD;
                    hwlib::cout << speeltijd << hwlib::endl;
                }
                 break;
            }
           

            case SEND_SPEELTIJD:
            {
                hwlib::cout << "120981203712093712389" << hwlib::endl;
                wait(KeyChannel);
                sendIRController.sendMessage(speeltijd_cmd);
                char KeyID = KeyChannel.read();  
                uint16_t masker = speeltijd;
                masker = masker << 6;
                speeltijd_cmd = speeltijd_cmd | masker;
                checksum(speeltijd_cmd);

                hwlib::cout << speeltijd_cmd << hwlib::endl;

                if (KeyID == '*')
                {
                    state = SEND_START_CMD;
                }
                break;
            }
            

            case SEND_START_CMD:
            {
                char KeyID = KeyChannel.read();
                uint16_t masker2 = countdown;
                masker2 = masker2 << 6;
                start_cmd = start_cmd | masker2;
                checksum(start_cmd);

                hwlib::cout << start_cmd << hwlib::endl;

                if (KeyID == '*')
                {
                    sendIRController.sendMessage(start_cmd);
                }
                break;
            }
            
            default:
                hwlib::cout << "help" << hwlib::endl;
                break;
            }
        }
    }
};