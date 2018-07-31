#pragma once
#include <array>
#include <bitset>
#include "Actor.H"

constexpr int kFirstWeapon = 471;
constexpr int kLastWeapon = 690;
constexpr int kNumWeapons = kLastWeapon + 1 - kFirstWeapon;

enum WeaponType {
    eWTNone,
    eWTKnife,
    eWTSword,
    eWTMace,
    eWTWand,
    eWTStaff,
    eWTAxe,
    eWTSpear,
    eWTHalberd,
    eWTNinja,
    eWTKatana,
    eWTGun,
    eWT_Count
};

constexpr std::array<int,eWT_Count+1> kWeaponOffsets = {
    0, /* None*/
    471, /* Knife */
    496, /* Sword */
    536, /* Mace */
    561, /* Wand */
    566, /* Staff */
    591, /* Axe */
    606, /* Spear */
    621, /* Halberd */
    636, /* Ninja */
    651, /* Katan */
    671, /* Gun */
    691 /* Slot after last weapon */
};

constexpr int getNumWeapons();

constexpr int getNumWeapons(WeaponType wt);

constexpr int getFirstWeapon(WeaponType wt);

constexpr int getLastWeapon(WeaponType wt);

void doWeaponCopies();

/*****************************
 * INLINES
 * **************************/

constexpr int getNumWeapons() {
    return kWeaponOffsets[eWT_Count] - kWeaponOffsets[eWTKnife];
}

constexpr int getNumWeapons(WeaponType wt) {
    if (wt == eWTNone) { return 0; }
    return kWeaponOffsets[wt+1] - kWeaponOffsets[wt];
}

constexpr int getFirstWeapon(WeaponType wt) {
    return kWeaponOffsets[wt];
}

constexpr int getLastWeapon(WeaponType wt) {
    return kWeaponOffsets[wt+1]-1;
}

