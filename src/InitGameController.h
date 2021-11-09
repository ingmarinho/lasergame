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
    InitGameController(Toetsenbord4x4<1> &keypad, unsigned int priority, SendIRController &sendIRController) : rtos::task<>(priority, "INITGAME_TAAK"), KeyChannel(this, "KEY_CHANNEL"), keypad(keypad), sendIRController(sendIRController)
    {
        keypad.addListener(this);
    }

    void KeyPressed(char KeyID)
    {
        KeyChannel.write(KeyID);
    }

    // void checksum(uint16_t &check)
    // {
    //     uint16_t bit1 = 0;
    //     uint16_t bit2 = 5;
    //     uint16_t bit3 = 10;

    //     for (int i = 0; i < 5; i++)
    //     {
    //         uint16_t mf = 0x1;
    //         mf = mf << bit1;
    //         uint16_t ml = 0x1;
    //         ml = ml << bit2;
    //         uint16_t mc = 0x1;
    //         mc = mc << bit3;

    //         uint16_t res = check & mf;
    //         uint16_t res2 = check & ml;
    //         uint16_t res3 = res ^ res2;
    //         check = check | res3;

    //         bit1++;
    //         bit2++;
    //         bit3++;
    //     }
    // }

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

private:
    int speeltijd = 0;
    int countdown = 20;
    uint16_t speeltijd_cmd = 0xA << 1;
    uint16_t start_cmd = 0xB << 1;
    uint16_t masked_start_cmd;

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
                hwlib::cout << "heeeey" << hwlib::endl;
                wait(KeyChannel);

                char KeyID = KeyChannel.read();

                if (KeyID >= '0' && KeyID <= '9')
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
                else if (KeyID == '#' && speeltijd >= 1 && speeltijd <= 19)
                {

                    state = SEND_SPEELTIJD;
                    hwlib::cout << speeltijd << hwlib::endl;
                }
                break;
            }

            case SEND_SPEELTIJD:
            {
                hwlib::cout << "Ik ben nu hierrrrr" << hwlib::endl;
                char KeyID = 'B';
                uint16_t masker = speeltijd;
                masker = masker << 6;
                speeltijd_cmd = speeltijd_cmd | masker;
                checksum(speeltijd_cmd);

                hwlib::cout << "SPEELTIJD:" << speeltijd << hwlib::endl;
                while (KeyID != '#')
                {
                    wait(KeyChannel);
                    KeyID = KeyChannel.read();

                    if (KeyID == '*')
                    {
                        hwlib::cout << speeltijd_cmd << hwlib::endl;

                        sendIRController.sendMessage(speeltijd_cmd);
                    }
                }
                state = SEND_START_CMD;

                break;
            }

            case SEND_START_CMD:
            {
                char KeyID = 'B';
                uint16_t masker2 = countdown;
                masker2 = masker2 << 6;
                masked_start_cmd = start_cmd | masker2;
                checksum(masked_start_cmd);

                hwlib::cout << masked_start_cmd << hwlib::endl;

                while (KeyID != '#')
                {
                    wait(KeyChannel);
                    KeyID = KeyChannel.read();
                    if (KeyID == '*')
                    {
                        hwlib::cout << masked_start_cmd << hwlib::endl;
                        sendIRController.sendMessage(masked_start_cmd);
                        hwlib::cout << "done" << hwlib::endl;
                    }
                }
                break;
            }

            default:
            {
                hwlib::cout << "help" << hwlib::endl;
                break;
            }
            }
        }
    }
};