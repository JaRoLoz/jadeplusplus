#include "resource.hpp"

#include "paths.hpp"

Resource::constructors_collection_t Resource::s_constructors(
    {{"bundle", [](const pugi::xml_node &node, std::shared_ptr<std::string> resource_name, std::shared_ptr<std::filesystem::path> resource_path) -> std::unique_ptr<builders::Builder>
      { return std::make_unique<builders::Bundler>(node, resource_name, resource_path); }},
     {"js_build", [](const pugi::xml_node &node, std::shared_ptr<std::string> resource_name, std::shared_ptr<std::filesystem::path> resource_path) -> std::unique_ptr<builders::Builder>
      { return std::make_unique<builders::JSBuilder>(node, resource_name, resource_path); }},
     {"parallel", [](const pugi::xml_node &node, std::shared_ptr<std::string> resource_name, std::shared_ptr<std::filesystem::path> resource_path) -> std::unique_ptr<builders::Builder>
      { return std::make_unique<builders::ParallelBuilder>(node, resource_name, resource_path); }},
     {"manifest", [](const pugi::xml_node &node, std::shared_ptr<std::string> resource_name, std::shared_ptr<std::filesystem::path> resource_path) -> std::unique_ptr<builders::Builder>
      { return std::make_unique<builders::ManifestBuilder>(node, resource_name, resource_path); }}});

Resource::Resource(const std::filesystem::path &base_path, const std::string_view &name)
    : m_base_path{std::make_shared<std::filesystem::path>(base_path)}, m_name{std::make_shared<std::string>(name)}
{
    parse_config();
}

void Resource::add_step(const pugi::xml_node &node)
{
    std::string node_name = node.name();
    if (!s_constructors.contains(node_name))
    {
        throw ConfigException("Failed to parse build config: unkown build step '" + node_name + '\'');
    }

    m_steps.emplace_back(s_constructors[node_name](node, m_name, m_base_path));
}

void Resource::parse_config()
{
    const BuildConfig &build_config = BuildConfig::const_instance();
    std::filesystem::path config_path(*m_base_path / paths::get_build_config_file_name(build_config.environment()));
    std::ifstream file(config_path.c_str());
    if (!file)
    {
        throw ConfigException("Failed to open file: " + config_path.string());
    }

    std::string config_str{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(config_str.c_str());
    if (!result)
    {
        throw ConfigException("Failed to parse XML config");
    }

    for (auto &node : doc.root().first_child().children())
    {
        add_step(node);
    }
}

void Resource::build_resources(std::vector<Resource> &resources)
{
    Logger::print_info(std::format("Found {} resource(s) to build\n", resources.size()));
    std::chrono::steady_clock::time_point build_start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    threads.reserve(resources.size());

    for (auto &resource : resources)
    {
        threads.emplace_back([&]
                             { resource.build(); });
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    auto time_delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - build_start);
    double seconds = time_delta.count() / 1000.0;
    Logger::print_ok(std::format("Build finished in {:.2f}s\n", seconds));
}

void Resource::build()
{
    m_build_start = std::chrono::high_resolution_clock::now();
    Logger::print_info(*m_name, std::nullopt, "Starting build\n");
    for (auto &step : m_steps)
    {
        try
        {
            step->build();
        }
        catch (const BuildException &ex)
        {
            Logger::print_error(*m_name, &step->name(), std::string{"An error ocurred while executing the build step: "} + ex.what());
        }
        catch (const std::exception &ex)
        {
            Logger::print_error(*m_name, &step->name(), std::string{"An unknown error ocurred while executing the build step: "} + ex.what());
        }
    }
    auto time_delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_build_start);
    double seconds = time_delta.count() / 1000.0;
    Logger::print_ok(*m_name, std::nullopt, std::format("Built successfully in {:.2f}s\n", seconds));
}
