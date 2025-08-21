#include "builders/manifest_builder.hpp"

#include "resource.hpp"

builders::ManifestBuilder::ManifestBuilder(const pugi::xml_node &node, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path)
    : Builder{node, resource_name, resource_path}
{
    m_fx_version = Builder::get_string_node(node, "fx_version");
    m_game = Builder::get_string_node(node, "game");
    m_author = Builder::get_optional_string_node(node, "author");
    m_description = Builder::get_optional_string_node(node, "description");
    m_version = Builder::get_optional_string_node(node, "version");
    m_loadscreen = Builder::get_optional_string_node(node, "loadscreen");
    m_ui_page = Builder::get_optional_string_node(node, "ui_page");
    m_is_a_map = Builder::get_bool_enable_tag(node, "is_a_map");
    m_lua54 = Builder::get_bool_enable_tag(node, "lua54");
    m_rdr3_warning = Builder::get_optional_string_node(node, "rdr3_warning");

    pugi::xml_node client_scripts = node.child("client_scripts");
    if (client_scripts)
    {
        for (auto &child : client_scripts)
        {
            if (std::string(child.name()) != "client_script")
            {
                continue;
            }

            pugi::xml_text text = child.text();
            if (!text)
            {
                continue;
            }

            m_client_scripts.emplace_back(child.text().as_string());
        }
    }

    pugi::xml_node server_scripts = node.child("server_scripts");
    if (server_scripts)
    {
        for (auto &child : server_scripts)
        {
            if (std::string(child.name()) != "server_script")
            {
                continue;
            }

            pugi::xml_text text = child.text();
            if (!text)
            {
                continue;
            }

            m_server_scripts.emplace_back(child.text().as_string());
        }
    }

    pugi::xml_node shared_scripts = node.child("shared_scripts");
    if (shared_scripts)
    {
        for (auto &child : shared_scripts)
        {
            if (std::string(child.name()) != "shared_script")
            {
                continue;
            }

            pugi::xml_text text = child.text();
            if (!text)
            {
                continue;
            }

            m_shared_scripts.emplace_back(child.text().as_string());
        }
    }

    pugi::xml_node dependencies = node.child("dependencies");
    if (dependencies)
    {
        for (auto &child : dependencies)
        {
            if (std::string(child.name()) != "dependency")
            {
                continue;
            }

            pugi::xml_text text = child.text();
            if (!text)
            {
                continue;
            }

            m_dependencies.emplace_back(child.text().as_string());
        }
    }
}

void builders::ManifestBuilder::build()
{
    Logger::print_info(*m_resource_name, &m_name, "Generating fxmanifest.lua\n");
    std::filesystem::path outfile = *m_resource_path / "fxmanifest.lua";
    std::ofstream out(outfile.string());
    out << "fx_version \"" << m_fx_version << "\"\n";
    out << "game \"" << m_game << "\"\n";
    if (m_author)
        out << "author \"" << m_author.value() << "\"\n";
    if (m_description)
        out << "description \"" << m_description.value() << "\"\n";
    if (m_version)
        out << "version \"" << m_version.value() << "\"\n";
    if (m_loadscreen)
        out << "loadscreen \"" << m_loadscreen.value() << "\"\n";
    if (m_ui_page)
        out << "ui_page \"" << m_ui_page.value() << "\"\n";
    if (m_rdr3_warning)
        out << "rdr3_warning \"" << m_rdr3_warning.value() << "\"\n";
    if (m_is_a_map)
        out << "this_is_a_map \"yes\"\n";
    if (m_lua54)
        out << "lua54 \"yes\"\n";

    if (!m_client_scripts.empty())
    {
        out << "client_scripts {\n";
        for (auto &script : m_client_scripts)
        {
            out << "    \"" << script << "\",\n";
        }
        out << "}\n";
    }

    if (!m_server_scripts.empty())
    {
        out << "server_scripts {\n";
        for (auto &script : m_server_scripts)
        {
            out << "    \"" << script << "\",\n";
        }
        out << "}\n";
    }

    if (!m_shared_scripts.empty())
    {
        out << "shared_scripts {\n";
        for (auto &script : m_shared_scripts)
        {
            out << "    \"" << script << "\",\n";
        }
        out << "}\n";
    }

    if (!m_dependencies.empty())
    {
        out << "dependencies {\n";
        for (auto &script : m_dependencies)
        {
            out << "    \"" << script << "\",\n";
        }
        out << "}\n";
    }
}
