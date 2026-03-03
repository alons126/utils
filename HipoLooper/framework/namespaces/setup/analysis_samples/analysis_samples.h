//
// Created by Alon Sportes on 27/03/2025.
//

#ifndef ANALYSIS_SAMPLES_H
#define ANALYSIS_SAMPLES_H

#include <iostream>

// Include libraries:
#include "../../general_utilities/utilities.h"

// Include structs:
#include "data_samples.h"
#include "simulation_samples.h"
#include "uniform_samples.h"

namespace yl = Yaml_loader;

namespace analysis_samples {
using namespace data_samples;
using namespace simulation_samples;
using namespace uniform_samples;

std::vector<std::string> RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v;
std::string RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles;

// TODO: get rid of:
// std::string &RecoSamplePath, std::string &ReconHipoDir, std::string &PathToHipoFilesDir, std::string &InputHipoFiles

// file_name definition -------------------------------------------------------------------------------------------------------------------------------------------------

std::string file_name = "general_file";

// HIPO files for Q2 study ----------------------------------------------------------------------------------------------------------------------------------------------

void ConfigSampleChain() {
    // ==================================================================================================================================================================
    // Data runs
    // ==================================================================================================================================================================

    // H1 ---------------------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* H1 2.07052GeV - run 015634 (ifarm DATA files - H1 DATA files) */
    // Config_H1_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* H1 5.98636GeV - run 015028 (ifarm DATA files - H1 DATA files) */
    // Config_H1_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // D2 ---------------------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* D2 2.07052GeV - all runs (ifarm DATA files - D2 DATA files) */
    // Config_D2_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* D2 5.98636GeV - run 015443 (ifarm DATA files - D2 DATA files) */
    // Config_D2_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // C12 --------------------------------------------------------------------------------------------------------------------------------------------------------------

    // /* C12 2.07052GeV - run 015664 (ifarm DATA files - C12 DATA files) */
    // Config_C12_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // /* C12 4.02962GeV - run 015778 (ifarm DATA files - C12 DATA files) */
    // Config_C12_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* C12x4 5.98636GeV - run 015188 (ifarm DATA files - C12x4 DATA files) */
    // Config_C12_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // Ar40 -------------------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* Ar40 2.07052GeV - run 015672 (ifarm DATA files - Ar40 DATA files) */
    // Config_Ar40_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* Ar40 4.02962GeV - run 015743 (ifarm DATA files - Ar40 DATA files) */
    // Config_Ar40_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* Ar40 5.98636GeV - run 015792 (ifarm DATA files - Ar40 DATA files) */
    // Config_Ar40_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // ==================================================================================================================================================================
    // Uniform simulation files
    // ==================================================================================================================================================================
    // These samples were used to generate the acceptance maps

    // Electron tester samples ------------------------------------------------------------------------------------------------------------------------------------------

    // /* Uniform electron tester samples */
    // Config_Uniform_tester_e_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // Uniform samples --------------------------------------------------------------------------------------------------------------------------------------------------

    // // /* Uniform samples @ 2GeV */
    // Config_Uniform_1e_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_en_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_ep_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* Uniform samples @ 4GeV */
    // Config_Uniform_1e_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_en_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_ep_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // // /* Uniform samples @ 6GeV */
    // Config_Uniform_1e_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_en_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);
    // Config_Uniform_ep_6GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // ==================================================================================================================================================================
    // GENIE simulation files
    // ==================================================================================================================================================================
    // Simulation samples -----------------------------------------------------------------------------------------------------------------------------------------------

    // /* C12 G18 2GeV - corrected geometry */
    // Config_C12_G18_2GeV_Corrected1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles,
    //                                            false);  // no FC
    // Config_C12_SuSa_2GeV_Corrected1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles,
    //                                             false);  // no FC

    // // /* C12 SuSa 4GeV - corrected geometry */
    // Config_C12_G18_4GeV_Corrected1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles,
    //                                            false);  // no FC
    // Config_C12_SuSa_4GeV_Corrected1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles,
    //                                             false);  // no FC

    // // /* Ar40 G18 2GeV - corrected geometry */
    // Config_Ar40_G18_2GeV_Corrected1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles,
    //                                             false);  // no FC
    Config_Ar40_SuSa_2GeV_Corrected1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles,
                                                 false);  // no FC

    // // /* Ar40 SuSa 2GeV - corrected geometry */
    // Config_Ar40_G18_4GeV_Corrected1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles,
    //                                             false);  // no FC
    // Config_Ar40_SuSa_4GeV_Corrected1Foil_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles,
    //                                              false);  // no FC

    // /* C12 G18 2GeV */
    // Config_C12_G18_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, false);  // no
    // FC Config_C12_G18_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, true);   //
    // with FC

    // /* C12 G18 4GeV */
    // Config_C12_G18_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, false);  // no
    // FC Config_C12_G18_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, true);   //
    // with FC

    // /* C12 SuSa 2GeV */
    // Config_C12_SuSa_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, false);  // no
    // FC Config_C12_SuSa_2GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, true);   //
    // with FC

    // /* C12 SuSa 4GeV */
    // Config_C12_SuSa_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, false);  // no
    // FC Config_C12_SuSa_4GeV_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles, true);   //
    // with FC

    // Small Q2 test samples --------------------------------------------------------------------------------------------------------------------------------------------

    // /* C12 G18 varing Q2 4GeV */
    // Config_C12_G18_4GeV_Q2_test_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // /* C12 SuSa varing Q2 4GeV */
    // Config_C12_SuSa_4GeV_Q2_test_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // /* Ar40 G18 varing Q2 6GeV */
    // Config_Ar40_G18_6GeV_Q2_test_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // /* Ar40 SuSa varing Q2 6GeV */
    // Config_Ar40_SuSa_6GeV_Q2_test_samples(RecoSamplePath_v, ReconHipoDir_v, PathToHipoFilesDir_v, InputHipoFiles_v, RecoSamplePath, ReconHipoDir, PathToHipoFilesDir, InputHipoFiles);

    // ==================================================================================================================================================================
    // Old simulation files for local runs
    // ==================================================================================================================================================================

    // RecoSamplePath = "mnt/d/e4nu/hipo_data_files"; // Storage (D:)
    // // RecoSamplePath = "mnt/h/e4nu/hipo_data_files";                                              // Alon's Passport (F:)
    // // RecoSamplePath = "mnt/g/e4nu/hipo_data_files";                                              // Alon's Portable (G:)
    // ReconHipoDir = "C12x4_G18_02a_00_000_Q204_598636MeV"; // my test with Justin's code; all particle data saved
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    // // InputHipoFiles = PathToHipoFilesDir + "*.hipo"; // All files
    // // InputHipoFiles = PathToHipoFilesDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_9_torus-1.0.hipo"; // 1 file
    // // InputHipoFiles = PathToHipoFilesDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_51*.hipo";         // 11 files
    // // InputHipoFiles = PathToHipoFilesDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_5*.hipo";          // 111 files
    // // InputHipoFiles = PathToHipoFilesDir + "recon_C12x4_G18_02a_00_000_Q204_598636MeV_2*.hipo";          // 512 files

    // ==================================================================================================================================================================
    // Other ROOT files
    // ==================================================================================================================================================================

    ////#pragma region /* root files */
    // RecoSamplePath = "home/alon/project";
    // ReconHipoDir = "Downloads";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    // InputHipoFiles = PathToHipoFilesDir + "/" + "12C_2222GeV_GEM21_11a_00_000_wfsi_mk2.root";
    ////InputHipoFiles = PathToHipoFilesDir + "/" + "e_on_1000060120_2222MeV_em.gst.root";
    ////InputHipoFiles = "./e_on_1000060120_598636MeV.gst.root";

    // #pragma region /* SuSAv2 fix for thesis */
    //  RecoSamplePath = "home/alon/project";
    //  ReconHipoDir = "Downloads";
    //  PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    ////InputHipoFiles = PathToHipoFilesDir + "/" + "12C_2222GeV_GTEST19_10b_00_000.root"; // Original problem
    // InputHipoFiles = PathToHipoFilesDir + "/" + "e_on_1000060120_EMMEC_test_myBranch.root"; // Initial fix with 1M events (myBranch)
    // #pragma endregion

    // RecoSamplePath = "mnt/d/e4nu/hipo_data_files";
    // ReconHipoDir = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";

    // RecoSamplePath = "home/alon/project";
    // ReconHipoDir = "Downloads";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    // InputHipoFiles = PathToHipoFilesDir + "e_on_1000060120_2222MeV_em.gst.root";

    // ReconHipoDir = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/" + "first_50_test" + "/";

    // RecoSamplePath = "pnfs/genie/scratch/users/asportes/script_jobs/detector_sim_truth_level";
    // ReconHipoDir = "detector_sim_truth_level/C12_G18_10a_02_11b_2GeV_QEL_test";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    ////InputHipoFiles = PathToHipoFilesDir + "Ca48_G18_10a_02_11b_207052MeV.root";
    // InputHipoFiles = PathToHipoFilesDir + "e_on_1000060120_2070MeV.root";

    // RecoSamplePath = "home/alon/project";
    // ReconHipoDir = "Downloads";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    ////InputHipoFiles = PathToHipoFilesDir + "Ca48_G18_10a_02_11b_207052MeV.root";
    // InputHipoFiles = PathToHipoFilesDir + "Ca48_G18_10a_02_11b_Q205_598636MeV.root";

    // RecoSamplePath = "lustre19/expphy/volatile/clas12/asportes/truthLevelROOTFiles";
    // ReconHipoDir = "C12_G18_10a_02_11b_207052MeV";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    // InputHipoFiles = PathToHipoFilesDir + "C12_G18_10a_02_11b_207052MeV.root";

    // RecoSamplePath = "mnt/d/e4nu/root_data_files/Detector_sim_truth_level_files";
    // ReconHipoDir = "598636MeV_Q2_0_5";
    // PathToHipoFilesDir = "/" + RecoSamplePath + "/" + ReconHipoDir + "/";
    //
    ////InputHipoFiles = PathToHipoFilesDir + "e_on_1000060120_598636MeV_Q2_0_5.gst.root";
    ////InputHipoFiles = "./nu_SuSAv2_fix_test_12C_2222GeV_CC_myBranch_10M.root";
    // InputHipoFiles = "./nu_SuSAv2_fix_test_12C_2222GeV_CCMEC_myBranch_10M.root";
}

}  // namespace analysis_samples

#endif  // ANALYSIS_SAMPLES_H
