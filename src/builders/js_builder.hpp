#pragma once

#include <iostream>
#include <filesystem>
#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include "builders/builder.hpp"

namespace builders
{
    class JSBuilder : public Builder
    {
    public:
        JSBuilder(const pugi::xml_node &node, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path) noexcept(false);
        void build() override;

    private:
        std::string m_package_manager;
        std::string m_build_script;
        std::filesystem::path m_folder;
        bool m_install_packages;

        static boost::asio::io_context s_ctx;
        static constexpr const char *s_default_package_manager = "npm";
    };
}