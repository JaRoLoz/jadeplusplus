#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <memory>
#include <unordered_map>
#include <chrono>
#include <thread>

#include <pugixml.hpp>

#include "logger.hpp"
#include "exceptions.hpp"
#include "build_config.hpp"
#include "builders/builder.hpp"
#include "builders/bundler.hpp"
#include "builders/js_builder.hpp"
#include "builders/parallel_builder.hpp"
#include "builders/manifest_builder.hpp"

class Resource
{
public:
    using constructors_collection_t = std::unordered_map<std::string, std::function<std::unique_ptr<builders::Builder>(const pugi::xml_node &, std::shared_ptr<std::string>, std::shared_ptr<std::filesystem::path>)>>;
    static constructors_collection_t s_constructors;

    static void build_resources(std::vector<Resource> &resources);

public:
    Resource(const std::filesystem::path &base_path, const std::string_view &name);

    Resource(const Resource &) = delete;
    Resource &operator=(const Resource &) = delete;

    Resource(Resource &&) = default;
    Resource &operator=(Resource &&) = default;

    void build();

private:
    void parse_config();
    void add_step(const pugi::xml_node &node);

private:
    std::shared_ptr<std::filesystem::path> m_base_path;
    std::shared_ptr<std::string> m_name;
    std::vector<std::unique_ptr<builders::Builder>> m_steps;
    std::chrono::steady_clock::time_point m_build_start;
};