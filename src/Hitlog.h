#pragma once
#include <array>

struct Hit
{
    uint8_t playerID;
    uint8_t weaponID;
    Hit(uint8_t PlayerID, uint8_t WeaponID)
        : playerID(PlayerID), weaponID(WeaponID)
    {
    }
    Hit()
        : Hit(0, 0)
    {
    }
};

class HitLog
{
private:
    unsigned int currentIndex = 0;
    std::array<Hit, 400> logs;

public:
    HitLog()
    {
    }

    void meldHit(uint8_t playerID, uint8_t weaponID)
    {
        Hit hit(playerID, weaponID);

        if (currentIndex < 100)
        {
            logs[currentIndex] = hit;
            currentIndex++;
        }
    }

    std::array<Hit, 400> getData()
    {
        return logs;
    }
};
