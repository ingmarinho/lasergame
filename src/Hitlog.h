#pragma once 
#include <array>

struct Hit
{
    uint8_t playerID;
    uint8_t weaponID;
};

class HitLog
{

private:
    unsigned int currentIndex = 0;
    std::array<Hit, 400> hitLog;

public:
    HitLog()
    {
    }

    void meldHit(Hit hit)
    {
        if (currentIndex < 100)
        {
            hitLog[currentIndex] = hit;
            currentIndex++;
        }
    }

    std::array<Hit, 400> getData()
    {
        return hitLog;
    }
};
