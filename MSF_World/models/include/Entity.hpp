/*
* @file:   entity.hpp
* @lib:    msfworld_libs
* @brief:  Base class for all entities in the simulation
* The file defines the base entity class for the Modular Simulation Framework. (MSF)
* IT includes basic functionality to give IDs to entities and track them from creation
* to destruction over the whole lifecycle of the simulation.
*
* @author: Brandon Coulter
* @date:   2026-02-22
*/
#pragma once

#include <atomic>
#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include "EventRequest.hpp"
#include "Vec3.hpp"
#include "Quat.hpp"

class Entity {

public:
    // Constructor that assigns a unique ID to each entity
    Entity(const std::string& name) : entity_id(id_counter++), entity_name(name) {}

    // Virtual destructor for proper cleanup of derived classes
    virtual ~Entity() = default;

    // Get the unique ID of this entity
    int get_id() const {
        return entity_id;
    }
    
    void set_name(const std::string& new_name) {
        entity_name = new_name;
    }
    std::string get_name() const {
        return entity_name;
    }

    // Set the shutdown callback that notifies the registry when this entity shuts down
    void set_shutdown_callback(std::function<void(int)> callback) {
        shutdown_callback = callback;
    }

    // Virtual functions
    // create function
    virtual std::unique_ptr<Entity> create() = 0;
    virtual void update(const double t, const double dt) = 0; // Pure virtual function to be implemented by derived classes
    virtual void shutdown();
    virtual void request_event(const EventRequest& event_request); // Event request function
    virtual void set_position(const Vec3& new_position) = 0;
    virtual Vec3 get_position() const = 0;
    virtual void set_orientation(const Quat& new_orientation) = 0;
    virtual Quat get_orientation() const = 0;

    // Object Variables
    // Object to hold the events requested by entities
    std::vector<EventRequest> pending_events;

protected:
    static std::atomic<int> id_counter; // Static counter for generating unique IDs
    const int entity_id; // Unique ID for this entity
    std::string entity_name; // Name of the entity (optional)
    std::function<void(int)> shutdown_callback; // Callback to notify registry of shutdown

};