#pragma once

#include <array>

struct Hit
{
    uint8_t playerID,
    uint8_t weaponID
};

class HitLog
{

private:
    unsigned int currentIndex = 0;
    std::array<Hit, 100> hitLog;

public:
    HitLog()
    {
        hitLog.fill(0);
    }

    void meldHit(Hit hit)
    {
        if (currentIndex < 100)
        {
            hitLog[currentIndex] = hit;
            currentIndex++;
        }
    }

    const std::array& getData()
    {
        return hitLog;
    }
};

// struct playerStats
// {
//     int playerID,
//     int damage
// }

template<int playerAmount>
class TransferHitsController : public rtos::task<>
{
    enum state_t
    {
        IDLE,
        TRANSFERHITS
    };

private:
    state_t state = IDLE;

    rtos::flag TransferHitsFlag;

    HitLog &hitLog;
    // std::array<playerAmount, playerStats>;

public:
    TransferHitsController(HitLog &hitLog, unsigned int priority)
        : rtos::task<>(priority, "TRANSFERHITSCONTROLLER_TASK"), TransferHitsFlag(this, "TRANSFERHITS_FLAG"), hitLog(hitLog)
    {
        
    }

private:
    void sortData(const std::array & hits)
    {
        for (auto &hit : hits)
        {
            if (hit.playerID == )
        }
    }

    void main()
    {
        for (;;)
        {
            switch (state)
            {
            case IDLE:
                wait(TransferHitsFlag)

                if (TransferHitsFlag)
                {
                    state = TRANSFERHITS;
                }
                break;

            case TRANSFERHITS:

                std::array data = hitlog.getData();



                break;
            }
        }
    }
};