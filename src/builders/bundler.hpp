#pragma once

#include <string>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#include <boost/regex.hpp>
#include "builders/builder.hpp"

namespace builders
{
    class Bundler : public Builder
    {
    private:
        class DependencyNode
        {
        public:
            DependencyNode() = default;
            DependencyNode(const DependencyNode &other) : m_path{other.m_path} {}
            DependencyNode(DependencyNode &&other) noexcept : m_path{std::move(other.m_path)} {}

            DependencyNode(const std::filesystem::path &path)
                : m_path{path} {}
            DependencyNode(const std::string &path)
                : m_path{path} {}

            DependencyNode &operator=(const DependencyNode &other)
            {
                if (this != &other)
                {
                    m_path = other.m_path;
                }
                return *this;
            }

            DependencyNode &operator=(DependencyNode &&other) noexcept
            {
                if (this != &other)
                {
                    m_path = std::move(other.m_path);
                }
                return *this;
            }

            DependencyNode &operator=(const std::string &path)
            {
                m_path = path;
                return *this;
            }

            DependencyNode &operator=(const std::filesystem::path &path)
            {
                m_path = path;
                return *this;
            }

            DependencyNode &operator=(std::filesystem::path &&path)
            {
                m_path = std::move(path);
                return *this;
            }

            std::string get_contents();

            void scan_dependencies(
                const std::filesystem::path &base_path,
                std::unordered_map<std::string, DependencyNode> &dependencies);

            constexpr const std::filesystem::path &path() const { return m_path; }

        private:
            std::filesystem::path m_path;
        };

    public:
        Bundler(const pugi::xml_node &node, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path) noexcept(false);
        void build() override;

    private:
        std::filesystem::path m_output;
        std::filesystem::path m_source_dir;
        std::filesystem::path m_entrypoint;
        DependencyNode m_main_node;
        std::unordered_map<std::string, DependencyNode> m_dependency_nodes;

        void build_dependency_graph();
        void write_bundle(std::ofstream &output);

        static const boost::basic_regex<char> s_require_regex;
        static const std::string s_bundle_template;
    };
}
