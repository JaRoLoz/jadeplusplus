#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <memory>
#include <optional>

#include <pugixml.hpp>

#include "exceptions.hpp"

class Resource; // forward declaration
namespace builders
{
    class Builder
    {
    public:
        Builder(const pugi::xml_node &node, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path) noexcept(false);
        Builder(const std::string_view &name, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path) noexcept;
        virtual ~Builder() {};
        virtual void build() = 0;

        constexpr const std::string &name() const { return m_name; }

    protected:
        std::string m_name;
        std::shared_ptr<std::string> m_resource_name;
        std::shared_ptr<std::filesystem::path> m_resource_path;

    protected:
        static std::string get_string_node(const pugi::xml_node &root, const char *name) noexcept(false);
        static std::optional<std::string> get_optional_string_node(const pugi::xml_node &root, const char *name) noexcept;
        static bool get_bool_node(const pugi::xml_node &root, const char *name) noexcept(false);
        static bool get_bool_enable_tag(const pugi::xml_node &root, const char *name) noexcept;
    };
}
