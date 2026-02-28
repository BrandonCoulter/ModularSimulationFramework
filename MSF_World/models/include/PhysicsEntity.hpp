#pragma once

#include "Entity.hpp"

class PhysicsEntity : public Entity {
public:
    // Implement everything from Entity, but add physics-related properties and methods
    PhysicsEntity(const std::string& name) : Entity(name) {}
    ~PhysicsEntity() override = default;

    // Override the update function to include physics updates
    std::unique_ptr<Entity> create() override = 0; 
    void update(const double t, const double dt) override;
    void shutdown() override { Entity::shutdown(); } 
    void request_event(const EventRequest& event_request) override {
        Entity::request_event(event_request); // Just call the base request_event for now
    };

protected:
    Vec3 position; // Position in 3D space
    Vec3 velocity; // Velocity vector
    Vec3 acceleration; // Acceleration vector

    Quat orientation; // Orientation as a quaternion
    Vec3 angular_velocity; // Angular velocity vector
    Vec3 angular_acceleration; // Angular acceleration vector

    double mass; // Mass of the entity
    Vec3 inertia; // Inertia tensor (simplified as a diagonal for this example)

    Vec3 force_accumulator; // Accumulated forces to be applied in the next update
    Vec3 torque_accumulator; // Accumulated torques to be applied in the next update


}; // PhysicsEntity.hpp 