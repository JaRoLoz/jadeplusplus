#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <memory>
#include <thread>

#include "builders/builder.hpp"

namespace builders
{
    class ParallelBuilder : public Builder
    {
    public:
        inline static const std::string s_tag_name{"parallel"};

        ParallelBuilder(const pugi::xml_node &node, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path);
        void build() override;

    private:
        std::vector<std::unique_ptr<Builder>> m_steps;
    };
}