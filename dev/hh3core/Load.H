#pragma once
#include <string>

struct LoadArgs {
    std::string game_dir;

    std::string ini_path;
    std::string ldb_path;
    std::string lmt_path;
    std::string map_dir;
};

void loadRPG(const LoadArgs& args);
