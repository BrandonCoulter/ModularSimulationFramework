#pragma once

#include "Entity.hpp"

class Missile : public Entity {
public:
    // Constructor that initializes the Missile with a name and assigns it a unique ID
    Missile(const std::string& name) : Entity(name) {}

    // Destructor that prints a message when a Missile object is destroyed (for debugging purposes)
    ~Missile() override {
        std::cout << "[DEBUG] Missile '" << get_name() << "' with ID " << get_id() << " destroyed." << std::endl;
    }

    void update(const double t, const double dt) override {
        // Implement missile-specific update logic here
        if (fmod(t, 1.0) < dt) { // Example: trigger an event every 1 second of simulation time
            std::cout << "[MISSILE] Missile '" << get_name() << "' with ID " << get_id() << " triggered an event at t=" << t << std::endl;
        }
    }

    void shutdown() override {
        std::cout << "[INFO] Shutting down Missile '" << get_name() << "' with ID " << get_id() << std::endl;
        // Call parent shutdown which will notify registry via callback
        Entity::shutdown();
    }

private:
    // Missile-specific attributes (e.g., speed, range, etc.) can be added here 
};