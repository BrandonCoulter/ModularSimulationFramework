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
    
    if (entity_nodes.empty()) {
        std::cerr << "[WARNING] No SimulationEntity elements found in SimulationEntities." << std::endl;
        return false; 
    }
    
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
        std::cout << "[INFO] Successfully loaded SCF file: " << filepath << "\n" << std::endl;
        return true;
    }
}

std::vector<Entity> SCF::parse_entities(EntityRegistry& registry, const XMLParser::XMLNode& entities_node) {
    std::vector<Entity> entities;
    
    if (!entities_node.is_valid()) {
        std::cerr << "[WARNING] entities_node is invalid, returning empty entity list." << std::endl;
        return entities;
    }
    
    // Get all SimulationEntity child elements
    auto entity_nodes = entities_node.get_children("SimulationEntity");
    
    for (const auto& entity_node : entity_nodes) {
        parse_entity(registry, entity_node);
    }
    
    return entities;
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
    
    std::cout << "[INFO] Parsed entity: " << name_attr.value() << std::endl;
    
    // Get model class
    auto model_class = entity_node.get_child("ModelClass");
    if (model_class.is_valid()) {
        auto class_text = model_class.get_text();
        if (class_text) {
            std::cout << "[INFO] Model Class: " << class_text.value() << std::endl;
            
            // Convert to lowercase just incase (e.g., "Missile" vs "missile")
            std::string class_str_lower = class_text.value();
            std::transform(class_str_lower.begin(), class_str_lower.end(), class_str_lower.begin(), [](unsigned char c){ return std::tolower(c); });

            if (class_str_lower == "missile") {
                std::cout << "[INFO] Creating Missile entity instance." << std::endl;
                std::unique_ptr<Missile> missile = std::make_unique<Missile>(name_attr.value());
                registry.register_entity(std::move(missile));
            } else if (class_str_lower == "waypoint") {
                std::cout << "[INFO] Creating Waypoint entity instance." << std::endl;
                std::unique_ptr<Waypoint> waypoint = std::make_unique<Waypoint>(name_attr.value());
                registry.register_entity(std::move(waypoint));
            }
        }
    }
}