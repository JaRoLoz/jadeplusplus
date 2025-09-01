#pragma once

#include <optional>
#include <vector>
#include <fstream>

#include "builders/builder.hpp"

namespace builders
{
    class ManifestBuilder : public Builder
    {
    public:
        inline static const std::string s_tag_name{"manifest"};

        ManifestBuilder(const pugi::xml_node &node, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path) noexcept(false);

        void build() override;

    private:
        std::string m_fx_version;
        std::string m_game;
        std::optional<std::string> m_author;
        std::optional<std::string> m_description;
        std::optional<std::string> m_version;
        std::vector<std::string> m_client_scripts;
        std::vector<std::string> m_server_scripts;
        std::vector<std::string> m_shared_scripts;
        std::vector<std::string> m_dependencies;
        std::vector<std::string> m_files;
        std::optional<std::string> m_loadscreen;
        std::optional<std::string> m_ui_page;
        bool m_is_a_map;
        bool m_lua54;
        std::optional<std::string> m_rdr3_warning;
    };
}
