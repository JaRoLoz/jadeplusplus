#include <format>

#include "builders/js_builder.hpp"

#include "resource.hpp"

boost::asio::io_context builders::JSBuilder::s_ctx;

builders::JSBuilder::JSBuilder(const pugi::xml_node &node, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path)
    : Builder{node, resource_name, resource_path}
{
    try
    {
        m_package_manager = Builder::get_string_node(node, "package_manager");
    }
    catch (const ConfigException &ex)
    {
        (void)ex;
        const BuildConfig &build_config = BuildConfig::const_instance();
        m_package_manager = build_config.package_manager() ? build_config.package_manager().value() : s_default_package_manager;
    }
    m_folder = *m_resource_path / Builder::get_string_node(node, "folder");
    if (!m_folder.is_absolute())
    {
        throw ConfigException("Coulnd't resolve path for: " + m_folder.string());
    }
    m_build_script = Builder::get_string_node(node, "build_script");
    m_install_packages = Builder::get_bool_node(node, "install_packages");
}

void builders::JSBuilder::build()
{
    namespace asio = boost::asio;
    namespace bp = boost::process;
    namespace bs = boost::process;

    const auto package_manager_path = bp::v2::environment::find_executable(m_package_manager);
    if (package_manager_path.empty())
    {
        throw BuildException(std::string("Error executing js_build step: '") + m_package_manager + std::string("' couldn't be found"));
    }

    if (m_install_packages)
    {
        Logger::print_info(*m_resource_name, &m_name, "Installing dependencies with " + m_package_manager + '\n');
        asio::readable_pipe out_pipe{s_ctx};
        asio::readable_pipe err_pipe{s_ctx};
        bp::process proc(
            s_ctx,
            package_manager_path.c_str(),
            {"install"},
            bp::process_stdio{{}, out_pipe, err_pipe},
            bp::process_start_dir{std::filesystem::absolute(m_folder).c_str()});
        std::string output;
        std::string error;
        bs::error_code _err_code;
        asio::read(out_pipe, asio::dynamic_buffer(output), _err_code);
        asio::read(err_pipe, asio::dynamic_buffer(error), _err_code);

        if (proc.wait() != 0)
        {
            throw BuildException(std::string{"Error installing packages during js_build step: "} + (error.empty() ? output : error));
        }
    }

    Logger::print_info(*m_resource_name, &m_name, std::format("Running \"{}\" with {}\n", m_build_script, m_package_manager));
    asio::readable_pipe out_pipe{s_ctx};
    asio::readable_pipe err_pipe{s_ctx};
    bp::process proc(
        s_ctx,
        package_manager_path.c_str(),
        {"run", m_build_script},
        bp::process_stdio{{}, out_pipe, err_pipe},
        bp::process_start_dir{std::filesystem::absolute(m_folder).c_str()});
    std::string output;
    std::string error;
    bs::error_code _err_code;
    asio::read(out_pipe, asio::dynamic_buffer(output), _err_code);
    asio::read(err_pipe, asio::dynamic_buffer(error), _err_code);

    if (proc.wait() != 0)
    {
        throw BuildException(std::string{"Error executing js_build step: "} + (error.empty() ? output : error));
    }
}
