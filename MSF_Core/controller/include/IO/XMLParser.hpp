#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <memory>

/**
 * @brief Lightweight XML parsing utility for reading and navigating XML documents
 * 
 * Simple, self-contained XML parser with no external dependencies.
 * Provides interface for:
 * - Loading XML files
 * - Finding elements by name and attributes
 * - Extracting element values and attributes
 * - Iterating through child elements
 */
class XMLParser {
public:
    // Internal XML Element Node structure
    struct XMLElement {
        std::string name;
        std::string text;
        std::map<std::string, std::string> attributes;
        std::vector<std::shared_ptr<XMLElement>> children;
        
        // Find first child by name
        std::shared_ptr<XMLElement> find_child(const std::string& child_name) const;
        
        // Find all children by name
        std::vector<std::shared_ptr<XMLElement>> find_children(const std::string& child_name) const;
    };
    
    // Represents a node in the XML tree
    class XMLNode {
    public:
        XMLNode(std::shared_ptr<XMLElement> element = nullptr) : element_(element) {}
        
        // Get attribute value
        std::optional<std::string> get_attribute(const std::string& attr_name) const;
        
        // Get element text content
        std::optional<std::string> get_text() const;
        
        // Get child element by name
        XMLNode get_child(const std::string& child_name) const;
        
        // Get all child elements with a given name
        std::vector<XMLNode> get_children(const std::string& child_name) const;
        
        // Check if node is valid
        bool is_valid() const { return element_ != nullptr; }
        
        // Get element name
        std::string get_name() const;
        
    private:
        friend class XMLParser;
        std::shared_ptr<XMLElement> element_;
    };
    
public:
    XMLParser() = default;
    ~XMLParser() = default;
    
    // Load XML from file
    bool load_file(const std::string& filepath);
    
    // Load XML from string content
    bool load_string(const std::string& xml_content);
    
    // Get root element
    XMLNode get_root() const;
    
    // Find first element by name (from root)
    XMLNode find_element(const std::string& element_name) const;
    
    // Get error message if load failed
    std::string get_error() const { return error_message_; }
    
private:
    std::shared_ptr<XMLElement> root_;
    std::string error_message_;
    
    // Parsing helpers
    std::shared_ptr<XMLElement> parse_element(const std::string& content, size_t& pos);
    std::string parse_tag_name(const std::string& content, size_t& pos);
    std::map<std::string, std::string> parse_attributes(const std::string& content, size_t& pos);
    std::string parse_text_content(const std::string& content, size_t& pos);
    void skip_whitespace(const std::string& content, size_t& pos);
    void skip_comment(const std::string& content, size_t& pos);
};
