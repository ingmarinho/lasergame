#pragma once

struct DamageInfo
{
    const char *weaponName = nullptr;
    int damage;
    int delay;
    DamageInfo(const char *weaponName, int damage, int delay) : weaponName(weaponName), damage(damage), delay(delay)
    {
    }
    DamageInfo()
    : DamageInfo(nullptr, 0, 0)
    {}
};