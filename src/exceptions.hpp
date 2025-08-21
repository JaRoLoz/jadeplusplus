#pragma once

#include <stdexcept>
#include <string>

class ConfigException : public std::runtime_error
{
public:
    explicit ConfigException(const std::string &message)
        : std::runtime_error{message}
    {
    }

    explicit ConfigException(const char *message)
        : std::runtime_error{message}
    {
    }
};

class BuildException : public std::runtime_error
{
public:
    explicit BuildException(const std::string &message)
        : std::runtime_error{message}
    {
    }

    explicit BuildException(const char *message)
        : std::runtime_error{message}
    {
    }
};
