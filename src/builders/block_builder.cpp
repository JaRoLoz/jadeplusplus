#include "builders/block_builder.hpp"

#include "resource.hpp"

builders::BlockBuilder::BlockBuilder(const pugi::xml_node &node, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path)
    : Builder{"block", resource_name, resource_path}
{
    for (auto &child : node.children())
    {
        std::string name = child.name();
        if (!Resource::s_constructors.contains(name))
        {
            continue;
        }

        m_steps.push_back(Resource::s_constructors[name](child, resource_name, resource_path));
    }
}

void builders::BlockBuilder::build()
{
    for (auto &step : m_steps)
    {
        try
        {
            step->build();
        }
        catch (const BuildException &ex)
        {
            Logger::print_error(*m_resource_name, &step->name(), std::string{"An error ocurred while executing the build step: "} + ex.what());
        }
        catch (const std::exception &ex)
        {
            Logger::print_error(*m_resource_name, &step->name(), std::string{"An unknown error ocurred while executing the build step: "} + ex.what());
        }
    }
}
