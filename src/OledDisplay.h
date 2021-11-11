#pragma once

enum displayScreens
{
    GAME_START,
    PLAYER_ALIVE,
    PLAYER_DEAD,
    GAME_END
};

class OledDisplay : public rtos::task<2000>
{
    enum state_t
    {
        IDLE,
        DISPLAYING,
        SWITCH_SCREEN
    };

private:
    state_t state = IDLE;
    displayScreens displayScreen;

    rtos::channel<displayScreens, 1024> screenChannel;
    hwlib::terminal_from &display8x8;
    hwlib::terminal_from &display16x16;
    const char *selectedGun = "ak-47";
    unsigned int playerID = 1;
    unsigned int timer;
    unsigned int hitPoints = 100;

public:
    OledDisplay(hwlib::terminal_from &display8x8, hwlib::terminal_from &display16x16, unsigned int priority)
        : rtos::task<2000>(priority, "OLEDDISPLAY_TASK"), screenChannel(this, "SCREEN_CHANNEL"), display8x8(display8x8), display16x16(display16x16)
    {
    }

    void setSelectedGun(const char *gun)
    {
        selectedGun = gun;
    }

    void setPlayerID(const int &PID)
    {
        playerID = PID;
    }

    void updateTimer(const int &time)
    {
        timer = time;
    }

    void setHitPoints(const int &HP)
    {

        hitPoints = HP;
    }

    void showDisplayScreen(displayScreens screen)
    {
        screenChannel.write(screen);
    }

private:
    void main()
    {
        displayScreens tempScreen;

        for (;;)
        {
            switch (state)
            {
            case IDLE:
                wait(screenChannel);

                displayScreen = screenChannel.read();

                if (displayScreen >= 0)
                {
                    state = DISPLAYING;
                }
                break;

            case SWITCH_SCREEN:
                hwlib::cout << "switching\n";
                displayScreen = tempScreen;
                state = DISPLAYING;
                break;

            case DISPLAYING:
                switch (displayScreen)
                {
                case GAME_START:
                    hwlib::cout << "test1\n";
                    display8x8
                        << "\f"
                        << "\n\nWaiting for game start.."
                        << hwlib::flush;
                    hwlib::wait_ms(100);

                    state = IDLE;
                    break;

                case PLAYER_ALIVE:
                hwlib::cout << "test2\n";
                    display8x8
                        << "\f"
                        << "Player: " << playerID << "\n"
                        << "HP: " << hitPoints << "\n"
                        << "Weapon: " << selectedGun << "\n"
                        << hwlib::flush;
                    hwlib::wait_ms(100);

                    state = IDLE;
                    break;

                case PLAYER_DEAD:
                    break;

                case GAME_END:
                    break;
                }
                break;
            }
        }
    }
};