#include "SCF.hpp"

bool SCF::parse_scf(EntityRegistry& registry) {
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
                registry.register_entity(std::move(entity));
            } else {
                std::cerr << "[ERROR] Failed to create entity of class: " << class_str_lower << std::endl;
            }
        }
    }
}