// framework nFD_eff_tester_run.sh
// git pull && clas12root -b -q testers/nFD_eff_test.cpp

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

#include <TCanvas.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TString.h>
#include <TStyle.h>
#include <TTree.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <typeinfo>
#include <vector>

// Include libraries:
#include "framework/namespaces/general_utilities/utilities.h"

// Include classes:
#include "framework/classes/clas12ana/clas12ana.h"

// Include CLAS12 libraries:
#include "framework/includes/clas12_include.h"

using namespace constants;

void HipoLooper() {
    std::cout << "\n\nInitiating HipoLooper.cpp\n";

#pragma region Sample selection
    std::string BaseDir = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples";

    // Data samples:

    // std::string InputFiles = "/cache/clas12/rg-m/production/pass1/2gev/C/dst/recon/015664/*.hipo";
    // std::string InputFiles = "/cache/clas12/rg-m/production/pass1/4gev/C/dst/recon/015778/*.hipo";

    std::string InputFiles = "/cache/clas12/rg-m/production/pass1/2gev/Ar/dst/recon/015672/*.hipo";
    // std::string InputFiles = "/cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon/015743/*.hipo";
    // std::string InputFiles = "/cache/clas12/rg-m/production/pass1/6gev/Ar/dst/recon/015792/*.hipo";

    // Simulation samples:

    // std::string InputFiles = BaseDir + "/C12/G18_10a_00_000/2070MeV_Q2_0_02_Ar40_test/reconhipo/*.hipo";
    // std::string InputFiles = BaseDir + "/C12/G18_10a_00_000/4029MeV_Q2_0_25_Ar40_test/reconhipo/*.hipo";

    // std::string InputFiles = BaseDir + "/Ar40/G18_10a_00_000/2070MeV_Q2_0_02_Ar40_test/reconhipo/*.hipo";
    // std::string InputFiles = BaseDir + "/Ar40/G18_10a_00_000/4029MeV_Q2_0_25_Ar40_test/reconhipo/*.hipo";
    // std::string InputFiles = BaseDir + "/Ar40/G18_10a_00_000/5986MeV_Q2_0_40_Ar40_test/reconhipo/*.hipo";
#pragma endregion

#pragma region Setup and configuration
    bool IsData = basic_tools::FindSubstring(InputFiles, "cache");

    double Ebeam = (basic_tools::FindSubstring(InputFiles, "2070MeV") || basic_tools::FindSubstring(InputFiles, "2gev"))   ? 2.07052
                   : (basic_tools::FindSubstring(InputFiles, "4029MeV") || basic_tools::FindSubstring(InputFiles, "4gev")) ? 4.02962
                   : (basic_tools::FindSubstring(InputFiles, "5986MeV") || basic_tools::FindSubstring(InputFiles, "6gev")) ? 5.98636
                                                                                                                           : 0.0;
    if (Ebeam == 0.0) {
        std::cerr << "\n\nError! Ebeam not found in InputFiles string! Aborting...\n\n";
        exit(1);
    }

    bool Is2GeV = (basic_tools::FindSubstring(InputFiles, "2070MeV") || basic_tools::FindSubstring(InputFiles, "2gev"));
    bool Is4GeV = (basic_tools::FindSubstring(InputFiles, "4029MeV") || basic_tools::FindSubstring(InputFiles, "4gev"));
    bool Is6GeV = (basic_tools::FindSubstring(InputFiles, "5986MeV") || basic_tools::FindSubstring(InputFiles, "6gev"));

    bool ApplyLimiter = true;
    // int Limiter = 10000000;  // 10M events (fo the data)
    int Limiter = 1000000;  // 100 files or 1M events (fo the data)
    // int Limiter = 100000;  // 10 files
    // int Limiter = 10000; // 1 file

    std::string OutFolderName_prefix = "04_HipoLooper";
    std::string OutFolderName_ver_status = "_v4_";
    std::string target_status = (basic_tools::FindSubstring(InputFiles, "/C12/") || basic_tools::FindSubstring(InputFiles, "/C/"))     ? "C12"
                                : (basic_tools::FindSubstring(InputFiles, "/Ar40/") || basic_tools::FindSubstring(InputFiles, "/Ar/")) ? "Ar40"
                                                                                                                                       : "_Unknown";

    if (target_status == "_Unknown") {
        std::cerr << "\n\nError! Target not found in InputFiles string! Aborting...\n\n";
        exit(1);
    }

    std::string sample_type_status = IsData ? "_data" : "_sim";
    std::string genie_tune_status = !IsData ? "_G18_" : "_";
    std::string Ebeam_status = Is2GeV ? "2070MeV" : Is4GeV ? "4029MeV" : Is6GeV ? "5986MeV" : "_Unknown";

    if (Ebeam_status == "_Unknown") {
        std::cerr << "\n\nError! Ebeam not found in InputFiles string! Aborting...\n\n";
        exit(1);
    }

    std::string Run_status = basic_tools::FindSubstring(InputFiles, "015664")   ? "_run_015664"
                             : basic_tools::FindSubstring(InputFiles, "015778") ? "_run_015778"
                             : basic_tools::FindSubstring(InputFiles, "015672") ? "_run_015672"
                             : basic_tools::FindSubstring(InputFiles, "015743") ? "_run_015743"
                             : basic_tools::FindSubstring(InputFiles, "015792") ? "_run_015792"
                                                                                : "";
    std::string General_status = "";

    std::string OutFolderName = OutFolderName_prefix + OutFolderName_ver_status + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + General_status;
    std::string OutFileName = OutFolderName;

    const std::string OutputDir = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName;
    system(("rm -rf " + OutputDir).c_str());
    system(("mkdir -p " + OutputDir).c_str());

    TFile *outFile = new TFile(("/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/" + OutFileName + ".root").c_str(), "RECREATE");

    std::string SampleName = target_status + genie_tune_status + Ebeam_status;
    TString Beam_energy_TString = Ebeam_status;

    clas12root::HipoChain chain;
    chain.Add(InputFiles);
    chain.SetReaderTags({0});
    chain.db()->turnOffQADB();
    auto config_c12 = chain.GetC12Reader();
    const std::unique_ptr<clas12::clas12reader> &c12 = chain.C12ref();
#pragma endregion

#pragma region Prepare histograms
    /////////////////////////////////////
    // Prepare histograms
    /////////////////////////////////////
    vector<TH1 *> HistoList;

    gStyle->SetTitleXSize(0.05);
    gStyle->SetTitleYSize(0.05);

    gStyle->SetTitleXOffset(0.8);
    gStyle->SetTitleYOffset(0.8);

    char temp_name[100];
    char temp_title[100];

#pragma region electron histograms - all sectors
    TH1D *h_Vz_e_BC_1e_cut =
        new TH1D("Vz_e_BC_1e_cut",
                 ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_BC_1e_cut);
    TH1D *h_Vz_e_AC_1e_cut =
        new TH1D("Vz_e_AC_1e_cut",
                 ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_AC_1e_cut);
    TH1D *h_Vx_e_BC_1e_cut =
        new TH1D("Vx_e_BC_1e_cut",
                 ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_BC_1e_cut);
    TH1D *h_Vx_e_AC_1e_cut =
        new TH1D("Vx_e_AC_1e_cut",
                 ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_AC_1e_cut);
    TH1D *h_Vy_e_BC_1e_cut =
        new TH1D("Vy_e_BC_1e_cut",
                 ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_BC_1e_cut);
    TH1D *h_Vy_e_AC_1e_cut =
        new TH1D("Vy_e_AC_1e_cut",
                 ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_AC_1e_cut);

    TH2D *h_dc_electron_hit_map_BC_1e_cut[4];  // 3 regions
    TH2D *h_dc_electron_hit_map_AC_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_electron_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_electron_hit_map_BC_%d", i), Form("DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_BC_1e_cut[i]);
        h_dc_electron_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_electron_hit_map_AC_%d", i), Form("DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_AC_1e_cut[i]);
    }

    TH1D *h_nphe_BC_1e_cut = new TH1D("nphe_BC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_BC_1e_cut);
    TH1D *h_nphe_AC_1e_cut = new TH1D("nphe_AC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_AC_1e_cut);

    TH2D *h_Edep_PCAL_VS_EC_BC_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_BC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0,
                 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_BC_1e_cut);
    TH2D *h_Edep_PCAL_VS_EC_AC_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_AC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2,
                 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_AC_1e_cut);

    TH2D *h_SF_VS_P_e_BC_1e_cut =
        new TH2D("SF_VS_P_e_BC_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (before cut);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_BC_1e_cut);
    TH2D *h_SF_VS_P_e_AC_1e_cut =
        new TH2D("SF_VS_P_e_AC_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (after cut);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_AC_1e_cut);

    TH2D *h_SF_VS_Lv_BC_1e_cut = new TH2D("SF_VS_Lv_BC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_BC_1e_cut);
    TH2D *h_SF_VS_Lv_AC_1e_cut = new TH2D("SF_VS_Lv_AC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_AC_1e_cut);

    TH2D *h_SF_VS_Lw_BC_1e_cut = new TH2D("SF_VS_Lw_BC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_BC_1e_cut);
    TH2D *h_SF_VS_Lw_AC_1e_cut = new TH2D("SF_VS_Lw_AC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_AC_1e_cut);

    TH2D *h_SF_VS_Lu_BC_1e_cut = new TH2D("SF_VS_Lu_BC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_BC_1e_cut);
    TH2D *h_SF_VS_Lu_AC_1e_cut = new TH2D("SF_VS_Lu_AC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_AC_1e_cut);

    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut);
    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 1
    TH1D *h_Vz_e_BC_sector1_1e_cut = new TH1D(
        "Vz_e_BC_sector1_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector1);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_BC_sector1_1e_cut);
    TH1D *h_Vz_e_AC_sector1_1e_cut = new TH1D(
        "Vz_e_AC_sector1_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector1);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_AC_sector1_1e_cut);
    TH1D *h_Vx_e_BC_sector1_1e_cut = new TH1D(
        "Vx_e_BC_sector1_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector1);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_BC_sector1_1e_cut);
    TH1D *h_Vx_e_AC_sector1_1e_cut = new TH1D(
        "Vx_e_AC_sector1_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector1);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_AC_sector1_1e_cut);
    TH1D *h_Vy_e_BC_sector1_1e_cut = new TH1D(
        "Vy_e_BC_sector1_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector1);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_BC_sector1_1e_cut);
    TH1D *h_Vy_e_AC_sector1_1e_cut = new TH1D(
        "Vy_e_AC_sector1_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector1);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_AC_sector1_1e_cut);

    TH2D *h_dc_electron_hit_map_BC_sector1_1e_cut[4];  // 3 regions
    TH2D *h_dc_electron_hit_map_AC_sector1_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_electron_hit_map_BC_sector1_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_BC_sector1_%d", i), Form("DC hitmap in region %d (before cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_BC_sector1_1e_cut[i]);
        h_dc_electron_hit_map_AC_sector1_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_AC_sector1_%d", i), Form("DC hitmap in region %d (after cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_AC_sector1_1e_cut[i]);
    }

    TH1D *h_nphe_BC_sector1_1e_cut = new TH1D("nphe_BC_sector1_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector1);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_BC_sector1_1e_cut);
    TH1D *h_nphe_AC_sector1_1e_cut = new TH1D("nphe_AC_sector1_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector1);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_AC_sector1_1e_cut);

    TH2D *h_Edep_PCAL_VS_EC_BC_sector1_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_BC_sector1_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector1);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector1_1e_cut);
    TH2D *h_Edep_PCAL_VS_EC_AC_sector1_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_AC_sector1_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector1);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector1_1e_cut);

    TH2D *h_SF_VS_P_e_BC_sector1_1e_cut = new TH2D(
        "SF_VS_P_e_BC_sector1_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (before cut, sector1);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_BC_sector1_1e_cut);
    TH2D *h_SF_VS_P_e_AC_sector1_1e_cut = new TH2D(
        "SF_VS_P_e_AC_sector1_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (after cut, sector1);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_AC_sector1_1e_cut);

    TH2D *h_SF_VS_Lv_BC_sector1_1e_cut =
        new TH2D("SF_VS_Lv_BC_sector1_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut, sector1);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_BC_sector1_1e_cut);
    TH2D *h_SF_VS_Lv_AC_sector1_1e_cut =
        new TH2D("SF_VS_Lv_AC_sector1_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut, sector1);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_AC_sector1_1e_cut);

    TH2D *h_SF_VS_Lw_BC_sector1_1e_cut =
        new TH2D("SF_VS_Lw_BC_sector1_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut, sector1);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_BC_sector1_1e_cut);
    TH2D *h_SF_VS_Lw_AC_sector1_1e_cut =
        new TH2D("SF_VS_Lw_AC_sector1_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut, sector1);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_AC_sector1_1e_cut);

    TH2D *h_SF_VS_Lu_BC_sector1_1e_cut =
        new TH2D("SF_VS_Lu_BC_sector1_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut, sector1);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_BC_sector1_1e_cut);
    TH2D *h_SF_VS_Lu_AC_sector1_1e_cut =
        new TH2D("SF_VS_Lu_AC_sector1_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut, sector1);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_AC_sector1_1e_cut);

    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector1_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC_sector1", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut, sector1);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0,
                 0.3, 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector1_1e_cut);
    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector1_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector1", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut, sector1);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3,
                 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector1_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 2
    TH1D *h_Vz_e_BC_sector2_1e_cut = new TH1D(
        "Vz_e_BC_sector2_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector2);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_BC_sector2_1e_cut);
    TH1D *h_Vz_e_AC_sector2_1e_cut = new TH1D(
        "Vz_e_AC_sector2_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector2);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_AC_sector2_1e_cut);
    TH1D *h_Vx_e_BC_sector2_1e_cut = new TH1D(
        "Vx_e_BC_sector2_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector2);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_BC_sector2_1e_cut);
    TH1D *h_Vx_e_AC_sector2_1e_cut = new TH1D(
        "Vx_e_AC_sector2_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector2);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_AC_sector2_1e_cut);
    TH1D *h_Vy_e_BC_sector2_1e_cut = new TH1D(
        "Vy_e_BC_sector2_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector2);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_BC_sector2_1e_cut);
    TH1D *h_Vy_e_AC_sector2_1e_cut = new TH1D(
        "Vy_e_AC_sector2_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector2);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_AC_sector2_1e_cut);

    TH2D *h_dc_electron_hit_map_BC_sector2_1e_cut[4];  // 3 regions
    TH2D *h_dc_electron_hit_map_AC_sector2_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_electron_hit_map_BC_sector2_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_BC_sector2_%d", i), Form("DC hitmap in region %d (before cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_BC_sector2_1e_cut[i]);
        h_dc_electron_hit_map_AC_sector2_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_AC_sector2_%d", i), Form("DC hitmap in region %d (after cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_AC_sector2_1e_cut[i]);
    }

    TH1D *h_nphe_BC_sector2_1e_cut = new TH1D("nphe_BC_sector2_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector2);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_BC_sector2_1e_cut);
    TH1D *h_nphe_AC_sector2_1e_cut = new TH1D("nphe_AC_sector2_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector2);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_AC_sector2_1e_cut);

    TH2D *h_Edep_PCAL_VS_EC_BC_sector2_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_BC_sector2_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector2);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector2_1e_cut);
    TH2D *h_Edep_PCAL_VS_EC_AC_sector2_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_AC_sector2_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector2);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector2_1e_cut);

    TH2D *h_SF_VS_P_e_BC_sector2_1e_cut = new TH2D(
        "SF_VS_P_e_BC_sector2_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (before cut, sector2);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_BC_sector2_1e_cut);
    TH2D *h_SF_VS_P_e_AC_sector2_1e_cut = new TH2D(
        "SF_VS_P_e_AC_sector2_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (after cut, sector2);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_AC_sector2_1e_cut);

    TH2D *h_SF_VS_Lv_BC_sector2_1e_cut =
        new TH2D("SF_VS_Lv_BC_sector2_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut, sector2);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_BC_sector2_1e_cut);
    TH2D *h_SF_VS_Lv_AC_sector2_1e_cut =
        new TH2D("SF_VS_Lv_AC_sector2_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut, sector2);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_AC_sector2_1e_cut);

    TH2D *h_SF_VS_Lw_BC_sector2_1e_cut =
        new TH2D("SF_VS_Lw_BC_sector2_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut, sector2);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_BC_sector2_1e_cut);
    TH2D *h_SF_VS_Lw_AC_sector2_1e_cut =
        new TH2D("SF_VS_Lw_AC_sector2_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut, sector2);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_AC_sector2_1e_cut);

    TH2D *h_SF_VS_Lu_BC_sector2_1e_cut =
        new TH2D("SF_VS_Lu_BC_sector2_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut, sector2);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_BC_sector2_1e_cut);
    TH2D *h_SF_VS_Lu_AC_sector2_1e_cut =
        new TH2D("SF_VS_Lu_AC_sector2_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut, sector2);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_AC_sector2_1e_cut);

    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector2_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_B_sector2", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut, sector2);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3,
                 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector2_1e_cut);
    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector2_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector2", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut, sector2);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3,
                 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector2_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 3
    TH1D *h_Vz_e_BC_sector3_1e_cut = new TH1D(
        "Vz_e_BC_sector3_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector3);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_BC_sector3_1e_cut);
    TH1D *h_Vz_e_AC_sector3_1e_cut = new TH1D(
        "Vz_e_AC_sector3_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector3);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_AC_sector3_1e_cut);
    TH1D *h_Vx_e_BC_sector3_1e_cut = new TH1D(
        "Vx_e_BC_sector3_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector3);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_BC_sector3_1e_cut);
    TH1D *h_Vx_e_AC_sector3_1e_cut = new TH1D(
        "Vx_e_AC_sector3_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector3);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_AC_sector3_1e_cut);
    TH1D *h_Vy_e_BC_sector3_1e_cut = new TH1D(
        "Vy_e_BC_sector3_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector3);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_BC_sector3_1e_cut);
    TH1D *h_Vy_e_AC_sector3_1e_cut = new TH1D(
        "Vy_e_AC_sector3_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector3);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_AC_sector3_1e_cut);

    TH2D *h_dc_electron_hit_map_BC_sector3_1e_cut[4];  // 3 regions
    TH2D *h_dc_electron_hit_map_AC_sector3_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_electron_hit_map_BC_sector3_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_BC_sector3_%d", i), Form("DC hitmap in region %d (before cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_BC_sector3_1e_cut[i]);
        h_dc_electron_hit_map_AC_sector3_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_AC_sector3_%d", i), Form("DC hitmap in region %d (after cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_AC_sector3_1e_cut[i]);
    }

    TH1D *h_nphe_BC_sector3_1e_cut = new TH1D("nphe_BC_sector3_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector3);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_BC_sector3_1e_cut);
    TH1D *h_nphe_AC_sector3_1e_cut = new TH1D("nphe_AC_sector3_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector3);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_AC_sector3_1e_cut);

    TH2D *h_Edep_PCAL_VS_EC_BC_sector3_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_BC_sector3_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector3);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector3_1e_cut);
    TH2D *h_Edep_PCAL_VS_EC_AC_sector3_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_AC_sector3_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector3);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector3_1e_cut);

    TH2D *h_SF_VS_P_e_BC_sector3_1e_cut = new TH2D(
        "SF_VS_P_e_BC_sector3_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (before cut, sector3);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_BC_sector3_1e_cut);
    TH2D *h_SF_VS_P_e_AC_sector3_1e_cut = new TH2D(
        "SF_VS_P_e_AC_sector3_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (after cut, sector3);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_AC_sector3_1e_cut);

    TH2D *h_SF_VS_Lv_BC_sector3_1e_cut =
        new TH2D("SF_VS_Lv_BC_sector3_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut, sector3);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_BC_sector3_1e_cut);
    TH2D *h_SF_VS_Lv_AC_sector3_1e_cut =
        new TH2D("SF_VS_Lv_AC_sector3_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut, sector3);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_AC_sector3_1e_cut);

    TH2D *h_SF_VS_Lw_BC_sector3_1e_cut =
        new TH2D("SF_VS_Lw_BC_sector3_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut, sector3);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_BC_sector3_1e_cut);
    TH2D *h_SF_VS_Lw_AC_sector3_1e_cut =
        new TH2D("SF_VS_Lw_AC_sector3_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut, sector3);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_AC_sector3_1e_cut);

    TH2D *h_SF_VS_Lu_BC_sector3_1e_cut =
        new TH2D("SF_VS_Lu_BC_sector3_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut, sector3);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_BC_sector3_1e_cut);
    TH2D *h_SF_VS_Lu_AC_sector3_1e_cut =
        new TH2D("SF_VS_Lu_AC_sector3_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut, sector3);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_AC_sector3_1e_cut);

    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector3_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC_sector3", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut, sector3);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0,
                 0.3, 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector3_1e_cut);
    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector3_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector3", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut, sector3);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3,
                 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector3_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 4
    TH1D *h_Vz_e_BC_sector4_1e_cut = new TH1D(
        "Vz_e_BC_sector4_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector4);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_BC_sector4_1e_cut);
    TH1D *h_Vz_e_AC_sector4_1e_cut = new TH1D(
        "Vz_e_AC_sector4_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector4);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_AC_sector4_1e_cut);
    TH1D *h_Vx_e_BC_sector4_1e_cut = new TH1D(
        "Vx_e_BC_sector4_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector4);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_BC_sector4_1e_cut);
    TH1D *h_Vx_e_AC_sector4_1e_cut = new TH1D(
        "Vx_e_AC_sector4_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector4);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_AC_sector4_1e_cut);
    TH1D *h_Vy_e_BC_sector4_1e_cut = new TH1D(
        "Vy_e_BC_sector4_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector4);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_BC_sector4_1e_cut);
    TH1D *h_Vy_e_AC_sector4_1e_cut = new TH1D(
        "Vy_e_AC_sector4_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector4);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_AC_sector4_1e_cut);

    TH2D *h_dc_electron_hit_map_BC_sector4_1e_cut[4];  // 3 regions
    TH2D *h_dc_electron_hit_map_AC_sector4_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_electron_hit_map_BC_sector4_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_BC_sector4_%d", i), Form("DC hitmap in region %d (before cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_BC_sector4_1e_cut[i]);
        h_dc_electron_hit_map_AC_sector4_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_AC_sector4_%d", i), Form("DC hitmap in region %d (after cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_AC_sector4_1e_cut[i]);
    }

    TH1D *h_nphe_BC_sector4_1e_cut = new TH1D("nphe_BC_sector4_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector4);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_BC_sector4_1e_cut);
    TH1D *h_nphe_AC_sector4_1e_cut = new TH1D("nphe_AC_sector4_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector4);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_AC_sector4_1e_cut);

    TH2D *h_Edep_PCAL_VS_EC_BC_sector4_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_BC_sector4_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector4);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector4_1e_cut);
    TH2D *h_Edep_PCAL_VS_EC_AC_sector4_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_AC_sector4_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector4);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector4_1e_cut);

    TH2D *h_SF_VS_P_e_BC_sector4_1e_cut = new TH2D(
        "SF_VS_P_e_BC_sector4_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (before cut, sector4);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_BC_sector4_1e_cut);
    TH2D *h_SF_VS_P_e_AC_sector4_1e_cut = new TH2D(
        "SF_VS_P_e_AC_sector4_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (after cut, sector4);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_AC_sector4_1e_cut);

    TH2D *h_SF_VS_Lv_BC_sector4_1e_cut =
        new TH2D("SF_VS_Lv_BC_sector4_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut, sector4);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_BC_sector4_1e_cut);
    TH2D *h_SF_VS_Lv_AC_sector4_1e_cut =
        new TH2D("SF_VS_Lv_AC_sector4_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut, sector4);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_AC_sector4_1e_cut);

    TH2D *h_SF_VS_Lw_BC_sector4_1e_cut =
        new TH2D("SF_VS_Lw_BC_sector4_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut, sector4);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_BC_sector4_1e_cut);
    TH2D *h_SF_VS_Lw_AC_sector4_1e_cut =
        new TH2D("SF_VS_Lw_AC_sector4_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut, sector4);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_AC_sector4_1e_cut);

    TH2D *h_SF_VS_Lu_BC_sector4_1e_cut =
        new TH2D("SF_VS_Lu_BC_sector4_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut, sector4);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_BC_sector4_1e_cut);
    TH2D *h_SF_VS_Lu_AC_sector4_1e_cut =
        new TH2D("SF_VS_Lu_AC_sector4_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut, sector4);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_AC_sector4_1e_cut);

    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector4_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC_sector4", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut, sector4);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0,
                 0.3, 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector4_1e_cut);
    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector4_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector4", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut, sector4);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3,
                 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector4_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 5
    TH1D *h_Vz_e_BC_sector5_1e_cut = new TH1D(
        "Vz_e_BC_sector5_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector5);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_BC_sector5_1e_cut);
    TH1D *h_Vz_e_AC_sector5_1e_cut = new TH1D(
        "Vz_e_AC_sector5_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector5);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_AC_sector5_1e_cut);
    TH1D *h_Vx_e_BC_sector5_1e_cut = new TH1D(
        "Vx_e_BC_sector5_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector5);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_BC_sector5_1e_cut);
    TH1D *h_Vx_e_AC_sector5_1e_cut = new TH1D(
        "Vx_e_AC_sector5_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector5);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_AC_sector5_1e_cut);
    TH1D *h_Vy_e_BC_sector5_1e_cut = new TH1D(
        "Vy_e_BC_sector5_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector5);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_BC_sector5_1e_cut);
    TH1D *h_Vy_e_AC_sector5_1e_cut = new TH1D(
        "Vy_e_AC_sector5_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector5);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_AC_sector5_1e_cut);

    TH2D *h_dc_electron_hit_map_BC_sector5_1e_cut[4];  // 3 regions
    TH2D *h_dc_electron_hit_map_AC_sector5_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_electron_hit_map_BC_sector5_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_BC_sector5_%d", i), Form("DC hitmap in region %d (before cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_BC_sector5_1e_cut[i]);
        h_dc_electron_hit_map_AC_sector5_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_AC_sector5_%d", i), Form("DC hitmap in region %d (after cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_AC_sector5_1e_cut[i]);
    }

    TH1D *h_nphe_BC_sector5_1e_cut = new TH1D("nphe_BC_sector5_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector5);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_BC_sector5_1e_cut);
    TH1D *h_nphe_AC_sector5_1e_cut = new TH1D("nphe_AC_sector5_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector5);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_AC_sector5_1e_cut);

    TH2D *h_Edep_PCAL_VS_EC_BC_sector5_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_BC_sector5_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector5);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector5_1e_cut);
    TH2D *h_Edep_PCAL_VS_EC_AC_sector5_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_AC_sector5_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector5);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector5_1e_cut);

    TH2D *h_SF_VS_P_e_BC_sector5_1e_cut = new TH2D(
        "SF_VS_P_e_BC_sector5_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (before cut, sector5);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_BC_sector5_1e_cut);
    TH2D *h_SF_VS_P_e_AC_sector5_1e_cut = new TH2D(
        "SF_VS_P_e_AC_sector5_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (after cut, sector5);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_AC_sector5_1e_cut);

    TH2D *h_SF_VS_Lv_BC_sector5_1e_cut =
        new TH2D("SF_VS_Lv_BC_sector5_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut, sector5);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_BC_sector5_1e_cut);
    TH2D *h_SF_VS_Lv_AC_sector5_1e_cut =
        new TH2D("SF_VS_Lv_AC_sector5_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut, sector5);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_AC_sector5_1e_cut);

    TH2D *h_SF_VS_Lw_BC_sector5_1e_cut =
        new TH2D("SF_VS_Lw_BC_sector5_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut, sector5);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_BC_sector5_1e_cut);
    TH2D *h_SF_VS_Lw_AC_sector5_1e_cut =
        new TH2D("SF_VS_Lw_AC_sector5_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut, sector5);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_AC_sector5_1e_cut);

    TH2D *h_SF_VS_Lu_BC_sector5_1e_cut =
        new TH2D("SF_VS_Lu_BC_sector5_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut, sector5);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_BC_sector5_1e_cut);
    TH2D *h_SF_VS_Lu_AC_sector5_1e_cut =
        new TH2D("SF_VS_Lu_AC_sector5_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut, sector5);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_AC_sector5_1e_cut);

    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector5_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC_sector5", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut, sector5);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0,
                 0.3, 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector5_1e_cut);
    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector5_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector5", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut, sector5);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3,
                 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector5_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 6
    TH1D *h_Vz_e_BC_sector6_1e_cut = new TH1D(
        "Vz_e_BC_sector6_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector6);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_BC_sector6_1e_cut);
    TH1D *h_Vz_e_AC_sector6_1e_cut = new TH1D(
        "Vz_e_AC_sector6_1e_cut",
        ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector6);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vz_e_AC_sector6_1e_cut);
    TH1D *h_Vx_e_BC_sector6_1e_cut = new TH1D(
        "Vx_e_BC_sector6_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector6);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_BC_sector6_1e_cut);
    TH1D *h_Vx_e_AC_sector6_1e_cut = new TH1D(
        "Vx_e_AC_sector6_1e_cut",
        ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector6);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vx_e_AC_sector6_1e_cut);
    TH1D *h_Vy_e_BC_sector6_1e_cut = new TH1D(
        "Vy_e_BC_sector6_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector6);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_BC_sector6_1e_cut);
    TH1D *h_Vy_e_AC_sector6_1e_cut = new TH1D(
        "Vy_e_AC_sector6_1e_cut",
        ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector6);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList.push_back(h_Vy_e_AC_sector6_1e_cut);

    TH2D *h_dc_electron_hit_map_BC_sector6_1e_cut[4];  // 3 regions
    TH2D *h_dc_electron_hit_map_AC_sector6_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_electron_hit_map_BC_sector6_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_BC_sector6_%d", i), Form("DC hitmap in region %d (before cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_BC_sector6_1e_cut[i]);
        h_dc_electron_hit_map_AC_sector6_1e_cut[i] =
            new TH2D(Form("dc_electron_hit_map_AC_sector6_%d", i), Form("DC hitmap in region %d (after cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_electron_hit_map_AC_sector6_1e_cut[i]);
    }

    TH1D *h_nphe_BC_sector6_1e_cut = new TH1D("nphe_BC_sector6_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector6);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_BC_sector6_1e_cut);
    TH1D *h_nphe_AC_sector6_1e_cut = new TH1D("nphe_AC_sector6_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector6);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList.push_back(h_nphe_AC_sector6_1e_cut);

    TH2D *h_Edep_PCAL_VS_EC_BC_sector6_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_BC_sector6_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector6);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector6_1e_cut);
    TH2D *h_Edep_PCAL_VS_EC_AC_sector6_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_AC_sector6_1e_cut",
                 "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector6);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector6_1e_cut);

    TH2D *h_SF_VS_P_e_BC_sector6_1e_cut = new TH2D(
        "SF_VS_P_e_BC_sector6_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (before cut, sector6);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_BC_sector6_1e_cut);
    TH2D *h_SF_VS_P_e_AC_sector6_1e_cut = new TH2D(
        "SF_VS_P_e_AC_sector6_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (after cut, sector6);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_P_e_AC_sector6_1e_cut);

    TH2D *h_SF_VS_Lv_BC_sector6_1e_cut =
        new TH2D("SF_VS_Lv_BC_sector6_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut, sector6);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_BC_sector6_1e_cut);
    TH2D *h_SF_VS_Lv_AC_sector6_1e_cut =
        new TH2D("SF_VS_Lv_AC_sector6_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut, sector6);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lv_AC_sector6_1e_cut);

    TH2D *h_SF_VS_Lw_BC_sector6_1e_cut =
        new TH2D("SF_VS_Lw_BC_sector6_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut, sector6);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_BC_sector6_1e_cut);
    TH2D *h_SF_VS_Lw_AC_sector6_1e_cut =
        new TH2D("SF_VS_Lw_AC_sector6_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut, sector6);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lw_AC_sector6_1e_cut);

    TH2D *h_SF_VS_Lu_BC_sector6_1e_cut =
        new TH2D("SF_VS_Lu_BC_sector6_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut, sector6);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_BC_sector6_1e_cut);
    TH2D *h_SF_VS_Lu_AC_sector6_1e_cut =
        new TH2D("SF_VS_Lu_AC_sector6_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut, sector6);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList.push_back(h_SF_VS_Lu_AC_sector6_1e_cut);

    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector6_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC_sector6", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut, sector6);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0,
                 0.3, 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector6_1e_cut);
    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector6_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector6", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut, sector6);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3,
                 100, 0, 0.35);
    HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector6_1e_cut);

#pragma endregion

#pragma region PiPlus histograms - all sectors
    TH1D *h_Vz_pip_BC_1e_cut =
        new TH1D("Vz_pip_BC_1e_cut",
                 ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vz_pip_BC_1e_cut);
    TH1D *h_Vz_pip_AC_1e_cut =
        new TH1D("Vz_pip_AC_1e_cut",
                 ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vz_pip_AC_1e_cut);
    TH1D *h_Vx_pip_BC_1e_cut =
        new TH1D("Vx_pip_BC_1e_cut",
                 ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vx_pip_BC_1e_cut);
    TH1D *h_Vx_pip_AC_1e_cut =
        new TH1D("Vx_pip_AC_1e_cut",
                 ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vx_pip_AC_1e_cut);
    TH1D *h_Vy_pip_BC_1e_cut =
        new TH1D("Vy_pip_BC_1e_cut",
                 ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vy_pip_BC_1e_cut);
    TH1D *h_Vy_pip_AC_1e_cut =
        new TH1D("Vy_pip_AC_1e_cut",
                 ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vy_pip_AC_1e_cut);

    TH2D *h_dc_pip_hit_map_BC_1e_cut[4];  // 3 regions
    TH2D *h_dc_pip_hit_map_AC_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pip_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_pip_hit_map_BC_%d", i), Form("DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_BC_1e_cut[i]);
        h_dc_pip_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_pip_hit_map_AC_%d", i), Form("DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_AC_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_1e_cut = new TH1D("nphe_BC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_1e_cut);
    // TH1D *h_nphe_AC_1e_cut = new TH1D("nphe_AC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0,
    //              0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0,
    //     0.2,
    //              100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_1e_cut);

    // TH2D *h_SF_VS_P_pip_BC_1e_cut = new TH2D("SF_VS_P_pip_BC_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (before cut);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction",
    // 100,
    //                                          0, Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_BC_1e_cut);
    // TH2D *h_SF_VS_P_pip_AC_1e_cut = new TH2D("SF_VS_P_pip_AC_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (after cut);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100,
    // 0,
    //                                          Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_AC_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_1e_cut = new TH2D("SF_VS_Lv_BC_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (before cut);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_1e_cut = new TH2D("SF_VS_Lv_AC_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (after cut);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_1e_cut = new TH2D("SF_VS_Lw_BC_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (before cut);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_1e_cut = new TH2D("SF_VS_Lw_AC_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (after cut);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_1e_cut = new TH2D("SF_VS_Lu_BC_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (before cut);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_1e_cut = new TH2D("SF_VS_Lu_AC_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (after cut);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_1e_cut);

    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_1e_cut =
    //     new TH2D("E_PCALoP_pip_VS_E_PCALoP_pip_BC", "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (before
    //     cut);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}",
    //              100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_1e_cut);
    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_1e_cut =
    //     new TH2D("E_PCALoP_pip_VS_E_PCALoP_pip_AC", "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (after
    //     cut);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}",
    //              100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_1e_cut);

#pragma endregion

#pragma region PiPlus histograms - sector 1
    TH1D *h_Vz_pip_BC_sector1_1e_cut = new TH1D(
        "Vz_pip_BC_sector1_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector1);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_BC_sector1_1e_cut);
    TH1D *h_Vz_pip_AC_sector1_1e_cut = new TH1D(
        "Vz_pip_AC_sector1_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector1);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_AC_sector1_1e_cut);
    TH1D *h_Vx_pip_BC_sector1_1e_cut = new TH1D(
        "Vx_pip_BC_sector1_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector1);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_BC_sector1_1e_cut);
    TH1D *h_Vx_pip_AC_sector1_1e_cut = new TH1D(
        "Vx_pip_AC_sector1_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector1);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_AC_sector1_1e_cut);
    TH1D *h_Vy_pip_BC_sector1_1e_cut = new TH1D(
        "Vy_pip_BC_sector1_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector1);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_BC_sector1_1e_cut);
    TH1D *h_Vy_pip_AC_sector1_1e_cut = new TH1D(
        "Vy_pip_AC_sector1_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector1);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_AC_sector1_1e_cut);

    TH2D *h_dc_pip_hit_map_BC_sector1_1e_cut[4];  // 3 regions
    TH2D *h_dc_pip_hit_map_AC_sector1_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pip_hit_map_BC_sector1_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_BC_sector1_%d", i), Form("DC hitmap in region %d (before cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_BC_sector1_1e_cut[i]);
        h_dc_pip_hit_map_AC_sector1_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_AC_sector1_%d", i), Form("DC hitmap in region %d (after cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_AC_sector1_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector1_1e_cut = new TH1D("nphe_BC_sector1_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector1);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector1_1e_cut);
    // TH1D *h_nphe_AC_sector1_1e_cut = new TH1D("nphe_AC_sector1_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector1);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector1_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector1_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector1_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector1);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector1_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector1_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector1_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector1);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector1_1e_cut);

    // TH2D *h_SF_VS_P_pip_BC_sector1_1e_cut =
    //     new TH2D("SF_VS_P_pip_BC_sector1_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (before cut, sector1);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_BC_sector1_1e_cut);
    // TH2D *h_SF_VS_P_pip_AC_sector1_1e_cut =
    //     new TH2D("SF_VS_P_pip_AC_sector1_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (after cut, sector1);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_AC_sector1_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector1_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (before cut, sector1);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector1_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector1_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (after cut, sector1);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector1_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector1_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (before cut, sector1);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector1_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector1_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (after cut, sector1);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector1_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector1_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (before cut, sector1);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector1_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector1_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (after cut, sector1);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector1_1e_cut);

    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector1_1e_cut = new TH2D(
    //     "E_PCALoP_pip_VS_E_PCALoP_pip_BC", "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (before cut,
    //     sector1);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector1_1e_cut);
    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector1_1e_cut =
    //     new TH2D("E_PCALoP_pip_VS_E_PCALoP_pip_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (after cut, sector1);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector1_1e_cut);

#pragma endregion

#pragma region PiPlus histograms - sector 2
    TH1D *h_Vz_pip_BC_sector2_1e_cut = new TH1D(
        "Vz_pip_BC_sector2_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector2);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_BC_sector2_1e_cut);
    TH1D *h_Vz_pip_AC_sector2_1e_cut = new TH1D(
        "Vz_pip_AC_sector2_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector2);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_AC_sector2_1e_cut);
    TH1D *h_Vx_pip_BC_sector2_1e_cut = new TH1D(
        "Vx_pip_BC_sector2_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector2);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_BC_sector2_1e_cut);
    TH1D *h_Vx_pip_AC_sector2_1e_cut = new TH1D(
        "Vx_pip_AC_sector2_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector2);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_AC_sector2_1e_cut);
    TH1D *h_Vy_pip_BC_sector2_1e_cut = new TH1D(
        "Vy_pip_BC_sector2_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector2);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_BC_sector2_1e_cut);
    TH1D *h_Vy_pip_AC_sector2_1e_cut = new TH1D(
        "Vy_pip_AC_sector2_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector2);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_AC_sector2_1e_cut);

    TH2D *h_dc_pip_hit_map_BC_sector2_1e_cut[4];  // 3 regions
    TH2D *h_dc_pip_hit_map_AC_sector2_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pip_hit_map_BC_sector2_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_BC_sector2_%d", i), Form("DC hitmap in region %d (before cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_BC_sector2_1e_cut[i]);
        h_dc_pip_hit_map_AC_sector2_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_AC_sector2_%d", i), Form("DC hitmap in region %d (after cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_AC_sector2_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector2_1e_cut = new TH1D("nphe_BC_sector2_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector2);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector2_1e_cut);
    // TH1D *h_nphe_AC_sector2_1e_cut = new TH1D("nphe_AC_sector2_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector2);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector2_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector2_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector2_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector2);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector2_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector2_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector2_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector2);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector2_1e_cut);

    // TH2D *h_SF_VS_P_pip_BC_sector2_1e_cut =
    //     new TH2D("SF_VS_P_pip_BC_sector2_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (before cut, sector2);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_BC_sector2_1e_cut);
    // TH2D *h_SF_VS_P_pip_AC_sector2_1e_cut =
    //     new TH2D("SF_VS_P_pip_AC_sector2_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (after cut, sector2);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_AC_sector2_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector2_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (before cut, sector2);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector2_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector2_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (after cut, sector2);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector2_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector2_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (before cut, sector2);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector2_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector2_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (after cut, sector2);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector2_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector2_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (before cut, sector2);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector2_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector2_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (after cut, sector2);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector2_1e_cut);

    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector2_1e_cut = new TH2D(
    //     "E_PCALoP_pip_VS_E_PCALoP_pip_BC", "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (before cut,
    //     sector2);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector2_1e_cut);
    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector2_1e_cut =
    //     new TH2D("E_PCALoP_pip_VS_E_PCALoP_pip_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (after cut, sector2);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector2_1e_cut);

#pragma endregion

#pragma region PiPlus histograms - sector 3
    TH1D *h_Vz_pip_BC_sector3_1e_cut = new TH1D(
        "Vz_pip_BC_sector3_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector3);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_BC_sector3_1e_cut);
    TH1D *h_Vz_pip_AC_sector3_1e_cut = new TH1D(
        "Vz_pip_AC_sector3_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector3);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_AC_sector3_1e_cut);
    TH1D *h_Vx_pip_BC_sector3_1e_cut = new TH1D(
        "Vx_pip_BC_sector3_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector3);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_BC_sector3_1e_cut);
    TH1D *h_Vx_pip_AC_sector3_1e_cut = new TH1D(
        "Vx_pip_AC_sector3_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector3);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_AC_sector3_1e_cut);
    TH1D *h_Vy_pip_BC_sector3_1e_cut = new TH1D(
        "Vy_pip_BC_sector3_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector3);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_BC_sector3_1e_cut);
    TH1D *h_Vy_pip_AC_sector3_1e_cut = new TH1D(
        "Vy_pip_AC_sector3_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector3);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_AC_sector3_1e_cut);

    TH2D *h_dc_pip_hit_map_BC_sector3_1e_cut[4];  // 3 regions
    TH2D *h_dc_pip_hit_map_AC_sector3_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pip_hit_map_BC_sector3_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_BC_sector3_%d", i), Form("DC hitmap in region %d (before cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_BC_sector3_1e_cut[i]);
        h_dc_pip_hit_map_AC_sector3_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_AC_sector3_%d", i), Form("DC hitmap in region %d (after cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_AC_sector3_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector3_1e_cut = new TH1D("nphe_BC_sector3_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector3);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector3_1e_cut);
    // TH1D *h_nphe_AC_sector3_1e_cut = new TH1D("nphe_AC_sector3_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector3);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector3_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector3_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector3_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector3);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector3_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector3_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector3_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector3);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector3_1e_cut);

    // TH2D *h_SF_VS_P_pip_BC_sector3_1e_cut =
    //     new TH2D("SF_VS_P_pip_BC_sector3_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (before cut, sector3);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_BC_sector3_1e_cut);
    // TH2D *h_SF_VS_P_pip_AC_sector3_1e_cut =
    //     new TH2D("SF_VS_P_pip_AC_sector3_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (after cut, sector3);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_AC_sector3_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector3_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (before cut, sector3);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector3_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector3_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (after cut, sector3);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector3_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector3_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (before cut, sector3);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector3_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector3_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (after cut, sector3);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector3_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector3_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (before cut, sector3);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector3_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector3_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (after cut, sector3);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector3_1e_cut);

    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector3_1e_cut = new TH2D(
    //     "E_PCALoP_pip_VS_E_PCALoP_pip_BC", "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (before cut,
    //     sector3);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector3_1e_cut);
    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector3_1e_cut =
    //     new TH2D("E_PCALoP_pip_VS_E_PCALoP_pip_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (after cut, sector3);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector3_1e_cut);

#pragma endregion

#pragma region PiPlus histograms - sector 4
    TH1D *h_Vz_pip_BC_sector4_1e_cut = new TH1D(
        "Vz_pip_BC_sector4_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector4);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_BC_sector4_1e_cut);
    TH1D *h_Vz_pip_AC_sector4_1e_cut = new TH1D(
        "Vz_pip_AC_sector4_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector4);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_AC_sector4_1e_cut);
    TH1D *h_Vx_pip_BC_sector4_1e_cut = new TH1D(
        "Vx_pip_BC_sector4_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector4);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_BC_sector4_1e_cut);
    TH1D *h_Vx_pip_AC_sector4_1e_cut = new TH1D(
        "Vx_pip_AC_sector4_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector4);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_AC_sector4_1e_cut);
    TH1D *h_Vy_pip_BC_sector4_1e_cut = new TH1D(
        "Vy_pip_BC_sector4_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector4);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_BC_sector4_1e_cut);
    TH1D *h_Vy_pip_AC_sector4_1e_cut = new TH1D(
        "Vy_pip_AC_sector4_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector4);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_AC_sector4_1e_cut);

    TH2D *h_dc_pip_hit_map_BC_sector4_1e_cut[4];  // 3 regions
    TH2D *h_dc_pip_hit_map_AC_sector4_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pip_hit_map_BC_sector4_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_BC_sector4_%d", i), Form("DC hitmap in region %d (before cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_BC_sector4_1e_cut[i]);
        h_dc_pip_hit_map_AC_sector4_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_AC_sector4_%d", i), Form("DC hitmap in region %d (after cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_AC_sector4_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector4_1e_cut = new TH1D("nphe_BC_sector4_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector4);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector4_1e_cut);
    // TH1D *h_nphe_AC_sector4_1e_cut = new TH1D("nphe_AC_sector4_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector4);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector4_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector4_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector4_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector4);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector4_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector4_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector4_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector4);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector4_1e_cut);

    // TH2D *h_SF_VS_P_pip_BC_sector4_1e_cut =
    //     new TH2D("SF_VS_P_pip_BC_sector4_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (before cut, sector4);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_BC_sector4_1e_cut);
    // TH2D *h_SF_VS_P_pip_AC_sector4_1e_cut =
    //     new TH2D("SF_VS_P_pip_AC_sector4_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (after cut, sector4);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_AC_sector4_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector4_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (before cut, sector4);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector4_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector4_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (after cut, sector4);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector4_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector4_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (before cut, sector4);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector4_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector4_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (after cut, sector4);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector4_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector4_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (before cut, sector4);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector4_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector4_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (after cut, sector4);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector4_1e_cut);

    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector4_1e_cut = new TH2D(
    //     "E_PCALoP_pip_VS_E_PCALoP_pip_BC", "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (before cut,
    //     sector4);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector4_1e_cut);
    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector4_1e_cut =
    //     new TH2D("E_PCALoP_pip_VS_E_PCALoP_pip_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (after cut, sector4);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector4_1e_cut);

#pragma endregion

#pragma region PiPlus histograms - sector 5
    TH1D *h_Vz_pip_BC_sector5_1e_cut = new TH1D(
        "Vz_pip_BC_sector5_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector5);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_BC_sector5_1e_cut);
    TH1D *h_Vz_pip_AC_sector5_1e_cut = new TH1D(
        "Vz_pip_AC_sector5_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector5);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_AC_sector5_1e_cut);
    TH1D *h_Vx_pip_BC_sector5_1e_cut = new TH1D(
        "Vx_pip_BC_sector5_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector5);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_BC_sector5_1e_cut);
    TH1D *h_Vx_pip_AC_sector5_1e_cut = new TH1D(
        "Vx_pip_AC_sector5_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector5);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_AC_sector5_1e_cut);
    TH1D *h_Vy_pip_BC_sector5_1e_cut = new TH1D(
        "Vy_pip_BC_sector5_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector5);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_BC_sector5_1e_cut);
    TH1D *h_Vy_pip_AC_sector5_1e_cut = new TH1D(
        "Vy_pip_AC_sector5_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector5);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_AC_sector5_1e_cut);

    TH2D *h_dc_pip_hit_map_BC_sector5_1e_cut[4];  // 3 regions
    TH2D *h_dc_pip_hit_map_AC_sector5_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pip_hit_map_BC_sector5_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_BC_sector5_%d", i), Form("DC hitmap in region %d (before cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_BC_sector5_1e_cut[i]);
        h_dc_pip_hit_map_AC_sector5_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_AC_sector5_%d", i), Form("DC hitmap in region %d (after cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_AC_sector5_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector5_1e_cut = new TH1D("nphe_BC_sector5_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector5);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector5_1e_cut);
    // TH1D *h_nphe_AC_sector5_1e_cut = new TH1D("nphe_AC_sector5_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector5);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector5_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector5_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector5_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector5);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector5_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector5_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector5_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector5);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector5_1e_cut);

    // TH2D *h_SF_VS_P_pip_BC_sector5_1e_cut =
    //     new TH2D("SF_VS_P_pip_BC_sector5_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (before cut, sector5);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_BC_sector5_1e_cut);
    // TH2D *h_SF_VS_P_pip_AC_sector5_1e_cut =
    //     new TH2D("SF_VS_P_pip_AC_sector5_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (after cut, sector5);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_AC_sector5_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector5_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (before cut, sector5);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector5_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector5_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (after cut, sector5);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector5_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector5_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (before cut, sector5);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector5_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector5_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (after cut, sector5);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector5_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector5_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (before cut, sector5);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector5_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector5_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (after cut, sector5);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector5_1e_cut);

    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector5_1e_cut = new TH2D(
    //     "E_PCALoP_pip_VS_E_PCALoP_pip_BC", "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (before cut,
    //     sector5);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector5_1e_cut);
    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector5_1e_cut =
    //     new TH2D("E_PCALoP_pip_VS_E_PCALoP_pip_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (after cut, sector5);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector5_1e_cut);

#pragma endregion

#pragma region PiPlus histograms - sector 6
    TH1D *h_Vz_pip_BC_sector6_1e_cut = new TH1D(
        "Vz_pip_BC_sector6_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector6);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_BC_sector6_1e_cut);
    TH1D *h_Vz_pip_AC_sector6_1e_cut = new TH1D(
        "Vz_pip_AC_sector6_1e_cut",
        ("V_{z}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector6);V_{z}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pip_AC_sector6_1e_cut);
    TH1D *h_Vx_pip_BC_sector6_1e_cut = new TH1D(
        "Vx_pip_BC_sector6_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector6);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_BC_sector6_1e_cut);
    TH1D *h_Vx_pip_AC_sector6_1e_cut = new TH1D(
        "Vx_pip_AC_sector6_1e_cut",
        ("V_{x}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector6);V_{x}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pip_AC_sector6_1e_cut);
    TH1D *h_Vy_pip_BC_sector6_1e_cut = new TH1D(
        "Vy_pip_BC_sector6_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector6);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_BC_sector6_1e_cut);
    TH1D *h_Vy_pip_AC_sector6_1e_cut = new TH1D(
        "Vy_pip_AC_sector6_1e_cut",
        ("V_{y}^{#pi^{+}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector6);V_{y}^{#pi^{+}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pip_AC_sector6_1e_cut);

    TH2D *h_dc_pip_hit_map_BC_sector6_1e_cut[4];  // 3 regions
    TH2D *h_dc_pip_hit_map_AC_sector6_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pip_hit_map_BC_sector6_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_BC_sector6_%d", i), Form("DC hitmap in region %d (before cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_BC_sector6_1e_cut[i]);
        h_dc_pip_hit_map_AC_sector6_1e_cut[i] =
            new TH2D(Form("dc_pip_hit_map_AC_sector6_%d", i), Form("DC hitmap in region %d (after cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pip_hit_map_AC_sector6_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector6_1e_cut = new TH1D("nphe_BC_sector6_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector6);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector6_1e_cut);
    // TH1D *h_nphe_AC_sector6_1e_cut = new TH1D("nphe_AC_sector6_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector6);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector6_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector6_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector6_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector6);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector6_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector6_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector6_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector6);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector6_1e_cut);

    // TH2D *h_SF_VS_P_pip_BC_sector6_1e_cut =
    //     new TH2D("SF_VS_P_pip_BC_sector6_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (before cut, sector6);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_BC_sector6_1e_cut);
    // TH2D *h_SF_VS_P_pip_AC_sector6_1e_cut =
    //     new TH2D("SF_VS_P_pip_AC_sector6_1e_cut", "#pi^{+} sampling fraction vs. P_{#pi^{+}} in 1e cut (after cut, sector6);P_{#pi^{+}} [GeV/c];#pi^{+} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pip_AC_sector6_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector6_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (before cut, sector6);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector6_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector6_1e_cut", "#pi^{+} SF vs. PCAL V coor. in 1e cut (after cut, sector6);PCAL V coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector6_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector6_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (before cut, sector6);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector6_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector6_1e_cut", "#pi^{+} SF vs. PCAL W coor. in 1e cut (after cut, sector6);PCAL W coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector6_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector6_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (before cut, sector6);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector6_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector6_1e_cut", "#pi^{+} SF vs. PCAL U coor. in 1e cut (after cut, sector6);PCAL U coor. [cm];#pi^{+} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector6_1e_cut);

    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector6_1e_cut = new TH2D(
    //     "E_PCALoP_pip_VS_E_PCALoP_pip_BC", "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (before cut,
    //     sector6);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_BC_sector6_1e_cut);
    // TH2D *h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector6_1e_cut =
    //     new TH2D("E_PCALoP_pip_VS_E_PCALoP_pip_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{+}} vs. E_{dep}^{ECIN}/P_{#pi^{+}} in 1e cut (after cut, sector6);E_{dep}^{PCAL}/P_{#pi^{+}};E_{dep}^{ECIN}/P_{#pi^{+}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pip_VS_E_PCALoP_pip_AC_sector6_1e_cut);

#pragma endregion

#pragma region pimlus histograms - all sectors
    TH1D *h_Vz_pim_BC_1e_cut =
        new TH1D("Vz_pim_BC_1e_cut",
                 ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vz_pim_BC_1e_cut);
    TH1D *h_Vz_pim_AC_1e_cut =
        new TH1D("Vz_pim_AC_1e_cut",
                 ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vz_pim_AC_1e_cut);
    TH1D *h_Vx_pim_BC_1e_cut =
        new TH1D("Vx_pim_BC_1e_cut",
                 ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vx_pim_BC_1e_cut);
    TH1D *h_Vx_pim_AC_1e_cut =
        new TH1D("Vx_pim_AC_1e_cut",
                 ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vx_pim_AC_1e_cut);
    TH1D *h_Vy_pim_BC_1e_cut =
        new TH1D("Vy_pim_BC_1e_cut",
                 ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vy_pim_BC_1e_cut);
    TH1D *h_Vy_pim_AC_1e_cut =
        new TH1D("Vy_pim_AC_1e_cut",
                 ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
                 75, -10, 5);
    HistoList.push_back(h_Vy_pim_AC_1e_cut);

    TH2D *h_dc_pim_hit_map_BC_1e_cut[4];  // 3 regions
    TH2D *h_dc_pim_hit_map_AC_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pim_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_pim_hit_map_BC_%d", i), Form("DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_BC_1e_cut[i]);
        h_dc_pim_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_pim_hit_map_AC_%d", i), Form("DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_AC_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_1e_cut = new TH1D("nphe_BC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_1e_cut);
    // TH1D *h_nphe_AC_1e_cut = new TH1D("nphe_AC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0,
    //              0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0,
    //     0.2,
    //              100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_1e_cut);

    // TH2D *h_SF_VS_P_pim_BC_1e_cut = new TH2D("SF_VS_P_pim_BC_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (before cut);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction",
    // 100,
    //                                          0, Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_BC_1e_cut);
    // TH2D *h_SF_VS_P_pim_AC_1e_cut = new TH2D("SF_VS_P_pim_AC_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (after cut);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100,
    // 0,
    //                                          Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_AC_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_1e_cut = new TH2D("SF_VS_Lv_BC_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (before cut);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_1e_cut = new TH2D("SF_VS_Lv_AC_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (after cut);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_1e_cut = new TH2D("SF_VS_Lw_BC_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (before cut);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_1e_cut = new TH2D("SF_VS_Lw_AC_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (after cut);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_1e_cut = new TH2D("SF_VS_Lu_BC_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (before cut);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_1e_cut = new TH2D("SF_VS_Lu_AC_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (after cut);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_1e_cut);

    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_1e_cut =
    //     new TH2D("E_PCALoP_pim_VS_E_PCALoP_pim_BC", "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (before
    //     cut);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}",
    //              100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_1e_cut);
    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_1e_cut =
    //     new TH2D("E_PCALoP_pim_VS_E_PCALoP_pim_AC", "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (after
    //     cut);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}",
    //              100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_1e_cut);

#pragma endregion

#pragma region pimlus histograms - sector 1
    TH1D *h_Vz_pim_BC_sector1_1e_cut = new TH1D(
        "Vz_pim_BC_sector1_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector1);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_BC_sector1_1e_cut);
    TH1D *h_Vz_pim_AC_sector1_1e_cut = new TH1D(
        "Vz_pim_AC_sector1_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector1);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_AC_sector1_1e_cut);
    TH1D *h_Vx_pim_BC_sector1_1e_cut = new TH1D(
        "Vx_pim_BC_sector1_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector1);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_BC_sector1_1e_cut);
    TH1D *h_Vx_pim_AC_sector1_1e_cut = new TH1D(
        "Vx_pim_AC_sector1_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector1);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_AC_sector1_1e_cut);
    TH1D *h_Vy_pim_BC_sector1_1e_cut = new TH1D(
        "Vy_pim_BC_sector1_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector1);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_BC_sector1_1e_cut);
    TH1D *h_Vy_pim_AC_sector1_1e_cut = new TH1D(
        "Vy_pim_AC_sector1_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector1);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_AC_sector1_1e_cut);

    TH2D *h_dc_pim_hit_map_BC_sector1_1e_cut[4];  // 3 regions
    TH2D *h_dc_pim_hit_map_AC_sector1_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pim_hit_map_BC_sector1_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_BC_sector1_%d", i), Form("DC hitmap in region %d (before cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_BC_sector1_1e_cut[i]);
        h_dc_pim_hit_map_AC_sector1_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_AC_sector1_%d", i), Form("DC hitmap in region %d (after cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_AC_sector1_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector1_1e_cut = new TH1D("nphe_BC_sector1_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector1);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector1_1e_cut);
    // TH1D *h_nphe_AC_sector1_1e_cut = new TH1D("nphe_AC_sector1_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector1);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector1_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector1_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector1_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector1);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector1_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector1_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector1_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector1);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector1_1e_cut);

    // TH2D *h_SF_VS_P_pim_BC_sector1_1e_cut =
    //     new TH2D("SF_VS_P_pim_BC_sector1_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (before cut, sector1);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_BC_sector1_1e_cut);
    // TH2D *h_SF_VS_P_pim_AC_sector1_1e_cut =
    //     new TH2D("SF_VS_P_pim_AC_sector1_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (after cut, sector1);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_AC_sector1_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector1_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (before cut, sector1);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector1_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector1_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (after cut, sector1);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector1_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector1_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (before cut, sector1);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector1_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector1_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (after cut, sector1);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector1_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector1_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (before cut, sector1);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector1_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector1_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector1_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (after cut, sector1);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector1_1e_cut);

    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector1_1e_cut = new TH2D(
    //     "E_PCALoP_pim_VS_E_PCALoP_pim_BC", "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (before cut,
    //     sector1);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector1_1e_cut);
    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector1_1e_cut =
    //     new TH2D("E_PCALoP_pim_VS_E_PCALoP_pim_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (after cut, sector1);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector1_1e_cut);

#pragma endregion

#pragma region pimlus histograms - sector 2
    TH1D *h_Vz_pim_BC_sector2_1e_cut = new TH1D(
        "Vz_pim_BC_sector2_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector2);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_BC_sector2_1e_cut);
    TH1D *h_Vz_pim_AC_sector2_1e_cut = new TH1D(
        "Vz_pim_AC_sector2_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector2);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_AC_sector2_1e_cut);
    TH1D *h_Vx_pim_BC_sector2_1e_cut = new TH1D(
        "Vx_pim_BC_sector2_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector2);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_BC_sector2_1e_cut);
    TH1D *h_Vx_pim_AC_sector2_1e_cut = new TH1D(
        "Vx_pim_AC_sector2_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector2);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_AC_sector2_1e_cut);
    TH1D *h_Vy_pim_BC_sector2_1e_cut = new TH1D(
        "Vy_pim_BC_sector2_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector2);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_BC_sector2_1e_cut);
    TH1D *h_Vy_pim_AC_sector2_1e_cut = new TH1D(
        "Vy_pim_AC_sector2_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector2);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_AC_sector2_1e_cut);

    TH2D *h_dc_pim_hit_map_BC_sector2_1e_cut[4];  // 3 regions
    TH2D *h_dc_pim_hit_map_AC_sector2_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pim_hit_map_BC_sector2_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_BC_sector2_%d", i), Form("DC hitmap in region %d (before cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_BC_sector2_1e_cut[i]);
        h_dc_pim_hit_map_AC_sector2_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_AC_sector2_%d", i), Form("DC hitmap in region %d (after cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_AC_sector2_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector2_1e_cut = new TH1D("nphe_BC_sector2_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector2);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector2_1e_cut);
    // TH1D *h_nphe_AC_sector2_1e_cut = new TH1D("nphe_AC_sector2_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector2);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector2_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector2_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector2_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector2);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector2_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector2_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector2_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector2);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector2_1e_cut);

    // TH2D *h_SF_VS_P_pim_BC_sector2_1e_cut =
    //     new TH2D("SF_VS_P_pim_BC_sector2_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (before cut, sector2);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_BC_sector2_1e_cut);
    // TH2D *h_SF_VS_P_pim_AC_sector2_1e_cut =
    //     new TH2D("SF_VS_P_pim_AC_sector2_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (after cut, sector2);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_AC_sector2_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector2_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (before cut, sector2);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector2_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector2_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (after cut, sector2);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector2_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector2_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (before cut, sector2);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector2_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector2_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (after cut, sector2);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector2_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector2_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (before cut, sector2);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector2_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector2_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector2_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (after cut, sector2);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector2_1e_cut);

    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector2_1e_cut = new TH2D(
    //     "E_PCALoP_pim_VS_E_PCALoP_pim_BC", "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (before cut,
    //     sector2);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector2_1e_cut);
    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector2_1e_cut =
    //     new TH2D("E_PCALoP_pim_VS_E_PCALoP_pim_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (after cut, sector2);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector2_1e_cut);

#pragma endregion

#pragma region pimlus histograms - sector 3
    TH1D *h_Vz_pim_BC_sector3_1e_cut = new TH1D(
        "Vz_pim_BC_sector3_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector3);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_BC_sector3_1e_cut);
    TH1D *h_Vz_pim_AC_sector3_1e_cut = new TH1D(
        "Vz_pim_AC_sector3_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector3);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_AC_sector3_1e_cut);
    TH1D *h_Vx_pim_BC_sector3_1e_cut = new TH1D(
        "Vx_pim_BC_sector3_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector3);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_BC_sector3_1e_cut);
    TH1D *h_Vx_pim_AC_sector3_1e_cut = new TH1D(
        "Vx_pim_AC_sector3_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector3);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_AC_sector3_1e_cut);
    TH1D *h_Vy_pim_BC_sector3_1e_cut = new TH1D(
        "Vy_pim_BC_sector3_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector3);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_BC_sector3_1e_cut);
    TH1D *h_Vy_pim_AC_sector3_1e_cut = new TH1D(
        "Vy_pim_AC_sector3_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector3);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_AC_sector3_1e_cut);

    TH2D *h_dc_pim_hit_map_BC_sector3_1e_cut[4];  // 3 regions
    TH2D *h_dc_pim_hit_map_AC_sector3_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pim_hit_map_BC_sector3_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_BC_sector3_%d", i), Form("DC hitmap in region %d (before cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_BC_sector3_1e_cut[i]);
        h_dc_pim_hit_map_AC_sector3_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_AC_sector3_%d", i), Form("DC hitmap in region %d (after cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_AC_sector3_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector3_1e_cut = new TH1D("nphe_BC_sector3_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector3);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector3_1e_cut);
    // TH1D *h_nphe_AC_sector3_1e_cut = new TH1D("nphe_AC_sector3_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector3);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector3_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector3_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector3_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector3);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector3_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector3_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector3_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector3);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector3_1e_cut);

    // TH2D *h_SF_VS_P_pim_BC_sector3_1e_cut =
    //     new TH2D("SF_VS_P_pim_BC_sector3_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (before cut, sector3);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_BC_sector3_1e_cut);
    // TH2D *h_SF_VS_P_pim_AC_sector3_1e_cut =
    //     new TH2D("SF_VS_P_pim_AC_sector3_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (after cut, sector3);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_AC_sector3_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector3_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (before cut, sector3);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector3_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector3_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (after cut, sector3);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector3_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector3_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (before cut, sector3);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector3_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector3_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (after cut, sector3);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector3_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector3_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (before cut, sector3);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector3_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector3_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector3_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (after cut, sector3);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector3_1e_cut);

    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector3_1e_cut = new TH2D(
    //     "E_PCALoP_pim_VS_E_PCALoP_pim_BC", "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (before cut,
    //     sector3);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector3_1e_cut);
    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector3_1e_cut =
    //     new TH2D("E_PCALoP_pim_VS_E_PCALoP_pim_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (after cut, sector3);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector3_1e_cut);

#pragma endregion

#pragma region pimlus histograms - sector 4
    TH1D *h_Vz_pim_BC_sector4_1e_cut = new TH1D(
        "Vz_pim_BC_sector4_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector4);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_BC_sector4_1e_cut);
    TH1D *h_Vz_pim_AC_sector4_1e_cut = new TH1D(
        "Vz_pim_AC_sector4_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector4);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_AC_sector4_1e_cut);
    TH1D *h_Vx_pim_BC_sector4_1e_cut = new TH1D(
        "Vx_pim_BC_sector4_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector4);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_BC_sector4_1e_cut);
    TH1D *h_Vx_pim_AC_sector4_1e_cut = new TH1D(
        "Vx_pim_AC_sector4_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector4);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_AC_sector4_1e_cut);
    TH1D *h_Vy_pim_BC_sector4_1e_cut = new TH1D(
        "Vy_pim_BC_sector4_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector4);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_BC_sector4_1e_cut);
    TH1D *h_Vy_pim_AC_sector4_1e_cut = new TH1D(
        "Vy_pim_AC_sector4_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector4);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_AC_sector4_1e_cut);

    TH2D *h_dc_pim_hit_map_BC_sector4_1e_cut[4];  // 3 regions
    TH2D *h_dc_pim_hit_map_AC_sector4_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pim_hit_map_BC_sector4_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_BC_sector4_%d", i), Form("DC hitmap in region %d (before cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_BC_sector4_1e_cut[i]);
        h_dc_pim_hit_map_AC_sector4_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_AC_sector4_%d", i), Form("DC hitmap in region %d (after cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_AC_sector4_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector4_1e_cut = new TH1D("nphe_BC_sector4_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector4);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector4_1e_cut);
    // TH1D *h_nphe_AC_sector4_1e_cut = new TH1D("nphe_AC_sector4_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector4);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector4_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector4_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector4_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector4);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector4_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector4_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector4_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector4);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector4_1e_cut);

    // TH2D *h_SF_VS_P_pim_BC_sector4_1e_cut =
    //     new TH2D("SF_VS_P_pim_BC_sector4_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (before cut, sector4);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_BC_sector4_1e_cut);
    // TH2D *h_SF_VS_P_pim_AC_sector4_1e_cut =
    //     new TH2D("SF_VS_P_pim_AC_sector4_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (after cut, sector4);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_AC_sector4_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector4_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (before cut, sector4);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector4_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector4_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (after cut, sector4);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector4_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector4_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (before cut, sector4);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector4_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector4_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (after cut, sector4);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector4_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector4_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (before cut, sector4);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector4_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector4_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector4_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (after cut, sector4);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector4_1e_cut);

    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector4_1e_cut = new TH2D(
    //     "E_PCALoP_pim_VS_E_PCALoP_pim_BC", "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (before cut,
    //     sector4);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector4_1e_cut);
    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector4_1e_cut =
    //     new TH2D("E_PCALoP_pim_VS_E_PCALoP_pim_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (after cut, sector4);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector4_1e_cut);

#pragma endregion

#pragma region pimlus histograms - sector 5
    TH1D *h_Vz_pim_BC_sector5_1e_cut = new TH1D(
        "Vz_pim_BC_sector5_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector5);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_BC_sector5_1e_cut);
    TH1D *h_Vz_pim_AC_sector5_1e_cut = new TH1D(
        "Vz_pim_AC_sector5_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector5);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_AC_sector5_1e_cut);
    TH1D *h_Vx_pim_BC_sector5_1e_cut = new TH1D(
        "Vx_pim_BC_sector5_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector5);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_BC_sector5_1e_cut);
    TH1D *h_Vx_pim_AC_sector5_1e_cut = new TH1D(
        "Vx_pim_AC_sector5_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector5);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_AC_sector5_1e_cut);
    TH1D *h_Vy_pim_BC_sector5_1e_cut = new TH1D(
        "Vy_pim_BC_sector5_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector5);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_BC_sector5_1e_cut);
    TH1D *h_Vy_pim_AC_sector5_1e_cut = new TH1D(
        "Vy_pim_AC_sector5_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector5);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_AC_sector5_1e_cut);

    TH2D *h_dc_pim_hit_map_BC_sector5_1e_cut[4];  // 3 regions
    TH2D *h_dc_pim_hit_map_AC_sector5_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pim_hit_map_BC_sector5_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_BC_sector5_%d", i), Form("DC hitmap in region %d (before cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_BC_sector5_1e_cut[i]);
        h_dc_pim_hit_map_AC_sector5_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_AC_sector5_%d", i), Form("DC hitmap in region %d (after cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_AC_sector5_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector5_1e_cut = new TH1D("nphe_BC_sector5_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector5);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector5_1e_cut);
    // TH1D *h_nphe_AC_sector5_1e_cut = new TH1D("nphe_AC_sector5_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector5);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector5_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector5_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector5_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector5);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector5_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector5_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector5_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector5);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector5_1e_cut);

    // TH2D *h_SF_VS_P_pim_BC_sector5_1e_cut =
    //     new TH2D("SF_VS_P_pim_BC_sector5_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (before cut, sector5);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_BC_sector5_1e_cut);
    // TH2D *h_SF_VS_P_pim_AC_sector5_1e_cut =
    //     new TH2D("SF_VS_P_pim_AC_sector5_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (after cut, sector5);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_AC_sector5_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector5_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (before cut, sector5);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector5_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector5_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (after cut, sector5);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector5_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector5_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (before cut, sector5);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector5_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector5_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (after cut, sector5);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector5_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector5_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (before cut, sector5);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector5_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector5_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector5_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (after cut, sector5);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector5_1e_cut);

    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector5_1e_cut = new TH2D(
    //     "E_PCALoP_pim_VS_E_PCALoP_pim_BC", "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (before cut,
    //     sector5);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector5_1e_cut);
    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector5_1e_cut =
    //     new TH2D("E_PCALoP_pim_VS_E_PCALoP_pim_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (after cut, sector5);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector5_1e_cut);

#pragma endregion

#pragma region pimlus histograms - sector 6
    TH1D *h_Vz_pim_BC_sector6_1e_cut = new TH1D(
        "Vz_pim_BC_sector6_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector6);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_BC_sector6_1e_cut);
    TH1D *h_Vz_pim_AC_sector6_1e_cut = new TH1D(
        "Vz_pim_AC_sector6_1e_cut",
        ("V_{z}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector6);V_{z}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vz_pim_AC_sector6_1e_cut);
    TH1D *h_Vx_pim_BC_sector6_1e_cut = new TH1D(
        "Vx_pim_BC_sector6_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector6);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_BC_sector6_1e_cut);
    TH1D *h_Vx_pim_AC_sector6_1e_cut = new TH1D(
        "Vx_pim_AC_sector6_1e_cut",
        ("V_{x}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector6);V_{x}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vx_pim_AC_sector6_1e_cut);
    TH1D *h_Vy_pim_BC_sector6_1e_cut = new TH1D(
        "Vy_pim_BC_sector6_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut, sector6);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_BC_sector6_1e_cut);
    TH1D *h_Vy_pim_AC_sector6_1e_cut = new TH1D(
        "Vy_pim_AC_sector6_1e_cut",
        ("V_{y}^{#pi^{-}} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut, sector6);V_{y}^{#pi^{-}} [cm];Counts").c_str(),
        75, -10, 5);
    HistoList.push_back(h_Vy_pim_AC_sector6_1e_cut);

    TH2D *h_dc_pim_hit_map_BC_sector6_1e_cut[4];  // 3 regions
    TH2D *h_dc_pim_hit_map_AC_sector6_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_pim_hit_map_BC_sector6_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_BC_sector6_%d", i), Form("DC hitmap in region %d (before cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_BC_sector6_1e_cut[i]);
        h_dc_pim_hit_map_AC_sector6_1e_cut[i] =
            new TH2D(Form("dc_pim_hit_map_AC_sector6_%d", i), Form("DC hitmap in region %d (after cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList.push_back(h_dc_pim_hit_map_AC_sector6_1e_cut[i]);
    }

    // TH1D *h_nphe_BC_sector6_1e_cut = new TH1D("nphe_BC_sector6_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut, sector6);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_BC_sector6_1e_cut);
    // TH1D *h_nphe_AC_sector6_1e_cut = new TH1D("nphe_AC_sector6_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut, sector6);Number of photo-electrons;Counts", 20, 0, 20);
    // HistoList.push_back(h_nphe_AC_sector6_1e_cut);

    // TH2D *h_Edep_PCAL_VS_EC_BC_sector6_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_BC_sector6_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut, sector6);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector6_1e_cut);
    // TH2D *h_Edep_PCAL_VS_EC_AC_sector6_1e_cut =
    //     new TH2D("Edep_PCAL_VS_EC_AC_sector6_1e_cut",
    //              "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut, sector6);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
    // HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector6_1e_cut);

    // TH2D *h_SF_VS_P_pim_BC_sector6_1e_cut =
    //     new TH2D("SF_VS_P_pim_BC_sector6_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (before cut, sector6);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_BC_sector6_1e_cut);
    // TH2D *h_SF_VS_P_pim_AC_sector6_1e_cut =
    //     new TH2D("SF_VS_P_pim_AC_sector6_1e_cut", "#pi^{-} sampling fraction vs. P_{#pi^{-}} in 1e cut (after cut, sector6);P_{#pi^{-}} [GeV/c];#pi^{-} sampling fraction", 100, 0,
    //              Ebeam * 1.1, 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_P_pim_AC_sector6_1e_cut);

    // TH2D *h_SF_VS_Lv_BC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lv_BC_sector6_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (before cut, sector6);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_BC_sector6_1e_cut);
    // TH2D *h_SF_VS_Lv_AC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lv_AC_sector6_1e_cut", "#pi^{-} SF vs. PCAL V coor. in 1e cut (after cut, sector6);PCAL V coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lv_AC_sector6_1e_cut);

    // TH2D *h_SF_VS_Lw_BC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lw_BC_sector6_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (before cut, sector6);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_BC_sector6_1e_cut);
    // TH2D *h_SF_VS_Lw_AC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lw_AC_sector6_1e_cut", "#pi^{-} SF vs. PCAL W coor. in 1e cut (after cut, sector6);PCAL W coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lw_AC_sector6_1e_cut);

    // TH2D *h_SF_VS_Lu_BC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lu_BC_sector6_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (before cut, sector6);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_BC_sector6_1e_cut);
    // TH2D *h_SF_VS_Lu_AC_sector6_1e_cut =
    //     new TH2D("SF_VS_Lu_AC_sector6_1e_cut", "#pi^{-} SF vs. PCAL U coor. in 1e cut (after cut, sector6);PCAL U coor. [cm];#pi^{-} SF", 100, 0, 60., 100, 0.16, 0.3);
    // HistoList.push_back(h_SF_VS_Lu_AC_sector6_1e_cut);

    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector6_1e_cut = new TH2D(
    //     "E_PCALoP_pim_VS_E_PCALoP_pim_BC", "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (before cut,
    //     sector6);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0, 0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_BC_sector6_1e_cut);
    // TH2D *h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector6_1e_cut =
    //     new TH2D("E_PCALoP_pim_VS_E_PCALoP_pim_AC",
    //              "E_{dep}^{PCAL}/P_{#pi^{-}} vs. E_{dep}^{ECIN}/P_{#pi^{-}} in 1e cut (after cut, sector6);E_{dep}^{PCAL}/P_{#pi^{-}};E_{dep}^{ECIN}/P_{#pi^{-}}", 100, 0, 0.3, 100, 0,
    //              0.35);
    // HistoList.push_back(h_E_PCALoP_pim_VS_E_PCALoP_pim_AC_sector6_1e_cut);

#pragma endregion

#pragma endregion

    clas12ana clasAna;

#pragma region Loop over events
    int counter = 0;

    while (chain.Next() == true) {
        // Display completed
        ++counter;
        if ((counter % 1000000) == 0) { std::cerr << "\n" << counter / 1000000 << " million completed"; }
        if ((counter % 100000) == 0) { std::cerr << "."; }

        if ((ApplyLimiter && counter > Limiter)) { break; }

        clasAna.Run(c12);
        auto electrons = clasAna.getByPid(11);
        auto protons = clasAna.getByPid(2212);
        auto piplus = clasAna.getByPid(211);
        auto piminus = clasAna.getByPid(-211);

        // get particles by type
        auto allParticles = c12->getDetParticles();
        auto electrons_det = c12->getByID(11);

        if (electrons_det.size() != 1) { continue; }

        double weight = 1;

        double starttime = c12->event()->getStartTime();

        TVector3 P_b(0, 0, Ebeam);

        TVector3 reco_P_e;
        reco_P_e.SetMagThetaPhi(electrons_det[0]->getP(), electrons_det[0]->getTheta(), electrons_det[0]->getPhi());

        double vtz_e = electrons_det[0]->par()->getVz();

        TVector3 P_q = P_b - reco_P_e;

        double nu = Ebeam - reco_P_e.Mag();
        double QSq = P_q.Mag2() - (nu * nu);
        double xB = QSq / (2 * m_n * nu);
        double WSq = (m_n * m_n) - QSq + (2 * nu * m_n);
        double theta_e = reco_P_e.Theta() * 180 / M_PI;
        double EoP_e = (electrons_det[0]->cal(clas12::PCAL)->getEnergy() + electrons_det[0]->cal(ECIN)->getEnergy() + electrons_det[0]->cal(ECOUT)->getEnergy()) / reco_P_e.Mag();
        double E_PCALoP_e = electrons_det[0]->cal(clas12::PCAL)->getEnergy() / reco_P_e.Mag();
        double E_ECINoP_e = electrons_det[0]->cal(clas12::ECIN)->getEnergy() / reco_P_e.Mag();
        double Edep_PCAL = electrons_det[0]->cal(clas12::PCAL)->getEnergy();
        double Edep_EC = electrons_det[0]->cal(clas12::ECIN)->getEnergy() + electrons_det[0]->cal(clas12::ECOUT)->getEnergy();

        bool ElectronInPCAL = (electrons_det[0]->cal(clas12::PCAL)->getDetector() == 7);                              // PCAL hit
        bool ElectronInECIN = (electrons_det[0]->cal(clas12::ECIN)->getDetector() == 7);                              // ECIN hit
        bool ElectronInECOUT = (electrons_det[0]->cal(clas12::ECOUT)->getDetector() == 7);                            // ECOUT hit
        auto Electron_ECAL_detlayer = ElectronInPCAL ? clas12::PCAL : ElectronInECIN ? clas12::ECIN : clas12::ECOUT;  // find first layer of hit

        //  =======================================================================================================================================================================
        //  1e cut (reco)
        //  =======================================================================================================================================================================

        //  Electron PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region Electrons BPID
        h_Vx_e_BC_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
        h_Vy_e_BC_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
        h_Vz_e_BC_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
        // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -4. || electrons_det[0]->par()->getVz() > -2.);
        // // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -6. ||
        // // electrons_det[0]->par()->getVz() > 0.);
        // if (!bad_Vz_e_CutCond) {
        //     h_Vx_e_AC_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
        //     h_Vy_e_AC_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
        //     h_Vz_e_AC_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
        // }

        // fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_1e_cut, weight);
        // bool bad_DC_edge_CutCond = (!DCEdgeCuts(electrons_det[0]));
        // if (!bad_DC_edge_CutCond) { fillDCdebug(electrons_det[0], h_dc_electron_hit_map_AC_1e_cut, weight); }

        h_nphe_BC_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);
        // bool bad_nphe_CutCond = (electrons_det[0]->che(clas12::HTCC)->getNphe() <= 2);
        // if (!bad_nphe_CutCond) { h_nphe_AC_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight); }

        h_Edep_PCAL_VS_EC_BC_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);
        // bool bad_Edep_PCAL_CutCond = (Edep_PCAL <= 0.06);
        // if (!bad_Edep_PCAL_CutCond) { h_Edep_PCAL_VS_EC_AC_1e_cut->Fill(Edep_PCAL, Edep_EC, weight); }

        h_SF_VS_P_e_BC_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);
        // bool bad_SF_CutCond = (EoP_e < 0.2 || EoP_e > 0.28);
        // if (!bad_SF_CutCond) { h_SF_VS_P_e_AC_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight); }

        h_SF_VS_Lv_BC_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
        h_SF_VS_Lw_BC_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
        h_SF_VS_Lu_BC_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
        // bool bad_PCAL_edge_CutCond = (electrons_det[0]->cal(clas12::PCAL)->getLv() < 14. || electrons_det[0]->cal(clas12::PCAL)->getLw() < 14.);
        // if (!bad_PCAL_edge_CutCond) {
        //     h_SF_VS_Lv_AC_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
        //     h_SF_VS_Lw_AC_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
        //     h_SF_VS_Lu_AC_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
        // }

        h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
        // bool bad_diag_CutCond = (!checkEcalDiagCuts(electrons_det[0]));
        // if (!bad_diag_CutCond) { h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight); }

        // if (bad_Vz_e_CutCond) { continue; }
        // if (bad_DC_edge_CutCond) { continue; }

        // if (bad_nphe_CutCond) { continue; }
        // if (bad_Edep_PCAL_CutCond) { continue; }
        // if (bad_SF_CutCond) { continue; }
        // if (bad_PCAL_edge_CutCond) { continue; }
        // if (bad_diag_CutCond) { continue; }

        // h_reco_P_e_1e_cut->Fill(reco_P_e.Mag(), weight);
        // h_reco_theta_e_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
        // h_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
        // h_reco_theta_e_VS_reco_phi_e_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);

        if (electrons_det[0]->getSector() == 1) {
            h_Vx_e_BC_sector1_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            h_Vy_e_BC_sector1_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            h_Vz_e_BC_sector1_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -4. || electrons_det[0]->par()->getVz() > -2.);
            // // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -6. ||
            // // electrons_det[0]->par()->getVz() > 0.);
            // if (!bad_Vz_e_CutCond) {
            //     h_Vx_e_AC_sector1_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            //     h_Vy_e_AC_sector1_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            //     h_Vz_e_AC_sector1_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // }

            // fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector1_1e_cut, weight);
            // bool bad_DC_edge_CutCond = (!DCEdgeCuts(electrons_det[0]));
            // if (!bad_DC_edge_CutCond) { fillDCdebug(electrons_det[0], h_dc_electron_hit_map_AC_sector1_1e_cut, weight); }

            h_nphe_BC_sector1_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);
            // bool bad_nphe_CutCond = (electrons_det[0]->che(clas12::HTCC)->getNphe() <= 2);
            // if (!bad_nphe_CutCond) { h_nphe_AC_sector1_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight); }

            h_Edep_PCAL_VS_EC_BC_sector1_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);
            // bool bad_Edep_PCAL_CutCond = (Edep_PCAL <= 0.06);
            // if (!bad_Edep_PCAL_CutCond) { h_Edep_PCAL_VS_EC_AC_sector1_1e_cut->Fill(Edep_PCAL, Edep_EC, weight); }

            h_SF_VS_P_e_BC_sector1_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);
            // bool bad_SF_CutCond = (EoP_e < 0.2 || EoP_e > 0.28);
            // if (!bad_SF_CutCond) { h_SF_VS_P_e_AC_sector1_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight); }

            h_SF_VS_Lv_BC_sector1_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_BC_sector1_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_BC_sector1_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // bool bad_PCAL_edge_CutCond = (electrons_det[0]->cal(clas12::PCAL)->getLv() < 14. || electrons_det[0]->cal(clas12::PCAL)->getLw() < 14.);
            // if (!bad_PCAL_edge_CutCond) {
            //     h_SF_VS_Lv_AC_sector1_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            //     h_SF_VS_Lw_AC_sector1_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            //     h_SF_VS_Lu_AC_sector1_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // }

            h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector1_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            // bool bad_diag_CutCond = (!checkEcalDiagCuts(electrons_det[0]));
            // if (!bad_diag_CutCond) { h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector1_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight); }

            // if (bad_Vz_e_CutCond) { continue; }
            // if (bad_DC_edge_CutCond) { continue; }

            // if (bad_nphe_CutCond) { continue; }
            // if (bad_Edep_PCAL_CutCond) { continue; }
            // if (bad_SF_CutCond) { continue; }
            // if (bad_PCAL_edge_CutCond) { continue; }
            // if (bad_diag_CutCond) { continue; }

            // h_reco_P_e_sector1_1e_cut->Fill(reco_P_e.Mag(), weight);
            // h_reco_theta_e_sector1_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
            // h_reco_phi_e_sector1_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
            // h_reco_theta_e_VS_reco_phi_e_sector1_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);
        } else if (electrons_det[0]->getSector() == 2) {
            h_Vx_e_BC_sector2_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            h_Vy_e_BC_sector2_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            h_Vz_e_BC_sector2_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -4. || electrons_det[0]->par()->getVz() > -2.);
            // // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -6. ||
            // // electrons_det[0]->par()->getVz() > 0.);
            // if (!bad_Vz_e_CutCond) {
            //     h_Vx_e_AC_sector2_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            //     h_Vy_e_AC_sector2_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            //     h_Vz_e_AC_sector2_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // }

            // fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector2_1e_cut, weight);
            // bool bad_DC_edge_CutCond = (!DCEdgeCuts(electrons_det[0]));
            // if (!bad_DC_edge_CutCond) { fillDCdebug(electrons_det[0], h_dc_electron_hit_map_AC_sector2_1e_cut, weight); }

            h_nphe_BC_sector2_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);
            // bool bad_nphe_CutCond = (electrons_det[0]->che(clas12::HTCC)->getNphe() <= 2);
            // if (!bad_nphe_CutCond) { h_nphe_AC_sector2_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight); }

            h_Edep_PCAL_VS_EC_BC_sector2_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);
            // bool bad_Edep_PCAL_CutCond = (Edep_PCAL <= 0.06);
            // if (!bad_Edep_PCAL_CutCond) { h_Edep_PCAL_VS_EC_AC_sector2_1e_cut->Fill(Edep_PCAL, Edep_EC, weight); }

            h_SF_VS_P_e_BC_sector2_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);
            // bool bad_SF_CutCond = (EoP_e < 0.2 || EoP_e > 0.28);
            // if (!bad_SF_CutCond) { h_SF_VS_P_e_AC_sector2_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight); }

            h_SF_VS_Lv_BC_sector2_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_BC_sector2_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_BC_sector2_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // bool bad_PCAL_edge_CutCond = (electrons_det[0]->cal(clas12::PCAL)->getLv() < 14. || electrons_det[0]->cal(clas12::PCAL)->getLw() < 14.);
            // if (!bad_PCAL_edge_CutCond) {
            //     h_SF_VS_Lv_AC_sector2_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            //     h_SF_VS_Lw_AC_sector2_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            //     h_SF_VS_Lu_AC_sector2_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // }

            h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector2_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            // bool bad_diag_CutCond = (!checkEcalDiagCuts(electrons_det[0]));
            // if (!bad_diag_CutCond) { h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector2_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight); }

            // if (bad_Vz_e_CutCond) { continue; }
            // if (bad_DC_edge_CutCond) { continue; }

            // if (bad_nphe_CutCond) { continue; }
            // if (bad_Edep_PCAL_CutCond) { continue; }
            // if (bad_SF_CutCond) { continue; }
            // if (bad_PCAL_edge_CutCond) { continue; }
            // if (bad_diag_CutCond) { continue; }

            // h_reco_P_e_sector2_1e_cut->Fill(reco_P_e.Mag(), weight);
            // h_reco_theta_e_sector2_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
            // h_reco_phi_e_sector2_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
            // h_reco_theta_e_VS_reco_phi_e_sector2_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);
        } else if (electrons_det[0]->getSector() == 3) {
            h_Vx_e_BC_sector3_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            h_Vy_e_BC_sector3_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            h_Vz_e_BC_sector3_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -4. || electrons_det[0]->par()->getVz() > -2.);
            // // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -6. ||
            // // electrons_det[0]->par()->getVz() > 0.);
            // if (!bad_Vz_e_CutCond) {
            //     h_Vx_e_AC_sector3_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            //     h_Vy_e_AC_sector3_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            //     h_Vz_e_AC_sector3_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // }

            // fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector3_1e_cut, weight);
            // bool bad_DC_edge_CutCond = (!DCEdgeCuts(electrons_det[0]));
            // if (!bad_DC_edge_CutCond) { fillDCdebug(electrons_det[0], h_dc_electron_hit_map_AC_sector3_1e_cut, weight); }

            h_nphe_BC_sector3_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);
            // bool bad_nphe_CutCond = (electrons_det[0]->che(clas12::HTCC)->getNphe() <= 2);
            // if (!bad_nphe_CutCond) { h_nphe_AC_sector3_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight); }

            h_Edep_PCAL_VS_EC_BC_sector3_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);
            // bool bad_Edep_PCAL_CutCond = (Edep_PCAL <= 0.06);
            // if (!bad_Edep_PCAL_CutCond) { h_Edep_PCAL_VS_EC_AC_sector3_1e_cut->Fill(Edep_PCAL, Edep_EC, weight); }

            h_SF_VS_P_e_BC_sector3_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);
            // bool bad_SF_CutCond = (EoP_e < 0.2 || EoP_e > 0.28);
            // if (!bad_SF_CutCond) { h_SF_VS_P_e_AC_sector3_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight); }

            h_SF_VS_Lv_BC_sector3_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_BC_sector3_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_BC_sector3_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // bool bad_PCAL_edge_CutCond = (electrons_det[0]->cal(clas12::PCAL)->getLv() < 14. || electrons_det[0]->cal(clas12::PCAL)->getLw() < 14.);
            // if (!bad_PCAL_edge_CutCond) {
            //     h_SF_VS_Lv_AC_sector3_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            //     h_SF_VS_Lw_AC_sector3_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            //     h_SF_VS_Lu_AC_sector3_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // }

            h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector3_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            // bool bad_diag_CutCond = (!checkEcalDiagCuts(electrons_det[0]));
            // if (!bad_diag_CutCond) { h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector3_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight); }

            // if (bad_Vz_e_CutCond) { continue; }
            // if (bad_DC_edge_CutCond) { continue; }

            // if (bad_nphe_CutCond) { continue; }
            // if (bad_Edep_PCAL_CutCond) { continue; }
            // if (bad_SF_CutCond) { continue; }
            // if (bad_PCAL_edge_CutCond) { continue; }
            // if (bad_diag_CutCond) { continue; }

            // h_reco_P_e_sector3_1e_cut->Fill(reco_P_e.Mag(), weight);
            // h_reco_theta_e_sector3_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
            // h_reco_phi_e_sector3_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
            // h_reco_theta_e_VS_reco_phi_e_sector3_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);
        } else if (electrons_det[0]->getSector() == 4) {
            h_Vx_e_BC_sector4_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            h_Vy_e_BC_sector4_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            h_Vz_e_BC_sector4_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -4. || electrons_det[0]->par()->getVz() > -2.);
            // // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -6. ||
            // // electrons_det[0]->par()->getVz() > 0.);
            // if (!bad_Vz_e_CutCond) {
            //     h_Vx_e_AC_sector4_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            //     h_Vy_e_AC_sector4_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            //     h_Vz_e_AC_sector4_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // }

            // fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector4_1e_cut, weight);
            // bool bad_DC_edge_CutCond = (!DCEdgeCuts(electrons_det[0]));
            // if (!bad_DC_edge_CutCond) { fillDCdebug(electrons_det[0], h_dc_electron_hit_map_AC_sector4_1e_cut, weight); }

            h_nphe_BC_sector4_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);
            // bool bad_nphe_CutCond = (electrons_det[0]->che(clas12::HTCC)->getNphe() <= 2);
            // if (!bad_nphe_CutCond) { h_nphe_AC_sector4_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight); }

            h_Edep_PCAL_VS_EC_BC_sector4_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);
            // bool bad_Edep_PCAL_CutCond = (Edep_PCAL <= 0.06);
            // if (!bad_Edep_PCAL_CutCond) { h_Edep_PCAL_VS_EC_AC_sector4_1e_cut->Fill(Edep_PCAL, Edep_EC, weight); }

            h_SF_VS_P_e_BC_sector4_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);
            // bool bad_SF_CutCond = (EoP_e < 0.2 || EoP_e > 0.28);
            // if (!bad_SF_CutCond) { h_SF_VS_P_e_AC_sector4_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight); }

            h_SF_VS_Lv_BC_sector4_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_BC_sector4_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_BC_sector4_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // bool bad_PCAL_edge_CutCond = (electrons_det[0]->cal(clas12::PCAL)->getLv() < 14. || electrons_det[0]->cal(clas12::PCAL)->getLw() < 14.);
            // if (!bad_PCAL_edge_CutCond) {
            //     h_SF_VS_Lv_AC_sector4_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            //     h_SF_VS_Lw_AC_sector4_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            //     h_SF_VS_Lu_AC_sector4_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // }

            h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector4_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            // bool bad_diag_CutCond = (!checkEcalDiagCuts(electrons_det[0]));
            // if (!bad_diag_CutCond) { h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector4_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight); }

            // if (bad_Vz_e_CutCond) { continue; }
            // if (bad_DC_edge_CutCond) { continue; }

            // if (bad_nphe_CutCond) { continue; }
            // if (bad_Edep_PCAL_CutCond) { continue; }
            // if (bad_SF_CutCond) { continue; }
            // if (bad_PCAL_edge_CutCond) { continue; }
            // if (bad_diag_CutCond) { continue; }

            // h_reco_P_e_sector4_1e_cut->Fill(reco_P_e.Mag(), weight);
            // h_reco_theta_e_sector4_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
            // h_reco_phi_e_sector4_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
            // h_reco_theta_e_VS_reco_phi_e_sector4_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);
        } else if (electrons_det[0]->getSector() == 5) {
            h_Vx_e_BC_sector5_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            h_Vy_e_BC_sector5_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            h_Vz_e_BC_sector5_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -4. || electrons_det[0]->par()->getVz() > -2.);
            // // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -6. ||
            // // electrons_det[0]->par()->getVz() > 0.);
            // if (!bad_Vz_e_CutCond) {
            //     h_Vx_e_AC_sector5_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            //     h_Vy_e_AC_sector5_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            //     h_Vz_e_AC_sector5_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // }

            // fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector5_1e_cut, weight);
            // bool bad_DC_edge_CutCond = (!DCEdgeCuts(electrons_det[0]));
            // if (!bad_DC_edge_CutCond) { fillDCdebug(electrons_det[0], h_dc_electron_hit_map_AC_sector5_1e_cut, weight); }

            h_nphe_BC_sector5_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);
            // bool bad_nphe_CutCond = (electrons_det[0]->che(clas12::HTCC)->getNphe() <= 2);
            // if (!bad_nphe_CutCond) { h_nphe_AC_sector5_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight); }

            h_Edep_PCAL_VS_EC_BC_sector5_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);
            // bool bad_Edep_PCAL_CutCond = (Edep_PCAL <= 0.06);
            // if (!bad_Edep_PCAL_CutCond) { h_Edep_PCAL_VS_EC_AC_sector5_1e_cut->Fill(Edep_PCAL, Edep_EC, weight); }

            h_SF_VS_P_e_BC_sector5_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);
            // bool bad_SF_CutCond = (EoP_e < 0.2 || EoP_e > 0.28);
            // if (!bad_SF_CutCond) { h_SF_VS_P_e_AC_sector5_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight); }

            h_SF_VS_Lv_BC_sector5_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_BC_sector5_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_BC_sector5_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // bool bad_PCAL_edge_CutCond = (electrons_det[0]->cal(clas12::PCAL)->getLv() < 14. || electrons_det[0]->cal(clas12::PCAL)->getLw() < 14.);
            // if (!bad_PCAL_edge_CutCond) {
            //     h_SF_VS_Lv_AC_sector5_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            //     h_SF_VS_Lw_AC_sector5_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            //     h_SF_VS_Lu_AC_sector5_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // }

            h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector5_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            // bool bad_diag_CutCond = (!checkEcalDiagCuts(electrons_det[0]));
            // if (!bad_diag_CutCond) { h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector5_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight); }

            // if (bad_Vz_e_CutCond) { continue; }
            // if (bad_DC_edge_CutCond) { continue; }

            // if (bad_nphe_CutCond) { continue; }
            // if (bad_Edep_PCAL_CutCond) { continue; }
            // if (bad_SF_CutCond) { continue; }
            // if (bad_PCAL_edge_CutCond) { continue; }
            // if (bad_diag_CutCond) { continue; }

            // h_reco_P_e_sector5_1e_cut->Fill(reco_P_e.Mag(), weight);
            // h_reco_theta_e_sector5_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
            // h_reco_phi_e_sector5_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
            // h_reco_theta_e_VS_reco_phi_e_sector5_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);
        } else if (electrons_det[0]->getSector() == 6) {
            h_Vx_e_BC_sector6_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            h_Vy_e_BC_sector6_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            h_Vz_e_BC_sector6_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -4. || electrons_det[0]->par()->getVz() > -2.);
            // // bool bad_Vz_e_CutCond = (electrons_det[0]->par()->getVz() < -6. ||
            // // electrons_det[0]->par()->getVz() > 0.);
            // if (!bad_Vz_e_CutCond) {
            //     h_Vx_e_AC_sector6_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            //     h_Vy_e_AC_sector6_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            //     h_Vz_e_AC_sector6_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            // }

            // fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector6_1e_cut, weight);
            // bool bad_DC_edge_CutCond = (!DCEdgeCuts(electrons_det[0]));
            // if (!bad_DC_edge_CutCond) { fillDCdebug(electrons_det[0], h_dc_electron_hit_map_AC_sector6_1e_cut, weight); }

            h_nphe_BC_sector6_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);
            // bool bad_nphe_CutCond = (electrons_det[0]->che(clas12::HTCC)->getNphe() <= 2);
            // if (!bad_nphe_CutCond) { h_nphe_AC_sector6_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight); }

            h_Edep_PCAL_VS_EC_BC_sector6_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);
            // bool bad_Edep_PCAL_CutCond = (Edep_PCAL <= 0.06);
            // if (!bad_Edep_PCAL_CutCond) { h_Edep_PCAL_VS_EC_AC_sector6_1e_cut->Fill(Edep_PCAL, Edep_EC, weight); }

            h_SF_VS_P_e_BC_sector6_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);
            // bool bad_SF_CutCond = (EoP_e < 0.2 || EoP_e > 0.28);
            // if (!bad_SF_CutCond) { h_SF_VS_P_e_AC_sector6_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight); }

            h_SF_VS_Lv_BC_sector6_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_BC_sector6_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_BC_sector6_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // bool bad_PCAL_edge_CutCond = (electrons_det[0]->cal(clas12::PCAL)->getLv() < 14. || electrons_det[0]->cal(clas12::PCAL)->getLw() < 14.);
            // if (!bad_PCAL_edge_CutCond) {
            //     h_SF_VS_Lv_AC_sector6_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            //     h_SF_VS_Lw_AC_sector6_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            //     h_SF_VS_Lu_AC_sector6_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
            // }

            h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector6_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            // bool bad_diag_CutCond = (!checkEcalDiagCuts(electrons_det[0]));
            // if (!bad_diag_CutCond) { h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector6_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight); }

            // if (bad_Vz_e_CutCond) { continue; }
            // if (bad_DC_edge_CutCond) { continue; }

            // if (bad_nphe_CutCond) { continue; }
            // if (bad_Edep_PCAL_CutCond) { continue; }
            // if (bad_SF_CutCond) { continue; }
            // if (bad_PCAL_edge_CutCond) { continue; }
            // if (bad_diag_CutCond) { continue; }

            // h_reco_P_e_sector6_1e_cut->Fill(reco_P_e.Mag(), weight);
            // h_reco_theta_e_sector6_1e_cut->Fill(reco_P_e.Theta() * 180 / M_PI, weight);
            // h_reco_phi_e_sector6_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, weight);
            // h_reco_theta_e_VS_reco_phi_e_sector6_1e_cut->Fill(reco_P_e.Phi() * 180 / M_PI, reco_P_e.Theta() * 180 / M_PI, weight);
        }

#pragma endregion

        if (electrons.size() != 1) { continue; }

#pragma region Electrons APID
        h_Vx_e_AC_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
        h_Vy_e_AC_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
        h_Vz_e_AC_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

        h_nphe_AC_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

        h_Edep_PCAL_VS_EC_AC_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

        h_SF_VS_P_e_AC_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

        h_SF_VS_Lv_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
        h_SF_VS_Lw_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
        h_SF_VS_Lu_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

        h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);

        if (electrons[0]->getSector() == 1) {
            h_Vx_e_AC_sector1_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
            h_Vy_e_AC_sector1_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
            h_Vz_e_AC_sector1_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

            h_nphe_AC_sector1_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

            h_Edep_PCAL_VS_EC_AC_sector1_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

            h_SF_VS_P_e_AC_sector1_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

            h_SF_VS_Lv_AC_sector1_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_AC_sector1_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_AC_sector1_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

            h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector1_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
        } else if (electrons[0]->getSector() == 2) {
            h_Vx_e_AC_sector2_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
            h_Vy_e_AC_sector2_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
            h_Vz_e_AC_sector2_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

            h_nphe_AC_sector2_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

            h_Edep_PCAL_VS_EC_AC_sector2_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

            h_SF_VS_P_e_AC_sector2_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

            h_SF_VS_Lv_AC_sector2_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_AC_sector2_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_AC_sector2_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

            h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector2_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
        } else if (electrons[0]->getSector() == 3) {
            h_Vx_e_AC_sector3_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
            h_Vy_e_AC_sector3_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
            h_Vz_e_AC_sector3_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

            h_nphe_AC_sector3_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

            h_Edep_PCAL_VS_EC_AC_sector3_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

            h_SF_VS_P_e_AC_sector3_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

            h_SF_VS_Lv_AC_sector3_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_AC_sector3_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_AC_sector3_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

            h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector3_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
        } else if (electrons[0]->getSector() == 4) {
            h_Vx_e_AC_sector4_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
            h_Vy_e_AC_sector4_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
            h_Vz_e_AC_sector4_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

            h_nphe_AC_sector4_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

            h_Edep_PCAL_VS_EC_AC_sector4_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

            h_SF_VS_P_e_AC_sector4_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

            h_SF_VS_Lv_AC_sector4_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_AC_sector4_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_AC_sector4_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

            h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector4_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
        } else if (electrons[0]->getSector() == 5) {
            h_Vx_e_AC_sector5_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
            h_Vy_e_AC_sector5_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
            h_Vz_e_AC_sector5_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

            h_nphe_AC_sector5_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

            h_Edep_PCAL_VS_EC_AC_sector5_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

            h_SF_VS_P_e_AC_sector5_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

            h_SF_VS_Lv_AC_sector5_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_AC_sector5_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_AC_sector5_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

            h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector5_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
        } else if (electrons[0]->getSector() == 6) {
            h_Vx_e_AC_sector6_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
            h_Vy_e_AC_sector6_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
            h_Vz_e_AC_sector6_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

            h_nphe_AC_sector6_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

            h_Edep_PCAL_VS_EC_AC_sector6_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

            h_SF_VS_P_e_AC_sector6_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

            h_SF_VS_Lv_AC_sector6_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_AC_sector6_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_AC_sector6_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

            h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector6_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
        }

#pragma endregion

        //  Piplus PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region Piplus PID cuts

        for (int i = 0; i < piplus.size(); i++) {
            h_Vx_pip_BC_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
            h_Vy_pip_BC_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
            h_Vz_pip_BC_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
            // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
            // // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -6. ||
            // // // piplus[i]->par()->getVz() > 0.);
            // // if (!bad_Vz_pip_CutCond) {
            // //     h_Vx_pip_AC_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
            // //     h_Vy_pip_AC_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
            // //     h_Vz_pip_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
            // // }

            // // fillDCdebug(piplus[i], h_dc_piplectron_hit_map_BC_1e_cut, weight);
            // // bool bad_DC_pipdge_CutCond = (!DCEdgeCuts(piplus[i]));
            // // if (!bad_DC_pipdge_CutCond) { fillDCdebug(piplus[i], h_dc_piplectron_hit_map_AC_1e_cut, weight); }

            // h_nphe_BC_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight);
            // // bool bad_nphe_CutCond = (piplus[i]->che(clas12::HTCC)->getNphe() <= 2);
            // // if (!bad_nphe_CutCond) { h_nphe_AC_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight); }

            // h_pipdep_PCAL_VS_pipC_BC_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight);
            // // bool bad_pipdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
            // // if (!bad_pipdep_PCAL_CutCond) { h_pipdep_PCAL_VS_pipC_AC_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight); }

            // h_SF_VS_P_pip_BC_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight);
            // // bool bad_SF_CutCond = (EoP_pip < 0.2 || EoP_pip > 0.28);
            // // if (!bad_SF_CutCond) { h_SF_VS_P_pip_AC_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight); }

            // h_SF_VS_Lv_BC_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
            // h_SF_VS_Lw_BC_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
            // h_SF_VS_Lu_BC_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
            // // bool bad_PCAL_pipdge_CutCond = (piplus[i]->cal(clas12::PCAL)->getLv() < 14. || piplus[i]->cal(clas12::PCAL)->getLw() < 14.);
            // // if (!bad_PCAL_pipdge_CutCond) {
            // //     h_SF_VS_Lv_AC_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
            // //     h_SF_VS_Lw_AC_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
            // //     h_SF_VS_Lu_AC_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
            // // }

            // h_pip_PCALoP_pip_VS_pip_PCALoP_pip_BC_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight);
            // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piplus[i]));
            // // if (!bad_diag_CutCond) { h_pip_PCALoP_pip_VS_pip_PCALoP_pip_AC_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight); }

            // // if (bad_Vz_pip_CutCond) { continue; }
            // // if (bad_DC_pipdge_CutCond) { continue; }

            // // if (bad_nphe_CutCond) { continue; }
            // // if (bad_pipdep_PCAL_CutCond) { continue; }
            // // if (bad_SF_CutCond) { continue; }
            // // if (bad_PCAL_pipdge_CutCond) { continue; }
            // // if (bad_diag_CutCond) { continue; }

            // // h_reco_P_pip_1e_cut->Fill(reco_P_pip.Mag(), weight);
            // // h_reco_theta_pip_1e_cut->Fill(reco_P_pip.Theta() * 180 / M_PI, weight);
            // // h_reco_phi_pip_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, weight);
            // // h_reco_theta_pip_VS_reco_phi_pip_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, reco_P_pip.Theta() * 180 / M_PI, weight);

            if (piplus[i]->getSector() == 1) {
                h_Vx_pip_BC_sector1_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                h_Vy_pip_BC_sector1_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                h_Vz_pip_BC_sector1_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -6. ||
                // // // piplus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pip_CutCond) {
                // //     h_Vx_pip_AC_sector1_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                // //     h_Vy_pip_AC_sector1_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                // //     h_Vz_pip_AC_sector1_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piplus[i], h_dc_piplectron_hit_map_BC_sector1_1e_cut, weight);
                // // bool bad_DC_pipdge_CutCond = (!DCEdgeCuts(piplus[i]));
                // // if (!bad_DC_pipdge_CutCond) { fillDCdebug(piplus[i], h_dc_piplectron_hit_map_AC_sector1_1e_cut, weight); }

                // h_nphe_BC_sector1_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piplus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector1_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pipdep_PCAL_VS_pipC_BC_sector1_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight);
                // // bool bad_pipdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pipdep_PCAL_CutCond) { h_pipdep_PCAL_VS_pipC_AC_sector1_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight); }

                // h_SF_VS_P_pip_BC_sector1_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight);
                // // bool bad_SF_CutCond = (EoP_pip < 0.2 || EoP_pip > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pip_AC_sector1_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight); }

                // h_SF_VS_Lv_BC_sector1_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // h_SF_VS_Lw_BC_sector1_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // h_SF_VS_Lu_BC_sector1_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // bool bad_PCAL_pipdge_CutCond = (piplus[i]->cal(clas12::PCAL)->getLv() < 14. || piplus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pipdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector1_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // //     h_SF_VS_Lw_AC_sector1_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // //     h_SF_VS_Lu_AC_sector1_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // }

                // h_pip_PCALoP_pip_VS_pip_PCALoP_pip_BC_sector1_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piplus[i]));
                // // if (!bad_diag_CutCond) { h_pip_PCALoP_pip_VS_pip_PCALoP_pip_AC_sector1_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight); }

                // // if (bad_Vz_pip_CutCond) { continue; }
                // // if (bad_DC_pipdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pipdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pipdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pip_sector1_1e_cut->Fill(reco_P_pip.Mag(), weight);
                // // h_reco_theta_pip_sector1_1e_cut->Fill(reco_P_pip.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pip_sector1_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pip_VS_reco_phi_pip_sector1_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, reco_P_pip.Theta() * 180 / M_PI, weight);
            } else if (piplus[i]->getSector() == 2) {
                h_Vx_pip_BC_sector2_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                h_Vy_pip_BC_sector2_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                h_Vz_pip_BC_sector2_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -6. ||
                // // // piplus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pip_CutCond) {
                // //     h_Vx_pip_AC_sector2_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                // //     h_Vy_pip_AC_sector2_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                // //     h_Vz_pip_AC_sector2_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piplus[i], h_dc_piplectron_hit_map_BC_sector2_1e_cut, weight);
                // // bool bad_DC_pipdge_CutCond = (!DCEdgeCuts(piplus[i]));
                // // if (!bad_DC_pipdge_CutCond) { fillDCdebug(piplus[i], h_dc_piplectron_hit_map_AC_sector2_1e_cut, weight); }

                // h_nphe_BC_sector2_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piplus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector2_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pipdep_PCAL_VS_pipC_BC_sector2_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight);
                // // bool bad_pipdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pipdep_PCAL_CutCond) { h_pipdep_PCAL_VS_pipC_AC_sector2_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight); }

                // h_SF_VS_P_pip_BC_sector2_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight);
                // // bool bad_SF_CutCond = (EoP_pip < 0.2 || EoP_pip > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pip_AC_sector2_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight); }

                // h_SF_VS_Lv_BC_sector2_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // h_SF_VS_Lw_BC_sector2_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // h_SF_VS_Lu_BC_sector2_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // bool bad_PCAL_pipdge_CutCond = (piplus[i]->cal(clas12::PCAL)->getLv() < 14. || piplus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pipdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector2_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // //     h_SF_VS_Lw_AC_sector2_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // //     h_SF_VS_Lu_AC_sector2_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // }

                // h_pip_PCALoP_pip_VS_pip_PCALoP_pip_BC_sector2_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piplus[i]));
                // // if (!bad_diag_CutCond) { h_pip_PCALoP_pip_VS_pip_PCALoP_pip_AC_sector2_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight); }

                // // if (bad_Vz_pip_CutCond) { continue; }
                // // if (bad_DC_pipdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pipdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pipdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pip_sector2_1e_cut->Fill(reco_P_pip.Mag(), weight);
                // // h_reco_theta_pip_sector2_1e_cut->Fill(reco_P_pip.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pip_sector2_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pip_VS_reco_phi_pip_sector2_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, reco_P_pip.Theta() * 180 / M_PI, weight);
            } else if (piplus[i]->getSector() == 3) {
                h_Vx_pip_BC_sector3_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                h_Vy_pip_BC_sector3_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                h_Vz_pip_BC_sector3_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -6. ||
                // // // piplus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pip_CutCond) {
                // //     h_Vx_pip_AC_sector3_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                // //     h_Vy_pip_AC_sector3_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                // //     h_Vz_pip_AC_sector3_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piplus[i], h_dc_piplectron_hit_map_BC_sector3_1e_cut, weight);
                // // bool bad_DC_pipdge_CutCond = (!DCEdgeCuts(piplus[i]));
                // // if (!bad_DC_pipdge_CutCond) { fillDCdebug(piplus[i], h_dc_piplectron_hit_map_AC_sector3_1e_cut, weight); }

                // h_nphe_BC_sector3_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piplus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector3_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pipdep_PCAL_VS_pipC_BC_sector3_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight);
                // // bool bad_pipdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pipdep_PCAL_CutCond) { h_pipdep_PCAL_VS_pipC_AC_sector3_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight); }

                // h_SF_VS_P_pip_BC_sector3_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight);
                // // bool bad_SF_CutCond = (EoP_pip < 0.2 || EoP_pip > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pip_AC_sector3_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight); }

                // h_SF_VS_Lv_BC_sector3_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // h_SF_VS_Lw_BC_sector3_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // h_SF_VS_Lu_BC_sector3_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // bool bad_PCAL_pipdge_CutCond = (piplus[i]->cal(clas12::PCAL)->getLv() < 14. || piplus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pipdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector3_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // //     h_SF_VS_Lw_AC_sector3_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // //     h_SF_VS_Lu_AC_sector3_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // }

                // h_pip_PCALoP_pip_VS_pip_PCALoP_pip_BC_sector3_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piplus[i]));
                // // if (!bad_diag_CutCond) { h_pip_PCALoP_pip_VS_pip_PCALoP_pip_AC_sector3_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight); }

                // // if (bad_Vz_pip_CutCond) { continue; }
                // // if (bad_DC_pipdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pipdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pipdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pip_sector3_1e_cut->Fill(reco_P_pip.Mag(), weight);
                // // h_reco_theta_pip_sector3_1e_cut->Fill(reco_P_pip.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pip_sector3_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pip_VS_reco_phi_pip_sector3_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, reco_P_pip.Theta() * 180 / M_PI, weight);
            } else if (piplus[i]->getSector() == 4) {
                h_Vx_pip_BC_sector4_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                h_Vy_pip_BC_sector4_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                h_Vz_pip_BC_sector4_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -6. ||
                // // // piplus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pip_CutCond) {
                // //     h_Vx_pip_AC_sector4_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                // //     h_Vy_pip_AC_sector4_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                // //     h_Vz_pip_AC_sector4_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piplus[i], h_dc_piplectron_hit_map_BC_sector4_1e_cut, weight);
                // // bool bad_DC_pipdge_CutCond = (!DCEdgeCuts(piplus[i]));
                // // if (!bad_DC_pipdge_CutCond) { fillDCdebug(piplus[i], h_dc_piplectron_hit_map_AC_sector4_1e_cut, weight); }

                // h_nphe_BC_sector4_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piplus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector4_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pipdep_PCAL_VS_pipC_BC_sector4_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight);
                // // bool bad_pipdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pipdep_PCAL_CutCond) { h_pipdep_PCAL_VS_pipC_AC_sector4_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight); }

                // h_SF_VS_P_pip_BC_sector4_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight);
                // // bool bad_SF_CutCond = (EoP_pip < 0.2 || EoP_pip > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pip_AC_sector4_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight); }

                // h_SF_VS_Lv_BC_sector4_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // h_SF_VS_Lw_BC_sector4_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // h_SF_VS_Lu_BC_sector4_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // bool bad_PCAL_pipdge_CutCond = (piplus[i]->cal(clas12::PCAL)->getLv() < 14. || piplus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pipdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector4_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // //     h_SF_VS_Lw_AC_sector4_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // //     h_SF_VS_Lu_AC_sector4_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // }

                // h_pip_PCALoP_pip_VS_pip_PCALoP_pip_BC_sector4_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piplus[i]));
                // // if (!bad_diag_CutCond) { h_pip_PCALoP_pip_VS_pip_PCALoP_pip_AC_sector4_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight); }

                // // if (bad_Vz_pip_CutCond) { continue; }
                // // if (bad_DC_pipdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pipdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pipdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pip_sector4_1e_cut->Fill(reco_P_pip.Mag(), weight);
                // // h_reco_theta_pip_sector4_1e_cut->Fill(reco_P_pip.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pip_sector4_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pip_VS_reco_phi_pip_sector4_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, reco_P_pip.Theta() * 180 / M_PI, weight);
            } else if (piplus[i]->getSector() == 5) {
                h_Vx_pip_BC_sector5_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                h_Vy_pip_BC_sector5_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                h_Vz_pip_BC_sector5_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -6. ||
                // // // piplus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pip_CutCond) {
                // //     h_Vx_pip_AC_sector5_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                // //     h_Vy_pip_AC_sector5_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                // //     h_Vz_pip_AC_sector5_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piplus[i], h_dc_piplectron_hit_map_BC_sector5_1e_cut, weight);
                // // bool bad_DC_pipdge_CutCond = (!DCEdgeCuts(piplus[i]));
                // // if (!bad_DC_pipdge_CutCond) { fillDCdebug(piplus[i], h_dc_piplectron_hit_map_AC_sector5_1e_cut, weight); }

                // h_nphe_BC_sector5_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piplus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector5_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pipdep_PCAL_VS_pipC_BC_sector5_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight);
                // // bool bad_pipdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pipdep_PCAL_CutCond) { h_pipdep_PCAL_VS_pipC_AC_sector5_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight); }

                // h_SF_VS_P_pip_BC_sector5_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight);
                // // bool bad_SF_CutCond = (EoP_pip < 0.2 || EoP_pip > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pip_AC_sector5_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight); }

                // h_SF_VS_Lv_BC_sector5_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // h_SF_VS_Lw_BC_sector5_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // h_SF_VS_Lu_BC_sector5_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // bool bad_PCAL_pipdge_CutCond = (piplus[i]->cal(clas12::PCAL)->getLv() < 14. || piplus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pipdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector5_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // //     h_SF_VS_Lw_AC_sector5_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // //     h_SF_VS_Lu_AC_sector5_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // }

                // h_pip_PCALoP_pip_VS_pip_PCALoP_pip_BC_sector5_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piplus[i]));
                // // if (!bad_diag_CutCond) { h_pip_PCALoP_pip_VS_pip_PCALoP_pip_AC_sector5_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight); }

                // // if (bad_Vz_pip_CutCond) { continue; }
                // // if (bad_DC_pipdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pipdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pipdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pip_sector5_1e_cut->Fill(reco_P_pip.Mag(), weight);
                // // h_reco_theta_pip_sector5_1e_cut->Fill(reco_P_pip.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pip_sector5_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pip_VS_reco_phi_pip_sector5_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, reco_P_pip.Theta() * 180 / M_PI, weight);
            } else if (piplus[i]->getSector() == 6) {
                h_Vx_pip_BC_sector6_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                h_Vy_pip_BC_sector6_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                h_Vz_pip_BC_sector6_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -4. || piplus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pip_CutCond = (piplus[i]->par()->getVz() < -6. ||
                // // // piplus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pip_CutCond) {
                // //     h_Vx_pip_AC_sector6_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                // //     h_Vy_pip_AC_sector6_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                // //     h_Vz_pip_AC_sector6_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piplus[i], h_dc_piplectron_hit_map_BC_sector6_1e_cut, weight);
                // // bool bad_DC_pipdge_CutCond = (!DCEdgeCuts(piplus[i]));
                // // if (!bad_DC_pipdge_CutCond) { fillDCdebug(piplus[i], h_dc_piplectron_hit_map_AC_sector6_1e_cut, weight); }

                // h_nphe_BC_sector6_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piplus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector6_1e_cut->Fill(piplus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pipdep_PCAL_VS_pipC_BC_sector6_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight);
                // // bool bad_pipdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pipdep_PCAL_CutCond) { h_pipdep_PCAL_VS_pipC_AC_sector6_1e_cut->Fill(Edep_PCAL, Edep_pipC, weight); }

                // h_SF_VS_P_pip_BC_sector6_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight);
                // // bool bad_SF_CutCond = (EoP_pip < 0.2 || EoP_pip > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pip_AC_sector6_1e_cut->Fill(reco_P_pip.Mag(), EoP_pip, weight); }

                // h_SF_VS_Lv_BC_sector6_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // h_SF_VS_Lw_BC_sector6_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // h_SF_VS_Lu_BC_sector6_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // bool bad_PCAL_pipdge_CutCond = (piplus[i]->cal(clas12::PCAL)->getLv() < 14. || piplus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pipdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector6_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLv(), EoP_pip, weight);
                // //     h_SF_VS_Lw_AC_sector6_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLw(), EoP_pip, weight);
                // //     h_SF_VS_Lu_AC_sector6_1e_cut->Fill(piplus[i]->cal(clas12::PCAL)->getLu(), EoP_pip, weight);
                // // }

                // h_pip_PCALoP_pip_VS_pip_PCALoP_pip_BC_sector6_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piplus[i]));
                // // if (!bad_diag_CutCond) { h_pip_PCALoP_pip_VS_pip_PCALoP_pip_AC_sector6_1e_cut->Fill(E_PCALoP_pip, E_pipCINoP_pip, weight); }

                // // if (bad_Vz_pip_CutCond) { continue; }
                // // if (bad_DC_pipdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pipdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pipdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pip_sector6_1e_cut->Fill(reco_P_pip.Mag(), weight);
                // // h_reco_theta_pip_sector6_1e_cut->Fill(reco_P_pip.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pip_sector6_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pip_VS_reco_phi_pip_sector6_1e_cut->Fill(reco_P_pip.Phi() * 180 / M_PI, reco_P_pip.Theta() * 180 / M_PI, weight);
            }
        }

#pragma endregion

        //  Piminus PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

#pragma region Piminus PID cuts

        for (int i = 0; i < piminus.size(); i++) {
            h_Vx_pim_BC_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
            h_Vy_pim_BC_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
            h_Vz_pim_BC_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
            // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
            // // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -6. ||
            // // // piminus[i]->par()->getVz() > 0.);
            // // if (!bad_Vz_pim_CutCond) {
            // //     h_Vx_pim_AC_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
            // //     h_Vy_pim_AC_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
            // //     h_Vz_pim_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
            // // }

            // // fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_BC_1e_cut, weight);
            // // bool bad_DC_pimdge_CutCond = (!DCEdgeCuts(piminus[i]));
            // // if (!bad_DC_pimdge_CutCond) { fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_AC_1e_cut, weight); }

            // h_nphe_BC_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight);
            // // bool bad_nphe_CutCond = (piminus[i]->che(clas12::HTCC)->getNphe() <= 2);
            // // if (!bad_nphe_CutCond) { h_nphe_AC_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight); }

            // h_pimdep_PCAL_VS_pimC_BC_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight);
            // // bool bad_pimdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
            // // if (!bad_pimdep_PCAL_CutCond) { h_pimdep_PCAL_VS_pimC_AC_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight); }

            // h_SF_VS_P_pim_BC_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight);
            // // bool bad_SF_CutCond = (EoP_pim < 0.2 || EoP_pim > 0.28);
            // // if (!bad_SF_CutCond) { h_SF_VS_P_pim_AC_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight); }

            // h_SF_VS_Lv_BC_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
            // h_SF_VS_Lw_BC_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
            // h_SF_VS_Lu_BC_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
            // // bool bad_PCAL_pimdge_CutCond = (piminus[i]->cal(clas12::PCAL)->getLv() < 14. || piminus[i]->cal(clas12::PCAL)->getLw() < 14.);
            // // if (!bad_PCAL_pimdge_CutCond) {
            // //     h_SF_VS_Lv_AC_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
            // //     h_SF_VS_Lw_AC_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
            // //     h_SF_VS_Lu_AC_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
            // // }

            // h_pim_PCALoP_pim_VS_pim_PCALoP_pim_BC_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight);
            // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piminus[i]));
            // // if (!bad_diag_CutCond) { h_pim_PCALoP_pim_VS_pim_PCALoP_pim_AC_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight); }

            // // if (bad_Vz_pim_CutCond) { continue; }
            // // if (bad_DC_pimdge_CutCond) { continue; }

            // // if (bad_nphe_CutCond) { continue; }
            // // if (bad_pimdep_PCAL_CutCond) { continue; }
            // // if (bad_SF_CutCond) { continue; }
            // // if (bad_PCAL_pimdge_CutCond) { continue; }
            // // if (bad_diag_CutCond) { continue; }

            // // h_reco_P_pim_1e_cut->Fill(reco_P_pim.Mag(), weight);
            // // h_reco_theta_pim_1e_cut->Fill(reco_P_pim.Theta() * 180 / M_PI, weight);
            // // h_reco_phi_pim_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, weight);
            // // h_reco_theta_pim_VS_reco_phi_pim_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, reco_P_pim.Theta() * 180 / M_PI, weight);

            if (piminus[i]->getSector() == 1) {
                h_Vx_pim_BC_sector1_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                h_Vy_pim_BC_sector1_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                h_Vz_pim_BC_sector1_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -6. ||
                // // // piminus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pim_CutCond) {
                // //     h_Vx_pim_AC_sector1_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                // //     h_Vy_pim_AC_sector1_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                // //     h_Vz_pim_AC_sector1_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_BC_sector1_1e_cut, weight);
                // // bool bad_DC_pimdge_CutCond = (!DCEdgeCuts(piminus[i]));
                // // if (!bad_DC_pimdge_CutCond) { fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_AC_sector1_1e_cut, weight); }

                // h_nphe_BC_sector1_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piminus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector1_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pimdep_PCAL_VS_pimC_BC_sector1_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight);
                // // bool bad_pimdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pimdep_PCAL_CutCond) { h_pimdep_PCAL_VS_pimC_AC_sector1_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight); }

                // h_SF_VS_P_pim_BC_sector1_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight);
                // // bool bad_SF_CutCond = (EoP_pim < 0.2 || EoP_pim > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pim_AC_sector1_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight); }

                // h_SF_VS_Lv_BC_sector1_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // h_SF_VS_Lw_BC_sector1_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // h_SF_VS_Lu_BC_sector1_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // bool bad_PCAL_pimdge_CutCond = (piminus[i]->cal(clas12::PCAL)->getLv() < 14. || piminus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pimdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector1_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // //     h_SF_VS_Lw_AC_sector1_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // //     h_SF_VS_Lu_AC_sector1_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // }

                // h_pim_PCALoP_pim_VS_pim_PCALoP_pim_BC_sector1_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piminus[i]));
                // // if (!bad_diag_CutCond) { h_pim_PCALoP_pim_VS_pim_PCALoP_pim_AC_sector1_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight); }

                // // if (bad_Vz_pim_CutCond) { continue; }
                // // if (bad_DC_pimdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pimdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pimdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pim_sector1_1e_cut->Fill(reco_P_pim.Mag(), weight);
                // // h_reco_theta_pim_sector1_1e_cut->Fill(reco_P_pim.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pim_sector1_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pim_VS_reco_phi_pim_sector1_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, reco_P_pim.Theta() * 180 / M_PI, weight);
            } else if (piminus[i]->getSector() == 2) {
                h_Vx_pim_BC_sector2_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                h_Vy_pim_BC_sector2_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                h_Vz_pim_BC_sector2_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -6. ||
                // // // piminus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pim_CutCond) {
                // //     h_Vx_pim_AC_sector2_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                // //     h_Vy_pim_AC_sector2_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                // //     h_Vz_pim_AC_sector2_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_BC_sector2_1e_cut, weight);
                // // bool bad_DC_pimdge_CutCond = (!DCEdgeCuts(piminus[i]));
                // // if (!bad_DC_pimdge_CutCond) { fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_AC_sector2_1e_cut, weight); }

                // h_nphe_BC_sector2_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piminus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector2_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pimdep_PCAL_VS_pimC_BC_sector2_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight);
                // // bool bad_pimdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pimdep_PCAL_CutCond) { h_pimdep_PCAL_VS_pimC_AC_sector2_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight); }

                // h_SF_VS_P_pim_BC_sector2_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight);
                // // bool bad_SF_CutCond = (EoP_pim < 0.2 || EoP_pim > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pim_AC_sector2_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight); }

                // h_SF_VS_Lv_BC_sector2_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // h_SF_VS_Lw_BC_sector2_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // h_SF_VS_Lu_BC_sector2_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // bool bad_PCAL_pimdge_CutCond = (piminus[i]->cal(clas12::PCAL)->getLv() < 14. || piminus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pimdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector2_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // //     h_SF_VS_Lw_AC_sector2_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // //     h_SF_VS_Lu_AC_sector2_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // }

                // h_pim_PCALoP_pim_VS_pim_PCALoP_pim_BC_sector2_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piminus[i]));
                // // if (!bad_diag_CutCond) { h_pim_PCALoP_pim_VS_pim_PCALoP_pim_AC_sector2_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight); }

                // // if (bad_Vz_pim_CutCond) { continue; }
                // // if (bad_DC_pimdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pimdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pimdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pim_sector2_1e_cut->Fill(reco_P_pim.Mag(), weight);
                // // h_reco_theta_pim_sector2_1e_cut->Fill(reco_P_pim.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pim_sector2_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pim_VS_reco_phi_pim_sector2_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, reco_P_pim.Theta() * 180 / M_PI, weight);
            } else if (piminus[i]->getSector() == 3) {
                h_Vx_pim_BC_sector3_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                h_Vy_pim_BC_sector3_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                h_Vz_pim_BC_sector3_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -6. ||
                // // // piminus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pim_CutCond) {
                // //     h_Vx_pim_AC_sector3_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                // //     h_Vy_pim_AC_sector3_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                // //     h_Vz_pim_AC_sector3_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_BC_sector3_1e_cut, weight);
                // // bool bad_DC_pimdge_CutCond = (!DCEdgeCuts(piminus[i]));
                // // if (!bad_DC_pimdge_CutCond) { fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_AC_sector3_1e_cut, weight); }

                // h_nphe_BC_sector3_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piminus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector3_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pimdep_PCAL_VS_pimC_BC_sector3_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight);
                // // bool bad_pimdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pimdep_PCAL_CutCond) { h_pimdep_PCAL_VS_pimC_AC_sector3_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight); }

                // h_SF_VS_P_pim_BC_sector3_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight);
                // // bool bad_SF_CutCond = (EoP_pim < 0.2 || EoP_pim > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pim_AC_sector3_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight); }

                // h_SF_VS_Lv_BC_sector3_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // h_SF_VS_Lw_BC_sector3_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // h_SF_VS_Lu_BC_sector3_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // bool bad_PCAL_pimdge_CutCond = (piminus[i]->cal(clas12::PCAL)->getLv() < 14. || piminus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pimdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector3_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // //     h_SF_VS_Lw_AC_sector3_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // //     h_SF_VS_Lu_AC_sector3_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // }

                // h_pim_PCALoP_pim_VS_pim_PCALoP_pim_BC_sector3_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piminus[i]));
                // // if (!bad_diag_CutCond) { h_pim_PCALoP_pim_VS_pim_PCALoP_pim_AC_sector3_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight); }

                // // if (bad_Vz_pim_CutCond) { continue; }
                // // if (bad_DC_pimdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pimdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pimdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pim_sector3_1e_cut->Fill(reco_P_pim.Mag(), weight);
                // // h_reco_theta_pim_sector3_1e_cut->Fill(reco_P_pim.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pim_sector3_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pim_VS_reco_phi_pim_sector3_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, reco_P_pim.Theta() * 180 / M_PI, weight);
            } else if (piminus[i]->getSector() == 4) {
                h_Vx_pim_BC_sector4_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                h_Vy_pim_BC_sector4_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                h_Vz_pim_BC_sector4_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -6. ||
                // // // piminus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pim_CutCond) {
                // //     h_Vx_pim_AC_sector4_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                // //     h_Vy_pim_AC_sector4_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                // //     h_Vz_pim_AC_sector4_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_BC_sector4_1e_cut, weight);
                // // bool bad_DC_pimdge_CutCond = (!DCEdgeCuts(piminus[i]));
                // // if (!bad_DC_pimdge_CutCond) { fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_AC_sector4_1e_cut, weight); }

                // h_nphe_BC_sector4_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piminus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector4_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pimdep_PCAL_VS_pimC_BC_sector4_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight);
                // // bool bad_pimdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pimdep_PCAL_CutCond) { h_pimdep_PCAL_VS_pimC_AC_sector4_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight); }

                // h_SF_VS_P_pim_BC_sector4_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight);
                // // bool bad_SF_CutCond = (EoP_pim < 0.2 || EoP_pim > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pim_AC_sector4_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight); }

                // h_SF_VS_Lv_BC_sector4_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // h_SF_VS_Lw_BC_sector4_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // h_SF_VS_Lu_BC_sector4_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // bool bad_PCAL_pimdge_CutCond = (piminus[i]->cal(clas12::PCAL)->getLv() < 14. || piminus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pimdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector4_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // //     h_SF_VS_Lw_AC_sector4_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // //     h_SF_VS_Lu_AC_sector4_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // }

                // h_pim_PCALoP_pim_VS_pim_PCALoP_pim_BC_sector4_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piminus[i]));
                // // if (!bad_diag_CutCond) { h_pim_PCALoP_pim_VS_pim_PCALoP_pim_AC_sector4_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight); }

                // // if (bad_Vz_pim_CutCond) { continue; }
                // // if (bad_DC_pimdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pimdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pimdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pim_sector4_1e_cut->Fill(reco_P_pim.Mag(), weight);
                // // h_reco_theta_pim_sector4_1e_cut->Fill(reco_P_pim.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pim_sector4_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pim_VS_reco_phi_pim_sector4_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, reco_P_pim.Theta() * 180 / M_PI, weight);
            } else if (piminus[i]->getSector() == 5) {
                h_Vx_pim_BC_sector5_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                h_Vy_pim_BC_sector5_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                h_Vz_pim_BC_sector5_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -6. ||
                // // // piminus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pim_CutCond) {
                // //     h_Vx_pim_AC_sector5_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                // //     h_Vy_pim_AC_sector5_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                // //     h_Vz_pim_AC_sector5_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_BC_sector5_1e_cut, weight);
                // // bool bad_DC_pimdge_CutCond = (!DCEdgeCuts(piminus[i]));
                // // if (!bad_DC_pimdge_CutCond) { fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_AC_sector5_1e_cut, weight); }

                // h_nphe_BC_sector5_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piminus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector5_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pimdep_PCAL_VS_pimC_BC_sector5_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight);
                // // bool bad_pimdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pimdep_PCAL_CutCond) { h_pimdep_PCAL_VS_pimC_AC_sector5_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight); }

                // h_SF_VS_P_pim_BC_sector5_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight);
                // // bool bad_SF_CutCond = (EoP_pim < 0.2 || EoP_pim > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pim_AC_sector5_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight); }

                // h_SF_VS_Lv_BC_sector5_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // h_SF_VS_Lw_BC_sector5_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // h_SF_VS_Lu_BC_sector5_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // bool bad_PCAL_pimdge_CutCond = (piminus[i]->cal(clas12::PCAL)->getLv() < 14. || piminus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pimdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector5_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // //     h_SF_VS_Lw_AC_sector5_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // //     h_SF_VS_Lu_AC_sector5_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // }

                // h_pim_PCALoP_pim_VS_pim_PCALoP_pim_BC_sector5_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piminus[i]));
                // // if (!bad_diag_CutCond) { h_pim_PCALoP_pim_VS_pim_PCALoP_pim_AC_sector5_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight); }

                // // if (bad_Vz_pim_CutCond) { continue; }
                // // if (bad_DC_pimdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pimdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pimdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pim_sector5_1e_cut->Fill(reco_P_pim.Mag(), weight);
                // // h_reco_theta_pim_sector5_1e_cut->Fill(reco_P_pim.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pim_sector5_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pim_VS_reco_phi_pim_sector5_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, reco_P_pim.Theta() * 180 / M_PI, weight);
            } else if (piminus[i]->getSector() == 6) {
                h_Vx_pim_BC_sector6_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                h_Vy_pim_BC_sector6_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                h_Vz_pim_BC_sector6_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -4. || piminus[i]->par()->getVz() > -2.);
                // // // bool bad_Vz_pim_CutCond = (piminus[i]->par()->getVz() < -6. ||
                // // // piminus[i]->par()->getVz() > 0.);
                // // if (!bad_Vz_pim_CutCond) {
                // //     h_Vx_pim_AC_sector6_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                // //     h_Vy_pim_AC_sector6_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                // //     h_Vz_pim_AC_sector6_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                // // }

                // // fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_BC_sector6_1e_cut, weight);
                // // bool bad_DC_pimdge_CutCond = (!DCEdgeCuts(piminus[i]));
                // // if (!bad_DC_pimdge_CutCond) { fillDCdebug(piminus[i], h_dc_pimlectron_hit_map_AC_sector6_1e_cut, weight); }

                // h_nphe_BC_sector6_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight);
                // // bool bad_nphe_CutCond = (piminus[i]->che(clas12::HTCC)->getNphe() <= 2);
                // // if (!bad_nphe_CutCond) { h_nphe_AC_sector6_1e_cut->Fill(piminus[i]->che(clas12::HTCC)->getNphe(), weight); }

                // h_pimdep_PCAL_VS_pimC_BC_sector6_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight);
                // // bool bad_pimdep_PCAL_CutCond = (Edep_PCAL <= 0.06);
                // // if (!bad_pimdep_PCAL_CutCond) { h_pimdep_PCAL_VS_pimC_AC_sector6_1e_cut->Fill(Edep_PCAL, Edep_pimC, weight); }

                // h_SF_VS_P_pim_BC_sector6_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight);
                // // bool bad_SF_CutCond = (EoP_pim < 0.2 || EoP_pim > 0.28);
                // // if (!bad_SF_CutCond) { h_SF_VS_P_pim_AC_sector6_1e_cut->Fill(reco_P_pim.Mag(), EoP_pim, weight); }

                // h_SF_VS_Lv_BC_sector6_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // h_SF_VS_Lw_BC_sector6_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // h_SF_VS_Lu_BC_sector6_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // bool bad_PCAL_pimdge_CutCond = (piminus[i]->cal(clas12::PCAL)->getLv() < 14. || piminus[i]->cal(clas12::PCAL)->getLw() < 14.);
                // // if (!bad_PCAL_pimdge_CutCond) {
                // //     h_SF_VS_Lv_AC_sector6_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLv(), EoP_pim, weight);
                // //     h_SF_VS_Lw_AC_sector6_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLw(), EoP_pim, weight);
                // //     h_SF_VS_Lu_AC_sector6_1e_cut->Fill(piminus[i]->cal(clas12::PCAL)->getLu(), EoP_pim, weight);
                // // }

                // h_pim_PCALoP_pim_VS_pim_PCALoP_pim_BC_sector6_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight);
                // // bool bad_diag_CutCond = (!checkEcalDiagCuts(piminus[i]));
                // // if (!bad_diag_CutCond) { h_pim_PCALoP_pim_VS_pim_PCALoP_pim_AC_sector6_1e_cut->Fill(E_PCALoP_pim, E_pimCINoP_pim, weight); }

                // // if (bad_Vz_pim_CutCond) { continue; }
                // // if (bad_DC_pimdge_CutCond) { continue; }

                // // if (bad_nphe_CutCond) { continue; }
                // // if (bad_pimdep_PCAL_CutCond) { continue; }
                // // if (bad_SF_CutCond) { continue; }
                // // if (bad_PCAL_pimdge_CutCond) { continue; }
                // // if (bad_diag_CutCond) { continue; }

                // // h_reco_P_pim_sector6_1e_cut->Fill(reco_P_pim.Mag(), weight);
                // // h_reco_theta_pim_sector6_1e_cut->Fill(reco_P_pim.Theta() * 180 / M_PI, weight);
                // // h_reco_phi_pim_sector6_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, weight);
                // // h_reco_theta_pim_VS_reco_phi_pim_sector6_1e_cut->Fill(reco_P_pim.Phi() * 180 / M_PI, reco_P_pim.Theta() * 180 / M_PI, weight);
            }
        }

#pragma endregion
    }

#pragma endregion

    std::cout << "\n\nPlotting and saving histograms..." << "\n\n";

    /////////////////////////////////////////////////////
    // Organize histograms
    /////////////////////////////////////////////////////
    for (int i = 0; i < HistoList.size(); i++) {
        if (HistoList[i]->InheritsFrom("TH1D")) {
            HistoList[i]->Sumw2();
            HistoList[i]->SetMinimum(0);
            HistoList[i]->SetLineWidth(2);
            HistoList[i]->SetLineColor(kBlue);
        }

        HistoList[i]->GetXaxis()->CenterTitle();
        HistoList[i]->GetYaxis()->CenterTitle();
    }

    /////////////////////////////////////////////////////
    // Now create the output PDFs
    /////////////////////////////////////////////////////
    int pixelx = 1980;
    int pixely = 1530;
    TCanvas *myText = new TCanvas("myText", "myText", pixelx, pixely);
    TLatex titles;
    TLatex text;
    titles.SetTextSize(0.1);
    text.SetTextSize(0.03);

    gStyle->SetOptStat("ourmen");

    TCanvas *myCanvas = new TCanvas("myPage", "myPage", pixelx, pixely);

    std::string PDF_fileName = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/" + OutFolderName + ".pdf";
    char fileName[PDF_fileName.length()];
    sprintf(fileName, "%s[", PDF_fileName.c_str());
    myText->SaveAs(fileName);
    sprintf(fileName, "%s", PDF_fileName.c_str());

    /////////////////////////////////////
    // CND Neutron Information
    /////////////////////////////////////
    myText->cd();

    titles.DrawLatex(0.05, 0.9, "HipoLooper Output");
    text.DrawLatex(0.05, 0.75, ("Plots from (e,e') events in: #font[42]{" + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + "}").c_str());

    if (IsData) {
        text.DrawLatex(0.05, 0.7, ("InputFiles: #font[42]{" + InputFiles + "}").c_str());
    } else {
        text.DrawLatex(0.05, 0.7, ("BaseDir: #font[42]{" + BaseDir + "}").c_str());
        text.DrawLatex(0.05, 0.65, ("InputFiles: #font[42]{BaseDir + " + InputFiles.substr(BaseDir.length()) + "}").c_str());
    }

    myText->Print(fileName, "pdf");
    myText->Clear();

    bool first_electron = true;
    bool first_electron_sector1 = true, first_electron_sector2 = true, first_electron_sector3 = true, first_electron_sector4 = true, first_electron_sector5 = true,
         first_electron_sector6 = true;

    bool first_piplus = true;
    bool first_piplus_sector1 = true, first_piplus_sector2 = true, first_piplus_sector3 = true, first_piplus_sector4 = true, first_piplus_sector5 = true, first_piplus_sector6 = true;

    bool first_piminus = true;
    bool first_piminus_sector1 = true, first_piminus_sector2 = true, first_piminus_sector3 = true, first_piminus_sector4 = true, first_piminus_sector5 = true, first_piminus_sector6 = true;

    for (int i = 0; i < HistoList.size(); i++) {
        // Maps to hold first-time flags
        std::map<std::string, bool *> first_flags = {{"{e}", &first_electron}, {"{#pi^{+}}", &first_piplus}, {"{#pi^{-}}", &first_piminus}};

        std::map<std::string, std::string> particle_labels = {{"{e}", "e^{-}"}, {"{#pi^{+}}", "#pi^{+}"}, {"{#pi^{-}}", "#pi^{-}"}};

        // Maps of sector flags (assumes these variables already exist)
        std::map<std::string, std::map<int, bool *>> sector_flags = {
            {"{e}",
             {{1, &first_electron_sector1},
              {2, &first_electron_sector2},
              {3, &first_electron_sector3},
              {4, &first_electron_sector4},
              {5, &first_electron_sector5},
              {6, &first_electron_sector6}}},
            {"{#pi^{+}}",
             {{1, &first_piplus_sector1}, {2, &first_piplus_sector2}, {3, &first_piplus_sector3}, {4, &first_piplus_sector4}, {5, &first_piplus_sector5}, {6, &first_piplus_sector6}}},
            {"{#pi^{-}}",
             {{1, &first_piminus_sector1}, {2, &first_piminus_sector2}, {3, &first_piminus_sector3}, {4, &first_piminus_sector4}, {5, &first_piminus_sector5}, {6, &first_piminus_sector6}}}};

        std::string title = HistoList[i]->GetTitle();

        for (const auto &[particle_key, label] : particle_labels) {
            if (basic_tools::FindSubstring(title, particle_key)) {
                if (*first_flags[particle_key] && !basic_tools::FindSubstring(title, "sector")) {
                    myText->cd();
                    titles.DrawLatex(0.3, 0.5, (label + " plots").c_str());
                    myText->Print(fileName, "pdf");
                    myText->Clear();
                    *first_flags[particle_key] = false;
                } else {
                    for (int sector = 1; sector <= 6; ++sector) {
                        std::string sector_str = "sector" + std::to_string(sector);
                        if (*sector_flags[particle_key][sector] && basic_tools::FindSubstring(title, sector_str)) {
                            myText->cd();
                            titles.DrawLatex(0.3, 0.5, (label + " plots - " + sector_str).c_str());
                            myText->Print(fileName, "pdf");
                            myText->Clear();
                            *sector_flags[particle_key][sector] = false;
                            break;
                        }
                    }
                }
                break;  // once matched with a particle type, no need to check others
            }
        }
        // if (basic_tools::FindSubstring(HistoList[i]->GetTitle(), "{e}")) {
        //     if (first_electron && !basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "e^{-} plots");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_electron = false;
        //     } else if (first_electron_sector1 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector1")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "e^{-} plots - sector1");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_electron_sector1 = false;
        //     } else if (first_electron_sector2 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector2")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "e^{-} plots - sector2");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_electron_sector2 = false;
        //     } else if (first_electron_sector3 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector3")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "e^{-} plots - sector3");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_electron_sector3 = false;
        //     } else if (first_electron_sector4 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector4")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "e^{-} plots - sector4");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_electron_sector4 = false;
        //     } else if (first_electron_sector5 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector5")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "e^{-} plots - sector5");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_electron_sector5 = false;
        //     } else if (first_electron_sector6 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector6")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "e^{-} plots - sector6");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_electron_sector6 = false;
        //     }
        // } else if (basic_tools::FindSubstring(HistoList[i]->GetTitle(), "{#pi^{+}}")) {
        //     if (first_piplus && !basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{+} plots");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piplus = false;
        //     } else if (first_piplus_sector1 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector1")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{+} plots - sector1");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piplus_sector1 = false;
        //     } else if (first_piplus_sector2 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector2")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{+} plots - sector2");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piplus_sector2 = false;
        //     } else if (first_piplus_sector3 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector3")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{+} plots - sector3");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piplus_sector3 = false;
        //     } else if (first_piplus_sector4 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector4")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{+} plots - sector4");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         myText->cd(1);
        //         first_piplus_sector4 = false;
        //     } else if (first_piplus_sector5 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector5")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{+} plots - sector5");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piplus_sector5 = false;
        //     } else if (first_piplus_sector6 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector6")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{+} plots - sector6");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piplus_sector6 = false;
        //     }
        // } else if (basic_tools::FindSubstring(HistoList[i]->GetTitle(), "{#pi^{-}}")) {
        //     if (first_piminus && !basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{-} plots");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piminus = false;
        //     } else if (first_piminus_sector1 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector1")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{-} plots - sector1");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         myText->cd(1);
        //         first_piminus_sector1 = false;
        //     } else if (first_piminus_sector2 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector2")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{-} plots - sector2");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piminus_sector2 = false;
        //     } else if (first_piminus_sector3 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector3")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{-} plots - sector3");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piminus_sector3 = false;
        //     } else if (first_piminus_sector4 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector4")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{-} plots - sector4");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piminus_sector4 = false;
        //     } else if (first_piminus_sector5 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector5")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{-} plots - sector5");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piminus_sector5 = false;
        //     } else if (first_piminus_sector6 && basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector6")) {
        //         myText->cd();
        //         titles.DrawLatex(0.3, 0.5, "#pi^{-} plots - sector6");
        //         myText->Print(fileName, "pdf");
        //         myText->Clear();

        //         first_piminus_sector6 = false;
        //     }
        // }

        myCanvas->cd();

        myCanvas->cd()->SetGrid();
        myCanvas->cd()->SetBottomMargin(0.14), myCanvas->cd()->SetLeftMargin(0.16), myCanvas->cd()->SetRightMargin(0.12);

        HistoList[i]->GetYaxis()->SetTitleOffset(1.5);
        HistoList[i]->GetXaxis()->SetTitleOffset(1.1);

        gPad->SetRightMargin(0.23);

        if (HistoList[i]->InheritsFrom("TH1D")) {
            HistoList[i]->Draw();

            gPad->Update();
            TLine *speac_target_location_TLine;
            double speac_target_location_value = 0.0;

            if (target_status == "C12") {
                speac_target_location_value = (2.5 - 3.0);
            } else if (target_status == "Ar40") {
                speac_target_location_value = (-2.5 - 3.0);
            }

            speac_target_location_TLine = new TLine(speac_target_location_value, 0., speac_target_location_value, gPad->GetFrame()->GetY2());
            speac_target_location_TLine->SetLineColor(kRed);
            speac_target_location_TLine->SetLineWidth(2);
            speac_target_location_TLine->Draw("same");

            TLine *measured_target_location_TLine;
            double measured_target_location_value = HistoList[i]->GetBinCenter(HistoList[i]->GetMaximumBin());

            measured_target_location_TLine = new TLine(measured_target_location_value, 0., measured_target_location_value, gPad->GetFrame()->GetY2());
            measured_target_location_TLine->SetLineColor(kGreen);
            measured_target_location_TLine->SetLineWidth(4);
            measured_target_location_TLine->SetLineStyle(2);
            measured_target_location_TLine->Draw("same");

            auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.25, gStyle->GetStatY() - 0.35);
            // auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.25, gStyle->GetStatY() - 0.3);
            TLegendEntry *speac_target_location_TLine_entry =
                Legend->AddEntry(speac_target_location_TLine, ("Spec target pos. = " + basic_tools::ToStringWithPrecision(speac_target_location_value, 2) + " cm").c_str(), "l");
            TLegendEntry *measured_target_location_TLine_entry =
                Legend->AddEntry(measured_target_location_TLine, ("Meas. target pos. = " + basic_tools::ToStringWithPrecision(measured_target_location_value, 2) + " cm").c_str(), "l");

            Legend->Draw("same");
        } else if (HistoList[i]->InheritsFrom("TH2D")) {
            HistoList[i]->Draw("colz");

            if (HistoList[i]->GetEntries() != 0) {
                gPad->Update();
                TPaletteAxis *palette = (TPaletteAxis *)HistoList[i]->GetListOfFunctions()->FindObject("palette");
                palette->SetY2NDC(0.475);
                gPad->Modified();
                gPad->Update();
            }
        }

        myCanvas->Print(fileName, "pdf");
        myCanvas->Clear();
    }

    sprintf(fileName, "%s]", PDF_fileName.c_str());
    myCanvas->Print(fileName, "pdf");

    outFile->cd();
    for (int i = 0; i < HistoList.size(); i++) { HistoList[i]->Write(); }
    outFile->Close();
}
