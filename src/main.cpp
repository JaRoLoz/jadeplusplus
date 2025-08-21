#include "main.hpp"

int main(int argc, char **argv)
{
    args::ArgumentParser parser{"The Byte Framework build tool."};
    args::HelpFlag help{parser, "help", "Display the list of options", {'h', "help"}};
    args::CompletionFlag completion{parser, {"complete"}};
    args::Positional<std::string> resource{parser, "string", "The resource to be built"};
    args::ValueFlag<std::string>
        environment{parser, "string", "The environment to be built", {"env"}};
    args::ValueFlag<std::string> package_manager{parser, "string", "The package managwer to use when not specified", {"pm", "package-manager"}};

    Logger::print_logo();

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (const args::Completion &e)
    {
        std::cout << e.what();
        return 0;
    }
    catch (const args::Help &)
    {
        std::cout << parser;
        return 0;
    }
    catch (const args::ParseError &e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    BuildConfig &build_config = BuildConfig::instance();
    build_config.set_package_manager(package_manager ? std::make_optional(args::get(package_manager)) : std::nullopt);
    build_config.set_environment(environment ? std::make_optional(args::get(environment)) : std::nullopt);

    if (resource)
    {
        if (args::get(resource) == ".")
        {
            try
            {
                Resource(std::filesystem::current_path(), std::filesystem::current_path().filename().string()).build();
            }
            catch (const ConfigException &ex)
            {
                Logger::print_error(std::string{ex.what()} + '\n');
            }
        }
        else
        {
            std::optional<std::filesystem::path> resources_path = paths::get_resources_folder(std::filesystem::current_path());
            if (!resources_path)
            {
                Logger::print_error("Unable to find resources folder\n");
                return 1;
            }

            auto resources_entries = paths::enumerate_resources(*resources_path, build_config.environment());

            for (auto &[name, path] : resources_entries)
            {
                if (name == args::get(resource))
                {
                    try
                    {
                        Resource(path, name).build();
                    }
                    catch (const ConfigException &ex)
                    {
                        Logger::print_error(std::string{ex.what()} + '\n');
                    }
                    return 0;
                }
            }

            Logger::print_error("Unable to find resource: " + args::get(resource) + '\n');
        }
    }
    else
    {
        std::optional<std::filesystem::path> resources_path = paths::get_resources_folder(std::filesystem::current_path());
        if (!resources_path)
        {
            Logger::print_error("Unable to find resources folder\n");
            return 1;
        }

        auto resources_entries = paths::enumerate_resources(*resources_path, build_config.environment());
        std::vector<Resource> resources;
        resources.reserve(resources_entries.size());

        for (auto &[name, path] : resources_entries)
        {
            try
            {
                resources.emplace_back(path, name);
            }
            catch (const ConfigException &ex)
            {
                Logger::print_error(std::format("[{}] ", name) + ex.what() + '\n');
            }
        }

        Resource::build_resources(resources);
    }

    return 0;
}