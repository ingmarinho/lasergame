#pragma once

enum displayScreens
{
    GAME_START,
    PLAYER_ALIVE,
    PLAYER_DEAD,
    GAME_END
};

class OledDisplay : public rtos::task<>
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
    hwlib::terminal_from &display;
    const char *selectedGun = "ak-47";
    unsigned int playerID = 1;
    // unsigned int timer;
    unsigned int hitPoints = 100;

public:
    OledDisplay(hwlib::terminal_from &display, unsigned int priority)
        : rtos::task<>(priority, "OLEDDISPLAY_TASK"), screenChannel(this, "SCREEN_CHANNEL"), display(display)
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

    // void updateTimer()
    // {
    // }

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

                if (displayScreen)
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
                    display
                        << "\f"
                        << "\n\nWaiting for game start.."
                        << hwlib::flush;
                    hwlib::wait_ms(100);

                    tempScreen = screenChannel.read();
                    if (tempScreen)
                    {
                        state = SWITCH_SCREEN;
                    }

                    break;

                case PLAYER_ALIVE:
                    display
                        << "\f"
                        << "Player: " << playerID << "\n"
                        << "HP: " << hitPoints << "\n"
                        << "Weapon: " << selectedGun << "\n"
                        << hwlib::flush;
                    hwlib::wait_ms(100);

                    tempScreen = screenChannel.read();
                    if (tempScreen)
                    {
                        state = SWITCH_SCREEN;
                    }
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