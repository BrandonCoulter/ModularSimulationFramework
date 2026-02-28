/*
* @file Controller.hpp
* @brief Controller class for managing the whole simulation framework
* This File defines the Controller class which serves as the main entry point for the Modular Simulation Framework. It initializes core components such as the SimulationClock, SimEventScheduler, and EntityRegistry, and manages the main application loop.
* @author Brandon Coulter
* @date 2026-02-22
*/

#pragma once
#include <iostream>
#include <atomic>
#include <string>

#include "Clock.hpp"
#include "Scheduler.hpp"
#include "EntityRegistry.hpp"

#include "Entity.hpp"
#include "Missile.hpp"
#include "SCF.hpp"

class Controller {
public:
    // Constructor initializes core components
    Controller() : clock(), scheduler(), registry() {

    }

    // Application loop entry point
    void run();
    void pause();
    void resume();

    // Simulation Initialization
    void initialize(const std::string& scenario_path);
    void shutdown();

private:
    SimulationClock clock;
    SimEventScheduler scheduler;
    EntityRegistry registry;
    SCF scf = SCF(); // Initialize SCF with reference to the entity registry

    msf::SimDt dt = 0.001; // Simulation time step (seconds)

    bool is_running = true; // Flag to track if the simulation is running
    bool is_paused = false; // Flag to track if the simulation is paused
}; // Controller class definition