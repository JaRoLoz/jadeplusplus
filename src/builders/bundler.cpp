#include "builders/bundler.hpp"

#include "resource.hpp"

const boost::basic_regex<char> builders::Bundler::s_require_regex("require\\(\"([^\"]+)\"\\)");

const std::string builders::Bundler::s_bundle_template = R"(--[[Generated with Jade (https://github.com/JaRoLoz)]]
local ____bundle__dict, ____bundle__cache = {}, {}
require = function(module)
    if ____bundle__cache[module] then
        return ____bundle__cache[module]
    end
    local module_func = ____bundle__dict[module]()
    ____bundle__cache[module] = module_func
    return module_func
end
)";

std::string builders::Bundler::DependencyNode::get_contents()
{
    std::ifstream file(m_path.c_str());
    if (!file)
    {
        throw BuildException("Failed to open file: " + m_path.string());
    }

    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}

void builders::Bundler::DependencyNode::scan_dependencies(
    const std::filesystem::path &base_path,
    std::unordered_map<std::string, DependencyNode> &dependencies)
{
    std::string contents = get_contents();
    boost::cmatch matches;
    boost::sregex_iterator m1(contents.begin(), contents.end(), s_require_regex);
    boost::sregex_iterator m2;

    for (auto it = m1; it != m2; it++)
    {
        auto &match = *it;
        std::string module_name = match[1].str();

        if (dependencies.contains(module_name))
        {
            continue;
        }

        std::string module_name_path = module_name;
        std::replace(module_name_path.begin(), module_name_path.end(), '.', static_cast<char>(std::filesystem::path::preferred_separator));
        module_name_path += ".lua";
        std::filesystem::path module_path = base_path / std::filesystem::path(module_name_path);

        if (!std::filesystem::exists(module_path))
        {
            throw BuildException("Module not found: " + module_path.string());
        }

        auto [iter, _inserted] = dependencies.emplace(module_name, module_path);
        iter->second.scan_dependencies(base_path, dependencies);
    }
}

builders::Bundler::Bundler(const pugi::xml_node &node, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path)
    : Builder{node, resource_name, resource_path}
{
    m_output = *m_resource_path / Builder::get_string_node(node, "output");
    if (!m_output.is_absolute())
        throw ConfigException("Coulnd't resolve path for: " + m_output.string());

    m_source_dir = *m_resource_path / Builder::get_string_node(node, "source_dir");
    if (!m_source_dir.is_absolute())
        throw ConfigException("Coulnd't resolve path for: " + m_source_dir.string());

    m_entrypoint = *m_resource_path / Builder::get_string_node(node, "entrypoint");
    if (!m_entrypoint.is_absolute())
        throw ConfigException("Coulnd't resolve path for: " + m_entrypoint.string());

    m_main_node = m_entrypoint;
}

void builders::Bundler::build_dependency_graph()
{
    m_main_node.scan_dependencies(m_source_dir, m_dependency_nodes);
}

void builders::Bundler::write_bundle(std::ofstream &output)
{
    output << s_bundle_template;
    for (auto &pair : m_dependency_nodes)
    {
        output << "____bundle__dict[\"" << pair.first << "\"] = function()\n"
               << pair.second.get_contents() << "\nend\n";
    }
    output << m_main_node.get_contents();
}

void builders::Bundler::build()
{
    Logger::print_info(*m_resource_name, &m_name, "Bundling lua\n");
    build_dependency_graph();
    std::filesystem::create_directories(m_output.parent_path());
    std::ofstream out_file(m_output);
    write_bundle(out_file);
}
