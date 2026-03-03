//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef DEBUGGING_H
#define DEBUGGING_H

#include <cstdlib>
#include <iostream>

// Include libraries:
#include "../general_utilities/environment.h"
#include "../settings/debugging_settings.h"

// Include classes:
#include "../../classes/Debugger/Debugger.cpp"

namespace env = environment;

namespace debugging {
using namespace debugging_settings;

// CodeDebugger declaration ---------------------------------------------------------------------------------------------------------------------------------------------

inline Debugger CodeDebugger;

}  // namespace debugging

#endif  // DEBUGGING_H
