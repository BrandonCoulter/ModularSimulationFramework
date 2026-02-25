#include "Controller.hpp"

void Controller::initialize() {
    std::cout << "[INFO] Initializing Simulation Controller" << std::endl;

    // Reset simulation time to 0 seconds
    clock.reset(0.0);

    // Temp: Register some entities for testing
    registry.register_entity(std::make_shared<Missile>("Missile1"));

    // NOTE: schedule_event delays are now in SECONDS (simulation time)
    scheduler.schedule_event(clock, [this]() {
        registry.print_all_entities();
    }, 3.0); // Print after 3 seconds

    scheduler.schedule_event(clock, [this]() {
        std::cout << "[EVENT] Scheduled Pause Event Triggered at t=" << clock.now() << "s" << std::endl;
        pause();
    }, 5.0); // Pause after 5 seconds

    scheduler.schedule_event(clock, [this]() {
        std::cout << "[EVENT] Scheduled Resume Event Triggered at t=" << clock.now() << "s" << std::endl;
        resume();
    }, 8.0); // Resume after 8 seconds

    scheduler.schedule_event(clock, [this]() {
        auto missile = registry.get_entity_by_name("Missile1");
        if (!missile) {
            std::cout << "[WARNING] Could not retrieve entity by name" << std::endl;
            return;
        }

        // Request an entity-owned event at an ABSOLUTE simulation time (seconds)
        const double target_time = clock.now() + 5.0; // 5 seconds in the future
        missile->request_event(EventRequest{
            missile->get_id(),
            target_time,
            "Test Event from Missile1",
            [missile]() {
                std::cout << "\n[EVENT] Executing event callback for " << missile->get_name()
                          << " with ID " << missile->get_id() << "\n" << std::endl;
                missile->shutdown();
            }
        });
    }, 10.0);

    scheduler.schedule_event(clock, [this]() {
        std::cout << "[EVENT] Scheduled Shutdown Event Triggered at t=" << clock.now() << "s" << std::endl;
        is_running = false; // Stop the main loop after this event
    }, 120.0);
}

void Controller::run() {
    // Fixed-step simulation tick (seconds). Start with 100 Hz.
    constexpr double dt = 0.001;

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