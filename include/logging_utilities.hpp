#ifndef LOGGING_UTILITIES_HPP
#define LOGGING_UTILITES_HPP

#include <iostream>

namespace logging {
    inline void DEBUG(const char* message) {
#ifndef NDEBUG
        std::cerr << "DEBUG: " << message << std::endl;
#endif
    }
    inline void ERROR(const char* message) {
#ifndef NDEBUG
        std::cerr << "ERROR: " << message << std::endl;
#endif
    }
    inline void SUCCESS(const char* message) {
#ifndef NDEBUG
        std::cerr << "SUCCESS: " << message << std::endl;
#endif
    }
    inline void INFO(const char* message) {
#ifndef NDEBUG
        std::cerr << "INFO: " << message << std::endl;
#endif
    }
};

#endif