#pragma once

#include "PhysicsEntity.hpp"

class Missile : public PhysicsEntity {
public:
    // Constructor that initializes the Missile with a name and assigns it a unique ID
    Missile(const std::string& name) : PhysicsEntity(name) {}

    // Destructor that prints a message when a Missile object is destroyed (for debugging purposes)
    ~Missile() override {
        std::cout << "[MISSILE] Missile '" << get_name() << "' with ID " << get_id() << " destroyed." << std::endl;
    }

    std::unique_ptr<Entity> create() override {
        return std::make_unique<Missile>("missile");
    }

    void update(const double t, const double dt) override final {
        // Implement missile-specific update logic here
        if (fmod(t, 1.0) < dt) { 
            std::cout << "[MISSILE] Missile '" << get_name() << "' with ID " << get_id() << " is running t=" << t << std::endl;
        }

        // Call parent update to handle physics integration (if needed)
        PhysicsEntity::update(t, dt);
    }
    
    void shutdown() override final {
        std::cout << "[MISSILE] Shutting down Missile '" << get_name() << "' with ID " << get_id() << std::endl;
        // Call parent shutdown which will notify registry via callback
        PhysicsEntity::shutdown();
    }

private:
    // Missile-specific attributes (e.g., speed, range, etc.) can be added here 
};