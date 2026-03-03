//
// Created by Alon Sportes on 26/02/2026.
//

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <string_view>

namespace environment {
std::string ERROR_COLOR = "\033[31m";       // Red
std::string COMPLETION_COLOR = "\033[32m";  // Green
std::string SYSTEM_COLOR = "\033[33m";      // Yellow
std::string WARNING_COLOR = "\033[35m";     // Magenta
std::string INFO_COLOR = "\033[36m";        // Cyan
std::string RESET_COLOR = "\033[0m";        // Reset color
};  // namespace environment

#endif  // ENVIRONMENT_H
