#pragma once
#include <string>
#include <functional>

struct EventRequest {
    int entity_id; // ID of the entity requesting the event
    double event_time; // Time at which the event should be processed (in milliseconds)
    std::string event_description; // Optional description of the event for logging
    std::function<void()> callback; // Callback function that the controller will execute
};