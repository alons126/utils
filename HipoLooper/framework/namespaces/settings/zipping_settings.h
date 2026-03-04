//
// Created by Alon Sportes on 24/04/2025.
//

#ifndef ZIPPING_SETTINGS_H
#define ZIPPING_SETTINGS_H

#include <iostream>

/**@name Zipping Settings
 * @brief Settings related to zipping the output of the code runs, including saving the Plots folder as a zip file and collecting all zip files from multiple samples into one directory.
 * @details These settings are used in the main code to control whether to save the Plots folder of each sample as a zip file, and whether to collect all zip files from multiple samples into
 * one directory. The zipping process is done using the system's zip command, and the zip files are saved in the same directory as the Plots folder, with the same name as the Plots folder
 * (with .zip extension).
 * @note The zipping process can take some time, especially if the Plots folder is large, so it is recommended to set Save_Plots_folder_to_zip_files to true only if you want to save the
 * Plots folder as a zip file. Additionally, the zipping_print_out setting can be set to true to print out the zipping process in the terminal, which can be useful for debugging or
 * monitoring the progress of the zipping process.
 */
namespace zipping_settings {

// General zipping of a sample's output ---------------------------------------------------------------------------------------------------------------------------------

bool Save_Plots_folder_to_zip_files =
    false;  // set to true to save the entire Plots folder of each sample as a zip file (saved in the same directory as the Plots folder, with the same name as the Plots folder)
bool zipping_print_out =
    (Save_Plots_folder_to_zip_files && false);  // set to true to print out the zipping process in the terminal (only if Save_Plots_folder_to_zip_files is true, otherwise it will be ignored)

// General zipping of all samples in the code run -----------------------------------------------------------------------------------------------------------------------

bool Collect_all_zip_files = (Save_Plots_folder_to_zip_files && false);
}  // namespace zipping_settings

#endif  // ZIPPING_SETTINGS_H
