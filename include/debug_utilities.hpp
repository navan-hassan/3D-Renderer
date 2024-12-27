#ifndef DEBUG_UTILITIES_HPP
#define DEBUG_UTILITIES_HPP
#include <iostream>

inline void debug_write(const char* message) {
#ifndef NDEBUG
    std::cout << "DEBUG: " << message << std::endl;
#endif // !NDEBUG
}

#endif