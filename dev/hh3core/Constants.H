#pragma once

//Game Specific Constants
namespace hh3 {

enum Switch {
    eSwMAPColdSnow = 17,
    eSwMAPCustomTint = 18,
    eSwENC_NOW = 29,
    eSwMAPUnderWater = 55,
    eSwMAPIndoorWeather = 56,
    eSwTELEPORT = 81,
    eSwESCAPE = 147
};

enum Variable {
    eVaMAPRainStrength = 17,
    eVaMAPIndoorLightLevel = 18,
    eVaMapThisMap = 38,
    eVaMapPrevMap = 39,
    eVaENC_RATE = 141,
    eVaENC_ROLL = 143,
};

enum CommonEvent {
    eCeDEBUG_MODE = 6,
    eCeRestingCleanupEvent = 9,
    eCeMANUAL_SAVE = 23,
    eCeONMAP_OUT = 40,
    eCeONMAP_IN = 41,
    eCeONMAP_DUNGOUT= 42,
    eCeONMAP_DUNGIN = 43,
    eCeONMAP_WORLD = 44,
    eCePREMENU_EquipSkills = 149,
    eCePREGAME_EquipSkills = 150,
    eCeLadderUp = 192,
    eCeLadderUpStayClimbing = 193,
    eCeLadderDown = 194,
    eCeLadderDownStayClimbing = 195,
};

enum Item {
    eItLvl1Wiz = 300,
    eItLvl2Wiz = 320,
    eItLvl3Wiz = 340,
    eItLvl4Wiz = 360,
    eItLvl5Wiz = 380,
    eItLvl6Wiz = 400,
    eItWeaponStart = 471,
    eItKnives = 471,
    eItSwords = 496,
    eItMaces = 536,
    eItWands = 561,
    eItStaves = 566,
    eItAxes = 591,
    eItSpears = 606,
    eItHalberds = 621,
    eItNinjaWeapons = 636,
    eItKatanas = 651,
    eItGuns = 671,
    eItLastWeapon = 690
};

bool isKnife(int id) { return id >= eItKnives && id < eItSwords; }
bool isSword(int id) { return id >= eItSwords && id < eItMaces; }
bool isMace(int id) { return id >= eItMaces && id < eItWands; }
bool isWand(int id) { return id >= eItWands && id < eItStaves; }
bool isStaff(int id) { return id >= eItStaves && id < eItAxes; }
bool isAxe(int id) { return id >= eItAxes && id < eItSpears; }
bool isSpear(int id) { return id >= eItSpears && id < eItHalberds; }
bool isHalberd(int id) { return id >= eItHalberds && id < eItNinjaWeapons; }
bool isNinjaWeapon(int id) { return id >= eItNinjaWeapons && id < eItKatanas; }
bool isKatana(int id) { return id >= eItKatanas && id < eItGuns; }
bool isGun(int id) { return id >= eItGuns && id <= eItLastWeapon; }

enum Attributes {
    eAtSlashing = 1,
    eAtPiercing = 2,
    eAtBlunt = 3,
    eAtMissile = 4,
};

constexpr auto kNumWeapons = eItLastWeapon + 1 - eItWeaponStart;

} //namespace hh3
