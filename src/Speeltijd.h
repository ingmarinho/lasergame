#pragma once

/// entity to save the gametime
class Speeltijd
{
private:
    int speeltijd = 0; /// gametime

public:

    /// update gametime
    void SetGameTime(int time)
    {
        speeltijd = time;
    }

    /// get game time
    int GetGameTime()
    {
        return speeltijd;
    }
};