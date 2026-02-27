#include "PhysicsEntity.hpp"
// Default implementation of update for physics integration
void PhysicsEntity::update(const double t, const double dt) {
    // Base implementation - derived classes should override this to add forces
    // For now, just integrate the current forces/torques
    
    // Simple Euler integration for translational motion
    // v = v + a*dt
    // p = p + v*dt
    velocity += acceleration * dt;
    position += velocity * dt;
    
    // Simple Euler integration for rotational motion
    // ω = ω + α*dt
    // integrate angular velocity into orientation
    angular_velocity += angular_acceleration * dt;
    
    // Clear accumulators for next iteration
    force_accumulator = Vec3(0, 0, 0);
    torque_accumulator = Vec3(0, 0, 0);
}