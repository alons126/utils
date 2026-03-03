
//
// Created by Alon Sportes on 03/04/2025.
//

#ifndef PRINTERS_H
#define PRINTERS_H

#include <iostream>

// Include libraries:
#include "../../namespaces/general_utilities/utilities.h"

// Include structs:
#include "../../structures/RecoAnalyzerSettings/AcceptanceMapsSettings.h"
#include "../../structures/RecoAnalyzerSettings/AnalysisCutSettings.h"
#include "../../structures/RecoAnalyzerSettings/CutValueManager.h"
#include "../../structures/RecoAnalyzerSettings/EventSelectionSettings.h"
#include "../../structures/RecoAnalyzerSettings/MomentumResolutionSettings.h"
#include "../../structures/RecoAnalyzerSettings/RunParameters.h"

namespace bt = basic_tools;
namespace env = environment;

namespace printers {

// RecoCutsPrintOut function --------------------------------------------------------------------------------------------------------------------------------------------

void RecoCutsPrintOut(const std::string& workingDirectory, const std::string& run_plots_path, const std::string& RecoSamplePath, const std::string& ReconHipoDir,
                      const std::string& InputHipoFiles, const std::string& file_name, const RunParameters& parameters, const AnalysisCutSettings& CutSettings) {
    /* Print out the cuts within the run (for self-observation) */

    std::cout << env::SYSTEM_COLOR << "-- Execution variables ----------------------------------------------------\n";
    std::cout << env::SYSTEM_COLOR << "WorkingDirectory:" << env::RESET_COLOR << "\t" << workingDirectory << "\n";
    std::cout << env::SYSTEM_COLOR << "run_plots_path:" << env::RESET_COLOR << "\t\t" << run_plots_path << "\n\n";

    std::cout << env::SYSTEM_COLOR << "RecoSamplePath:" << env::RESET_COLOR << "\t" << "/" << RecoSamplePath << "/" << "\n";
    std::cout << env::SYSTEM_COLOR << "ReconHipoDir:" << env::RESET_COLOR << "\t" << "/" << ReconHipoDir << "/" << "\n";
    std::cout << env::SYSTEM_COLOR << "InputHipoFiles:" << env::RESET_COLOR << "\t\t" << InputHipoFiles << "\n";
    std::cout << env::SYSTEM_COLOR << "Settings mode:" << env::RESET_COLOR << "\t\t'" << file_name << "'\n\n";

    std::cout << env::SYSTEM_COLOR << "SampleName:" << env::RESET_COLOR << "\t\t" << parameters.SampleName << "\n";
    std::cout << env::SYSTEM_COLOR << "VaryingSampleName:" << env::RESET_COLOR << "\t" << parameters.VaryingSampleName << "\n";
    std::cout << env::SYSTEM_COLOR << "Target:" << env::RESET_COLOR << "\t\t\t" << parameters.Target << " (PDG: " << parameters.TargetPDG << ")\n";
    std::cout << env::SYSTEM_COLOR << "Beam Energy:" << env::RESET_COLOR << "\t\t" << parameters.beamE << " [GeV]\n\n\n\n";

    // Cuts output
    std::cout << env::SYSTEM_COLOR << ((!CutSettings.apply_cuts) ? "Cuts are disabled" : "Cuts are enabled") << "\n";

    std::cout << env::SYSTEM_COLOR << "apply_cuts:" << env::RESET_COLOR << "                              \t" << bt::BoolToString(CutSettings.apply_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "clas12ana_particles:" << env::RESET_COLOR << "                     \t" << bt::BoolToString(CutSettings.clas12ana_particles) << "\n";
    std::cout << env::SYSTEM_COLOR << "only_preselection_cuts:" << env::RESET_COLOR << "                  \t" << bt::BoolToString(CutSettings.only_preselection_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "only_electron_quality_cuts:" << env::RESET_COLOR << "              \t" << bt::BoolToString(CutSettings.only_electron_quality_cuts) << "\n\n";

    std::cout << env::SYSTEM_COLOR << "apply_preselection_cuts:" << env::RESET_COLOR << "                 \t" << bt::BoolToString(CutSettings.apply_preselection_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_Vz_e_cuts:" << env::RESET_COLOR << "                         \t" << bt::BoolToString(CutSettings.apply_Vz_e_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_Vz_cuts:" << env::RESET_COLOR << "                           \t" << bt::BoolToString(CutSettings.apply_Vz_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_dVz_cuts:" << env::RESET_COLOR << "                          \t" << bt::BoolToString(CutSettings.apply_dVz_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_DC_e_fiducial_cuts:" << env::RESET_COLOR << "                \t" << bt::BoolToString(CutSettings.apply_DC_e_fiducial_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_DC_fiducial_cuts:" << env::RESET_COLOR << "                  \t" << bt::BoolToString(CutSettings.apply_DC_fiducial_cuts) << "\n\n";

    std::cout << env::SYSTEM_COLOR << "apply_electron_quality_cuts:" << env::RESET_COLOR << "             \t" << bt::BoolToString(CutSettings.apply_electron_quality_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_Nphe_cut:" << env::RESET_COLOR << "                          \t" << bt::BoolToString(CutSettings.apply_Nphe_cut) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_ECAL_SF_cuts:" << env::RESET_COLOR << "                      \t" << bt::BoolToString(CutSettings.apply_ECAL_SF_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_ECAL_P_cuts:" << env::RESET_COLOR << "                       \t" << bt::BoolToString(CutSettings.apply_ECAL_P_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_ECAL_fiducial_cuts:" << env::RESET_COLOR << "                \t" << bt::BoolToString(CutSettings.apply_ECAL_fiducial_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_Electron_beta_cut:" << env::RESET_COLOR << "                 \t" << bt::BoolToString(CutSettings.apply_Electron_beta_cut) << "\n\n";

    std::cout << env::SYSTEM_COLOR << "apply_chi2_cuts_1e_cut:" << env::RESET_COLOR << "                  \t" << bt::BoolToString(CutSettings.apply_chi2_cuts_1e_cut) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_nucleon_cuts:" << env::RESET_COLOR << "                      \t" << bt::BoolToString(CutSettings.apply_nucleon_cuts) << "\n\n";
    std::cout << env::SYSTEM_COLOR << "apply_nucleon_physical_cuts:" << env::RESET_COLOR << "             \t" << bt::BoolToString(CutSettings.apply_nucleon_physical_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_CDEdgeCuts:" << env::RESET_COLOR << "                        \t" << bt::BoolToString(CutSettings.apply_CDEdgeCuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_CDRegionCuts:" << env::RESET_COLOR << "                      \t" << bt::BoolToString(CutSettings.apply_CDRegionCuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_ProtonPIDCuts:" << env::RESET_COLOR << "                     \t" << bt::BoolToString(CutSettings.apply_ProtonPIDCuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_GhostTrackCuts:" << env::RESET_COLOR << "                    \t" << bt::BoolToString(CutSettings.apply_GhostTrackCuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_charged_ECAL_veto:" << env::RESET_COLOR << "                 \t" << bt::BoolToString(CutSettings.apply_charged_ECAL_veto) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_neutral_ECAL_veto:" << env::RESET_COLOR << "                 \t" << bt::BoolToString(CutSettings.apply_neutral_ECAL_veto) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_nBeta_fit_cuts:" << env::RESET_COLOR << "                    \t" << bt::BoolToString(CutSettings.apply_nBeta_fit_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_fiducial_cuts:" << env::RESET_COLOR << "                     \t" << bt::BoolToString(CutSettings.apply_fiducial_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_kinematical_cuts:" << env::RESET_COLOR << "                  \t" << bt::BoolToString(CutSettings.apply_kinematical_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_kinematical_weights:" << env::RESET_COLOR << "               \t" << bt::BoolToString(CutSettings.apply_kinematical_weights) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_nucleon_SmearAndCorr:" << env::RESET_COLOR << "              \t" << bt::BoolToString(CutSettings.apply_nucleon_SmearAndCorr) << "\n\n";

    std::cout << env::SYSTEM_COLOR << "apply_reaction_monitoring_cuts:" << env::RESET_COLOR << "          \t" << bt::BoolToString(CutSettings.apply_reaction_monitoring_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_P_miss_in_QE_range_cuts:" << env::RESET_COLOR << "           \t" << bt::BoolToString(CutSettings.apply_P_miss_in_QE_range_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_E_miss_in_QE_range_cuts:" << env::RESET_COLOR << "           \t" << bt::BoolToString(CutSettings.apply_E_miss_in_QE_range_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_P_miss_in_MECandSRC_range_cuts:" << env::RESET_COLOR << "    \t" << bt::BoolToString(CutSettings.apply_P_miss_in_MECandSRC_range_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_E_miss_in_MECandSRC_range_cuts:" << env::RESET_COLOR << "    \t" << bt::BoolToString(CutSettings.apply_E_miss_in_MECandSRC_range_cuts) << "\n";
    std::cout << env::SYSTEM_COLOR << "apply_xB_in_QE_range_cuts:" << env::RESET_COLOR << "               \t" << bt::BoolToString(CutSettings.apply_xB_in_QE_range_cuts) << "\n\n";
    std::cout << env::SYSTEM_COLOR << "apply_theta_q_pCD_in_lower_FSI_range_cut:" << env::RESET_COLOR << "\t" << bt::BoolToString(CutSettings.apply_theta_q_pCD_in_lower_FSI_range_cut)
              << "\n\n";
}

};  // namespace printers

#endif  // PRINTERS_H
