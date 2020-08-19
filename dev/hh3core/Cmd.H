#pragma once
#include <lcf/rpg/eventcommand.h>
#include "Log.H"

enum EventSpecial {
    eEventPlayer = 10001,
    eEventBoat = 10002,
    eEventShip = 10003,
    eEventAirship = 10004,
    eEventThisEvent = 10005
};

namespace Cmd {

lcf::rpg::EventCommand END();

namespace ControlSwitches {

enum SwitchOp {
    eOn = 0,
    eOff = 1,
    eToggle = 2
};

lcf::rpg::EventCommand Single(int switch_id, SwitchOp op);

lcf::rpg::EventCommand Batch(int first_switch_id, int last_switch_id, SwitchOp op);

lcf::rpg::EventCommand Variable(int var_id, SwitchOp op);

} //namespace ControlSwitches

namespace ControlVars {

enum VarOp {
    eSet = 0,
    eAdd = 1,
    eSub = 2,
    eMul = 3,
    eDiv = 4,
    eMod = 5
};

enum PlayerAttribute {
    eLevel = 0,
    eExp = 1,
    eHP = 2,
    eMP = 3,
    eMaxHP = 4,
    eMaxMP = 5,
    eAttack = 6,
    eDefense = 7,
    eMind = 8,
    eAgility = 9,
    eWeaponID = 10,
    eShieldID = 11,
    eBodyID = 12,
    eHeadID = 13,
    eAccessoryID = 14,
};

enum EventQuery {
    eMapID = 0,
    eMapX = 1,
    eMapY = 2,
    eDirection = 3,
    eScreenX = 4,
    eScreenY = 5
};

enum VarOther {
    eMoney = 0,
    eTimerRemainingTime = 1,
    ePartySize = 2,
    eSaveCount = 3,
    eBattleCount = 4,
    eWinCount = 5,
    eLossCount = 6,
    eEscapeCount = 7,
    eMIDIPlayLocationTick = 8
};

namespace Single {
lcf::rpg::EventCommand Constant(int var_id, VarOp op, int constant);
lcf::rpg::EventCommand Variable(int var_id, VarOp op, int other_var_id);
lcf::rpg::EventCommand VariableID(int var_id, VarOp op, int other_var_id);
lcf::rpg::EventCommand Random(int var_id, VarOp op, int rand_min, int rand_max);
lcf::rpg::EventCommand Item(int var_id, VarOp op, int item_id, bool has_item);
lcf::rpg::EventCommand Player(int var_id, VarOp op, int actor_id, PlayerAttribute pa);
lcf::rpg::EventCommand Event(int var_id, VarOp op, int event_id, EventQuery q);
lcf::rpg::EventCommand Other(int var_id, VarOp op, VarOther vo);
}
namespace Batch {
lcf::rpg::EventCommand Constant(int first_var_id, int last_var_id, VarOp op, int constant);
lcf::rpg::EventCommand Variable(int first_var_id, int last_var_id, VarOp op, int other_var_id);
lcf::rpg::EventCommand VariableID(int first_var_id, int last_var_id, VarOp op, int other_var_id);
lcf::rpg::EventCommand Random(int first_var_id, int last_var_id, VarOp op, int rand_min, int rand_max);
lcf::rpg::EventCommand Item(int first_var_id, int last_var_id, VarOp op, int item_id, bool has_item);
lcf::rpg::EventCommand Player(int first_var_id, int last_var_id, VarOp op, int actor_id, PlayerAttribute pa);
lcf::rpg::EventCommand Event(int first_var_id, int last_var_id, VarOp op, int event_id, EventQuery q);
lcf::rpg::EventCommand Other(int first_var_id, int last_var_id, VarOp op, VarOther vo);
}
namespace Variable {
lcf::rpg::EventCommand Constant(int var_id, VarOp op, int constant);
lcf::rpg::EventCommand Variable(int var_id, VarOp op, int other_var_id);
lcf::rpg::EventCommand VariableID(int var_id, VarOp op, int other_var_id);
lcf::rpg::EventCommand Random(int var_id, VarOp op, int rand_min, int rand_max);
lcf::rpg::EventCommand Item(int var_id, VarOp op, int item_id, bool has_item);
lcf::rpg::EventCommand Player(int var_id, VarOp op, int actor_id, PlayerAttribute pa);
lcf::rpg::EventCommand Event(int var_id, VarOp op, int event_id, EventQuery q);
lcf::rpg::EventCommand Other(int var_id, VarOp op, VarOther vo);
}
} //namespace ControlVars

namespace ChangeItems {
namespace Increase {
namespace Fixed {
lcf::rpg::EventCommand Constant(int item_id, int amt);
lcf::rpg::EventCommand Variable(int item_id, int amt_var_id);
}
namespace Variable {
lcf::rpg::EventCommand Constant(int var_item_id, int amt);
lcf::rpg::EventCommand Variable(int var_item_id, int amt_var_id);
}
}
namespace Decrease {
namespace Fixed {
lcf::rpg::EventCommand Constant(int item_id, int amt);
lcf::rpg::EventCommand Variable(int item_id, int amt_var_id);
}
namespace Variable {
lcf::rpg::EventCommand Constant(int var_item_id, int amt);
lcf::rpg::EventCommand Variable(int var_item_id, int amt_var_id);
}
}
} //namespace ChangeItems

namespace ChangeEquipment {
enum RemoveType {
    eWeapon = 0,
    eShield = 1,
    eBody = 2,
    eHead = 3,
    eAccessory = 4,
    eAll = 5
};

namespace EntireParty {
namespace Change {
lcf::rpg::EventCommand Fixed(int item_id);
lcf::rpg::EventCommand Variable(int item_var_id);
} //Change
lcf::rpg::EventCommand Remove(RemoveType rt);
} //EntireParty
namespace Fixed {
namespace Change {
lcf::rpg::EventCommand Fixed(int actor_id, int item_id);
lcf::rpg::EventCommand Variable(int actor_id, int item_var_id);
} //Change
lcf::rpg::EventCommand Remove(int actor_id, RemoveType rt);
} //Fixed
namespace Variable {
namespace Change {
lcf::rpg::EventCommand Fixed(int var_id, int item_id);
lcf::rpg::EventCommand Variable(int var_id, int item_var_id);
} //Change
lcf::rpg::EventCommand Remove(int var_id, RemoveType rt);
} //Variable
} //namespace ChangeEquipment

namespace ConditionalBranch {

enum Compare {
    eCmpEQ = 0,
    eCmpGE = 1,
    eCmpLE = 2,
    eCmpGT = 3,
    eCmpLT = 4,
    eCmpNE = 5
};

lcf::rpg::EventCommand Switch(int swid, bool on, bool has_else);

namespace Variable {

lcf::rpg::EventCommand Constant(int varid, Compare cmp, int constant, bool has_else);
lcf::rpg::EventCommand Variable(int varid, Compare cmp, int other_varid, bool has_else);

} //namespace Variable
namespace Actor {

lcf::rpg::EventCommand IsInParty(int actor_id, bool has_else);

lcf::rpg::EventCommand NameIs(int actor_id, std::string name, bool has_else);

lcf::rpg::EventCommand LevelIsGE(int actor_id, int level, bool has_else);

lcf::rpg::EventCommand HPIsGE(int actor_id, int hp, bool has_else);

lcf::rpg::EventCommand SkillLearned(int actor_id, int skill_id, bool has_else);

lcf::rpg::EventCommand ItemEquipped(int actor_id, int item_id, bool has_else);

lcf::rpg::EventCommand StateInflicted(int actor_id, int state_id, bool has_else);

} //namespace Actor
} //ConditionalBranch

lcf::rpg::EventCommand PlayBGM(std::string bgm, int fadein_ms, int volume, int tempo, int balance);

lcf::rpg::EventCommand Comment(std::string msg);

lcf::rpg::EventCommand Comment_2(std::string msg);

lcf::rpg::EventCommand ElseBranch();

lcf::rpg::EventCommand EndBranch();

void logCmd(int idx, const lcf::rpg::EventCommand& cmd, LogLevel ll = eLogInfo);

} //namespace cmd
