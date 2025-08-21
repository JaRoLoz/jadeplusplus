#include "paths.hpp"

const std::string paths::config_file_name = "jade.xml";

std::string paths::get_build_config_file_name(const std::optional<const std::string> &build_env)
{
    return build_env ? *build_env + '.' + config_file_name : config_file_name;
}

std::optional<std::filesystem::path> paths::get_resources_folder(const std::filesystem::path &base_path)
{
    std::filesystem::path current_path = base_path;

    while (true)
    {
        if (current_path.filename().string() == "resources")
        {
            return current_path;
        }

        std::filesystem::path parent = current_path.parent_path();
        if (parent == current_path)
        {
            return std::nullopt;
        }

        current_path = parent;
    }
}

bool paths::is_buildable_resource(
    const std::filesystem::path &path,
    const std::optional<const std::string> &build_env)
{
    using namespace std;
    string config_name = get_build_config_file_name(build_env);
    for (auto &entry : filesystem::directory_iterator(path))
    {
        if (entry.path().filename().string() == config_name)
        {
            return true;
        }
    }

    return false;
}

std::unordered_map<std::string, std::filesystem::path>
paths::enumerate_resources(
    const std::filesystem::path &resources_path,
    const std::optional<const std::string> &build_env)
{
    using namespace std;
    unordered_map<string, filesystem::path> resources;

    for (auto &entry : filesystem::directory_iterator(resources_path))
    {
        if (entry.is_regular_file())
        {
            continue;
        }

        std::string name = entry.path().filename().string();
        if (name.starts_with('[') && name.ends_with(']'))
        {
            auto sub_resources = enumerate_resources(entry.path(), build_env);
            resources.insert(sub_resources.begin(), sub_resources.end());
        }
        else
        {
            if (is_buildable_resource(entry.path(), build_env))
            {
                resources[name] = entry.path();
            }
        }
    }

    return resources;
}
