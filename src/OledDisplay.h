#pragma once

class OledDisplay
{
private:
    hwlib::terminal_from &display8x8;
    hwlib::terminal_from &display16x16;
    int gametime;

public:
    OledDisplay(hwlib::terminal_from &display8x8, hwlib::terminal_from &display16x16)
        : display8x8(display8x8), display16x16(display16x16)
    {
    }

    void updateTimer(int time)
    {
        gametime = time;
    }

    void showGameStart()
    {
        display8x8
            << "\f"
            << "\n\nWaiting for game start.."
            << hwlib::flush;
        hwlib::wait_ms(100);
    }

    void showPlayerAlive(int playerID, int hitPoints, const char *weaponName)
    {
        display8x8
            << "\f"
            << "Player: " << playerID << "\n"
            << "HP: " << hitPoints << "\n"
            << "Weapon: " << weaponName << "\n"
            << "Time: " << gametime << "\n"
            << hwlib::flush;
        hwlib::wait_ms(100);
    }

    void showPlayerDead()
    {
        display16x16
            << "\f"
            << "   YOU DIED"
            << "\n"
            << hwlib::flush;
        hwlib::wait_ms(100);
    }

    void showGameEnd()
    {
        display16x16
            << "\f"
            << "   GAMEOVER"
            << "\n"
            << hwlib::flush;
        hwlib::wait_ms(100);
    }

    void showZombieMode()
    {
        display16x16
            << "\f"
            << "  ZOMBIEMODE"
            << "\n"
            << hwlib::flush;
        hwlib::wait_ms(100);
    }

    void showHit()
    {
        display16x16
            << "\f"
            << "    HIT"
            << "\n"
            << hwlib::flush;
        hwlib::wait_ms(100);
    }

    void showCountdown(unsigned int time)
    {
        display8x8
            << "\f"
            << "TIME LEFT:\n"
            << time
            << hwlib::flush;
        hwlib::wait_ms(100);
    }
};