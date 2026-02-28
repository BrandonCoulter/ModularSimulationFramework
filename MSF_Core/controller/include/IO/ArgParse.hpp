#pragma once

#include <string>

class ArgParse {
public:
    struct Options {
        std::string scenario_path = "Test/scenario/basic.xml";
        bool show_help = false;
    };

    static bool parse(int argc, char** argv, Options& options, std::string& error_message);
    static void print_usage(const std::string& program_name = "msf_simulation");
};
