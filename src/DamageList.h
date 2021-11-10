#pragma once

#include <array>

template <unsigned int maxNumberOfWeapons>
class DamageList
{
private:
    struct info
    {
        const char *WeaponName;
        int damage;
        int delay;
        info(const char *WeaponName, int damage, int delay) : WeaponName(WeaponName), damage(damage), delay(delay) {}
    };

    std::array<info, maxNumberOfWeapons> Weapons;

public:
    DamageList()
    {

        info Weapon0("Knife", 10, 1'500'000);
        Weapons[0] = Weapon0;
        info Weapon1("Eagle", 15, 2'000'000);
        Weapons[1] = Weapon1;
        info Weapon2("Machine Gun", 5, 700'000);
        Weapons[2] = Weapon2;
        info Weapon3("AUG", 5, 500'000);
        Weapons[3] = Weapon3;
        info Weapon4("AK-47", 25, 3'500'000);
        Weapons[4] = Weapon4;
        info Weapon5("M4A4", 20, 3'000'000);
        Weapons[5] = Weapon5;
        info Weapon6("AWP", 40, 5'500'000);
        Weapons[6] = Weapon6;
        info Weapon7("M249", 3, 500'000);
        Weapons[7] = Weapon7;
        info Weapon8("Shotgun", 30, 4'000'000);
        Weapons[8] = Weapon8;
        info Weapon9("rbg", 50, 6'500'000);
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
    }

    int GetDelay(unsigned int i)
    {
        if (i < maxNumberOfWeapons)
        {
            return Weapons[i].delay;
        }
    }
};