#include "builders/parallel_builder.hpp"

#include "resource.hpp"

builders::ParallelBuilder::ParallelBuilder(const pugi::xml_node &node, const std::shared_ptr<std::string> &resource_name, const std::shared_ptr<std::filesystem::path> &resource_path)
    : Builder{"parallel", resource_name, resource_path}
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

void builders::ParallelBuilder::build()
{
    std::vector<std::thread> threads;
    threads.reserve(m_steps.size());

    for (auto &step : m_steps)
    {
        threads.emplace_back([&]
                             { step->build(); });
    }

    for (auto &thread : threads)
    {
        thread.join();
    }
}