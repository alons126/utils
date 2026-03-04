//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef DEBUGGING_SETTINGS_H
#define DEBUGGING_SETTINGS_H

#include <iostream>

/**@name Debugging Settings
 * @brief Settings related to debugging the code, including general debugging, acceptance maps and weight maps debugging, and proton veto debugging.
 * @details These settings are used in the main code to control various aspects of debugging, such as whether to enable Debugger mode (which can be used to print out specific steps in the
 * code), whether to enable acceptance maps and weight maps debugging, and whether to enable proton veto debugging (which includes monitoring good protons and printing out events).
 * @note The DebuggerMode setting is a master switch for debugging, and should be set to true to enable any of the debugging features. The OnlyPrintNamedTesterSteps setting can be used to
 * print out only specific steps in the code, which can be useful for focusing on certain parts of the code during debugging. The AMapsDebuggerMode setting can be used to enable debugging
 * related to acceptance maps and weight maps, while the GoodProtonsMonitorPlots, PrintEvents, Ne_in_event, Nf_in_event, and nEvents2print settings can be used to control various aspects of
 * proton veto debugging.
 */
namespace debugging_settings {

// General debugging ----------------------------------------------------------------------------------------------------------------------------------------------------

const bool DebuggerMode = false;  // true = Debugger mode (master switch), false = Normal mode
const bool OnlyPrintNamedTesterSteps = (DebuggerMode && true);

// Acceptance maps and weight maps debugging ----------------------------------------------------------------------------------------------------------------------------

const bool AMapsDebuggerMode = (DebuggerMode && true);

// Proton veto debugging ------------------------------------------------------------------------------------------------------------------------------------------------

const bool GoodProtonsMonitorPlots = true;

const bool PrintEvents = false;
const int Ne_in_event = 1, Nf_in_event = 2, nEvents2print = 10000;
}  // namespace debugging_settings

#endif  // DEBUGGING_SETTINGS_H
