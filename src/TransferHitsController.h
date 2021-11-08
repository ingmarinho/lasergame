#pragma once

class TransferHitsController : public rtos::task<>
{
    enum state_t
    {
        IDLE,
        TRANSFERHITS
    };

private:
    state_t state = IDLE;

    rtos::flag(TransferHitsFlag);

    Hitlog hitlog;
    auto Data[];
    TransferHitsController hits;

public:
    TransferHitsController(unsigned int priority)
    : rtos::task<>(priority, "TRANSFERHITSCONTROLLER_TASK"), hits(hits)
    {
    }

private:
    void main()
    {
        for (;;)
        {
            switch (state)
            {
                case IDLE:
                    wait(TransferHitsFlag)

                    if(TransferHitsFlag)
                    {
                        state = TRANSFERHITS;
                    }
                    break;

                case TRANSFERHITS:
                    Data = hitlog.getData;
                    break;

            }
        }
    }
};