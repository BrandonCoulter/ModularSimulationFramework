/*
* @file:   Entity.cpp
* @lib:    msfworld_libs
* @brief:  Implementation of Entity class static members
*
* @author: Brandon Coulter
* @date:   2026-02-22
*/

#include "Entity.hpp"

// Define the static member variable
std::atomic<int> Entity::id_counter(0);

// Default implementation of shutdown
void Entity::shutdown() {
    // Default implementation - can be overridden by derived classes
    pending_events.clear();
    
    // Notify the registry that this entity is shutting down
    if (shutdown_callback) {
        shutdown_callback(entity_id);
    }
}

// Default implementation of request_event
void Entity::request_event(const EventRequest& event_request) {
    // Store the event request for processing by the controller
    pending_events.push_back(event_request);
}
