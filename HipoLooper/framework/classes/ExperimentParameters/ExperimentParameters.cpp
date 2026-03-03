#ifndef EXPERIMENTPARAMETERS_H
#define EXPERIMENTPARAMETERS_H

#include "ExperimentParameters.h"

// ======================================================================================================================================================================
// ExperimentParameters
//
// This class derives from TargetParameters and centralizes all run-time configuration derived from a sample path:
//   - Target type (H1/D2/C12/Cx4/Ar40/Uniform)
//   - Sample type (GENIE simulation / data / uniform)
//   - GENIE tune (G18 vs SuSa)
//   - Beam energy bucket (2, 4, 6 GeV) inferred from the path
//   - Q2 cut category (for GENIE truth generation)
//   - A canonical `SampleName` string used throughout the analysis
//   - Vz and dVz cut objects chosen by sample name
//
// The constructor runs a sequence of \"Config*\" methods that parse `RecoSamplePath` and set member variables.
// IMPORTANT: These Config* methods are *mutators* (void functions). They must be called from the constructor body
// (not from the initializer list) because initializer lists can only initialize data members and base classes.
// ======================================================================================================================================================================

// ======================================================================================================================================================================
// Constructor
// ======================================================================================================================================================================

#pragma region /* Constructor */
ExperimentParameters::ExperimentParameters(const std::string& RecoSamplePath, const std::string& ReconHipoDir) {
    // 1) Determine the target element from the sample path (also sets PDG and nuclear numbers in TargetParameters).
    this->ConfigSampleTarget(RecoSamplePath);

    // 2) Determine whether this is GENIE simulation, farm data, or a uniform sample.
    this->ConfigSampleType(RecoSamplePath);

    // 3) If this is GENIE simulation, infer the GENIE tune from the path (G18 vs SuSa).
    this->ConfigGENIETune(RecoSamplePath);

    // 4) Infer the beam-energy bucket from the path (2/4/6 GeV). This also sets `SampleBeamEnergyStr`.
    this->ConfigBeamEnergy(RecoSamplePath);

    // 5) For GENIE simulation, infer the Q2 cut category based on beam energy.
    this->ConfigQ2Cut(RecoSamplePath);

    // 6) Build a canonical analysis sample name string (depends on target/type/tune/Q2/energy, or run directory for data).
    this->ConfigSampleName(RecoSamplePath, ReconHipoDir);

    // 7) Derive a "varying" sample name used by acceptance maps / weight maps / neutron resolution inputs.
    this->ConfigureVaryingSampleName(SampleName);

    // 8) Configure vertex-z cuts (Vz) and delta-vertex cuts (dVz) based on the canonical sample name.
    this->ConfigureVz_cuts(SampleName);
    this->ConfiguredVz_cuts(SampleName);

    // 9) Check if sample is local or on the ifarm:
    // Currently based on path, but can be changed to a more robust way in the future.
    // If the path contains "volatile", it is simulation. If it contains "clas12/rg-m/production", data on the ifarm. Otherwise, it is local.
    LocalSample = (bt::FindSubstring(RecoSamplePath, "volatile") || bt::FindSubstring(RecoSamplePath, "clas12/rg-m/production")) ? false : true;
}
#pragma endregion

// ======================================================================================================================================================================
// Configure functions
// ======================================================================================================================================================================

// ConfigSampleTarget function ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigSampleTarget function */
/**
 * Infer the target element from the sample path.
 *
 * This fills both:
 *   - ExperimentParameters members: SampleTargetStr
 *   - TargetParameters members: TargetElement, PDG code, A/Z/L/I
 *
 * Matching is substring-based so the path can include additional tokens (run-group tags, suffixes, etc.).
 */
void ExperimentParameters::ConfigSampleTarget(const std::string& RecoSamplePath) {
    const bool PrintOut = false;

    if (bt::FindSubstring(RecoSamplePath, "/H1/") || bt::FindSubstring(RecoSamplePath, "/H/") || bt::FindSubstring(RecoSamplePath, "/H1_")) {  // Hydrogen
        TargetElement = SampleTargetStr = "H1";
        TargetElementPDG = 1000010010;
        TotalBaryonNumber_A = 1;
        TotalChargeNumber_Z = 1;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else if (bt::FindSubstring(RecoSamplePath, "/D2/") || bt::FindSubstring(RecoSamplePath, "/D/") || bt::FindSubstring(RecoSamplePath, "/D2_")) {  // Deuterium
        TargetElement = SampleTargetStr = "D2";
        TargetElementPDG = 1000010020;
        TotalBaryonNumber_A = 2;
        TotalChargeNumber_Z = 1;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else if ((bt::FindSubstring(RecoSamplePath, "/C12/") || bt::FindSubstring(RecoSamplePath, "/C/") || bt::FindSubstring(RecoSamplePath, "/C12_"))  // Carbon (1-foil)
               || bt::FindSubstring(RecoSamplePath, "/Cx4/") || bt::FindSubstring(RecoSamplePath, "/Cx4_")                                             // Carbon (4-foil)
    ) {
        TargetElement = SampleTargetStr = "C12";
        TargetElementPDG = 1000060120;
        TotalBaryonNumber_A = 12;
        TotalChargeNumber_Z = 6;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else if (bt::FindSubstring(RecoSamplePath, "/Ar40/") || bt::FindSubstring(RecoSamplePath, "/Ar/") || bt::FindSubstring(RecoSamplePath, "/Ar40_")) {  // Argon
        TargetElement = SampleTargetStr = "Ar40";
        TargetElementPDG = 1000180400;
        TotalBaryonNumber_A = 40;
        TotalChargeNumber_Z = 18;
        StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else if (bt::FindSubstring(RecoSamplePath, "/Uniform")) {  // Uniform samples
        TargetElement = SampleTargetStr = "Uniform";
        TargetElementPDG = -9999;
        TotalBaryonNumber_A = TotalChargeNumber_Z = StrangeQuarksNumber_L = IsomerNumber_I = 0;
    } else {
        SampleTargetStr = "UNKOWN";
        TargetElementPDG = -9999;
        TotalBaryonNumber_A = TotalChargeNumber_Z = StrangeQuarksNumber_L = IsomerNumber_I = 0;
    }

    if (SampleTargetStr == "UNKOWN") { bt::ExitWithError("ExperimentParameters::ConfigSampleTarget", __FILE__, __LINE__, "Unable to configure sample target!"); }

    if (PrintOut) {
        std::cout << std::endl;
        std::cout << env::SYSTEM_COLOR << "Target element:          " << env::RESET_COLOR << TargetElement << std::endl;
        std::cout << env::SYSTEM_COLOR << "Sample target:           " << env::RESET_COLOR << SampleTargetStr << std::endl;
        std::cout << env::SYSTEM_COLOR << "Target element PDG:      " << env::RESET_COLOR << TargetElementPDG << std::endl;
        std::cout << env::SYSTEM_COLOR << "Total Baryon Number A:   " << env::RESET_COLOR << TotalBaryonNumber_A << std::endl;
        std::cout << env::SYSTEM_COLOR << "Total Charge Number Z:   " << env::RESET_COLOR << TotalChargeNumber_Z << std::endl;
        std::cout << env::SYSTEM_COLOR << "Strange Quarks Number L: " << env::RESET_COLOR << StrangeQuarksNumber_L << std::endl;
        std::cout << env::SYSTEM_COLOR << "Isomer Number I:         " << env::RESET_COLOR << IsomerNumber_I << std::endl;
        std::cout << std::endl;
    }
}
#pragma endregion

// ConfigSampleType function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigSampleType function */
/**
 * Infer sample type from path heuristics:
 *   - GENIE simulation: path contains \"GENIE_Reco_Samples\"
 *   - farm data: path contains the canonical production directory \"clas12/rg-m/production\"
 *   - uniform: path contains \"Uniform\"
 *
 * If none match, we abort because subsequent configuration depends on knowing which naming rules to apply.
 */
void ExperimentParameters::ConfigSampleType(const std::string& RecoSamplePath) {
    const bool PrintOut = false;

    sampleType = (bt::FindSubstring(RecoSamplePath, "GENIE_Reco_Samples"))       ? GENIE_SIMULATION_TYPE
                 : (bt::FindSubstring(RecoSamplePath, "clas12/rg-m/production")) ? DATA_TYPE
                 : (bt::FindSubstring(RecoSamplePath, "Uniform"))                ? UNIFORM_TYPE
                                                                                 : UNKNOWN_TYPE;
    if (sampleType == UNKNOWN_TYPE) { bt::ExitWithError("ExperimentParameters::ConfigSampleType", __FILE__, __LINE__, "Unable to configure sample type!"); }

    // if (sampleType == GENIE_SIMULATION_TYPE || sampleType == UNIFORM_TYPE) {
    //     SimulationSample = true;
    //     DataSample = false;
    // } else if (sampleType == DATA_TYPE) {
    //     DataSample = true;
    //     SimulationSample = false;
    // }

    SampleTypeStr = (sampleType == GENIE_SIMULATION_TYPE) ? "sim" : (sampleType == DATA_TYPE) ? "data" : "";

    if (PrintOut) {
        std::cout << std::endl;
        std::cout << env::SYSTEM_COLOR << "Sample type: " << env::RESET_COLOR << sampleType << std::endl;
        std::cout << env::SYSTEM_COLOR << "Sample type string: " << env::RESET_COLOR << SampleTypeStr << std::endl;
        std::cout << std::endl;
    }
}
#pragma endregion

// ConfigGENIETune function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigGENIETune function */
/**
 * Configure GENIE tune information.
 *
 * Only meaningful for GENIE simulation samples. For data and uniform samples we leave the tune unset/empty.
 * Mapping is heuristic:
 *   - if path contains \"G18\"      -> G18_TUNE
 *   - else if contains \"GEM21\"   -> SUSA_TUNE (SuSa)
 *   - else                          UNKNOWN_TUNE (will later trigger an error for GENIE samples)
 */
void ExperimentParameters::ConfigGENIETune(const std::string& RecoSamplePath) {
    if (sampleType == GENIE_SIMULATION_TYPE) {
        genieTune = (bt::FindSubstring(RecoSamplePath, "G18")) ? G18_TUNE : (bt::FindSubstring(RecoSamplePath, "GEM21")) ? SUSA_TUNE : UNKNOWN_TUNE;

        SampleGENIETuneStr = (genieTune == G18_TUNE) ? "G18" : (genieTune == SUSA_TUNE) ? "SuSa" : "";
    } else {
        genieTune = UNSET_TUNE;

        SampleGENIETuneStr = "";
    }
}
#pragma endregion

// ConfigQ2Cut function -------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigQ2Cut function */
/**
 * Infer beam-energy bucket from path substrings.
 *
 * Heuristic approach:
 *   - 2070 or 2GeV tokens -> BEAM_AT_2GEV
 *   - 4029 or 4GeV tokens -> BEAM_AT_4GEV
 *   - 5986 or 6GeV tokens -> BEAM_AT_6GEV
 *
 * Note: substring heuristics can produce false matches if the digits appear in unrelated tokens.
 * In practice your directory naming convention is the contract that makes this safe.
 */
void ExperimentParameters::ConfigQ2Cut(const std::string& RecoSamplePath) {
    if (sampleType == GENIE_SIMULATION_TYPE) {
        Q2cut = (beamEnergy == BEAM_AT_2GEV) ? GENIE_DEF_CUT : (beamEnergy == BEAM_AT_4GEV) ? Q2_0_25_CUT : (beamEnergy == BEAM_AT_6GEV) ? Q2_0_4_CUT : UNKNOWN_CUT;

        SampleQ2CutStr = (Q2cut == GENIE_DEF_CUT) ? "Q2_0_02" : (Q2cut == Q2_0_25_CUT) ? "Q2_0_25" : (Q2cut == Q2_0_4_CUT) ? "Q2_0_40" : "";
    } else {
        Q2cut = UNSET_CUT;

        SampleQ2CutStr = "";
    }
}
#pragma endregion

// ConfigBeamEnergy function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigBeamEnergy function */
/**
 * Configure Q2 cut category for GENIE simulation samples.
 *
 * In this analysis code the Q2 cut label is inferred from beam-energy bucket:
 *   - 2 GeV -> GENIE_DEF_CUT -> \"Q2_0_02\"
 *   - 4 GeV -> Q2_0_25_CUT   -> \"Q2_0_25\"
 *   - 6 GeV -> Q2_0_4_CUT    -> \"Q2_0_40\"
 *
 * For data/uniform, we leave it unset/empty.
 */
void ExperimentParameters::ConfigBeamEnergy(const std::string& RecoSamplePath) {
    beamEnergy =
        (bt::FindSubstring(RecoSamplePath, "2070") || (bt::FindSubstring(RecoSamplePath, "2GeV") || bt::FindSubstring(RecoSamplePath, "2Gev") || bt::FindSubstring(RecoSamplePath, "2gev")))
            ? BEAM_AT_2GEV
        : (bt::FindSubstring(RecoSamplePath, "4029") || (bt::FindSubstring(RecoSamplePath, "4GeV") || bt::FindSubstring(RecoSamplePath, "4Gev") || bt::FindSubstring(RecoSamplePath, "4gev")))
            ? BEAM_AT_4GEV
        : (bt::FindSubstring(RecoSamplePath, "5986") || (bt::FindSubstring(RecoSamplePath, "6GeV") || bt::FindSubstring(RecoSamplePath, "6Gev") || bt::FindSubstring(RecoSamplePath, "6gev")))
            ? BEAM_AT_6GEV
            : UNKNOWN_ENERGY;
    if (beamEnergy == UNKNOWN_ENERGY) { bt::ExitWithError("ExperimentParameters::ConfigBeamEnergy", __FILE__, __LINE__, "Unable to configure beam energy!"); }

    if (beamEnergy == BEAM_AT_2GEV) {
        SampleBeamEnergyStr = "2070MeV";
        BeamEnergyValue = 2.07052;
    } else if (beamEnergy == BEAM_AT_4GEV) {
        SampleBeamEnergyStr = "4029MeV";
        BeamEnergyValue = 4.02962;
    } else if (beamEnergy == BEAM_AT_6GEV) {
        SampleBeamEnergyStr = "5986MeV";
        BeamEnergyValue = 5.98636;
    }
}
#pragma endregion

// ConfigSampleName function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigSampleName function */
/**
 * Build the canonical `SampleName` used across the analysis.
 *
 * Naming conventions:
 *   - GENIE simulation:
 *       <Target>_<sim>_<Tune>_<Q2Label>_<EnergyLabel>
 *       Example: C12_sim_G18_Q2_0_02_2070MeV
 *
 *   - Data:
 *       <Target>_<data>_<EnergyLabel>[_run_<ReconHipoDir>]
 *       Example: C12_data_2070MeV_run_015664
 *
 *   - Uniform:
 *       Either tester labels, or:
 *       <Target>_<ParticleType>_sample_<EnergyLabel>
 *       Example: Uniform_en_sample_2070MeV
 */
void ExperimentParameters::ConfigSampleName(const std::string& RecoSamplePath, const std::string& ReconHipoDir) {
    if (sampleType == UNSET_TYPE) { bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name - type is unset!"); }

    if (sampleType == GENIE_SIMULATION_TYPE) {
        if (genieTune == UNKNOWN_TUNE) { bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name - GENIE tune is unknown!"); }
        if (Q2cut == UNKNOWN_CUT) { bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name - Q2 cut is unknown!"); }
    }

    if (beamEnergy == UNSET_ENERGY) { bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name - beam energy is unset!"); }

    if (sampleType == GENIE_SIMULATION_TYPE) {
        SampleName = SampleTargetStr + "_" + SampleTypeStr + "_" + SampleGENIETuneStr + "_" + SampleQ2CutStr + "_" + SampleBeamEnergyStr;
    } else if (sampleType == DATA_TYPE) {
        SampleName = SampleTargetStr + "_" + SampleTypeStr + "_" + SampleBeamEnergyStr + ((ReconHipoDir != "") ? "_run_" + ReconHipoDir : "");
    } else if (sampleType == UNIFORM_TYPE) {
        if (bt::FindSubstring(RecoSamplePath, "OutPut_Tester_e_Tester_e")) {  // Electron tester samples
            if (beamEnergy == BEAM_AT_2GEV) {
                SampleName = "Uniform_sample_e_tester_2070MeV";
            } else if (beamEnergy == BEAM_AT_4GEV) {
                SampleName = "Uniform_sample_e_tester_4029MeV";
            } else if (beamEnergy == BEAM_AT_6GEV) {
                SampleName = "Uniform_sample_e_tester_5986MeV";
            }
        } else {
            std::string ParticleType = (bt::FindSubstring(RecoSamplePath, "OutPut_1e"))   ? "1e"
                                       : (bt::FindSubstring(RecoSamplePath, "OutPut_ep")) ? "ep"
                                       : (bt::FindSubstring(RecoSamplePath, "OutPut_en")) ? "en"
                                                                                          : "";
            if (ParticleType == "") {
                bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name - unknown particle type in unform sample!");
            }

            SampleName = SampleTargetStr + "_" + ParticleType + "_sample_" + SampleBeamEnergyStr;
        }
    } else {
        bt::ExitWithError("ExperimentParameters::ConfigSampleName", __FILE__, __LINE__, "Unable to configure sample name!");
    }
}
#pragma endregion

// ConfigureVaryingSampleName function ----------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigureVaryingSampleName function */
/**
 * Configure the name used to select varying inputs (acceptance maps, weight maps, neutron resolution, etc.).
 *
 * Current behavior:
 *   - Use the configured sample name directly for all supported sample types.
 */
void ExperimentParameters::ConfigureVaryingSampleName(const std::string& sn) {
    if ((sampleType == GENIE_SIMULATION_TYPE) || (sampleType == UNIFORM_TYPE) || (sampleType == DATA_TYPE)) {
        VaryingSampleName = sn;
    } else {
        bt::ExitWithError("ExperimentParameters::ConfigureVaryingSampleName", __FILE__, __LINE__, "Varying sample name can't be configured!");
    }
}
#pragma endregion

// ConfigureVz_cuts function --------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigureVz_cuts function */
void ExperimentParameters::ConfigureVz_cuts(const std::string& sn) {
    const char* kTitle = "Vertex z component";
    const char* kCutLabel = "1e cut";

    auto set_cuts = [&](double lo, double hi, double loFD, double hiFD, double loCD, double hiCD) {
        Vz_cuts = DSCuts(kTitle, "", "", kCutLabel, 0, lo, hi);
        Vz_cuts_FD = DSCuts(kTitle, "FD", "", kCutLabel, 0, loFD, hiFD);
        Vz_cuts_CD = DSCuts(kTitle, "CD", "", kCutLabel, 0, loCD, hiCD);
    };

    auto set_same_all = [&](double lo, double hi) { set_cuts(lo, hi, lo, hi, lo, hi); };

    if (sampleType == GENIE_SIMULATION_TYPE) {         // Sample is simulation
        if (sn == "C12x4_simulation_G18_Q204_6GeV") {  // 4-foil
            set_cuts(-7., 2., -8., 3., -7., 2.);
        } else if (sn == "C12_simulation_G18_Q204_6GeV") {  // 1-foil
            set_cuts(-5., 5., -5., 5., -2., 1.);
        } else if (sn == "C12_simulation_G18_Q2_0_02_2070MeV" || sn == "C12_simulation_G18_Q2_0_02_2070MeV_wFC" || sn == "C12_simulation_G18_Q2_0_25_4029MeV" ||
                   sn == "C12_simulation_G18_Q2_0_25_4029MeV_wFC" || sn == "C12_simulation_SuSa_Q2_0_02_2070MeV" || sn == "C12_simulation_SuSa_Q2_0_02_2070MeV_wFC" ||
                   sn == "C12_simulation_SuSa_Q2_0_25_4029MeV" || sn == "C12_simulation_SuSa_Q2_0_25_4029MeV_wFC") {
            set_cuts(-5., 4., -5., 4., -2., 1.);
        }
        // V_{z} cuts for the new simulation samples with corrected geometry. Cuts to be defined based on the RGM analysis note (RGM_Analysis_Note_Feb11-2026.pdf):
        else if (sn == "C12_sim_G18_Q2_0_02_2070MeV" || sn == "C12_sim_SuSa_Q2_0_02_2070MeV"     // C12; 1-foil GENIE G18/SuSa samples at 2 GeV (small foils)
                 || sn == "C12_sim_G18_Q2_0_25_4029MeV" || sn == "C12_sim_SuSa_Q2_0_25_4029MeV"  // C12; 1-foil GENIE G18/SuSa samples at 4 GeV (large foils)
        ) {
            set_cuts(-5., 0., -6., 0.7, -2.8, -1.5);  // Cuts from RGM_Analysis_Note_Feb11-2026.pdf
        } else if (sn == "Ar40_sim_G18_Q2_0_02_2070MeV" || sn == "Ar40_sim_G18_Q2_0_25_4029MeV" || sn == "Ar40_sim_SuSa_Q2_0_02_2070MeV" || sn == "Ar40_sim_SuSa_Q2_0_25_4029MeV") {
            set_cuts(-8.5, -3.5, -9.9, -2.6, -6.7, -4.8);  // Cuts from RGM_Analysis_Note_Feb11-2026.pdf
        }
    } else if (sampleType == DATA_TYPE) {  // Sample is data
        if (sn == "Ar40_data_2GeV_run_015672") {
            set_cuts(-7., 2., -12., -3., -7., -4.);
        } else if (sn == "Ar40_data_4GeV_run_015743") {
            set_cuts(-7., 2., -9., -2., -7., -4.);
        } else if (sn == "Ar40_data_6GeV_run_015792") {
            set_cuts(-7., 2., -9., -2., -7., -4.);
        } else if (sn == "C12_data_2GeV_run_015664") {
            set_cuts(-7., 2., -9., -1., -4., 0.);
        } else if (sn == "C12_data_4GeV_run_015778") {
            set_cuts(-7., 2., -6., 1., -4., -1.);
        } else if (sn == "D2_data_2GeV") {
            set_cuts(-7., 2., -10., 1., -7., 0.);
        } else if (sn == "D2_data_6GeV_run_015443") {
            set_cuts(-7., 2., -7., 3., -7., 1.);
        } else if (sn == "H1_data_2GeV_run_015634") {
            set_cuts(-7., 2., -11., 1., -7., 0.);
        } else if (sn == "H1_data_6GeV_run_015028") {
            set_cuts(-7., 2., -7., 1., -7., 1.);
        } else if (bt::FindSubstring(sn, "C12x4_data_6GeV_run_0151")) {
            if (bt::FindSubstring(sn, "C12x4_data_6GeV_run_015186") || bt::FindSubstring(sn, "C12x4_data_6GeV_run_015187")) {  // 4-foil
                set_same_all(-7., 2.);
            } else if (bt::FindSubstring(sn, "C12x4_data_6GeV_run_015188")) {  // 4-foil
                set_cuts(-7., 2., -8., 3., -7., 2.);
            } else {
                Vz_cuts = Vz_cuts_def;
                Vz_cuts_FD = Vz_cuts_FD_def;
                Vz_cuts_CD = Vz_cuts_CD_def;
            }
        } else if (sn == "C12x4_data_6GeV") {  // Sample is data (full 4-foil run)
            set_cuts(-7., 2., -8., 3., -7., 2.);

        } else if (sn == "LH2_data_6GeV_run_015032") {
            set_same_all(-15., 5.);
        }
    } else if (sampleType == UNIFORM_TYPE) {  // Sample is uniform simulation
        if (beamEnergy == BEAM_AT_2GEV) {
            if (sn == "Uniform_1e_sample_2070MeV") {
                set_same_all(-6., 0.);
            } else if (sn == "Uniform_ep_sample_2070MeV") {
                set_cuts(-6., 0., -6., 0., -5., -1.);
            } else if (sn == "Uniform_en_sample_2070MeV") {
                set_same_all(-6., 0.);
            }
        } else if (beamEnergy == BEAM_AT_4GEV) {
            if (sn == "Uniform_1e_sample_4029MeV") {
                set_same_all(-5., -1.);
            } else if (sn == "Uniform_ep_sample_4029MeV") {
                set_cuts(-6., 0., -6., 0., -5., -1.);
            } else if (sn == "Uniform_en_sample_4029MeV") {
                set_same_all(-6., 0.);
            }
        } else if (beamEnergy == BEAM_AT_6GEV) {
            if (sn == "Uniform_1e_sample_5986MeV") {
                set_same_all(-5., -1.);
            } else if (sn == "Uniform_ep_sample_5986MeV") {
                set_cuts(-6., 0., -6., 0., -5., -1.);
            } else if (sn == "Uniform_en_sample_5986MeV") {
                set_same_all(-6., 0.);
            }
        }
    } else {
        set_same_all(-5., 5.);
    }
}
#pragma endregion

// ConfiguredVz_cuts function -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfiguredVz_cuts function */
void ExperimentParameters::ConfiguredVz_cuts(const std::string& sn) {
    const char* kTitle = "dVz";
    const char* kCutLabel = "1e cut";

    auto set_cuts = [&](double lo, double hi, double loFD, double hiFD, double loCD, double hiCD) {
        dVz_cuts = DSCuts(kTitle, "", "", kCutLabel, 0, lo, hi);
        dVz_cuts_FD = DSCuts(kTitle, "FD", "", kCutLabel, 0, loFD, hiFD);
        dVz_cuts_CD = DSCuts(kTitle, "CD", "", kCutLabel, 0, loCD, hiCD);
    };

    auto set_same_all = [&](double lo, double hi) { set_cuts(lo, hi, lo, hi, lo, hi); };

    if (sampleType == UNIFORM_TYPE) {  // Sample is uniform simulation
        if (beamEnergy == BEAM_AT_2GEV) {
            if (sn == "Uniform_1e_sample_2070MeV") {
                set_same_all(-9999., 9999.);
            } else if (sn == "Uniform_ep_sample_2070MeV") {
                set_same_all(-2.5, 2.5);
            } else if (sn == "Uniform_en_sample_2070MeV") {
                set_same_all(-9999., 9999.);
            }
        } else if (beamEnergy == BEAM_AT_4GEV) {
            if (sn == "Uniform_1e_sample_4029MeV") {
                set_same_all(-9999., 9999.);
            } else if (sn == "Uniform_ep_sample_4029MeV") {
                set_same_all(-2.5, 2.5);
            } else if (sn == "Uniform_en_sample_4029MeV") {
                set_same_all(-9999., 9999.);
            }
        } else if (beamEnergy == BEAM_AT_6GEV) {
            if (sn == "Uniform_1e_sample_5986MeV") {
                set_same_all(-9999., 9999.);
            } else if (sn == "Uniform_ep_sample_5986MeV") {
                set_same_all(-2.5, 2.5);
            } else if (sn == "Uniform_en_sample_5986MeV") {
                set_same_all(-9999., 9999.);
            }
        }
    } else if (sampleType == GENIE_SIMULATION_TYPE) {  // Sample is simulation
        if (sn == "C12x4_simulation_G18_Q204_6GeV") {  // 4-foil
            set_same_all(-5., 4.);
        } else if (sn == "C12_simulation_G18_Q204_6GeV") {  // 1-foil
            set_same_all(-5., 4.);
        } else if (sn == "C12_simulation_G18_Q2_0_02_2070MeV" || sn == "C12_simulation_G18_Q2_0_02_2070MeV_wFC" || sn == "C12_simulation_G18_Q2_0_25_4029MeV" ||
                   sn == "C12_simulation_G18_Q2_0_25_4029MeV_wFC" || sn == "C12_simulation_SuSa_Q2_0_02_2070MeV" || sn == "C12_simulation_SuSa_Q2_0_02_2070MeV_wFC" ||
                   sn == "C12_simulation_SuSa_Q2_0_25_4029MeV" || sn == "C12_simulation_SuSa_Q2_0_25_4029MeV_wFC") {
            set_cuts(-6., 6., -6., 6., -4., 4.);
        }
        // dV_{z} cuts for the new simulation samples with corrected geometry. Cuts to be defined based on the RGM analysis note (RGM_Analysis_Note_Feb11-2026.pdf):
        else if (sn == "C12_sim_G18_Q2_0_02_2070MeV" || sn == "C12_sim_SuSa_Q2_0_02_2070MeV"     // C12; 1-foil GENIE G18/SuSa samples at 2 GeV (small foils)
                 || sn == "C12_sim_G18_Q2_0_25_4029MeV" || sn == "C12_sim_SuSa_Q2_0_25_4029MeV"  // C12; 1-foil GENIE G18/SuSa samples at 4 GeV (large foils)
        ) {
            set_cuts(-3.5, 5.8, -3.5, 5.8, -1.8, 3.1);  // Cuts from RGM_Analysis_Note_Feb11-2026.pdf
        } else if (sn == "Ar40_sim_G18_Q2_0_02_2070MeV" || sn == "Ar40_sim_G18_Q2_0_25_4029MeV" || sn == "Ar40_sim_SuSa_Q2_0_02_2070MeV" || sn == "Ar40_sim_SuSa_Q2_0_25_4029MeV") {
            set_cuts(-3.5, 5.8, -3.5, 5.8, -1.8, 3.1);  // Cuts from RGM_Analysis_Note_Feb11-2026.pdf
        }
    } else if (sampleType == DATA_TYPE) {  // Sample is data
        if (sn == "Ar40_data_2GeV_run_015672") {
            set_cuts(-5., 4., -4., 8., -2., 6.);
        } else if (sn == "Ar40_data_4GeV_run_015743") {
            set_cuts(-5., 4., -8., 4., -4., 2.);
        } else if (sn == "Ar40_data_6GeV_run_015792") {
            set_cuts(-5., 4., -8., 4., -4., 2.);
        } else if (sn == "C12_data_2GeV_run_015664") {
            set_cuts(-5., 4., -3., 5., -2., 5.);
        } else if (sn == "C12_data_4GeV_run_015778") {
            set_cuts(-5., 4., -6., 3., -4., 2.);
        } else if (sn == "D2_data_2GeV") {
            set_cuts(-5., 4., -5., 7., -2., 5.);
        } else if (sn == "D2_data_6GeV_run_015443") {
            set_cuts(-5., 4., -7., 3., -5., 3.);
        } else if (sn == "H1_data_2GeV_run_015634") {
            set_cuts(-5., 4., -3., 6., -2., 5.);
        } else if (sn == "H1_data_6GeV_run_015028") {
            set_cuts(-5., 4., -6., 2., -4., 2.);
        } else if (bt::FindSubstring(sn, "C12x4_data_6GeV_run_0151")) {
            if (bt::FindSubstring(sn, "C12x4_data_6GeV_run_015186") || bt::FindSubstring(sn, "C12x4_data_6GeV_run_015187") ||
                bt::FindSubstring(sn, "C12x4_data_6GeV_run_015188")) {  // 4-foil
                set_same_all(-5., 4.);
            } else {
                dVz_cuts = dVz_cuts_def;
                dVz_cuts_FD = dVz_cuts_FD_def;
                dVz_cuts_CD = dVz_cuts_CD_def;
            }
        } else if (sn == "C12x4_data_6GeV") {  // Sample is data (full 4-foil run)
            set_same_all(-5., 4.);

        } else if (sn == "LH2_data_6GeV_run_015032") {
            set_same_all(-8., 4.);
        }
    } else {
        set_same_all(-8., 4.);
    }
}
#pragma endregion

// ConfigureBeamEnergy function -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ConfigureBeamEnergy function */
double ExperimentParameters::ConfigureBeamEnergy(const std::string& sn) {
    double be;

    if (bt::FindSubstring(sn, "598636MeV") || bt::FindSubstring(sn, "598636mev") || bt::FindSubstring(sn, "598636") || bt::FindSubstring(sn, "5986MeV") || bt::FindSubstring(sn, "5986mev") ||
        bt::FindSubstring(sn, "5986") || bt::FindSubstring(sn, "6GeV") || bt::FindSubstring(sn, "6Gev") || bt::FindSubstring(sn, "6gev")) {
        be = 5.98636;
    } else if (bt::FindSubstring(sn, "402962MeV") || bt::FindSubstring(sn, "402962mev") || bt::FindSubstring(sn, "402962") || bt::FindSubstring(sn, "4029MeV") ||
               bt::FindSubstring(sn, "4029mev") || bt::FindSubstring(sn, "4029") || bt::FindSubstring(sn, "4GeV") || bt::FindSubstring(sn, "4Gev") || bt::FindSubstring(sn, "4gev")) {
        be = 4.02962;
    } else if (bt::FindSubstring(sn, "207052MeV") || bt::FindSubstring(sn, "207052mev") || bt::FindSubstring(sn, "207052") || bt::FindSubstring(sn, "2070MeV") ||
               bt::FindSubstring(sn, "2070mev") || bt::FindSubstring(sn, "2070") || bt::FindSubstring(sn, "2GeV") || bt::FindSubstring(sn, "2Gev") || bt::FindSubstring(sn, "2gev")) {
        be = 2.07052;
    }

    return be;
}
#pragma endregion

// AddToHipoChain function ----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* AddToHipoChain function */
// This is the old function used to add runs to the HipoChain
void ExperimentParameters::AddToHipoChain(HipoChain& chain, const std::string& sn, const std::string& RecoSamplePath, const std::string& ReconHipoDir, const std::string& InputHipoFiles) {
    bool PrintOut = true;

    if (sampleType == DATA_TYPE) {
        if (sn == "D2_data_2070MeV") {
            if (ReconHipoDir == "") {
                /* Data in cache/clas12/rg-m/production/pass1/2gev/D/dst/recon */
                std::vector<std::string> Runs = {"015567", "015573", "015578", "015583", "015590", "015595", "015602", "015608", "015613", "015618", "015624", "015568", "015574", "015579",
                                                 "015586", "015591", "015598", "015603", "015609", "015614", "015619", "015625", "015569", "015575", "015580", "015587", "015592", "015599",
                                                 "015604", "015610", "015615", "015620", "015626", "015570", "015576", "015581", "015588", "015593", "015600", "015606", "015611", "015616",
                                                 "015622", "015627", "015572", "015577", "015582", "015589", "015594", "015601", "015607", "015612", "015617", "015623"};

                for (int i = 0; i < Runs.size(); i++) {
                    std::string TempInputHipoFiles = "/" + RecoSamplePath + "/" + Runs.at(i) + "/*.hipo";
                    chain.Add(TempInputHipoFiles.c_str());

                    if (PrintOut) { std::cout << TempInputHipoFiles << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { std::cout << "\n"; }
            }
        } else if (sn == "C12x4_data_5986MeV") {
            if (ReconHipoDir == "") {
                /* Data in cache/clas12/rg-m/production/pass1/6gev/Cx4/dst/recon */
                std::vector<std::string> Runs = {"015186", "015187", "015188", "015189", "015190", "015191", "015192", "015193", "015194", "015196", "015199", "015200", "015202",
                                                 "015203", "015204", "015205", "015206", "015207", "015210", "015212", "015213", "015214", "015215", "015217", "015219", "015220",
                                                 "015221", "015223", "015224", "015225", "015226", "015228", "015234", "015235", "015236", "015238", "015239", "015240", "015241",
                                                 "015242", "015243", "015245", "015246", "015247", "015248", "015249", "015250", "015252", "015253", "015254", "015255", "015257",
                                                 "015258", "015259", "015260", "015261", "015262", "015263", "015264", "015265", "015266", "015269", "015270", "015271", "015272",
                                                 "015273", "015274", "015275", "015278", "015279", "015280", "015282", "015283", "015284", "015286", "015287", "015288", "015289",
                                                 "015290", "015291", "015292", "015293", "015294", "015295", "015296", "015298", "015300", "015301", "015302", "015303", "015304",
                                                 "015305", "015306", "015307", "015308", "015309", "015310", "015311", "015312", "015313", "015314", "015316", "015317"};

                for (int i = 0; i < Runs.size(); i++) {
                    std::string TempInputHipoFiles = "/" + RecoSamplePath + "/" + Runs.at(i) + "/*.hipo";
                    chain.Add(TempInputHipoFiles.c_str());

                    if (PrintOut) { std::cout << TempInputHipoFiles << " directory added to HipoChain!\n"; }
                }

                if (PrintOut) { std::cout << "\n"; }
            }
        } else {
            chain.Add(InputHipoFiles.c_str());
        }
    } else if (sampleType == GENIE_SIMULATION_TYPE) {
        chain.Add(InputHipoFiles.c_str());

        if (PrintOut) { std::cout << InputHipoFiles << " directory added to HipoChain!\n\n"; }
    }
}
#pragma endregion

// AddToHipoChainFromList function --------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* AddToHipoChainFromList function */
void ExperimentParameters::AddToHipoChainFromList(HipoChain& chain, const std::string& sn, const std::string& RecoSamplePath, const std::string& ReconHipoDir,
                                                  const std::string& InputHipoFiles) {
    const bool PrintOut = true;

    // Determine the effective sample type as robustly as possible.
    // The legacy implementation keyed off DataSample/SimulationSample, which are not guaranteed to be set in all code paths.
    // Here we prefer the configured `sampleType`, and fall back to inference from `sn` and `RecoSamplePath` if needed.
    auto infer_sample_type = [&](void) -> int {
        if (sampleType == DATA_TYPE || sampleType == GENIE_SIMULATION_TYPE || sampleType == UNIFORM_TYPE) { return sampleType; }

        // Fallback: infer from `sn`
        if (bt::FindSubstring(sn, "_data_") || bt::FindSubstring(sn, "_data")) { return DATA_TYPE; }
        if (bt::FindSubstring(sn, "_simulation_") || bt::FindSubstring(sn, "GENIE") || bt::FindSubstring(sn, "G18") || bt::FindSubstring(sn, "GEM21") || bt::FindSubstring(sn, "SuSa") ||
            bt::FindSubstring(sn, "Uniform")) {
            // Note: "Uniform" here is used as a proxy for uniform samples.
            return (bt::FindSubstring(sn, "Uniform")) ? UNIFORM_TYPE : GENIE_SIMULATION_TYPE;
        }

        // Fallback: infer from `RecoSamplePath`
        if (bt::FindSubstring(RecoSamplePath, "clas12/rg-m/production") || (bt::FindSubstring(RecoSamplePath, "rg-m") && bt::FindSubstring(RecoSamplePath, "dst"))) { return DATA_TYPE; }
        if (bt::FindSubstring(RecoSamplePath, "GENIE_Reco_Samples")) { return GENIE_SIMULATION_TYPE; }
        if (bt::FindSubstring(RecoSamplePath, "Uniform")) { return UNIFORM_TYPE; }

        return UNKNOWN_TYPE;
    };

    const int effectiveType = infer_sample_type();

    // Ensure we build the same absolute paths as the legacy code:
    // legacy used "/" + AnalyzeFilePath + "/" + run + "/*.hipo" assuming AnalyzeFilePath had no leading '/'.
    auto make_abs_run_glob = [&](const std::string& base, const std::string& run) -> std::string {
        const bool base_has_leading_slash = (!base.empty() && base.front() == '/');
        return std::string(base_has_leading_slash ? "" : "/") + base + "/" + run + "/*.hipo";
    };

    auto add_runs = [&](const std::vector<std::string>& runs) {
        for (size_t i = 0; i < runs.size(); ++i) {
            const std::string TempInputHipoFiles = make_abs_run_glob(RecoSamplePath, runs.at(i));
            chain.Add(TempInputHipoFiles.c_str());
            if (PrintOut) { std::cout << env::SYSTEM_COLOR << TempInputHipoFiles << " directory added to HipoChain!\n" << env::RESET_COLOR; }
        }
        if (PrintOut) { std::cout << env::SYSTEM_COLOR << "\n" << env::RESET_COLOR; }
    };

    // Simulation-like samples: the caller should provide an explicit glob.
    if (effectiveType == GENIE_SIMULATION_TYPE || effectiveType == UNIFORM_TYPE) {
        chain.Add(InputHipoFiles.c_str());
        if (PrintOut) { std::cout << env::SYSTEM_COLOR << InputHipoFiles << " directory added to HipoChain!\n\n" << env::RESET_COLOR; }
        return;
    }

    // Data samples
    if (effectiveType == DATA_TYPE) {
        const std::vector<std::string>* runs = nullptr;

        // Allow names that include a run suffix (e.g. "..._run_015186").
        if (bt::FindSubstring(sn, "H1_data_2GeV") || bt::FindSubstring(sn, "H1_data_2070MeV")) {
            runs = &lists::H1_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "D2_data_2GeV") || bt::FindSubstring(sn, "D2_data_2070MeV")) {
            runs = &lists::D2_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "C12_data_2GeV") || bt::FindSubstring(sn, "C12_data_2070MeV")) {
            runs = &lists::C12_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "Ar40_data_2GeV") || bt::FindSubstring(sn, "Ar40_data_2070MeV")) {
            runs = &lists::Ar40_data_2GeV_runs;
        } else if (bt::FindSubstring(sn, "C12_data_4GeV") || bt::FindSubstring(sn, "C12_data_4029MeV")) {
            runs = &lists::C12_data_4GeV_runs;
        } else if (bt::FindSubstring(sn, "Ar40_data_4GeV") || bt::FindSubstring(sn, "Ar40_data_4029MeV")) {
            runs = &lists::Ar40_data_4GeV_runs;
        } else if (bt::FindSubstring(sn, "H1_data_6GeV") || bt::FindSubstring(sn, "H1_data_5986MeV")) {
            runs = &lists::H1_data_6GeV_runs;
        } else if (bt::FindSubstring(sn, "D2_data_6GeV") || bt::FindSubstring(sn, "D2_data_5986MeV")) {
            runs = &lists::D2_data_6GeV_runs;
        } else if (bt::FindSubstring(sn, "C12x4_data_6GeV") || bt::FindSubstring(sn, "C12x4_data_5986MeV")) {
            runs = &lists::C12x4_data_6GeV_runs;
        } else if (bt::FindSubstring(sn, "Ar40_data_6GeV") || bt::FindSubstring(sn, "Ar40_data_5986MeV")) {
            runs = &lists::Ar40_data_6GeV_runs;
        }

        // Match the legacy behavior exactly:
        // Expand the run list only when the user did not request a specific run directory.
        if (runs != nullptr && ReconHipoDir == "") {
            add_runs(*runs);
            return;
        }

        // Otherwise, fall back to the explicit file/glob passed by the caller.
        chain.Add(InputHipoFiles.c_str());
        return;
    }

    // Unknown/unsupported sample type: fall back to whatever the caller provided.
    chain.Add(InputHipoFiles.c_str());
}
#pragma endregion

#endif  // EXPERIMENTPARAMETERS_H
