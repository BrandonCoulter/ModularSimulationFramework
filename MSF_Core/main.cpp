// Main entry point for the MSF_Core application

#include <iostream>

#include "Controller.hpp"

int main() {
    std::cout << "[INFO] Starting Modular Simulation Framework" << std::endl;

    Controller controller;
    controller.initialize();
    controller.run();

    std::cout << "[INFO] Exiting Modular Simulation Framework" << std::endl;
    return 0;
}