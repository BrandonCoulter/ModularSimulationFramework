/*
* @file EntityRegistry.hpp
* @brief Class registers and manages entities in the Modular Simulation Framework (MSF) world.
* This class is responsible for taking models that are created and then registering them in the world
* so that they can be used by other components of the simulation. It provides functionality to add,
* remove, and retrieve entities based on their unique identifiers. The registry ensures that all
* entities are properly tracked and can be accessed efficiently during the simulation.
*
* @author Brandon Coulter
* @date 2026-02-22
*/

#pragma once

#include <memory>
#include <unordered_map>
#include <iostream>
#include <typeinfo>
#include <functional>
#include <map>

// Demangling is GCC/Clang-specific; keep it for convenience.
#include <cxxabi.h>
#include <cstdlib>

#include "Entity.hpp"
#include "Clock.hpp"
#include "Scheduler.hpp"
#include "Missile.hpp"
#include "Waypoint.hpp"

using CreateEntityFunc = std::function<std::unique_ptr<Entity>()>;

class EntityRegistry {
public:
    // Add an entity to the registry
    void register_entity(std::shared_ptr<Entity> entity) {
        entities_[entity->get_id()] = entity;

        // Set up shutdown callback so entity can notify us when it shuts down
        entity->set_shutdown_callback([this](int id) {
            std::cout << "[INFO] Entity with ID " << id
                      << " has requested shutdown. Removing from registry." << std::endl;
            this->remove_entity(id);
        });

        std::cout << "[INFO] Registered entity: ID=" << entity->get_id()
                  << ", Name='" << entity->get_name() << "'\n" << std::endl;
    }

    void register_classes() {
        entity_registry["missile"] = []() { return std::make_unique<Missile>("missile"); };
        entity_registry["waypoint"] = []() { return std::make_unique<Waypoint>("waypoint"); };
    }

    std::unique_ptr<Entity> create_entity_from_string(const std::string& class_name) {
        auto it = entity_registry.find(class_name);
        if (it != entity_registry.end()) {
            return it->second();
        } else {
            std::cerr << "[ERROR] No factory function registered for class name: " << class_name << std::endl;
            return nullptr;
        }
    }

    // Remove an entity from the registry by ID
    void remove_entity(int id) {
        entities_.erase(id);
    }

    void shutdown() {
        entities_.clear();
    }

    // Schedule any events requested by entities.
    // Convention:
    //  - EventRequest.event_time is an ABSOLUTE simulation time in SECONDS.
    //  - Scheduler delay argument is SECONDS.
    void schedule_entitiy_events(SimEventScheduler& scheduler, SimulationClock& clock) {
        for (const auto& pair : entities_) {
            const auto& entity = pair.second;

            // pending_events is expected to be a std::vector<EventRequest>
            for (const auto& event_request : entity->pending_events) {
                const double current_time = clock.now();
                const double delay = event_request.event_time - current_time;

                if (delay > 0.0) {
                    std::cout << "[INFO] Scheduling event for Entity ID " << event_request.entity_id
                              << ": " << event_request.event_description
                              << " (current: " << current_time << "s, target: " << event_request.event_time
                              << "s, delay: " << delay << "s)" << std::endl;
                    scheduler.schedule_event(clock, event_request.callback, delay);
                } else {
                    std::cout << "[WARNING] Event for Entity ID " << event_request.entity_id
                              << " requested for past time (" << event_request.event_time
                              << "s). Current time: " << current_time << "s. Skipping." << std::endl;
                }
            }

            // Clear the requested events for this entity after scheduling them
            entity->pending_events.clear();
        }
    }

    // Iterate over all entities (useful for ticking update(t, dt) from the controller)
    template <typename Fn>
    void for_each_entity(Fn&& fn) {
        for (auto& kv : entities_) {
            fn(*kv.second);
        }
    }

    // -----------------
    // GETTER FUNCTIONS
    // -----------------

    // Retrieve an entity from the registry by ID
    std::shared_ptr<Entity> get_entity(int id) const {
        auto it = entities_.find(id);
        if (it != entities_.end()) {
            return it->second;
        }
        return nullptr;
    }

    // Retrieve an entity by name
    std::shared_ptr<Entity> get_entity_by_name(const std::string& name) const {
        for (const auto& pair : entities_) {
            if (pair.second->get_name() == name) {
                return pair.second;
            }
        }
        return nullptr;
    }

    // Get the total number of registered entities
    size_t get_entity_count() const {
        return entities_.size();
    }

    // -----------------
    // DEBUGGING
    // -----------------

    void print_all_entities() const {
        std::cout << "Registered Entities:" << std::endl;
        for (const auto& pair : entities_) {
            int status = 0;
            const char* mangled_name = typeid(*pair.second).name();
            char* demangled = abi::__cxa_demangle(mangled_name, nullptr, nullptr, &status);
            std::string type_name = (status == 0 && demangled) ? demangled : mangled_name;

            std::cout << "ID: " << pair.first
                      << ", Name: " << pair.second->get_name()
                      << ", Type: " << type_name << std::endl;

            if (status == 0 && demangled) {
                free(demangled);
            }
        }
    }

private:
    std::unordered_map<int, std::shared_ptr<Entity>> entities_; // Map of entity IDs to entity instances
    std::map<std::string, CreateEntityFunc> entity_registry; // Map of entity names to factory functions
};