#pragma once
#include <rpg_eventcommand.h>
#include "Log.H"

enum EventSpecial {
    eEventPlayer = 10001,
    eEventBoat = 10002,
    eEventShip = 10003,
    eEventAirship = 10004,
    eEventThisEvent = 10005
};

namespace Cmd {

RPG::EventCommand END();

namespace ControlSwitches {

enum SwitchOp {
    eOn = 0,
    eOff = 1,
    eToggle = 2
};

RPG::EventCommand Single(int switch_id, SwitchOp op);

RPG::EventCommand Batch(int first_switch_id, int last_switch_id, SwitchOp op);

RPG::EventCommand Variable(int var_id, SwitchOp op);

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
RPG::EventCommand Constant(int var_id, VarOp op, int constant);
RPG::EventCommand Variable(int var_id, VarOp op, int other_var_id);
RPG::EventCommand VariableID(int var_id, VarOp op, int other_var_id);
RPG::EventCommand Random(int var_id, VarOp op, int rand_min, int rand_max);
RPG::EventCommand Item(int var_id, VarOp op, int item_id, bool has_item);
RPG::EventCommand Player(int var_id, VarOp op, int actor_id, PlayerAttribute pa);
RPG::EventCommand Event(int var_id, VarOp op, int event_id, EventQuery q);
RPG::EventCommand Other(int var_id, VarOp op, VarOther vo);
}
namespace Batch {
RPG::EventCommand Constant(int first_var_id, int last_var_id, VarOp op, int constant);
RPG::EventCommand Variable(int first_var_id, int last_var_id, VarOp op, int other_var_id);
RPG::EventCommand VariableID(int first_var_id, int last_var_id, VarOp op, int other_var_id);
RPG::EventCommand Random(int first_var_id, int last_var_id, VarOp op, int rand_min, int rand_max);
RPG::EventCommand Item(int first_var_id, int last_var_id, VarOp op, int item_id, bool has_item);
RPG::EventCommand Player(int first_var_id, int last_var_id, VarOp op, int actor_id, PlayerAttribute pa);
RPG::EventCommand Event(int first_var_id, int last_var_id, VarOp op, int event_id, EventQuery q);
RPG::EventCommand Other(int first_var_id, int last_var_id, VarOp op, VarOther vo);
}
namespace Variable {
RPG::EventCommand Constant(int var_id, VarOp op, int constant);
RPG::EventCommand Variable(int var_id, VarOp op, int other_var_id);
RPG::EventCommand VariableID(int var_id, VarOp op, int other_var_id);
RPG::EventCommand Random(int var_id, VarOp op, int rand_min, int rand_max);
RPG::EventCommand Item(int var_id, VarOp op, int item_id, bool has_item);
RPG::EventCommand Player(int var_id, VarOp op, int actor_id, PlayerAttribute pa);
RPG::EventCommand Event(int var_id, VarOp op, int event_id, EventQuery q);
RPG::EventCommand Other(int var_id, VarOp op, VarOther vo);
}
} //namespace ControlVars

namespace ChangeItems {
namespace Increase {
namespace Fixed {
RPG::EventCommand Constant(int item_id, int amt);
RPG::EventCommand Variable(int item_id, int amt_var_id);
}
namespace Variable {
RPG::EventCommand Constant(int var_item_id, int amt);
RPG::EventCommand Variable(int var_item_id, int amt_var_id);
}
}
namespace Decrease {
namespace Fixed {
RPG::EventCommand Constant(int item_id, int amt);
RPG::EventCommand Variable(int item_id, int amt_var_id);
}
namespace Variable {
RPG::EventCommand Constant(int var_item_id, int amt);
RPG::EventCommand Variable(int var_item_id, int amt_var_id);
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
RPG::EventCommand Fixed(int item_id);
RPG::EventCommand Variable(int item_var_id);
} //Change
RPG::EventCommand Remove(RemoveType rt);
} //EntireParty
namespace Fixed {
namespace Change {
RPG::EventCommand Fixed(int actor_id, int item_id);
RPG::EventCommand Variable(int actor_id, int item_var_id);
} //Change
RPG::EventCommand Remove(int actor_id, RemoveType rt);
} //Fixed
namespace Variable {
namespace Change {
RPG::EventCommand Fixed(int var_id, int item_id);
RPG::EventCommand Variable(int var_id, int item_var_id);
} //Change
RPG::EventCommand Remove(int var_id, RemoveType rt);
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

RPG::EventCommand Switch(int swid, bool on, bool has_else);

namespace Variable {

RPG::EventCommand Constant(int varid, Compare cmp, int constant, bool has_else);
RPG::EventCommand Variable(int varid, Compare cmp, int other_varid, bool has_else);

} //namespace Variable
namespace Actor {

RPG::EventCommand IsInParty(int actor_id, bool has_else);

RPG::EventCommand NameIs(int actor_id, std::string name, bool has_else);

RPG::EventCommand LevelIsGE(int actor_id, int level, bool has_else);

RPG::EventCommand HPIsGE(int actor_id, int hp, bool has_else);

RPG::EventCommand SkillLearned(int actor_id, int skill_id, bool has_else);

RPG::EventCommand ItemEquipped(int actor_id, int item_id, bool has_else);

RPG::EventCommand StateInflicted(int actor_id, int state_id, bool has_else);

} //namespace Actor
} //ConditionalBranch

RPG::EventCommand Comment(std::string msg);

RPG::EventCommand Comment_2(std::string msg);

RPG::EventCommand ElseBranch();

RPG::EventCommand EndBranch();

void logCmd(int idx, const RPG::EventCommand& cmd, LogLevel ll = eLogInfo);

} //namespace cmd
