#include "IO/XMLParser.hpp"
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>

// XMLElement implementation
std::shared_ptr<XMLParser::XMLElement> XMLParser::XMLElement::find_child(const std::string& child_name) const {
    for (const auto& child : children) {
        if (child->name == child_name) {
            return child;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<XMLParser::XMLElement>> XMLParser::XMLElement::find_children(const std::string& child_name) const {
    std::vector<std::shared_ptr<XMLElement>> result;
    for (const auto& child : children) {
        if (child->name == child_name) {
            result.push_back(child);
        }
    }
    return result;
}

// XMLNode implementation
std::optional<std::string> XMLParser::XMLNode::get_attribute(const std::string& attr_name) const {
    if (!element_) return std::nullopt;
    
    auto it = element_->attributes.find(attr_name);
    if (it != element_->attributes.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<std::string> XMLParser::XMLNode::get_text() const {
    if (!element_) return std::nullopt;
    
    if (!element_->text.empty()) {
        return element_->text;
    }
    return std::nullopt;
}

XMLParser::XMLNode XMLParser::XMLNode::get_child(const std::string& child_name) const {
    if (!element_) return XMLNode(nullptr);
    
    auto child = element_->find_child(child_name);
    return XMLNode(child);
}

std::vector<XMLParser::XMLNode> XMLParser::XMLNode::get_children(const std::string& child_name) const {
    std::vector<XMLNode> result;
    if (!element_) return result;
    
    auto children = element_->find_children(child_name);
    for (const auto& child : children) {
        result.emplace_back(child);
    }
    return result;
}

std::string XMLParser::XMLNode::get_name() const {
    if (!element_) return "";
    return element_->name;
}

// XMLParser parsing helpers
void XMLParser::skip_whitespace(const std::string& content, size_t& pos) {
    while (pos < content.length() && std::isspace(content[pos])) {
        ++pos;
    }
}

void XMLParser::skip_comment(const std::string& content, size_t& pos) {
    if (pos + 4 < content.length() && content.substr(pos, 4) == "<!--") {
        pos += 4;
        size_t end = content.find("-->", pos);
        if (end != std::string::npos) {
            pos = end + 3;
        }
    }
}

std::string XMLParser::parse_tag_name(const std::string& content, size_t& pos) {
    skip_whitespace(content, pos);
    
    std::string name;
    while (pos < content.length() && (std::isalnum(content[pos]) || content[pos] == '_' || content[pos] == '-' || content[pos] == ':')) {
        name += content[pos];
        ++pos;
    }
    return name;
}

std::map<std::string, std::string> XMLParser::parse_attributes(const std::string& content, size_t& pos) {
    std::map<std::string, std::string> attrs;
    
    while (pos < content.length() && content[pos] != '>' && content[pos] != '/') {
        skip_whitespace(content, pos);
        
        if (content[pos] == '>' || content[pos] == '/') break;
        
        // Parse attribute name
        std::string attr_name;
        while (pos < content.length() && (std::isalnum(content[pos]) || content[pos] == '_' || content[pos] == '-' || content[pos] == ':')) {
            attr_name += content[pos];
            ++pos;
        }
        
        skip_whitespace(content, pos);
        
        // Expect '='
        if (pos >= content.length() || content[pos] != '=') {
            continue;
        }
        ++pos;
        
        skip_whitespace(content, pos);
        
        // Parse attribute value (quoted)
        std::string attr_value;
        if (pos < content.length() && (content[pos] == '"' || content[pos] == '\'')) {
            char quote = content[pos];
            ++pos;
            while (pos < content.length() && content[pos] != quote) {
                attr_value += content[pos];
                ++pos;
            }
            if (pos < content.length()) ++pos; // Skip closing quote
        }
        
        if (!attr_name.empty()) {
            attrs[attr_name] = attr_value;
        }
    }
    
    return attrs;
}

std::string XMLParser::parse_text_content(const std::string& content, size_t& pos) {
    std::string text;
    while (pos < content.length() && content[pos] != '<') {
        text += content[pos];
        ++pos;
    }
    
    // Trim whitespace
    text.erase(0, text.find_first_not_of(" \t\n\r"));
    text.erase(text.find_last_not_of(" \t\n\r") + 1);
    
    return text;
}

std::shared_ptr<XMLParser::XMLElement> XMLParser::parse_element(const std::string& content, size_t& pos) {
    skip_whitespace(content, pos);
    
    // Skip comments
    while (pos < content.length() && content.substr(pos, 4) == "<!--") {
        skip_comment(content, pos);
        skip_whitespace(content, pos);
    }
    
    if (pos >= content.length() || content[pos] != '<') {
        error_message_ = "Expected '<' at position " + std::to_string(pos);
        return nullptr;
    }
    
    ++pos; // Skip '<'
    
    // Check for closing tag or self-closing
    if (content[pos] == '/') {
        // This is a closing tag, return null to signal end
        return nullptr;
    }
    
    auto element = std::make_shared<XMLElement>();
    
    // Parse tag name
    element->name = parse_tag_name(content, pos);
    if (element->name.empty()) {
        error_message_ = "Empty tag name";
        return nullptr;
    }
    
    // Parse attributes
    element->attributes = parse_attributes(content, pos);
    
    skip_whitespace(content, pos);
    
    // Check for self-closing tag
    if (pos < content.length() && content[pos] == '/') {
        ++pos; // Skip '/'
        skip_whitespace(content, pos);
        if (pos < content.length() && content[pos] == '>') {
            ++pos; // Skip '>'
        }
        return element;
    }
    
    // Expect '>'
    if (pos >= content.length() || content[pos] != '>') {
        error_message_ = "Expected '>' at position " + std::to_string(pos);
        return nullptr;
    }
    ++pos;
    
    // Parse content until closing tag
    while (pos < content.length()) {
        skip_whitespace(content, pos);
        
        if (pos >= content.length()) break;
        
        // Check for closing tag
        if (content[pos] == '<' && pos + 1 < content.length() && content[pos + 1] == '/') {
            // This is closing tag, skip it
            ++pos; // Skip '<'
            ++pos; // Skip '/'
            
            std::string closing_name = parse_tag_name(content, pos);
            
            skip_whitespace(content, pos);
            if (pos < content.length() && content[pos] == '>') {
                ++pos;
            }
            
            if (closing_name != element->name) {
                error_message_ = "Mismatched closing tag: expected </" + element->name + ">, got </" + closing_name + ">";
                return nullptr;
            }
            
            return element;
        }
        
        // Check for comment
        if (content.substr(pos, 4) == "<!--") {
            skip_comment(content, pos);
            continue;
        }
        
        // Check for child element
        if (content[pos] == '<') {
            auto child = parse_element(content, pos);
            if (child) {
                element->children.push_back(child);
            }
        } else {
            // Text content
            std::string text = parse_text_content(content, pos);
            if (!text.empty()) {
                element->text = text;
            }
        }
    }
    
    return element;
}

// XMLParser implementation
bool XMLParser::load_file(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        error_message_ = "Failed to open file: " + filepath;
        return false;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return load_string(buffer.str());
}

bool XMLParser::load_string(const std::string& xml_content) {
    size_t pos = 0;
    
    // Skip XML declaration if present
    if (xml_content.substr(pos, 5) == "<?xml") {
        size_t decl_end = xml_content.find("?>", pos);
        if (decl_end != std::string::npos) {
            pos = decl_end + 2;
        }
    }
    
    root_ = parse_element(xml_content, pos);
    
    if (!root_) {
        if (error_message_.empty()) {
            error_message_ = "Failed to parse XML content";
        }
        return false;
    }
    
    return true;
}

XMLParser::XMLNode XMLParser::get_root() const {
    return XMLNode(root_);
}

XMLParser::XMLNode XMLParser::find_element(const std::string& element_name) const {
    XMLNode root = get_root();
    if (!root.is_valid()) return XMLNode(nullptr);
    
    return root.get_child(element_name);
}
