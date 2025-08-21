#pragma once
#include <iostream>
#include <string>
#ifdef WIN32
#include <windows.h>
#endif
class Color
{
public:
    constexpr explicit Color(uint32_t color) : m_color{color} {}
#ifdef WIN32
    friend std::ostream &operator<<(std::ostream &os, const Color &color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color.m_color);
        return os;
    }
#else
    friend std::ostream &operator<<(std::ostream &os, const Color &color)
    {
        return os << "\033[" << color.m_color << "m";
    }
#endif
private:
    uint32_t m_color;

public:
    static const Color blue;
    static const Color red;
    static const Color yellow;
    static const Color green;
    static const Color reset;
};