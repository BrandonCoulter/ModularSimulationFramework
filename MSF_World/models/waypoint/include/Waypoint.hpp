#pragma once

#include "PhysicsEntity.hpp"

class Waypoint : public PhysicsEntity {
public:
    Waypoint(const std::string& name) : PhysicsEntity(name) {}
    ~Waypoint() override {
        std::cout << "[WAYPOINT] Waypoint '" << get_name() << "' with ID " << get_id() << " destroyed." << std::endl;
    }

    std::unique_ptr<Entity> create() override {
        return std::make_unique<Waypoint>("waypoint");
    }

    void update(const double t, const double dt) override {
        
    } 

    void set_waypoint(const Vec3& new_position, double tolerance_radius) {
        position = new_position;
        tolerance = tolerance_radius;
    }

    bool is_reached(const Vec3& target_position) const {
        Vec3 distance_to_target = target_position - position;
        return distance_to_target.magnitude() <= tolerance;
    }

private:
    double tolerance; // Tolerance radius for reaching the waypoint
};
