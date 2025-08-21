#pragma once

#include <optional>
#include <string>

class BuildConfig
{
public:
    static BuildConfig &instance()
    {
        static BuildConfig inst;
        return inst;
    }

    static const BuildConfig &const_instance()
    {
        return instance();
    }

    void set_package_manager(std::optional<std::string> &&package_manager) { m_package_manager = std::move(package_manager); }
    void set_environment(std::optional<std::string> &&environment) { m_environment = std::move(environment); }
    void set_package_manager(const std::optional<std::string> &package_manager) { m_package_manager = package_manager; }
    void set_environment(const std::optional<std::string> &environment) { m_environment = environment; }

    const std::optional<std::string> &package_manager() const { return m_package_manager; }
    const std::optional<std::string> &environment() const { return m_environment; }

private:
    BuildConfig() = default;

    std::optional<std::string> m_package_manager;
    std::optional<std::string> m_environment;
};