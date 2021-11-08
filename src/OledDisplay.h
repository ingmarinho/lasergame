#pragma once

class OledDisplay : public rtos::task<>
{
private:
    hwlib::terminal_from &display;
    const char* selectedGun;
    unsigned int playerID;
    unsigned int timer;
    unsigned int hitPoints;

public:
    OledDisplay(hwlib::terminal_from &display)
        : display(display)
    {
    }

    void updateSelectedGun(const char* gun)
    {}

    void updatePlayerID(const int &playerID)
    {}

    void updateTimer()
    {}

    void updateHitPoints(const int & HP)
    {}

private:
    void main()
    {
        for (;;)
        {


        }
    }
};