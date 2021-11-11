#pragma once

#include <array>

#include "DamageInfo.h"

template <unsigned int maxNumberOfWeapons>
class DamageList
{
private:

    std::array<DamageInfo, maxNumberOfWeapons> Weapons;

public:
    DamageList()
    {

        DamageInfo Weapon0("Knife", 10, 1'500'000);
        Weapons[0] = Weapon0;
        DamageInfo Weapon1("Eagle", 15, 2'000'000);
        Weapons[1] = Weapon1;
        DamageInfo Weapon2("Machine Gun", 5, 700'000);
        Weapons[2] = Weapon2;
        DamageInfo Weapon3("AUG", 5, 500'000);
        Weapons[3] = Weapon3;
        DamageInfo Weapon4("AK-47", 25, 3'500'000);
        Weapons[4] = Weapon4;
        DamageInfo Weapon5("M4A4", 20, 3'000'000);
        Weapons[5] = Weapon5;
        DamageInfo Weapon6("AWP", 40, 5'500'000);
        Weapons[6] = Weapon6;
        DamageInfo Weapon7("M249", 3, 500'000);
        Weapons[7] = Weapon7;
        DamageInfo Weapon8("Shotgun", 30, 4'000'000);
        Weapons[8] = Weapon8;
        DamageInfo Weapon9("rbg", 50, 6'500'000);
        Weapons[9] = Weapon9;
    }

    const char *GetName(int i)
    {
        if (i < maxNumberOfWeapons)
        {
            return Weapons[i].WeaponName;
        }
    }

    int GetDamage(unsigned int i)
    {
        if (i < maxNumberOfWeapons)
        {
            return Weapons[i].damage;
        }
        return 0;
    }

    int GetDelay(unsigned int i)
    {
        if (i < maxNumberOfWeapons)
        {
            return Weapons[i].delay;
        }
        return 0;
    }
};