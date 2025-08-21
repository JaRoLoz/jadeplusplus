#include "builders/builder.hpp"

#include "resource.hpp"

builders::Builder::Builder(const pugi::xml_node &node, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path)
    : m_resource_name{resource_name}, m_resource_path{resource_path}
{
    pugi::xml_attribute name = node.attribute("name");
    if (!name)
    {
        throw ConfigException("Error parsing build step: 'name' attribute misssing");
    }
    m_name = name.as_string();
}

builders::Builder::Builder(const std::string_view &name, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path) noexcept
    : m_name{name}, m_resource_name{resource_name}, m_resource_path{resource_path}
{
}

std::string builders::Builder::get_string_node(const pugi::xml_node &root, const char *name) noexcept(false)
{
    pugi::xml_node node = root.child(name);
    if (!node)
    {
        throw ConfigException(std::string{"Error parsing build step: '"} + name + std::string{"' node missing"});
    }

    pugi::xml_text text = node.text();
    if (!text)
    {
        throw ConfigException(std::string{"Error parsing build step: '"} + name + std::string{"' node is empty"});
    }

    return text.as_string();
}

std::optional<std::string> builders::Builder::get_optional_string_node(const pugi::xml_node &root, const char *name) noexcept
{
    pugi::xml_node node = root.child(name);
    if (!node)
    {
        return std::nullopt;
    }

    pugi::xml_text text = node.text();
    if (!text)
    {
        return std::nullopt;
    }

    return text.as_string();
}

bool builders::Builder::get_bool_node(const pugi::xml_node &root, const char *name) noexcept(false)
{
    pugi::xml_node node = root.child(name);
    if (!node)
    {
        throw ConfigException(std::string{"Error parsing build step: '"} + name + std::string{"' node missing"});
    }

    pugi::xml_text text = node.text();
    if (!text)
    {
        throw ConfigException(std::string{"Error parsing build step: '"} + name + std::string{"' node is empty"});
    }

    return std::string{text.as_string()} == "true";
}

bool builders::Builder::get_bool_enable_tag(const pugi::xml_node &root, const char *name) noexcept
{
    pugi::xml_node node = root.child(name);
    if (!node)
    {
        return false;
    }

    pugi::xml_attribute attr = node.attribute("enable");
    if (!attr)
    {
        return false;
    }

    return std::string{attr.as_string()} == "true";
}
