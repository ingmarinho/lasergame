#pragma once

/// Oled display for displaying messsages, gamestats, and player stats
class OledDisplay
{
private:
    hwlib::terminal_from &display8x8; /// window for small font
    hwlib::terminal_from &display16x16; /// window for big font
    int gametime; /// gametime to show on display when playing

public:
    OledDisplay(hwlib::terminal_from &display8x8, hwlib::terminal_from &display16x16)
        : display8x8(display8x8), display16x16(display16x16)
    {
    }
    
    /// update gametime
    void updateTimer(int time)
    {
        gametime = time;
    }
    
    /// show player the program is ready for the start of the game
    void showGameStart()
    {
        display8x8
            << "\f"
            << "\n\nWaiting for game start.."
            << hwlib::flush;
        hwlib::wait_ms(100);
    }

    /// show player the player stats
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

    /// show player that he died
    void showPlayerDead()
    {
        display16x16
            << "\f"
            << "   YOU DIED"
            << "\n"
            << hwlib::flush;
        hwlib::wait_ms(100);
    }

    /// show player that the game is over
    void showGameEnd()
    {
        display16x16
            << "\f"
            << "   GAMEOVER"
            << "\n"
            << hwlib::flush;
        hwlib::wait_ms(100);
    }

    /// show player that he is a zombie because he got hit
    void showZombieMode()
    {
        display16x16
            << "\f"
            << "  ZOMBIEMODE"
            << "\n"
            << hwlib::flush;
        hwlib::wait_ms(100);
    }

    /// show player that he got hit
    void showHit()
    {
        display16x16
            << "\f"
            << "    HIT"
            << "\n"
            << hwlib::flush;
        hwlib::wait_ms(100);
    }

    /// show player the countdown has started and when the game is starting
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