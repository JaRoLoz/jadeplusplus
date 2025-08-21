#pragma once

#include <iostream>
#include <string>
#include <format>
#include <optional>
#include <mutex>
#ifdef WIN32
#include <windows.h>
#endif

#include <rang.hpp>

#include "colors.hpp"

class Logger
{
public:
    inline static void print_logo()
    {
#ifdef WIN32
        SetConsoleOutputCP(CP_UTF8);
#endif
        std::cout << s_logo;
    }

    inline static void print_info(
        const std::string_view &resource_name,
        const std::optional<const std::string *> &step,
        const std::string_view &msg)
    {
        std::lock_guard<std::mutex> guard{m_out_lock};
        std::cout << Color::blue << "[ INFO] " << std::flush;
        print(resource_name, step, msg);
    }

    inline static void print_info(const std::string_view &msg)
    {
        std::lock_guard<std::mutex> guard{m_out_lock};
        std::cout << Color::blue << "[ INFO] " << std::flush;
        std::cout << Color::reset << msg << std::flush;
    }

    inline static void print_ok(
        const std::string_view &resource_name,
        const std::optional<const std::string *> &step,
        const std::string_view &msg)
    {
        std::lock_guard<std::mutex> guard{m_out_lock};
        std::cout << Color::green << "[   OK] " << std::flush;
        print(resource_name, step, msg);
    }

    inline static void print_ok(const std::string_view &msg)
    {
        std::lock_guard<std::mutex> guard{m_out_lock};
        std::cout << Color::green << "[   OK] " << std::flush;
        std::cout << Color::reset << msg << std::flush;
    }

    inline static void print_error(
        const std::string_view &resource_name,
        const std::optional<const std::string *> &step,
        const std::string_view &msg)
    {
        std::lock_guard<std::mutex> guard{m_out_lock};
        std::cout << Color::red << "[ERROR] " << std::flush;
        print(resource_name, step, msg);
    }

    inline static void print_error(const std::string_view &msg)
    {
        std::lock_guard<std::mutex> guard{m_out_lock};
        std::cout << Color::red << "[ERROR] " << std::flush;
        std::cout << Color::reset << msg << std::flush;
    }

    inline static void print_warn(
        const std::string_view &resource_name,
        const std::optional<const std::string *> &step,
        const std::string_view &msg)
    {
        std::lock_guard<std::mutex> guard{m_out_lock};
        std::cout << Color::yellow << "[ WARN] " << std::flush;
        print(resource_name, step, msg);
    }

    inline static void print_warn(const std::string_view &msg)
    {
        std::lock_guard<std::mutex> guard{m_out_lock};
        std::cout << Color::yellow << "[ WARN] " << std::flush;
        std::cout << Color::reset << msg << std::flush;
    }

private:
    inline static void print(
        const std::string_view &resource_name,
        const std::optional<const std::string *> &step,
        const std::string_view &msg)
    {
        std::cout << Color::reset << std::format("[{}{}] {}", resource_name, (step ? std::format("/{}", *step.value()) : ""), msg) << std::flush;
    }

    inline static std::mutex m_out_lock;

    inline static const char *s_logo = R"(
⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣦⣄⠀⠀⠀⠀⣠⡞⠀⠀⠀⣠⣴⣶⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⡛⢿⣿⣷⣄⢀⣾⠏⣀⣤⣶⣿⢿⣿⡏⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠑⢤⣀⡀⠀⢻⣷⡄⠻⣿⣿⣿⣿⣿⣿⡿⢋⣵⣿⠟⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠉⠛⢷⣿⣿⣿⣤⣼⠿⢿⣿⡟⢁⣴⣾⡿⠋⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢀⣤⣾⣿⣿⣿⠿⣿⠋⠀⠀⠀⢹⣿⣿⣿⣿⣿⠶⠶⠤⣄⡀⠀⠀
⠀⠀⠀⣠⣾⡿⠿⢛⣋⣉⣤⣤⣽⣷⣤⣤⣶⣟⣁⠉⠛⠿⣿⣷⣦⡀⠀⠀⠀⠀
⠀⠐⠻⠿⠿⠿⠿⠿⠿⠿⣿⣿⣿⣿⠃⢰⣿⣿⣿⣿⣷⣦⣬⣙⣿⣿⣄⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⢿⣿⣿⠀⣿⣿⣿⣿⡉⠉⠉⠛⠻⠿⣿⣿⣦⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⢠⡾⠁⢸⣿⡏⢸⣿⣿⠇⠙⢿⡄⠀⠀⠀⠀⠀⠀⠉⠁⠀
⠀⠀⠀⠀⠀⠀⠀⢀⠏⠀⠀⢸⣿⣧⣿⣿⠏⠀⠀⠈⠻⣆⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⡿⠃⠀⠀⠀⠀⠀⠈⠄⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
)";
};