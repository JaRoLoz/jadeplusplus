#pragma once

#include <filesystem>
#include <string>
#include <optional>
#include <unordered_map>

class paths
{
public:
    static std::unordered_map<std::string, std::filesystem::path> enumerate_resources(
        const std::filesystem::path &resources_path,
        const std::optional<const std::string> &build_env);
    static std::string get_build_config_file_name(const std::optional<const std::string> &build_env);
    static std::optional<std::filesystem::path> get_resources_folder(const std::filesystem::path& base_path);

private:
    static const std::string config_file_name;
    static bool is_buildable_resource(
        const std::filesystem::path &path,
        const std::optional<const std::string> &build_env);
};
