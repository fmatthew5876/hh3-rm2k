#include "Tools.H"

void dispatch(const Args& args) {
    auto& tool_args = args.tool_args;
    if (std::holds_alternative<SearchTextArgs>(tool_args)) {
        doSearchText(std::get<SearchTextArgs>(tool_args));
    }
}
