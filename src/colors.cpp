#include "colors.hpp"

#ifdef WIN32
const Color Color::blue{FOREGROUND_BLUE | FOREGROUND_INTENSITY};
const Color Color::red{FOREGROUND_RED | FOREGROUND_INTENSITY};
const Color Color::yellow{FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY};
const Color Color::green{FOREGROUND_GREEN | FOREGROUND_INTENSITY};
const Color Color::reset{FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE};
#else
const Color Color::blue{34};
const Color Color::red{31};
const Color Color::yellow{33};
const Color Color::green{32};
const Color Color::reset{0};
#endif