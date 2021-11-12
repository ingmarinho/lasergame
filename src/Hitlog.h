#pragma once
#include <array>

/// struct to display player number and weapon type in one entry of the array
struct Hit
{
    uint8_t playerID; /// player number
    uint8_t weaponID; /// weapon type
    Hit(uint8_t PlayerID, uint8_t WeaponID)
        : playerID(PlayerID), weaponID(WeaponID)
    {
    }
    Hit()
        : Hit(0, 0)
    {
    }
};

/// entity to save all the hits reveived from other players
class HitLog
{
private:
    unsigned int currentIndex = 0;
    std::array<Hit, 400> logs; /// array of structs

public:
    HitLog()
    {
    }

    /// save one reveived hit into hitlog
    void meldHit(uint8_t playerID, uint8_t weaponID)
    {
        Hit hit(playerID, weaponID);

        if (currentIndex < 100)
        {
            logs[currentIndex] = hit;
            currentIndex++;
        }
    }

    /// get all the data from hitlog
    std::array<Hit, 400> getData()
    {
        return logs;
    }
};
