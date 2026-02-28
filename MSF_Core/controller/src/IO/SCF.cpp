#include "SCF.hpp"

bool SCF::parse_scf(EntityRegistry& registry, msf::SimDt& timestep) {
    if (scf_filepath.empty()) {
        std::cerr << "[ERROR] SCF file path is empty. Cannot parse SCF." << std::endl;
        return false;
    }

    // Load the file first
    if (!load_scf(scf_filepath)) {
        return false;
    }

    XMLParser::XMLNode root = parser.get_root();
    if (!root.is_valid()) {
        std::cerr << "[ERROR] SCF parse failed: XML root is invalid." << std::endl;
        return false;
    }

    // Generic Simulation Setup:
    auto simulation_setup = root.get_child("SimulationSetup");
    if (simulation_setup.is_valid()) {
        try {
            // Parse timestep and set it in the registry
            msf::SimDt dt = std::stod(simulation_setup.get_child("TimeStepInterval").get_text().value());
            // Set the simulation timestep in the registry
            timestep = dt;
            
        } catch (const std::invalid_argument& e) {
            std::cerr << "[ERROR] Invalid timestep value in SCF file: " << e.what() << std::endl;
            timestep = 0.001; // Default timestep
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Failed to parse SimulationSetup: " << e.what() << std::endl;
            return false;
        }
    } else {
        std::cerr << "[WARNING] No SimulationSetup found in SCF file. Using default settings." << std::endl;
    }

    // ENTITY PARSING LOGIC
    // Get SimulationEntities wrapper node first
    XMLParser::XMLNode entities_wrapper = root.get_child("SimulationEntities");
    if (!entities_wrapper.is_valid()) {
        std::cerr << "[WARNING] No SimulationEntities wrapper found in scenario file." << std::endl;
        return false;
    }

    // Now get SimulationEntity nodes from within the wrapper
    auto entity_nodes = entities_wrapper.get_children("SimulationEntity");
    
    // Make sure there are simulation entities found
    if (entity_nodes.empty()) {
        std::cerr << "[WARNING] No SimulationEntity elements found in SimulationEntities." << std::endl;
        return false; 
    }
    
    // Parse each SimulationEntity node to create and register entities in the registry
    for (const auto& entity_node : entity_nodes) {
        parse_entity(registry, entity_node);
    }

    return true;
}

bool SCF::load_scf(const std::string& filepath) {
    scf_filepath = filepath;
    if (!parser.load_file(filepath)) {
        std::cerr << "[ERROR] Failed to load SCF file: " << filepath << ". Error: " << parser.get_error() << std::endl;
        return false;
    } else {
        return true;
    }
}


/*
* @func parse_entity
* @param:
*  registry - Reference to the EntityRegistry where the parsed entity will be registered.
*  entity_node - An XMLNode representing a <SimulationEntity> element from the SCF file.
* @brief: Parse and create entities from SCF 
This function takes an XML node corresponding to a SimulationEntity, extracts the relevant information such as model class, emplacement data (position and orientation), entity parameters, and event triggers. It then uses this information to create an instance of the appropriate Entity subclass using the EntityRegistry's factory functions, sets its properties, and registers it with the EntityRegistry.
*/
void SCF::parse_entity(EntityRegistry& registry, const XMLParser::XMLNode& entity_node) {
    if (!entity_node.is_valid()) {
        std::cerr << "[WARNING] entity_node is invalid." << std::endl;
        return;
    }
    
    // Get entity name attribute
    auto name_attr = entity_node.get_attribute("name");
    if (!name_attr) {
        std::cerr << "[WARNING] Entity missing 'name' attribute." << std::endl;
        return;
    }
    
    // Get model class
    auto model_class = entity_node.get_child("ModelClass");
    if (model_class.is_valid()) {
        auto class_text = model_class.get_text();
        if (class_text) {
            std::cout << "[INFO] Model Class: " << class_text.value() << std::endl;
            
            // Convert to lowercase just incase (e.g., "Missile" vs "missile")
            std::string class_str_lower = class_text.value();
            std::transform(class_str_lower.begin(), class_str_lower.end(), class_str_lower.begin(), [](unsigned char c){ return std::tolower(c); });
            
            // Create entity using the registry factory function
            auto entity = registry.create_entity_from_string(class_str_lower);
            if (entity) {
                entity->set_name(name_attr.value());
                auto emplacement_data = entity_node.get_child("EmplacementData");

                // Position Parsing (Vec3)
                auto position_node = emplacement_data.get_child("position");
                if (position_node.is_valid()) {
                    try {
                    double x = std::stod(position_node.get_attribute("x").value());
                    double y = std::stod(position_node.get_attribute("y").value());
                    double z = std::stod(position_node.get_attribute("z").value());
                    
                    entity->set_position(Vec3(x, y, z));

                    } catch (const std::exception& e) {
                        std::cerr << "[WARNING] Missing x/y/z attributes in position for entity '" << name_attr.value() << "'." << std::endl;
                    }
                }

                // Orientation Parsing (Quat)
                auto orientation_node = emplacement_data.get_child("orientation");
                if (orientation_node.is_valid()) {
                    try {
                        double w = std::stod(orientation_node.get_attribute("w").value());
                        double x = std::stod(orientation_node.get_attribute("x").value());
                        double y = std::stod(orientation_node.get_attribute("y").value());
                        double z = std::stod(orientation_node.get_attribute("z").value());

                        entity->set_orientation(Quat(w, x, y, z));
                    } catch (const std::exception& e) {
                        std::cerr << "[WARNING] Missing w/x/y/z attributes in orientation for entity '" << name_attr.value() << "'." << std::endl;
                    }
                }

                // TODO:Parse EntityParameters: This is more in depth as each entity may have different parameters. We can either do a generic key-value pair parsing and store it in the entity for later use by the entity's update function, or we can try to parse specific parameters based on the model class. The former is more flexible but requires more work to implement in the entity classes, while the latter is less flexible but easier to implement.

                // It might be possible to register "namelists" as parameters and then override specific parameters in a given namelist.

                auto event_triggers_node = entity_node.get_child("EventTriggers");
                if (event_triggers_node.is_valid()) {
                    auto trigger_nodes = event_triggers_node.get_children("trigger");
                    for (const auto& trigger_node : trigger_nodes) {
                        try {
                            double time = std::stod(trigger_node.get_attribute("time").value());
                            std::string type = trigger_node.get_attribute("type").value();
                            double delay = std::stod(trigger_node.get_attribute("delay").value());

                            // TODO: Right now this only schedules a generic event with a name. But it should use the same factory paradigm to create predefined event types with specific callbacks that entities can then request when parsing the SCF.

                            entity->request_event(EventRequest{
                                .entity_id = entity->get_id(),
                                .event_time = time,
                                .event_description = type,
                                .callback = [type, name = entity->get_name()]() {
                                    std::cout << "[EVENT] Triggered event '" << type << "' for entity '" << name << "'." << std::endl;
                                }
                            });
                            
                        } catch (const std::exception& e) {
                            std::cerr << "[WARNING] Failed to parse event trigger for entity '" << name_attr.value() << "'." << std::endl;
                        }
                    }
                }

                registry.register_entity(std::move(entity));
            } else {
                std::cerr << "[ERROR] Failed to create entity of class: " << class_str_lower << std::endl;
            }
        }
    }
}