#pragma once

/// entity to link weapon type to damage and reload time
struct DamageInfo
{
    const char *weaponName = nullptr; /// weapon type
    int damage; /// damage
    int delay; /// reload time
    DamageInfo(const char *weaponName, int damage, int delay) : weaponName(weaponName), damage(damage), delay(delay)
    {
    }
    DamageInfo()
    : DamageInfo(nullptr, 0, 0)
    {}
};