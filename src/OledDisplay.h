#pragma once

class OledDisplay : public rtos::task<>
{
private:
    hwlib::terminal_from &display;
    const char *selectedGun = "ak-47";
    unsigned int playerID = 1;
    // unsigned int timer;
    unsigned int hitPoints = 100;

public:
    OledDisplay(hwlib::terminal_from &display, unsigned int priority)
        : rtos::task<>(priority, "OLEDDISPLAY_TASK"), display(display)
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

private:
    void main()
    {
        for (;;)
        {
            display
                << "\f"
                << "Player: " << playerID << "\n" 
                << "HP: " << hitPoints << "\n"
                << "Weapon: " << selectedGun << "\n"
                << hwlib::flush;
            hwlib::wait_ms(1'000);
            display << "\f" << hwlib::flush;
            hwlib::wait_ms(1'000);
        }
    }
};