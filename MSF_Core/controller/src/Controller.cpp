#include "Controller.hpp"

void Controller::initialize(const std::string& scenario_path) {
    std::cout << "[INFO] Initializing Simulation Controller" << std::endl;

    // Register All Entity Classes
    registry.register_classes();

    scf.set_scf_filepath(scenario_path);
    if (!scf.parse_scf(registry, dt)) {
        std::cerr << "[ERROR] Failed to parse SCF file: " << scf.get_scf_filepath() << std::endl;
        exit(EXIT_FAILURE);
    }

    registry.print_all_entities();

    scheduler.schedule_event(clock, [this]() {
        std::cout << "[EVENT] Scheduled Shutdown Event Triggered at t=" << clock.now() << "s" << std::endl;
        is_running = false; // Stop the main loop after this event
        shutdown();
    }, 120.0);

    // Reset simulation time to 0 seconds
    clock.reset(0.0);
}

void Controller::run() {
    // Main application loop
    while (is_running) {
        // 1) Schedule any new events requested by entities (uses ABSOLUTE sim time)
        registry.schedule_entitiy_events(scheduler, clock);

        // 2) Execute any due scheduled events at the CURRENT simulation time
        scheduler.process_events(clock);

        // 3) If paused, do not tick entities but still advance time so scheduled events can fire
        if (!is_paused) {
            // 4) Tick entities (your Entity base/derived classes should implement update(t, dt))
            const double t = clock.now();
            registry.for_each_entity([&](Entity& e) {
                e.update(t, dt);
            });
        }

        // 5) Advance simulation time deterministically (even when paused)
        clock.advance(dt);

        // Optional: wall-clock logging every ~1 second of wall time
        if (clock.get_elapsed_wall_time_ms().count() > 1000.0) {
            std::cout << "[INFO] SimTime: " << clock.now() << " s | dt: " << dt
                      << " s | Registered Entities: " << registry.get_entity_count() << std::endl;
            clock.reset_elapsed_wall_time();
        }
    }

    shutdown();
}

void Controller::shutdown() {
    std::cout << "[INFO] Shutting down Simulation Controller" << std::endl;
    registry.shutdown(); // Clean up entities
    std::cout << "[INFO] Shutdown complete for Simulation Controller" << std::endl;
    exit(EXIT_SUCCESS);
}

void Controller::pause() {
    if (!is_paused) {
        std::cout << "[INFO] Pausing Simulation" << std::endl;
        is_paused = true;
    }
}

void Controller::resume() {
    if (is_paused) {
        std::cout << "[INFO] Resuming Simulation" << std::endl;
        is_paused = false;
    }
}