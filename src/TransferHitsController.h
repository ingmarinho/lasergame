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

public:
private:
    void main()
    {
        for (;;)
        {
        }
    }
};