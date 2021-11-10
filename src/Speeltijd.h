#pragma once


class Speeltijd
{
private:
    int speeltijd = 0;

public:
    void SetGameTime(int time)
    {
        speeltijd = time;
    }

    int GetGameTime()
    {
        return speeltijd;
    }
};