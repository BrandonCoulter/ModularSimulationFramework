#include "ArgParse.hpp"

#include <iostream>

bool ArgParse::parse(int argc, char** argv, Options& options, std::string& error_message) {
    if (argc <= 0 || argv == nullptr) {
        error_message = "Invalid process arguments.";
        return false;
    }

    for (int arg_index = 1; arg_index < argc; ++arg_index) {
        std::string argument = argv[arg_index];

        if (argument == "-h" || argument == "--help") {
            options.show_help = true;
            return true;
        }

        if (argument == "-s" || argument == "--scenario") {
            if (arg_index + 1 >= argc) {
                error_message = "Missing value for " + argument;
                return false;
            }

            options.scenario_path = argv[++arg_index];
            continue;
        }

        const std::string scenario_prefix = "--scenario=";
        if (argument.rfind(scenario_prefix, 0) == 0) {
            options.scenario_path = argument.substr(scenario_prefix.size());
            if (options.scenario_path.empty()) {
                error_message = "Missing value for --scenario.";
                return false;
            }
            continue;
        }

        error_message = "Unknown argument: " + argument;
        return false;
    }

    return true;
}

void ArgParse::print_usage(const std::string& program_name) {
    std::cout << "Usage: " << program_name << " [--scenario <path>]\n"
              << "Options:\n"
              << "  -s, --scenario <path>  Path to scenario XML file\n"
              << "      --scenario=<path>  Path to scenario XML file\n"
              << "  -h, --help             Show this help message\n";
}
