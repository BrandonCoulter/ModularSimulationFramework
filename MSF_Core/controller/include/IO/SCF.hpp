/*
* @file: SCFParser.hpp
* @lib: msfcore_libs
* @brief: Header file for the SCFParser class, which is responsible for parsing Simulation Configuration Files (SCF) in the Modular Simulation Framework (MSF).
* The SCFParser class provides functionality to read and interpret SCF files, which define the initial conditions, entities, and events for a simulation run. It converts the SCF data into a format that can be used by the Controller to set up and execute the simulation according to the specified parameters.
* author: Brandon Coulter
*/

#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Entity.hpp"
#include "EntityRegistry.hpp"
#include "EventRequest.hpp"
#include "Missile.hpp"
#include "SimTime.hpp"
#include "Waypoint.hpp"
#include "XMLParser.hpp"

#include "Quat.hpp"
#include "Vec3.hpp"

// Include necessary headers for file I/O, xml parsing, string manipulation, and data structures

class SCF {
public:
    SCF() = default;
    SCF(const std::string& filepath) : scf_filepath(filepath) {}

    ~SCF() = default;

    bool parse_scf(EntityRegistry& registry, msf::SimDt& timestep);
    bool load_scf(const std::string& filepath);

    void parse_entity(EntityRegistry& registry, const XMLParser::XMLNode& entity_node);

    // Getters and Setters
    void set_scf_filepath(const std::string& filepath) {
        scf_filepath = filepath;
    }
    std::string get_scf_filepath() const {
        return scf_filepath;
    }

protected:

private:
    // Private Variables
    std::string scf_filepath;
    XMLParser parser;
};