#pragma once
#include "Hitlog.h"
#include <array>


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
    void sortData(std::array<Hit ,400> hits)
    {
        for (auto &hit : hits)
        {
            // if (hit.playerID == )
        }
    }

    void main()
    {
        for (;;)
        {
            switch (state)
            {
            case IDLE:
                wait(TransferHitsFlag);

                state = TRANSFERHITS;
            
                break;

            case TRANSFERHITS:

                std::array data = hitLog.getData();



                break;
            }
        }
    }
};