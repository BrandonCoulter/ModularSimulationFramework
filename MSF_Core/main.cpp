// Main entry point for the MSF_Core application

#include <cstdlib>
#include <iostream>
#include <string>

#include "ArgParse.hpp"
#include "Controller.hpp"

int main(int argc, char** argv) {
    std::cout << "[INFO] Starting Modular Simulation Framework" << std::endl;

    ArgParse::Options options;
    std::string parse_error;
    if (!ArgParse::parse(argc, argv, options, parse_error)) {
        std::cerr << "[ERROR] " << parse_error << std::endl;
        ArgParse::print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (options.show_help) {
        ArgParse::print_usage(argv[0]);
        return EXIT_SUCCESS;
    }

    Controller controller;
    controller.initialize(options.scenario_path);
    controller.run();

    std::cout << "[INFO] Exiting Modular Simulation Framework" << std::endl;
    return 0;
}