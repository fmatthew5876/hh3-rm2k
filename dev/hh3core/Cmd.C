#include "Cmd.H"
#include "Log.H"
#include "Code.H"
#include "Exception.H"

namespace Cmd {

RPG::EventCommand END() {
    RPG::EventCommand cmd;
    cmd.code = RPG::EventCommand::Code::END;
    return cmd;
}

static RPG::EventCommand makeControlSwitches(int sub, int arg1, int arg2, ControlSwitches::SwitchOp op) {
    RPG::EventCommand cmd;
    cmd.code = RPG::EventCommand::Code::ControlSwitches;
    cmd.parameters.resize(4, 0);
    cmd.parameters[0] = sub;
    cmd.parameters[1] = arg1;
    cmd.parameters[2] = arg2;
    cmd.parameters[3] = int(op);

    return cmd;
}

RPG::EventCommand ControlSwitches::Single(int switch_id, SwitchOp op) {
    return makeControlSwitches(0, switch_id, switch_id, op);
}

RPG::EventCommand ControlSwitches::Batch(int first_switch_id, int last_switch_id, SwitchOp op) {
    return makeControlSwitches(1, first_switch_id, last_switch_id, op);
}

RPG::EventCommand ControlSwitches::Variable(int var_id, SwitchOp op) {
    return makeControlSwitches(2, var_id, 0, op);
}


static RPG::EventCommand makeControlVars(int select, int var1, int var2, ControlVars::VarOp op,
        int arg1, int arg2, int arg3) {
    RPG::EventCommand cmd;
    cmd.code = RPG::EventCommand::Code::ControlVars;
    cmd.parameters.resize(7, 0);
    cmd.parameters[0] = select;
    cmd.parameters[1] = var1;
    cmd.parameters[2] = var2;
    cmd.parameters[3] = int(op);
    cmd.parameters[4] = arg1;
    cmd.parameters[5] = arg2;
    cmd.parameters[6] = arg3;

    return cmd;
}

RPG::EventCommand ControlVars::Single::Constant(int var_id, VarOp op, int constant) {
    return makeControlVars(0, var_id, var_id, op, 0, constant, 0);
}
RPG::EventCommand ControlVars::Single::Variable(int var_id, VarOp op, int other_var_id) {
    return makeControlVars(0, var_id, var_id, op, 1, other_var_id, 0);
}
RPG::EventCommand ControlVars::Single::VariableID(int var_id, VarOp op, int other_var_id) {
    return makeControlVars(0, var_id, var_id, op, 2, other_var_id, 0);
}
RPG::EventCommand ControlVars::Single::Random(int var_id, VarOp op, int rand_min, int rand_max) {
    return makeControlVars(0, var_id, var_id, op, 3, rand_min, rand_max);
}
RPG::EventCommand ControlVars::Single::Item(int var_id, VarOp op, int item_id, bool has_item) {
    return makeControlVars(0, var_id, var_id, op, 4, item_id, !has_item);
}

RPG::EventCommand ControlVars::Single::Player(int var_id, VarOp op, int actor_id, PlayerAttribute pa) {
    return makeControlVars(0, var_id, var_id, op, 5, actor_id, int(pa));
}

RPG::EventCommand ControlVars::Single::Event(int var_id, VarOp op, int event_id, EventQuery q) {
    return makeControlVars(0, var_id, var_id, op, 6, event_id, int(q));
}

RPG::EventCommand ControlVars::Single::Other(int var_id, VarOp op, VarOther vo) {
    return makeControlVars(0, var_id, var_id, op, 7, int(vo), 0);
}

RPG::EventCommand ControlVars::Batch::Constant(int first_var_id, int last_var_id, VarOp op, int constant) {
    return makeControlVars(1, first_var_id, last_var_id, op, 0, constant, 0);
}
RPG::EventCommand ControlVars::Batch::Variable(int first_var_id, int last_var_id, VarOp op, int other_var_id) {
    return makeControlVars(1, first_var_id, last_var_id, op, 1, other_var_id, 0);
}
RPG::EventCommand ControlVars::Batch::VariableID(int first_var_id, int last_var_id, VarOp op, int other_var_id) {
    return makeControlVars(1, first_var_id, last_var_id, op, 2, other_var_id, 0);
}
RPG::EventCommand ControlVars::Batch::Random(int first_var_id, int last_var_id, VarOp op, int rand_min, int rand_max) {
    return makeControlVars(1, first_var_id, last_var_id, op, 3, rand_min, rand_max);
}
RPG::EventCommand ControlVars::Batch::Item(int first_var_id, int last_var_id, VarOp op, int item_id, bool has_item) {
    return makeControlVars(1, first_var_id, last_var_id, op, 4, item_id, !has_item);
}
RPG::EventCommand ControlVars::Batch::Player(int first_var_id, int last_var_id, VarOp op, int actor_id, PlayerAttribute pa) {
    return makeControlVars(1, first_var_id, last_var_id, op, 5, actor_id, int(pa));
}
RPG::EventCommand ControlVars::Batch::Event(int first_var_id, int last_var_id, VarOp op, int event_id, EventQuery q) {
    return makeControlVars(1, first_var_id, last_var_id, op, 6, event_id, int(q));
}
RPG::EventCommand ControlVars::Batch::Other(int first_var_id, int last_var_id, VarOp op, VarOther vo) {
    return makeControlVars(1, first_var_id, last_var_id, op, 7, int(vo), 0);
}

RPG::EventCommand ControlVars::Variable::Constant(int var_id, VarOp op, int constant) {
    return makeControlVars(2, var_id, 0, op, 0, constant, 0);
}
RPG::EventCommand ControlVars::Variable::Variable(int var_id, VarOp op, int other_var_id) {
    return makeControlVars(2, var_id, 0, op, 1, other_var_id, 0);
}
RPG::EventCommand ControlVars::Variable::VariableID(int var_id, VarOp op, int other_var_id) {
    return makeControlVars(2, var_id, 0, op, 2, other_var_id, 0);
}
RPG::EventCommand ControlVars::Variable::Random(int var_id, VarOp op, int rand_min, int rand_max) {
    return makeControlVars(2, var_id, 0, op, 3, rand_min, rand_max);
}
RPG::EventCommand ControlVars::Variable::Item(int var_id, VarOp op, int item_id, bool has_item) {
    return makeControlVars(2, var_id, 0, op, 4, item_id, !has_item);
}

RPG::EventCommand ControlVars::Variable::Player(int var_id, VarOp op, int actor_id, PlayerAttribute pa) {
    return makeControlVars(2, var_id, 0, op, 5, actor_id, int(pa));
}

RPG::EventCommand ControlVars::Variable::Event(int var_id, VarOp op, int event_id, EventQuery q) {
    return makeControlVars(2, var_id, 0, op, 6, event_id, int(q));
}

RPG::EventCommand ControlVars::Variable::Other(int var_id, VarOp op, VarOther vo) {
    return makeControlVars(2, var_id, 0, op, 7, int(vo), 0);
}

static RPG::EventCommand makeChangeItems(int incdec, int fv1, int item, int fv2, int amt) {
    RPG::EventCommand cmd;
    cmd.code = RPG::EventCommand::Code::ChangeItems;
    cmd.parameters = { incdec, fv1, item, fv2, amt };
    return cmd;
}

RPG::EventCommand ChangeItems::Increase::Fixed::Constant(int item_id, int amt) {
    return makeChangeItems(0, 0, item_id, 0, amt);
}
RPG::EventCommand ChangeItems::Increase::Fixed::Variable(int item_id, int amt_var_id) {
    return makeChangeItems(0, 0, item_id, 1, amt_var_id);
}
RPG::EventCommand ChangeItems::Increase::Variable::Constant(int var_item_id, int amt) {
    return makeChangeItems(0, 1, var_item_id, 0, amt);
}
RPG::EventCommand ChangeItems::Increase::Variable::Variable(int var_item_id, int amt_var_id) {
    return makeChangeItems(0, 1, var_item_id, 1, amt_var_id);
}
RPG::EventCommand ChangeItems::Decrease::Fixed::Constant(int item_id, int amt) {
    return makeChangeItems(1, 0, item_id, 0, amt);
}
RPG::EventCommand ChangeItems::Decrease::Fixed::Variable(int item_id, int amt_var_id) {
    return makeChangeItems(1, 0, item_id, 1, amt_var_id);
}
RPG::EventCommand ChangeItems::Decrease::Variable::Constant(int var_item_id, int amt) {
    return makeChangeItems(1, 1, var_item_id, 0, amt);
}
RPG::EventCommand ChangeItems::Decrease::Variable::Variable(int var_item_id, int amt_var_id) {
    return makeChangeItems(1, 1, var_item_id, 1, amt_var_id);
}


static RPG::EventCommand makeChangeEquipment(int select, int which, int addrem, int arg1, int arg2) {
    RPG::EventCommand cmd;
    cmd.code = RPG::EventCommand::Code::ChangeEquipment;
    cmd.parameters = { select, which, addrem, arg1, arg2 };
    return cmd;
}

RPG::EventCommand ChangeEquipment::EntireParty::Change::Fixed(int item_id) {
    return makeChangeEquipment(0, 0, 0, 0, item_id);
}
RPG::EventCommand ChangeEquipment::EntireParty::Change::Variable(int item_var_id) {
    return makeChangeEquipment(0, 0, 0, 1, item_var_id);
}
RPG::EventCommand ChangeEquipment::EntireParty::Remove(RemoveType rt) {
    return makeChangeEquipment(0, 0, 1, int(rt), 0);
}
RPG::EventCommand ChangeEquipment::Fixed::Change::Fixed(int actor_id, int item_id) {
    return makeChangeEquipment(1, actor_id, 0, 0, item_id);
}
RPG::EventCommand ChangeEquipment::Fixed::Change::Variable(int actor_id, int item_var_id) {
    return makeChangeEquipment(1, actor_id, 0, 1, item_var_id);
}
RPG::EventCommand ChangeEquipment::Fixed::Remove(int actor_id, RemoveType rt) {
    return makeChangeEquipment(1, actor_id, 1, int(rt), 0);
}
RPG::EventCommand ChangeEquipment::Variable::Change::Fixed(int var_id, int item_id) {
    return makeChangeEquipment(2, var_id, 0, 1, item_id);
}
RPG::EventCommand ChangeEquipment::Variable::Change::Variable(int var_id, int item_var_id) {
    return makeChangeEquipment(2, var_id, 0, 1, item_var_id);
}
RPG::EventCommand ChangeEquipment::Variable::Remove(int var_id, RemoveType rt) {
    return makeChangeEquipment(2, var_id, 1, int(rt), 0);
}

static RPG::EventCommand makeConditionalBranch(bool has_else) {
    RPG::EventCommand cmd;
    cmd.code = RPG::EventCommand::Code::ConditionalBranch;
    cmd.parameters.resize(6, 0);
    cmd.parameters[5] = has_else;

    return cmd;
}

RPG::EventCommand ConditionalBranch::Switch(int swid, bool on, bool has_else) {
    auto cmd = makeConditionalBranch(has_else);
    cmd.parameters[1] = swid;
    cmd.parameters[2] = !on;

    return cmd;
}

RPG::EventCommand ConditionalBranch::Variable::Constant(int varid, Compare cmp, int constant, bool has_else) {
    auto cmd = makeConditionalBranch(has_else);
    cmd.parameters[0] = 1;
    cmd.parameters[1] = varid;
    cmd.parameters[2] = 0;
    cmd.parameters[3] = constant;
    cmd.parameters[4] = int(cmp);

    return cmd;
}

RPG::EventCommand ConditionalBranch::Variable::Variable(int varid, Compare cmp, int other_varid, bool has_else) {
    auto cmd = makeConditionalBranch(has_else);
    cmd.parameters[0] = 1;
    cmd.parameters[1] = varid;
    cmd.parameters[2] = 1;
    cmd.parameters[3] = other_varid;
    cmd.parameters[4] = int(cmp);

    return cmd;
}

static RPG::EventCommand makeConditionalBranchActor(int actor_id, int subcmd, int arg, bool has_else) {
    auto cmd = makeConditionalBranch(has_else);
    cmd.parameters[0] = 5;
    cmd.parameters[1] = actor_id;
    cmd.parameters[2] = subcmd;
    cmd.parameters[3] = arg;
    return cmd;
}

RPG::EventCommand ConditionalBranch::Actor::IsInParty(int actor_id, bool has_else) {
    return makeConditionalBranchActor(actor_id, 0, 0, has_else);
}

RPG::EventCommand ConditionalBranch::Actor::NameIs(int actor_id, std::string name, bool has_else) {
    auto cmd = makeConditionalBranchActor(actor_id, 1, 0, has_else);
    cmd.string = std::move(name);
    return cmd;
}

RPG::EventCommand ConditionalBranch::Actor::LevelIsGE(int actor_id, int level, bool has_else) {
    return makeConditionalBranchActor(actor_id, 2, level, has_else);
}

RPG::EventCommand ConditionalBranch::Actor::HPIsGE(int actor_id, int hp, bool has_else) {
    return makeConditionalBranchActor(actor_id, 3, hp, has_else);
}

RPG::EventCommand ConditionalBranch::Actor::SkillLearned(int actor_id, int skill_id, bool has_else) {
    return makeConditionalBranchActor(actor_id, 4, skill_id, has_else);
}

RPG::EventCommand ConditionalBranch::Actor::ItemEquipped(int actor_id, int item_id, bool has_else) {
    return makeConditionalBranchActor(actor_id, 5, item_id, has_else);
}

RPG::EventCommand ConditionalBranch::Actor::StateInflicted(int actor_id, int state_id, bool has_else) {
    return makeConditionalBranchActor(actor_id, 6, state_id, has_else);
}

RPG::EventCommand PlayBGM(std::string bgm, int fadein_ms, int volume, int tempo, int balance) {
    RPG::EventCommand cmd;
    cmd.code = RPG::EventCommand::Code::PlayBGM;
    cmd.parameters = { fadein_ms, volume, tempo, balance };
    cmd.string = std::move(bgm);
    return cmd;
}

RPG::EventCommand Comment(std::string msg) {
    RPG::EventCommand cmd;
    cmd.code = RPG::EventCommand::Code::Comment;
    cmd.string = std::move(msg);
    return cmd;
}

RPG::EventCommand Comment_2(std::string msg) {
    RPG::EventCommand cmd;
    cmd.code = RPG::EventCommand::Code::Comment_2;
    cmd.string = std::move(msg);
    return cmd;
}


RPG::EventCommand ElseBranch() {
    RPG::EventCommand cmd;
    cmd.code = RPG::EventCommand::Code::ElseBranch;
    return cmd;
}

RPG::EventCommand EndBranch() {
    RPG::EventCommand cmd;
    cmd.code = RPG::EventCommand::Code::EndBranch;
    return cmd;
}

void logCmd(int idx, const RPG::EventCommand& cmd, LogLevel ll) {
    logLvlStart(ll);
    for(int i = 0; i < cmd.indent; ++i) {
        logLvlRaw(ll, ' ');
    }
    logLvlRaw(ll, "Cmd (", idx, ") : code=", Code(cmd.code), " indent=", cmd.indent, " str=\"", cmd.string, "\" params=");
    char sep = '[';
    if(cmd.parameters.empty()) {
        logLvlRaw(ll, sep);
    } else {
        for (auto& p: cmd.parameters) {
            logLvlRaw(ll, sep, p);
            sep = ' ';
        }
    }
    logLvlEnd(ll, ']');
}



} //namespace Cmd
