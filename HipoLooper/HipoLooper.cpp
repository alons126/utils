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

// const std::string CLAS12ANA_DIR = std::string(gSystem->WorkingDirectory()) + "/framework/classes/clas12ana/";

// Include libraries:
#include "framework/namespaces/general_utilities/utilities.h"
#include "framework/namespaces/setup/path_definitions.h"

// Include classes:
#include "framework/classes/clas12ana/clas12ana.cpp"
// #include "framework/classes/clas12ana/clas12ana.h"

// Include CLAS12 libraries:
#include "framework/includes/clas12_include.h"

using namespace constants;

void HipoLooper() {
    std::cout << "\n\nInitiating HipoLooper.cpp\n";

    int version = 12;  // Version of the code
    std::string OutFolderName_prefix = "0" + basic_tools::ToStringWithPrecision(version, 0) + "_HipoLooper";
    std::string OutFolderName_ver_status = "_v" + basic_tools::ToStringWithPrecision(version, 0) + "_";

    // std::string General_status = "";
    // std::string General_status = "_NewClas12Ana_Cuts_wdVz_cuts_regTest";
    std::string General_status = "_NewClas12Ana_Cuts_wdVz_cuts_on_both_electrons_and_pions";

    bool ApplyLimiter = true;
    int Limiter = 10000000;  // 10M events (fo the data)
    // int Limiter = 1000000;  // 100 files or 1M events (fo the data)
    // int Limiter = 100000;  // 10 files or 100K events (fo the data)
    // int Limiter = 10000;  // 1 file

    std::string BaseDir = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples";

    std::vector<std::string> InputFiles;

    // // Data samples:

    // InputFiles.push_back("/cache/clas12/rg-m/production/pass1/2gev/C/dst/recon/015664/*.hipo");
    // InputFiles.push_back("/cache/clas12/rg-m/production/pass1/4gev/C/dst/recon/015778/*.hipo");

    InputFiles.push_back("/cache/clas12/rg-m/production/pass1/2gev/Ar/dst/recon/015672/*.hipo");
    // InputFiles.push_back("/cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon/015743/*.hipo");
    // InputFiles.push_back("/cache/clas12/rg-m/production/pass1/6gev/Ar/dst/recon/015792/*.hipo");

    // // Simulation samples:

    // InputFiles.push_back(BaseDir + "/C12/G18_10a_00_000/2070MeV_Q2_0_02_Ar40_test/reconhipo/*.hipo");
    // InputFiles.push_back(BaseDir + "/C12/G18_10a_00_000/4029MeV_Q2_0_25_Ar40_test/reconhipo/*.hipo");

    // InputFiles.push_back(BaseDir + "/Ar40/G18_10a_00_000/2070MeV_Q2_0_02_Ar40_test/reconhipo/*.hipo");
    // InputFiles.push_back(BaseDir + "/Ar40/G18_10a_00_000/4029MeV_Q2_0_25_Ar40_test/reconhipo/*.hipo");
    // InputFiles.push_back(BaseDir + "/Ar40/G18_10a_00_000/5986MeV_Q2_0_40_Ar40_test/reconhipo/*.hipo");

    for (int sample = 0; sample < InputFiles.size(); sample++) {
#pragma region Setup and configuration
        bool IsData = basic_tools::FindSubstring(InputFiles.at(sample), "cache");

        double Ebeam = (basic_tools::FindSubstring(InputFiles.at(sample), "2070MeV") || basic_tools::FindSubstring(InputFiles.at(sample), "2gev"))   ? 2.07052
                       : (basic_tools::FindSubstring(InputFiles.at(sample), "4029MeV") || basic_tools::FindSubstring(InputFiles.at(sample), "4gev")) ? 4.02962
                       : (basic_tools::FindSubstring(InputFiles.at(sample), "5986MeV") || basic_tools::FindSubstring(InputFiles.at(sample), "6gev")) ? 5.98636
                                                                                                                                                     : 0.0;
        if (Ebeam == 0.0) {
            std::cerr << "\n\nError! Ebeam not found in InputFiles string! Aborting...\n\n";
            exit(1);
        }

        bool Is2GeV = (basic_tools::FindSubstring(InputFiles.at(sample), "2070MeV") || basic_tools::FindSubstring(InputFiles.at(sample), "2gev"));
        bool Is4GeV = (basic_tools::FindSubstring(InputFiles.at(sample), "4029MeV") || basic_tools::FindSubstring(InputFiles.at(sample), "4gev"));
        bool Is6GeV = (basic_tools::FindSubstring(InputFiles.at(sample), "5986MeV") || basic_tools::FindSubstring(InputFiles.at(sample), "6gev"));

        std::string target_status = (basic_tools::FindSubstring(InputFiles.at(sample), "/C12/") || basic_tools::FindSubstring(InputFiles.at(sample), "/C/"))     ? "C12"
                                    : (basic_tools::FindSubstring(InputFiles.at(sample), "/Ar40/") || basic_tools::FindSubstring(InputFiles.at(sample), "/Ar/")) ? "Ar40"
                                                                                                                                                                 : "_Unknown";

        if (target_status == "_Unknown") {
            std::cerr << "\n\nError! Target not found in InputFiles string! Aborting...\n\n";
            exit(1);
        }

        std::string sample_type_status = IsData ? "_data" : "_sim";
        std::string genie_tune_status = !IsData ? "_G18_" : "_";
        std::string Ebeam_status_1 = Is2GeV ? "2GeV" : Is4GeV ? "4GeV" : Is6GeV ? "6GeV" : "_Unknown";
        std::string Ebeam_status_2 = Is2GeV ? "_2GeV" : Is4GeV ? "_4GeV" : Is6GeV ? "_6GeV" : "_Unknown";

        if (Ebeam_status_1 == "_Unknown") {
            std::cerr << "\n\nError! Ebeam not found in InputFiles string! Aborting...\n\n";
            exit(1);
        }

        if (Ebeam_status_2 == "_Unknown") {
            std::cerr << "\n\nError! Ebeam not found in InputFiles string! Aborting...\n\n";
            exit(1);
        }

        std::string Run_status = basic_tools::FindSubstring(InputFiles.at(sample), "015664")   ? "_run_015664"
                                 : basic_tools::FindSubstring(InputFiles.at(sample), "015778") ? "_run_015778"
                                 : basic_tools::FindSubstring(InputFiles.at(sample), "015672") ? "_run_015672"
                                 : basic_tools::FindSubstring(InputFiles.at(sample), "015743") ? "_run_015743"
                                 : basic_tools::FindSubstring(InputFiles.at(sample), "015792") ? "_run_015792"
                                                                                               : "";

        std::string CodeRun_status = target_status + sample_type_status + genie_tune_status + Ebeam_status_1 + Run_status;

        std::string OutFolderName = OutFolderName_prefix + OutFolderName_ver_status + CodeRun_status + General_status;
        std::string OutFileName = OutFolderName;

        const std::string OutputDir = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName;
        system(("rm -rf " + OutputDir).c_str());
        system(("mkdir -p " + OutputDir).c_str());

        const std::string IndividualPlotsOutputDir = OutputDir + "/Individual_plots/";
        system(("rm -rf " + IndividualPlotsOutputDir).c_str());
        system(("mkdir -p " + IndividualPlotsOutputDir).c_str());

        TFile *outFile = new TFile((OutputDir + "/" + OutFileName + ".root").c_str(), "RECREATE");

        std::string SampleName = target_status + sample_type_status + Ebeam_status_2 + Run_status;
        TString Beam_energy_TString = Ebeam_status_1;

        clas12root::HipoChain chain;
        chain.Add(InputFiles.at(sample));
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

#pragma region electron histograms

#pragma region electron histograms - all sectors
        TH1D *h_Vz_e_BC_1e_cut = new TH1D("Vz_e_BC_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_BC_1e_cut);
        TH1D *h_Vz_e_AC_1e_cut = new TH1D("Vz_e_AC_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_AC_1e_cut);

        TH1D *h_Vz_e_BC_zoomin_1e_cut, *h_Vz_e_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_e_BC_zoomin_1e_cut =
                new TH1D("Vz_e_BC_zoomin_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts);V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_BC_zoomin_1e_cut);
            h_Vz_e_AC_zoomin_1e_cut =
                new TH1D("Vz_e_AC_zoomin_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts);V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_e_BC_zoomin_1e_cut =
                new TH1D("Vz_e_BC_zoomin_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts);V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_BC_zoomin_1e_cut);
            h_Vz_e_AC_zoomin_1e_cut =
                new TH1D("Vz_e_AC_zoomin_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts);V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_AC_zoomin_1e_cut);
        }

        TH1D *h_Vx_e_BC_1e_cut = new TH1D("Vx_e_BC_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_BC_1e_cut);
        TH1D *h_Vx_e_AC_1e_cut = new TH1D("Vx_e_AC_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_AC_1e_cut);
        TH1D *h_Vy_e_BC_1e_cut = new TH1D("Vy_e_BC_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_BC_1e_cut);
        TH1D *h_Vy_e_AC_1e_cut = new TH1D("Vy_e_AC_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_AC_1e_cut);

        TH2D *h_dc_electron_hit_map_BC_1e_cut[4];  // 3 regions
        TH2D *h_dc_electron_hit_map_AC_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_electron_hit_map_BC_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_BC_%d", i), Form("e^{-} DC hitmap in region %d (before e^{-} cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_BC_1e_cut[i]);
            h_dc_electron_hit_map_AC_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_AC_%d", i), Form("e^{-} DC hitmap in region %d (after e^{-} cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_AC_1e_cut[i]);
        }

        TH1D *h_nphe_BC_1e_cut = new TH1D("nphe_BC_1e_cut", "Number of photo-electrons in HTCC in (e,e') (before e^{-} cuts);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_BC_1e_cut);
        TH1D *h_nphe_AC_1e_cut = new TH1D("nphe_AC_1e_cut", "Number of photo-electrons in HTCC in (e,e') (after e^{-} cuts);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_AC_1e_cut);

        TH2D *h_Edep_PCAL_VS_EC_BC_1e_cut =
            new TH2D("Edep_PCAL_VS_EC_BC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (before e^{-} cuts);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]",
                     100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_BC_1e_cut);
        TH2D *h_Edep_PCAL_VS_EC_AC_1e_cut =
            new TH2D("Edep_PCAL_VS_EC_AC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (after e^{-} cuts);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]",
                     100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_AC_1e_cut);

        TH2D *h_SF_VS_Edep_PCAL_BC_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_BC_1e_cut", "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (before e^{-} cuts);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25,
                     100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_BC_1e_cut);
        TH2D *h_SF_VS_Edep_PCAL_AC_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_AC_1e_cut", "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (after e^{-} cuts);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25,
                     100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_AC_1e_cut);

        TH2D *h_SF_VS_P_e_BC_1e_cut = new TH2D("SF_VS_P_e_BC_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (before e^{-} cuts);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                                               Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_BC_1e_cut);
        TH2D *h_SF_VS_P_e_AC_1e_cut = new TH2D("SF_VS_P_e_AC_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (after e^{-} cuts);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                                               Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_AC_1e_cut);

        TH2D *h_SF_VS_Lv_BC_1e_cut =
            new TH2D("SF_VS_Lv_BC_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (before e^{-} cuts);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_BC_1e_cut);
        TH2D *h_SF_VS_Lv_AC_1e_cut =
            new TH2D("SF_VS_Lv_AC_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (after e^{-} cuts);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_AC_1e_cut);

        TH2D *h_SF_VS_Lw_BC_1e_cut =
            new TH2D("SF_VS_Lw_BC_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (before e^{-} cuts);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_BC_1e_cut);
        TH2D *h_SF_VS_Lw_AC_1e_cut =
            new TH2D("SF_VS_Lw_AC_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (after e^{-} cuts);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_AC_1e_cut);

        TH2D *h_SF_VS_Lu_BC_1e_cut =
            new TH2D("SF_VS_Lu_BC_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (before e^{-} cuts);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_BC_1e_cut);
        TH2D *h_SF_VS_Lu_AC_1e_cut =
            new TH2D("SF_VS_Lu_AC_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (after e^{-} cuts);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_AC_1e_cut);

        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (before e^{-} cuts);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100,
                     0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut);
        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut = new TH2D(
            "E_PCALoP_e_VS_E_PCALoP_e_AC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (after e^{-} cuts);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 1
        TH1D *h_Vz_e_BC_sector1_1e_cut =
            new TH1D("Vz_e_BC_sector1_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector1);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_BC_sector1_1e_cut);
        TH1D *h_Vz_e_AC_sector1_1e_cut =
            new TH1D("Vz_e_AC_sector1_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector1);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_AC_sector1_1e_cut);

        TH1D *h_Vz_e_BC_zoomin_sector1_1e_cut, *h_Vz_e_AC_zoomin_sector1_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_e_BC_zoomin_sector1_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector1_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector1);V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector1_1e_cut);
            h_Vz_e_AC_zoomin_sector1_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector1_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector1);V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector1_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_e_BC_zoomin_sector1_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector1_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector1);V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector1_1e_cut);
            h_Vz_e_AC_zoomin_sector1_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector1_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector1);V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector1_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector1_1e_cut =
            new TH1D("Vx_e_BC_sector1_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector1);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_BC_sector1_1e_cut);
        TH1D *h_Vx_e_AC_sector1_1e_cut =
            new TH1D("Vx_e_AC_sector1_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector1);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_AC_sector1_1e_cut);
        TH1D *h_Vy_e_BC_sector1_1e_cut =
            new TH1D("Vy_e_BC_sector1_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector1);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_BC_sector1_1e_cut);
        TH1D *h_Vy_e_AC_sector1_1e_cut =
            new TH1D("Vy_e_AC_sector1_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector1);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_AC_sector1_1e_cut);

        TH2D *h_dc_electron_hit_map_BC_sector1_1e_cut[4];  // 3 regions
        TH2D *h_dc_electron_hit_map_AC_sector1_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_electron_hit_map_BC_sector1_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_BC_sector1_%d", i), Form("e^{-} DC hitmap in region %d (before e^{-} cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_BC_sector1_1e_cut[i]);
            h_dc_electron_hit_map_AC_sector1_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_AC_sector1_%d", i), Form("e^{-} DC hitmap in region %d (after e^{-} cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_AC_sector1_1e_cut[i]);
        }

        TH1D *h_nphe_BC_sector1_1e_cut =
            new TH1D("nphe_BC_sector1_1e_cut", "Number of photo-electrons in HTCC in (e,e') (before e^{-} cuts, sector1);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_BC_sector1_1e_cut);
        TH1D *h_nphe_AC_sector1_1e_cut =
            new TH1D("nphe_AC_sector1_1e_cut", "Number of photo-electrons in HTCC in (e,e') (after e^{-} cuts, sector1);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_AC_sector1_1e_cut);

        TH2D *h_Edep_PCAL_VS_EC_BC_sector1_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_BC_sector1_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (before e^{-} cuts, sector1);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector1_1e_cut);
        TH2D *h_Edep_PCAL_VS_EC_AC_sector1_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_AC_sector1_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (after e^{-} cuts, sector1);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector1_1e_cut);

        TH2D *h_SF_VS_Edep_PCAL_BC_sector1_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_BC_sector1_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (before e^{-} cuts, sector1);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_BC_sector1_1e_cut);
        TH2D *h_SF_VS_Edep_PCAL_AC_sector1_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_AC_sector1_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (after e^{-} cuts, sector1);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_AC_sector1_1e_cut);

        TH2D *h_SF_VS_P_e_BC_sector1_1e_cut =
            new TH2D("SF_VS_P_e_BC_sector1_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (before e^{-} cuts, sector1);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_BC_sector1_1e_cut);
        TH2D *h_SF_VS_P_e_AC_sector1_1e_cut =
            new TH2D("SF_VS_P_e_AC_sector1_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (after e^{-} cuts, sector1);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_AC_sector1_1e_cut);

        TH2D *h_SF_VS_Lv_BC_sector1_1e_cut =
            new TH2D("SF_VS_Lv_BC_sector1_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (before e^{-} cuts, sector1);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_BC_sector1_1e_cut);
        TH2D *h_SF_VS_Lv_AC_sector1_1e_cut =
            new TH2D("SF_VS_Lv_AC_sector1_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (after e^{-} cuts, sector1);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_AC_sector1_1e_cut);

        TH2D *h_SF_VS_Lw_BC_sector1_1e_cut =
            new TH2D("SF_VS_Lw_BC_sector1_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (before e^{-} cuts, sector1);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_BC_sector1_1e_cut);
        TH2D *h_SF_VS_Lw_AC_sector1_1e_cut =
            new TH2D("SF_VS_Lw_AC_sector1_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (after e^{-} cuts, sector1);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_AC_sector1_1e_cut);

        TH2D *h_SF_VS_Lu_BC_sector1_1e_cut =
            new TH2D("SF_VS_Lu_BC_sector1_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (before e^{-} cuts, sector1);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_BC_sector1_1e_cut);
        TH2D *h_SF_VS_Lu_AC_sector1_1e_cut =
            new TH2D("SF_VS_Lu_AC_sector1_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (after e^{-} cuts, sector1);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_AC_sector1_1e_cut);

        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector1_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC_sector1", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (before e^{-} cuts, sector1);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector1_1e_cut);
        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector1_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector1", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (after e^{-} cuts, sector1);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector1_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 2
        TH1D *h_Vz_e_BC_sector2_1e_cut =
            new TH1D("Vz_e_BC_sector2_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector2);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_BC_sector2_1e_cut);
        TH1D *h_Vz_e_AC_sector2_1e_cut =
            new TH1D("Vz_e_AC_sector2_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector2);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_AC_sector2_1e_cut);

        TH1D *h_Vz_e_BC_zoomin_sector2_1e_cut, *h_Vz_e_AC_zoomin_sector2_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_e_BC_zoomin_sector2_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector2_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector2);V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector2_1e_cut);
            h_Vz_e_AC_zoomin_sector2_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector2_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector2);V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector2_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_e_BC_zoomin_sector2_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector2_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector2);V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector2_1e_cut);
            h_Vz_e_AC_zoomin_sector2_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector2_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector2);V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector2_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector2_1e_cut =
            new TH1D("Vx_e_BC_sector2_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector2);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_BC_sector2_1e_cut);
        TH1D *h_Vx_e_AC_sector2_1e_cut =
            new TH1D("Vx_e_AC_sector2_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector2);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_AC_sector2_1e_cut);
        TH1D *h_Vy_e_BC_sector2_1e_cut =
            new TH1D("Vy_e_BC_sector2_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector2);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_BC_sector2_1e_cut);
        TH1D *h_Vy_e_AC_sector2_1e_cut =
            new TH1D("Vy_e_AC_sector2_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector2);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_AC_sector2_1e_cut);

        TH2D *h_dc_electron_hit_map_BC_sector2_1e_cut[4];  // 3 regions
        TH2D *h_dc_electron_hit_map_AC_sector2_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_electron_hit_map_BC_sector2_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_BC_sector2_%d", i), Form("e^{-} DC hitmap in region %d (before e^{-} cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_BC_sector2_1e_cut[i]);
            h_dc_electron_hit_map_AC_sector2_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_AC_sector2_%d", i), Form("e^{-} DC hitmap in region %d (after e^{-} cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_AC_sector2_1e_cut[i]);
        }

        TH1D *h_nphe_BC_sector2_1e_cut =
            new TH1D("nphe_BC_sector2_1e_cut", "Number of photo-electrons in HTCC in (e,e') (before e^{-} cuts, sector2);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_BC_sector2_1e_cut);
        TH1D *h_nphe_AC_sector2_1e_cut =
            new TH1D("nphe_AC_sector2_1e_cut", "Number of photo-electrons in HTCC in (e,e') (after e^{-} cuts, sector2);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_AC_sector2_1e_cut);

        TH2D *h_Edep_PCAL_VS_EC_BC_sector2_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_BC_sector2_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (before e^{-} cuts, sector2);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector2_1e_cut);
        TH2D *h_Edep_PCAL_VS_EC_AC_sector2_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_AC_sector2_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (after e^{-} cuts, sector2);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector2_1e_cut);

        TH2D *h_SF_VS_Edep_PCAL_BC_sector2_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_BC_sector2_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (before e^{-} cuts, sector2);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_BC_sector2_1e_cut);
        TH2D *h_SF_VS_Edep_PCAL_AC_sector2_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_AC_sector2_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (after e^{-} cuts, sector2);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_AC_sector2_1e_cut);

        TH2D *h_SF_VS_P_e_BC_sector2_1e_cut =
            new TH2D("SF_VS_P_e_BC_sector2_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (before e^{-} cuts, sector2);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_BC_sector2_1e_cut);
        TH2D *h_SF_VS_P_e_AC_sector2_1e_cut =
            new TH2D("SF_VS_P_e_AC_sector2_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (after e^{-} cuts, sector2);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_AC_sector2_1e_cut);

        TH2D *h_SF_VS_Lv_BC_sector2_1e_cut =
            new TH2D("SF_VS_Lv_BC_sector2_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (before e^{-} cuts, sector2);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_BC_sector2_1e_cut);
        TH2D *h_SF_VS_Lv_AC_sector2_1e_cut =
            new TH2D("SF_VS_Lv_AC_sector2_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (after e^{-} cuts, sector2);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_AC_sector2_1e_cut);

        TH2D *h_SF_VS_Lw_BC_sector2_1e_cut =
            new TH2D("SF_VS_Lw_BC_sector2_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (before e^{-} cuts, sector2);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_BC_sector2_1e_cut);
        TH2D *h_SF_VS_Lw_AC_sector2_1e_cut =
            new TH2D("SF_VS_Lw_AC_sector2_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (after e^{-} cuts, sector2);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_AC_sector2_1e_cut);

        TH2D *h_SF_VS_Lu_BC_sector2_1e_cut =
            new TH2D("SF_VS_Lu_BC_sector2_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (before e^{-} cuts, sector2);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_BC_sector2_1e_cut);
        TH2D *h_SF_VS_Lu_AC_sector2_1e_cut =
            new TH2D("SF_VS_Lu_AC_sector2_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (after e^{-} cuts, sector2);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_AC_sector2_1e_cut);

        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector2_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_B_sector2", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (before e^{-} cuts, sector2);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector2_1e_cut);
        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector2_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector2", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (after e^{-} cuts, sector2);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector2_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 3
        TH1D *h_Vz_e_BC_sector3_1e_cut =
            new TH1D("Vz_e_BC_sector3_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector3);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_BC_sector3_1e_cut);
        TH1D *h_Vz_e_AC_sector3_1e_cut =
            new TH1D("Vz_e_AC_sector3_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector3);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_AC_sector3_1e_cut);

        TH1D *h_Vz_e_BC_zoomin_sector3_1e_cut, *h_Vz_e_AC_zoomin_sector3_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_e_BC_zoomin_sector3_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector3_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector3);V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector3_1e_cut);
            h_Vz_e_AC_zoomin_sector3_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector3_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector3);V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector3_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_e_BC_zoomin_sector3_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector3_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector3);V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector3_1e_cut);
            h_Vz_e_AC_zoomin_sector3_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector3_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector3);V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector3_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector3_1e_cut =
            new TH1D("Vx_e_BC_sector3_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector3);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_BC_sector3_1e_cut);
        TH1D *h_Vx_e_AC_sector3_1e_cut =
            new TH1D("Vx_e_AC_sector3_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector3);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_AC_sector3_1e_cut);
        TH1D *h_Vy_e_BC_sector3_1e_cut =
            new TH1D("Vy_e_BC_sector3_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector3);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_BC_sector3_1e_cut);
        TH1D *h_Vy_e_AC_sector3_1e_cut =
            new TH1D("Vy_e_AC_sector3_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector3);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_AC_sector3_1e_cut);

        TH2D *h_dc_electron_hit_map_BC_sector3_1e_cut[4];  // 3 regions
        TH2D *h_dc_electron_hit_map_AC_sector3_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_electron_hit_map_BC_sector3_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_BC_sector3_%d", i), Form("e^{-} DC hitmap in region %d (before e^{-} cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_BC_sector3_1e_cut[i]);
            h_dc_electron_hit_map_AC_sector3_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_AC_sector3_%d", i), Form("e^{-} DC hitmap in region %d (after e^{-} cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_AC_sector3_1e_cut[i]);
        }

        TH1D *h_nphe_BC_sector3_1e_cut =
            new TH1D("nphe_BC_sector3_1e_cut", "Number of photo-electrons in HTCC in (e,e') (before e^{-} cuts, sector3);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_BC_sector3_1e_cut);
        TH1D *h_nphe_AC_sector3_1e_cut =
            new TH1D("nphe_AC_sector3_1e_cut", "Number of photo-electrons in HTCC in (e,e') (after e^{-} cuts, sector3);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_AC_sector3_1e_cut);

        TH2D *h_Edep_PCAL_VS_EC_BC_sector3_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_BC_sector3_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (before e^{-} cuts, sector3);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector3_1e_cut);
        TH2D *h_Edep_PCAL_VS_EC_AC_sector3_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_AC_sector3_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (after e^{-} cuts, sector3);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector3_1e_cut);

        TH2D *h_SF_VS_Edep_PCAL_BC_sector3_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_BC_sector3_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (before e^{-} cuts, sector3);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_BC_sector3_1e_cut);
        TH2D *h_SF_VS_Edep_PCAL_AC_sector3_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_AC_sector3_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (after e^{-} cuts, sector3);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_AC_sector3_1e_cut);

        TH2D *h_SF_VS_P_e_BC_sector3_1e_cut =
            new TH2D("SF_VS_P_e_BC_sector3_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (before e^{-} cuts, sector3);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_BC_sector3_1e_cut);
        TH2D *h_SF_VS_P_e_AC_sector3_1e_cut =
            new TH2D("SF_VS_P_e_AC_sector3_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (after e^{-} cuts, sector3);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_AC_sector3_1e_cut);

        TH2D *h_SF_VS_Lv_BC_sector3_1e_cut =
            new TH2D("SF_VS_Lv_BC_sector3_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (before e^{-} cuts, sector3);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_BC_sector3_1e_cut);
        TH2D *h_SF_VS_Lv_AC_sector3_1e_cut =
            new TH2D("SF_VS_Lv_AC_sector3_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (after e^{-} cuts, sector3);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_AC_sector3_1e_cut);

        TH2D *h_SF_VS_Lw_BC_sector3_1e_cut =
            new TH2D("SF_VS_Lw_BC_sector3_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (before e^{-} cuts, sector3);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_BC_sector3_1e_cut);
        TH2D *h_SF_VS_Lw_AC_sector3_1e_cut =
            new TH2D("SF_VS_Lw_AC_sector3_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (after e^{-} cuts, sector3);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_AC_sector3_1e_cut);

        TH2D *h_SF_VS_Lu_BC_sector3_1e_cut =
            new TH2D("SF_VS_Lu_BC_sector3_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (before e^{-} cuts, sector3);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_BC_sector3_1e_cut);
        TH2D *h_SF_VS_Lu_AC_sector3_1e_cut =
            new TH2D("SF_VS_Lu_AC_sector3_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (after e^{-} cuts, sector3);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_AC_sector3_1e_cut);

        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector3_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC_sector3", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (before e^{-} cuts, sector3);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector3_1e_cut);
        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector3_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector3", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (after e^{-} cuts, sector3);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector3_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 4
        TH1D *h_Vz_e_BC_sector4_1e_cut =
            new TH1D("Vz_e_BC_sector4_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector4);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_BC_sector4_1e_cut);
        TH1D *h_Vz_e_AC_sector4_1e_cut =
            new TH1D("Vz_e_AC_sector4_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector4);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_AC_sector4_1e_cut);

        TH1D *h_Vz_e_BC_zoomin_sector4_1e_cut, *h_Vz_e_AC_zoomin_sector4_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_e_BC_zoomin_sector4_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector4_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector4);V_{z}}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector4_1e_cut);
            h_Vz_e_AC_zoomin_sector4_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector4_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector4);V_{z}}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector4_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_e_BC_zoomin_sector4_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector4_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector4);V_{z}}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector4_1e_cut);
            h_Vz_e_AC_zoomin_sector4_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector4_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector4);V_{z}}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector4_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector4_1e_cut =
            new TH1D("Vx_e_BC_sector4_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector4);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_BC_sector4_1e_cut);
        TH1D *h_Vx_e_AC_sector4_1e_cut =
            new TH1D("Vx_e_AC_sector4_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector4);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_AC_sector4_1e_cut);
        TH1D *h_Vy_e_BC_sector4_1e_cut =
            new TH1D("Vy_e_BC_sector4_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector4);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_BC_sector4_1e_cut);
        TH1D *h_Vy_e_AC_sector4_1e_cut =
            new TH1D("Vy_e_AC_sector4_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector4);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_AC_sector4_1e_cut);

        TH2D *h_dc_electron_hit_map_BC_sector4_1e_cut[4];  // 3 regions
        TH2D *h_dc_electron_hit_map_AC_sector4_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_electron_hit_map_BC_sector4_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_BC_sector4_%d", i), Form("e^{-} DC hitmap in region %d (before e^{-} cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_BC_sector4_1e_cut[i]);
            h_dc_electron_hit_map_AC_sector4_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_AC_sector4_%d", i), Form("e^{-} DC hitmap in region %d (after e^{-} cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_AC_sector4_1e_cut[i]);
        }

        TH1D *h_nphe_BC_sector4_1e_cut =
            new TH1D("nphe_BC_sector4_1e_cut", "Number of photo-electrons in HTCC in (e,e') (before e^{-} cuts, sector4);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_BC_sector4_1e_cut);
        TH1D *h_nphe_AC_sector4_1e_cut =
            new TH1D("nphe_AC_sector4_1e_cut", "Number of photo-electrons in HTCC in (e,e') (after e^{-} cuts, sector4);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_AC_sector4_1e_cut);

        TH2D *h_Edep_PCAL_VS_EC_BC_sector4_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_BC_sector4_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (before e^{-} cuts, sector4);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector4_1e_cut);
        TH2D *h_Edep_PCAL_VS_EC_AC_sector4_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_AC_sector4_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (after e^{-} cuts, sector4);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector4_1e_cut);

        TH2D *h_SF_VS_Edep_PCAL_BC_sector4_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_BC_sector4_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (before e^{-} cuts, sector4);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_BC_sector4_1e_cut);
        TH2D *h_SF_VS_Edep_PCAL_AC_sector4_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_AC_sector4_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (after e^{-} cuts, sector4);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_AC_sector4_1e_cut);

        TH2D *h_SF_VS_P_e_BC_sector4_1e_cut =
            new TH2D("SF_VS_P_e_BC_sector4_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (before e^{-} cuts, sector4);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_BC_sector4_1e_cut);
        TH2D *h_SF_VS_P_e_AC_sector4_1e_cut =
            new TH2D("SF_VS_P_e_AC_sector4_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (after e^{-} cuts, sector4);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_AC_sector4_1e_cut);

        TH2D *h_SF_VS_Lv_BC_sector4_1e_cut =
            new TH2D("SF_VS_Lv_BC_sector4_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (before e^{-} cuts, sector4);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_BC_sector4_1e_cut);
        TH2D *h_SF_VS_Lv_AC_sector4_1e_cut =
            new TH2D("SF_VS_Lv_AC_sector4_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (after e^{-} cuts, sector4);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_AC_sector4_1e_cut);

        TH2D *h_SF_VS_Lw_BC_sector4_1e_cut =
            new TH2D("SF_VS_Lw_BC_sector4_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (before e^{-} cuts, sector4);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_BC_sector4_1e_cut);
        TH2D *h_SF_VS_Lw_AC_sector4_1e_cut =
            new TH2D("SF_VS_Lw_AC_sector4_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (after e^{-} cuts, sector4);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_AC_sector4_1e_cut);

        TH2D *h_SF_VS_Lu_BC_sector4_1e_cut =
            new TH2D("SF_VS_Lu_BC_sector4_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (before e^{-} cuts, sector4);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_BC_sector4_1e_cut);
        TH2D *h_SF_VS_Lu_AC_sector4_1e_cut =
            new TH2D("SF_VS_Lu_AC_sector4_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (after e^{-} cuts, sector4);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_AC_sector4_1e_cut);

        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector4_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC_sector4", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (before e^{-} cuts, sector4);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector4_1e_cut);
        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector4_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector4", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (after e^{-} cuts, sector4);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector4_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 5
        TH1D *h_Vz_e_BC_sector5_1e_cut =
            new TH1D("Vz_e_BC_sector5_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector5);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_BC_sector5_1e_cut);
        TH1D *h_Vz_e_AC_sector5_1e_cut =
            new TH1D("Vz_e_AC_sector5_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector5);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_AC_sector5_1e_cut);

        TH1D *h_Vz_e_BC_zoomin_sector5_1e_cut, *h_Vz_e_AC_zoomin_sector5_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_e_BC_zoomin_sector5_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector5_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector5);V_{z}}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector5_1e_cut);
            h_Vz_e_AC_zoomin_sector5_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector5_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector5);V_{z}}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector5_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_e_BC_zoomin_sector5_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector5_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector5);V_{z}}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector5_1e_cut);
            h_Vz_e_AC_zoomin_sector5_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector5_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector5);V_{z}}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector5_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector5_1e_cut =
            new TH1D("Vx_e_BC_sector5_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector5);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_BC_sector5_1e_cut);
        TH1D *h_Vx_e_AC_sector5_1e_cut =
            new TH1D("Vx_e_AC_sector5_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector5);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_AC_sector5_1e_cut);
        TH1D *h_Vy_e_BC_sector5_1e_cut =
            new TH1D("Vy_e_BC_sector5_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector5);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_BC_sector5_1e_cut);
        TH1D *h_Vy_e_AC_sector5_1e_cut =
            new TH1D("Vy_e_AC_sector5_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector5);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_AC_sector5_1e_cut);

        TH2D *h_dc_electron_hit_map_BC_sector5_1e_cut[4];  // 3 regions
        TH2D *h_dc_electron_hit_map_AC_sector5_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_electron_hit_map_BC_sector5_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_BC_sector5_%d", i), Form("e^{-} DC hitmap in region %d (before e^{-} cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_BC_sector5_1e_cut[i]);
            h_dc_electron_hit_map_AC_sector5_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_AC_sector5_%d", i), Form("e^{-} DC hitmap in region %d (after e^{-} cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_AC_sector5_1e_cut[i]);
        }

        TH1D *h_nphe_BC_sector5_1e_cut =
            new TH1D("nphe_BC_sector5_1e_cut", "Number of photo-electrons in HTCC in (e,e') (before e^{-} cuts, sector5);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_BC_sector5_1e_cut);
        TH1D *h_nphe_AC_sector5_1e_cut =
            new TH1D("nphe_AC_sector5_1e_cut", "Number of photo-electrons in HTCC in (e,e') (after e^{-} cuts, sector5);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_AC_sector5_1e_cut);

        TH2D *h_Edep_PCAL_VS_EC_BC_sector5_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_BC_sector5_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (before e^{-} cuts, sector5);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector5_1e_cut);
        TH2D *h_Edep_PCAL_VS_EC_AC_sector5_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_AC_sector5_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (after e^{-} cuts, sector5);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector5_1e_cut);

        TH2D *h_SF_VS_Edep_PCAL_BC_sector5_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_BC_sector5_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (before e^{-} cuts, sector5);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_BC_sector5_1e_cut);
        TH2D *h_SF_VS_Edep_PCAL_AC_sector5_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_AC_sector5_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (after e^{-} cuts, sector5);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_AC_sector5_1e_cut);

        TH2D *h_SF_VS_P_e_BC_sector5_1e_cut =
            new TH2D("SF_VS_P_e_BC_sector5_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (before e^{-} cuts, sector5);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_BC_sector5_1e_cut);
        TH2D *h_SF_VS_P_e_AC_sector5_1e_cut =
            new TH2D("SF_VS_P_e_AC_sector5_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (after e^{-} cuts, sector5);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_AC_sector5_1e_cut);

        TH2D *h_SF_VS_Lv_BC_sector5_1e_cut =
            new TH2D("SF_VS_Lv_BC_sector5_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (before e^{-} cuts, sector5);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_BC_sector5_1e_cut);
        TH2D *h_SF_VS_Lv_AC_sector5_1e_cut =
            new TH2D("SF_VS_Lv_AC_sector5_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (after e^{-} cuts, sector5);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_AC_sector5_1e_cut);

        TH2D *h_SF_VS_Lw_BC_sector5_1e_cut =
            new TH2D("SF_VS_Lw_BC_sector5_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (before e^{-} cuts, sector5);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_BC_sector5_1e_cut);
        TH2D *h_SF_VS_Lw_AC_sector5_1e_cut =
            new TH2D("SF_VS_Lw_AC_sector5_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (after e^{-} cuts, sector5);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_AC_sector5_1e_cut);

        TH2D *h_SF_VS_Lu_BC_sector5_1e_cut =
            new TH2D("SF_VS_Lu_BC_sector5_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (before e^{-} cuts, sector5);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_BC_sector5_1e_cut);
        TH2D *h_SF_VS_Lu_AC_sector5_1e_cut =
            new TH2D("SF_VS_Lu_AC_sector5_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (after e^{-} cuts, sector5);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_AC_sector5_1e_cut);

        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector5_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC_sector5", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (before e^{-} cuts, sector5);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector5_1e_cut);
        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector5_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector5", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (after e^{-} cuts, sector5);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector5_1e_cut);

#pragma endregion

#pragma region electron histograms - sector 6
        TH1D *h_Vz_e_BC_sector6_1e_cut =
            new TH1D("Vz_e_BC_sector6_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector6);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_BC_sector6_1e_cut);
        TH1D *h_Vz_e_AC_sector6_1e_cut =
            new TH1D("Vz_e_AC_sector6_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector6);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_AC_sector6_1e_cut);

        TH1D *h_Vz_e_BC_zoomin_sector6_1e_cut, *h_Vz_e_AC_zoomin_sector6_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_e_BC_zoomin_sector6_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector6_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector6);V_{z}}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector6_1e_cut);
            h_Vz_e_AC_zoomin_sector6_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector6_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector6);V_{z}}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector6_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_e_BC_zoomin_sector6_1e_cut =
                new TH1D("Vz_e_BC_zoomin_sector6_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts, sector6);V_{z}}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_BC_zoomin_sector6_1e_cut);
            h_Vz_e_AC_zoomin_sector6_1e_cut =
                new TH1D("Vz_e_AC_zoomin_sector6_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector6);V_{z}}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_e_AC_zoomin_sector6_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector6_1e_cut =
            new TH1D("Vx_e_BC_sector6_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector6);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_BC_sector6_1e_cut);
        TH1D *h_Vx_e_AC_sector6_1e_cut =
            new TH1D("Vx_e_AC_sector6_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector6);V_{x}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_e_AC_sector6_1e_cut);
        TH1D *h_Vy_e_BC_sector6_1e_cut =
            new TH1D("Vy_e_BC_sector6_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector6);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_BC_sector6_1e_cut);
        TH1D *h_Vy_e_AC_sector6_1e_cut =
            new TH1D("Vy_e_AC_sector6_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector6);V_{y}^{e} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_e_AC_sector6_1e_cut);

        TH2D *h_dc_electron_hit_map_BC_sector6_1e_cut[4];  // 3 regions
        TH2D *h_dc_electron_hit_map_AC_sector6_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_electron_hit_map_BC_sector6_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_BC_sector6_%d", i), Form("e^{-} DC hitmap in region %d (before e^{-} cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_BC_sector6_1e_cut[i]);
            h_dc_electron_hit_map_AC_sector6_1e_cut[i] =
                new TH2D(Form("dc_electron_hit_map_AC_sector6_%d", i), Form("e^{-} DC hitmap in region %d (after e^{-} cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_electron_hit_map_AC_sector6_1e_cut[i]);
        }

        TH1D *h_nphe_BC_sector6_1e_cut =
            new TH1D("nphe_BC_sector6_1e_cut", "Number of photo-electrons in HTCC in (e,e') (before e^{-} cuts, sector6);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_BC_sector6_1e_cut);
        TH1D *h_nphe_AC_sector6_1e_cut =
            new TH1D("nphe_AC_sector6_1e_cut", "Number of photo-electrons in HTCC in (e,e') (after e^{-} cuts, sector6);Number of photo-electrons;Counts", 20, 0, 20);
        HistoList.push_back(h_nphe_AC_sector6_1e_cut);

        TH2D *h_Edep_PCAL_VS_EC_BC_sector6_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_BC_sector6_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (before e^{-} cuts, sector6);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_BC_sector6_1e_cut);
        TH2D *h_Edep_PCAL_VS_EC_AC_sector6_1e_cut = new TH2D(
            "Edep_PCAL_VS_EC_AC_sector6_1e_cut",
            "E_{dep}^{PCAL} vs. E_{dep}^{EC} in (e,e') (after e^{-} cuts, sector6);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2, 100, 0, 0.3);
        HistoList.push_back(h_Edep_PCAL_VS_EC_AC_sector6_1e_cut);

        TH2D *h_SF_VS_Edep_PCAL_BC_sector6_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_BC_sector6_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (before e^{-} cuts, sector6);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_BC_sector6_1e_cut);
        TH2D *h_SF_VS_Edep_PCAL_AC_sector6_1e_cut =
            new TH2D("SF_VS_Edep_PCAL_AC_sector6_1e_cut",
                     "Electron sampling fraction vs. E_{dep}^{PCAL} in (e,e') (after e^{-} cuts, sector6);E_{dep}^{PCAL} [GeV];Electron sampling fraction", 100, 0, 1.25, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Edep_PCAL_AC_sector6_1e_cut);

        TH2D *h_SF_VS_P_e_BC_sector6_1e_cut =
            new TH2D("SF_VS_P_e_BC_sector6_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (before e^{-} cuts, sector6);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_BC_sector6_1e_cut);
        TH2D *h_SF_VS_P_e_AC_sector6_1e_cut =
            new TH2D("SF_VS_P_e_AC_sector6_1e_cut", "Electron sampling fraction vs. P_{e} in (e,e') (after e^{-} cuts, sector6);P_{e} [GeV/c];Electron sampling fraction", 100, 0,
                     Ebeam * 1.1, 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_P_e_AC_sector6_1e_cut);

        TH2D *h_SF_VS_Lv_BC_sector6_1e_cut =
            new TH2D("SF_VS_Lv_BC_sector6_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (before e^{-} cuts, sector6);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_BC_sector6_1e_cut);
        TH2D *h_SF_VS_Lv_AC_sector6_1e_cut =
            new TH2D("SF_VS_Lv_AC_sector6_1e_cut", "Electron SF vs. PCAL V coor. in (e,e') (after e^{-} cuts, sector6);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lv_AC_sector6_1e_cut);

        TH2D *h_SF_VS_Lw_BC_sector6_1e_cut =
            new TH2D("SF_VS_Lw_BC_sector6_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (before e^{-} cuts, sector6);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_BC_sector6_1e_cut);
        TH2D *h_SF_VS_Lw_AC_sector6_1e_cut =
            new TH2D("SF_VS_Lw_AC_sector6_1e_cut", "Electron SF vs. PCAL W coor. in (e,e') (after e^{-} cuts, sector6);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lw_AC_sector6_1e_cut);

        TH2D *h_SF_VS_Lu_BC_sector6_1e_cut =
            new TH2D("SF_VS_Lu_BC_sector6_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (before e^{-} cuts, sector6);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_BC_sector6_1e_cut);
        TH2D *h_SF_VS_Lu_AC_sector6_1e_cut =
            new TH2D("SF_VS_Lu_AC_sector6_1e_cut", "Electron SF vs. PCAL U coor. in (e,e') (after e^{-} cuts, sector6);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.125, 0.325);
        HistoList.push_back(h_SF_VS_Lu_AC_sector6_1e_cut);

        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector6_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC_sector6", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (before e^{-} cuts, sector6);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector6_1e_cut);
        TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector6_1e_cut =
            new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC_sector6", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in (e,e') (after e^{-} cuts, sector6);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}",
                     100, 0, 0.3, 100, 0, 0.35);
        HistoList.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector6_1e_cut);

#pragma endregion

#pragma endregion

#pragma region FD piplus histograms

#pragma region FD piplus histograms - all sectors
        TH1D *h_Vz_pipFD_BC_1e_cut =
            new TH1D("Vz_pipFD_BC_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_BC_1e_cut);
        TH1D *h_Vz_pipFD_AC_1e_cut =
            new TH1D("Vz_pipFD_AC_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_AC_1e_cut);

        TH1D *h_Vz_pipFD_BC_zoomin_1e_cut, *h_Vz_pipFD_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pipFD_BC_zoomin_1e_cut = new TH1D(
                "Vz_pipFD_BC_zoomin_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_1e_cut);
            h_Vz_pipFD_AC_zoomin_1e_cut = new TH1D("Vz_pipFD_AC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pipFD_BC_zoomin_1e_cut = new TH1D("Vz_pipFD_BC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_1e_cut);
            h_Vz_pipFD_AC_zoomin_1e_cut = new TH1D("Vz_pipFD_AC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_1e_cut =
            new TH1D("Vx_pipFD_BC_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_BC_1e_cut);
        TH1D *h_Vx_pipFD_AC_1e_cut =
            new TH1D("Vx_pipFD_AC_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_AC_1e_cut);
        TH1D *h_Vy_pipFD_BC_1e_cut =
            new TH1D("Vy_pipFD_BC_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_BC_1e_cut);
        TH1D *h_Vy_pipFD_AC_1e_cut =
            new TH1D("Vy_pipFD_AC_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_AC_1e_cut);

        TH1D *h_dVz_pipFD_BC_1e_cut =
            new TH1D("dVz_pipFD_BC_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_BC_1e_cut);
        TH1D *h_dVz_pipFD_AC_1e_cut =
            new TH1D("dVz_pipFD_AC_1e_cut", ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                     75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_AC_1e_cut);

        TH1D *h_dVz_pipFD_BC_zoomin_1e_cut, *h_dVz_pipFD_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pipFD_BC_zoomin_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_1e_cut);
            h_dVz_pipFD_AC_zoomin_1e_cut = new TH1D(
                "dVz_pipFD_AC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pipFD_BC_zoomin_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_1e_cut);
            h_dVz_pipFD_AC_zoomin_1e_cut = new TH1D(
                "dVz_pipFD_AC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_1e_cut);
        }

        TH2D *h_dc_pipFD_hit_map_BC_1e_cut[4];  // 3 regions
        TH2D *h_dc_pipFD_hit_map_AC_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pipFD_hit_map_BC_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_BC_%d", i), Form("#pi^{+} DC hitmap in region %d (before #pi^{+} cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_BC_1e_cut[i]);
            h_dc_pipFD_hit_map_AC_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_AC_%d", i), Form("#pi^{+} DC hitmap in region %d (after #pi^{+} cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_AC_1e_cut[i]);
        }

        TH1D *h_Chi2_pipFD_BC_1e_cut = new TH1D(
            "Chi2_pipFD_BC_1e_cut", ("#chi^{2}_{#pi^{+}FD} of FD #pi^{+} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts);#chi^{2}_{#pi^{+}FD};Counts").c_str(), 75, -20, 20);
        HistoList.push_back(h_Chi2_pipFD_BC_1e_cut);
        TH1D *h_Chi2_pipFD_AC_1e_cut =
            new TH1D("Chi2_pipFD_AC_1e_cut", ("#chi^{2}_{#pi^{+}FD} of FD #pi^{+} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts);#chi^{2}_{#pi^{+}FD};Counts").c_str(), 75, -20, 20);
        HistoList.push_back(h_Chi2_pipFD_AC_1e_cut);
#pragma endregion

#pragma region FD piplus histograms - sector 1
        TH1D *h_Vz_pipFD_BC_sector1_1e_cut =
            new TH1D("Vz_pipFD_BC_sector1_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector1);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_BC_sector1_1e_cut);
        TH1D *h_Vz_pipFD_AC_sector1_1e_cut =
            new TH1D("Vz_pipFD_AC_sector1_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector1);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_AC_sector1_1e_cut);

        TH1D *h_Vz_pipFD_BC_zoomin_sector1_1e_cut, *h_Vz_pipFD_AC_zoomin_sector1_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pipFD_BC_zoomin_sector1_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector1_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector1);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector1_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector1_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector1_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector1);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector1_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pipFD_BC_zoomin_sector1_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector1_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector1);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector1_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector1_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector1_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector1);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector1_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_sector1_1e_cut =
            new TH1D("Vx_pipFD_BC_sector1_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector1);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_BC_sector1_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector1_1e_cut =
            new TH1D("Vx_pipFD_AC_sector1_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector1);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_AC_sector1_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector1_1e_cut =
            new TH1D("Vy_pipFD_BC_sector1_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector1);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_BC_sector1_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector1_1e_cut =
            new TH1D("Vy_pipFD_AC_sector1_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector1);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_AC_sector1_1e_cut);

        TH1D *h_dVz_pipFD_BC_sector1_1e_cut =
            new TH1D("dVz_pipFD_BC_sector1_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector1);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_BC_sector1_1e_cut);
        TH1D *h_dVz_pipFD_AC_sector1_1e_cut =
            new TH1D("dVz_pipFD_AC_sector1_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector1);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_AC_sector1_1e_cut);

        TH1D *h_dVz_pipFD_BC_zoomin_sector1_1e_cut, *h_dVz_pipFD_AC_zoomin_sector1_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pipFD_BC_zoomin_sector1_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector1_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector1);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector1_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector1_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector1_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector1);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector1_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pipFD_BC_zoomin_sector1_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector1_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector1);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector1_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector1_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector1_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector1);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector1_1e_cut);
        }

        TH2D *h_dc_pipFD_hit_map_BC_sector1_1e_cut[4];  // 3 regions
        TH2D *h_dc_pipFD_hit_map_AC_sector1_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pipFD_hit_map_BC_sector1_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_BC_sector1_%d", i), Form("#pi^{+} DC hitmap in region %d (before #pi^{+} cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_BC_sector1_1e_cut[i]);
            h_dc_pipFD_hit_map_AC_sector1_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_AC_sector1_%d", i), Form("#pi^{+} DC hitmap in region %d (after #pi^{+} cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_AC_sector1_1e_cut[i]);
        }

#pragma endregion

#pragma region FD piplus histograms - sector 2
        TH1D *h_Vz_pipFD_BC_sector2_1e_cut =
            new TH1D("Vz_pipFD_BC_sector2_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector2);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_BC_sector2_1e_cut);
        TH1D *h_Vz_pipFD_AC_sector2_1e_cut =
            new TH1D("Vz_pipFD_AC_sector2_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector2);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_AC_sector2_1e_cut);

        TH1D *h_Vz_pipFD_BC_zoomin_sector2_1e_cut, *h_Vz_pipFD_AC_zoomin_sector2_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pipFD_BC_zoomin_sector2_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector2_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector2);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector2_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector2_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector2_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector2);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector2_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pipFD_BC_zoomin_sector2_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector2_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector2);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector2_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector2_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector2_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector2);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector2_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_sector2_1e_cut =
            new TH1D("Vx_pipFD_BC_sector2_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector2);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_BC_sector2_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector2_1e_cut =
            new TH1D("Vx_pipFD_AC_sector2_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector2);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_AC_sector2_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector2_1e_cut =
            new TH1D("Vy_pipFD_BC_sector2_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector2);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_BC_sector2_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector2_1e_cut =
            new TH1D("Vy_pipFD_AC_sector2_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector2);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_AC_sector2_1e_cut);

        TH1D *h_dVz_pipFD_BC_sector2_1e_cut =
            new TH1D("dVz_pipFD_BC_sector2_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector2);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_BC_sector2_1e_cut);
        TH1D *h_dVz_pipFD_AC_sector2_1e_cut =
            new TH1D("dVz_pipFD_AC_sector2_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector2);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_AC_sector2_1e_cut);

        TH1D *h_dVz_pipFD_BC_zoomin_sector2_1e_cut, *h_dVz_pipFD_AC_zoomin_sector2_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pipFD_BC_zoomin_sector2_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector2_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector2);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector2_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector2_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector2_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector2);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector2_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pipFD_BC_zoomin_sector2_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector2_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector2);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector2_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector2_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector2_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector2);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector2_1e_cut);
        }

        TH2D *h_dc_pipFD_hit_map_BC_sector2_1e_cut[4];  // 3 regions
        TH2D *h_dc_pipFD_hit_map_AC_sector2_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pipFD_hit_map_BC_sector2_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_BC_sector2_%d", i), Form("#pi^{+} DC hitmap in region %d (before #pi^{+} cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_BC_sector2_1e_cut[i]);
            h_dc_pipFD_hit_map_AC_sector2_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_AC_sector2_%d", i), Form("#pi^{+} DC hitmap in region %d (after #pi^{+} cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_AC_sector2_1e_cut[i]);
        }

#pragma endregion

#pragma region FD piplus histograms - sector 3
        TH1D *h_Vz_pipFD_BC_sector3_1e_cut =
            new TH1D("Vz_pipFD_BC_sector3_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector3);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_BC_sector3_1e_cut);
        TH1D *h_Vz_pipFD_AC_sector3_1e_cut =
            new TH1D("Vz_pipFD_AC_sector3_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector3);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_AC_sector3_1e_cut);

        TH1D *h_Vz_pipFD_BC_zoomin_sector3_1e_cut, *h_Vz_pipFD_AC_zoomin_sector3_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pipFD_BC_zoomin_sector3_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector3_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector3);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector3_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector3_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector3_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector3);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector3_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pipFD_BC_zoomin_sector3_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector3_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector3);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector3_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector3_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector3_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector3);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector3_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_sector3_1e_cut =
            new TH1D("Vx_pipFD_BC_sector3_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector3);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_BC_sector3_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector3_1e_cut =
            new TH1D("Vx_pipFD_AC_sector3_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector3);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_AC_sector3_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector3_1e_cut =
            new TH1D("Vy_pipFD_BC_sector3_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector3);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_BC_sector3_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector3_1e_cut =
            new TH1D("Vy_pipFD_AC_sector3_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector3);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_AC_sector3_1e_cut);

        TH1D *h_dVz_pipFD_BC_sector3_1e_cut =
            new TH1D("dVz_pipFD_BC_sector3_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector3);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_BC_sector3_1e_cut);
        TH1D *h_dVz_pipFD_AC_sector3_1e_cut =
            new TH1D("dVz_pipFD_AC_sector3_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector3);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_AC_sector3_1e_cut);

        TH1D *h_dVz_pipFD_BC_zoomin_sector3_1e_cut, *h_dVz_pipFD_AC_zoomin_sector3_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pipFD_BC_zoomin_sector3_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector3_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector3);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector3_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector3_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector3_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector3);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector3_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pipFD_BC_zoomin_sector3_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector3_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector3);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector3_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector3_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector3_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector3);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector3_1e_cut);
        }

        TH2D *h_dc_pipFD_hit_map_BC_sector3_1e_cut[4];  // 3 regions
        TH2D *h_dc_pipFD_hit_map_AC_sector3_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pipFD_hit_map_BC_sector3_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_BC_sector3_%d", i), Form("#pi^{+} DC hitmap in region %d (before #pi^{+} cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_BC_sector3_1e_cut[i]);
            h_dc_pipFD_hit_map_AC_sector3_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_AC_sector3_%d", i), Form("#pi^{+} DC hitmap in region %d (after #pi^{+} cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_AC_sector3_1e_cut[i]);
        }

#pragma endregion

#pragma region FD piplus histograms - sector 4
        TH1D *h_Vz_pipFD_BC_sector4_1e_cut =
            new TH1D("Vz_pipFD_BC_sector4_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector4);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_BC_sector4_1e_cut);
        TH1D *h_Vz_pipFD_AC_sector4_1e_cut =
            new TH1D("Vz_pipFD_AC_sector4_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector4);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_AC_sector4_1e_cut);

        TH1D *h_Vz_pipFD_BC_zoomin_sector4_1e_cut, *h_Vz_pipFD_AC_zoomin_sector4_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pipFD_BC_zoomin_sector4_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector4_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector4);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector4_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector4_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector4_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector4);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector4_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pipFD_BC_zoomin_sector4_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector4_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector4);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector4_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector4_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector4_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector4);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector4_1e_cut);
        }

        TH1D *h_dVz_pipFD_BC_sector4_1e_cut =
            new TH1D("dVz_pipFD_BC_sector4_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector4);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_BC_sector4_1e_cut);
        TH1D *h_dVz_pipFD_AC_sector4_1e_cut =
            new TH1D("dVz_pipFD_AC_sector4_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector4);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_AC_sector4_1e_cut);

        TH1D *h_Vx_pipFD_BC_sector4_1e_cut =
            new TH1D("Vx_pipFD_BC_sector4_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector4);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_BC_sector4_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector4_1e_cut =
            new TH1D("Vx_pipFD_AC_sector4_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector4);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_AC_sector4_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector4_1e_cut =
            new TH1D("Vy_pipFD_BC_sector4_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector4);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_BC_sector4_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector4_1e_cut =
            new TH1D("Vy_pipFD_AC_sector4_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector4);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_AC_sector4_1e_cut);

        TH1D *h_dVz_pipFD_BC_zoomin_sector4_1e_cut, *h_dVz_pipFD_AC_zoomin_sector4_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pipFD_BC_zoomin_sector4_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector4_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector4);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector4_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector4_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector4_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector4);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector4_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pipFD_BC_zoomin_sector4_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector4_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector4);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector4_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector4_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector4_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector4);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector4_1e_cut);
        }

        TH2D *h_dc_pipFD_hit_map_BC_sector4_1e_cut[4];  // 3 regions
        TH2D *h_dc_pipFD_hit_map_AC_sector4_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pipFD_hit_map_BC_sector4_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_BC_sector4_%d", i), Form("#pi^{+} DC hitmap in region %d (before #pi^{+} cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_BC_sector4_1e_cut[i]);
            h_dc_pipFD_hit_map_AC_sector4_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_AC_sector4_%d", i), Form("#pi^{+} DC hitmap in region %d (after #pi^{+} cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_AC_sector4_1e_cut[i]);
        }

#pragma endregion

#pragma region FD piplus histograms - sector 5
        TH1D *h_Vz_pipFD_BC_sector5_1e_cut =
            new TH1D("Vz_pipFD_BC_sector5_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector5);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_BC_sector5_1e_cut);
        TH1D *h_Vz_pipFD_AC_sector5_1e_cut =
            new TH1D("Vz_pipFD_AC_sector5_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector5);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_AC_sector5_1e_cut);

        TH1D *h_Vz_pipFD_BC_zoomin_sector5_1e_cut, *h_Vz_pipFD_AC_zoomin_sector5_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pipFD_BC_zoomin_sector5_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector5_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector5);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector5_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector5_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector5_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector5);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector5_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pipFD_BC_zoomin_sector5_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector5_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector5);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector5_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector5_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector5_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector5);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector5_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_sector5_1e_cut =
            new TH1D("Vx_pipFD_BC_sector5_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector5);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_BC_sector5_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector5_1e_cut =
            new TH1D("Vx_pipFD_AC_sector5_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector5);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_AC_sector5_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector5_1e_cut =
            new TH1D("Vy_pipFD_BC_sector5_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector5);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_BC_sector5_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector5_1e_cut =
            new TH1D("Vy_pipFD_AC_sector5_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector5);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_AC_sector5_1e_cut);

        TH1D *h_dVz_pipFD_BC_sector5_1e_cut =
            new TH1D("dVz_pipFD_BC_sector5_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector5);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_BC_sector5_1e_cut);
        TH1D *h_dVz_pipFD_AC_sector5_1e_cut =
            new TH1D("dVz_pipFD_AC_sector5_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector5);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_AC_sector5_1e_cut);

        TH1D *h_dVz_pipFD_BC_zoomin_sector5_1e_cut, *h_dVz_pipFD_AC_zoomin_sector5_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pipFD_BC_zoomin_sector5_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector5_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector5);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector5_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector5_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector5_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector5);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector5_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pipFD_BC_zoomin_sector5_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector5_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector5);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector5_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector5_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector5_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector5);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector5_1e_cut);
        }

        TH2D *h_dc_pipFD_hit_map_BC_sector5_1e_cut[4];  // 3 regions
        TH2D *h_dc_pipFD_hit_map_AC_sector5_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pipFD_hit_map_BC_sector5_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_BC_sector5_%d", i), Form("#pi^{+} DC hitmap in region %d (before #pi^{+} cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_BC_sector5_1e_cut[i]);
            h_dc_pipFD_hit_map_AC_sector5_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_AC_sector5_%d", i), Form("#pi^{+} DC hitmap in region %d (after #pi^{+} cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_AC_sector5_1e_cut[i]);
        }

#pragma endregion

#pragma region FD piplus histograms - sector 6
        TH1D *h_Vz_pipFD_BC_sector6_1e_cut =
            new TH1D("Vz_pipFD_BC_sector6_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector6);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_BC_sector6_1e_cut);
        TH1D *h_Vz_pipFD_AC_sector6_1e_cut =
            new TH1D("Vz_pipFD_AC_sector6_1e_cut", ("V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector6);V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipFD_AC_sector6_1e_cut);

        TH1D *h_Vz_pipFD_BC_zoomin_sector6_1e_cut, *h_Vz_pipFD_AC_zoomin_sector6_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pipFD_BC_zoomin_sector6_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector6_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector6);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector6_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector6_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector6_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector6);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector6_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pipFD_BC_zoomin_sector6_1e_cut =
                new TH1D("Vz_pipFD_BC_zoomin_sector6_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector6);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_BC_zoomin_sector6_1e_cut);
            h_Vz_pipFD_AC_zoomin_sector6_1e_cut =
                new TH1D("Vz_pipFD_AC_zoomin_sector6_1e_cut",
                         ("V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector6);V_{z}}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipFD_AC_zoomin_sector6_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_sector6_1e_cut =
            new TH1D("Vx_pipFD_BC_sector6_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector6);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_BC_sector6_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector6_1e_cut =
            new TH1D("Vx_pipFD_AC_sector6_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector6);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pipFD_AC_sector6_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector6_1e_cut =
            new TH1D("Vy_pipFD_BC_sector6_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector6);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_BC_sector6_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector6_1e_cut =
            new TH1D("Vy_pipFD_AC_sector6_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector6);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pipFD_AC_sector6_1e_cut);

        TH1D *h_dVz_pipFD_BC_sector6_1e_cut =
            new TH1D("dVz_pipFD_BC_sector6_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector6);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_BC_sector6_1e_cut);
        TH1D *h_dVz_pipFD_AC_sector6_1e_cut =
            new TH1D("dVz_pipFD_AC_sector6_1e_cut",
                     ("dV_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector6);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipFD_AC_sector6_1e_cut);

        TH1D *h_dVz_pipFD_BC_zoomin_sector6_1e_cut, *h_dVz_pipFD_AC_zoomin_sector6_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pipFD_BC_zoomin_sector6_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector6_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector6);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector6_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector6_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector6_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector6);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector6_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pipFD_BC_zoomin_sector6_1e_cut = new TH1D(
                "dVz_pipFD_BC_zoomin_sector6_1e_cut",
                ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector6);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pipFD_BC_zoomin_sector6_1e_cut);
            h_dVz_pipFD_AC_zoomin_sector6_1e_cut =
                new TH1D("dVz_pipFD_AC_zoomin_sector6_1e_cut",
                         ("dV_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector6);dV^{#pi^{+}FD}_{z}=V^{e}_{z}-V^{#pi^{+}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pipFD_AC_zoomin_sector6_1e_cut);
        }

        TH2D *h_dc_pipFD_hit_map_BC_sector6_1e_cut[4];  // 3 regions
        TH2D *h_dc_pipFD_hit_map_AC_sector6_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pipFD_hit_map_BC_sector6_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_BC_sector6_%d", i), Form("#pi^{+} DC hitmap in region %d (before #pi^{+} cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_BC_sector6_1e_cut[i]);
            h_dc_pipFD_hit_map_AC_sector6_1e_cut[i] =
                new TH2D(Form("dc_pipFD_hit_map_AC_sector6_%d", i), Form("#pi^{+} DC hitmap in region %d (after #pi^{+} cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pipFD_hit_map_AC_sector6_1e_cut[i]);
        }

#pragma endregion

#pragma endregion

#pragma region FD piminus histograms

#pragma region FD piminus histograms - all sectors
        TH1D *h_Vz_pimFD_BC_1e_cut =
            new TH1D("Vz_pimFD_BC_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_BC_1e_cut);
        TH1D *h_Vz_pimFD_AC_1e_cut =
            new TH1D("Vz_pimFD_AC_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_AC_1e_cut);

        TH1D *h_Vz_pimFD_BC_zoomin_1e_cut, *h_Vz_pimFD_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pimFD_BC_zoomin_1e_cut = new TH1D(
                "Vz_pimFD_BC_zoomin_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_1e_cut);
            h_Vz_pimFD_AC_zoomin_1e_cut = new TH1D("Vz_pimFD_AC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pimFD_BC_zoomin_1e_cut = new TH1D("Vz_pimFD_BC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_1e_cut);
            h_Vz_pimFD_AC_zoomin_1e_cut = new TH1D("Vz_pimFD_AC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_1e_cut =
            new TH1D("Vx_pimFD_BC_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_BC_1e_cut);
        TH1D *h_Vx_pimFD_AC_1e_cut =
            new TH1D("Vx_pimFD_AC_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_AC_1e_cut);
        TH1D *h_Vy_pimFD_BC_1e_cut =
            new TH1D("Vy_pimFD_BC_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_BC_1e_cut);
        TH1D *h_Vy_pimFD_AC_1e_cut =
            new TH1D("Vy_pimFD_AC_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_AC_1e_cut);

        TH1D *h_dVz_pimFD_BC_1e_cut =
            new TH1D("dVz_pimFD_BC_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_BC_1e_cut);
        TH1D *h_dVz_pimFD_AC_1e_cut =
            new TH1D("dVz_pimFD_AC_1e_cut", ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                     75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_AC_1e_cut);

        TH1D *h_dVz_pimFD_BC_zoomin_1e_cut, *h_dVz_pimFD_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pimFD_BC_zoomin_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_1e_cut);
            h_dVz_pimFD_AC_zoomin_1e_cut = new TH1D(
                "dVz_pimFD_AC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pimFD_BC_zoomin_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_1e_cut);
            h_dVz_pimFD_AC_zoomin_1e_cut = new TH1D(
                "dVz_pimFD_AC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_1e_cut);
        }

        TH2D *h_dc_pimFD_hit_map_BC_1e_cut[4];  // 3 regions
        TH2D *h_dc_pimFD_hit_map_AC_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pimFD_hit_map_BC_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_BC_%d", i), Form("#pi^{-} DC hitmap in region %d (before #pi^{-} cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_BC_1e_cut[i]);
            h_dc_pimFD_hit_map_AC_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_AC_%d", i), Form("#pi^{-} DC hitmap in region %d (after #pi^{-} cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_AC_1e_cut[i]);
        }

        TH1D *h_Chi2_pimFD_BC_1e_cut = new TH1D(
            "Chi2_pimFD_BC_1e_cut", ("#chi^{2}_{#pi^{-}FD} of FD #pi^{-} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts);#chi^{2}_{#pi^{-}FD};Counts").c_str(), 75, -20, 20);
        HistoList.push_back(h_Chi2_pimFD_BC_1e_cut);
        TH1D *h_Chi2_pimFD_AC_1e_cut =
            new TH1D("Chi2_pimFD_AC_1e_cut", ("#chi^{2}_{#pi^{-}FD} of FD #pi^{-} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts);#chi^{2}_{#pi^{-}FD};Counts").c_str(), 75, -20, 20);
        HistoList.push_back(h_Chi2_pimFD_AC_1e_cut);
#pragma endregion

#pragma region FD piminus histograms - sector 1
        TH1D *h_Vz_pimFD_BC_sector1_1e_cut =
            new TH1D("Vz_pimFD_BC_sector1_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector1);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_BC_sector1_1e_cut);
        TH1D *h_Vz_pimFD_AC_sector1_1e_cut =
            new TH1D("Vz_pimFD_AC_sector1_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector1);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_AC_sector1_1e_cut);

        TH1D *h_Vz_pimFD_BC_zoomin_sector1_1e_cut, *h_Vz_pimFD_AC_zoomin_sector1_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pimFD_BC_zoomin_sector1_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector1_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector1);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector1_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector1_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector1_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector1);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector1_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pimFD_BC_zoomin_sector1_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector1_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector1);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector1_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector1_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector1_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector1);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector1_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector1_1e_cut =
            new TH1D("Vx_pimFD_BC_sector1_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector1);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_BC_sector1_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector1_1e_cut =
            new TH1D("Vx_pimFD_AC_sector1_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector1);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_AC_sector1_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector1_1e_cut =
            new TH1D("Vy_pimFD_BC_sector1_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector1);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_BC_sector1_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector1_1e_cut =
            new TH1D("Vy_pimFD_AC_sector1_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector1);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_AC_sector1_1e_cut);

        TH1D *h_dVz_pimFD_BC_sector1_1e_cut =
            new TH1D("dVz_pimFD_BC_sector1_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector1);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_BC_sector1_1e_cut);
        TH1D *h_dVz_pimFD_AC_sector1_1e_cut =
            new TH1D("dVz_pimFD_AC_sector1_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector1);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_AC_sector1_1e_cut);

        TH1D *h_dVz_pimFD_BC_zoomin_sector1_1e_cut, *h_dVz_pimFD_AC_zoomin_sector1_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pimFD_BC_zoomin_sector1_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector1_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector1);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector1_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector1_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector1_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector1);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector1_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pimFD_BC_zoomin_sector1_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector1_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector1);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector1_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector1_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector1_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector1);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector1_1e_cut);
        }

        TH2D *h_dc_pimFD_hit_map_BC_sector1_1e_cut[4];  // 3 regions
        TH2D *h_dc_pimFD_hit_map_AC_sector1_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pimFD_hit_map_BC_sector1_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_BC_sector1_%d", i), Form("#pi^{-} DC hitmap in region %d (before #pi^{-} cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_BC_sector1_1e_cut[i]);
            h_dc_pimFD_hit_map_AC_sector1_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_AC_sector1_%d", i), Form("#pi^{-} DC hitmap in region %d (after #pi^{-} cuts, sector1);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_AC_sector1_1e_cut[i]);
        }

#pragma endregion

#pragma region FD piminus histograms - sector 2
        TH1D *h_Vz_pimFD_BC_sector2_1e_cut =
            new TH1D("Vz_pimFD_BC_sector2_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector2);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_BC_sector2_1e_cut);
        TH1D *h_Vz_pimFD_AC_sector2_1e_cut =
            new TH1D("Vz_pimFD_AC_sector2_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector2);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_AC_sector2_1e_cut);

        TH1D *h_Vz_pimFD_BC_zoomin_sector2_1e_cut, *h_Vz_pimFD_AC_zoomin_sector2_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pimFD_BC_zoomin_sector2_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector2_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector2);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector2_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector2_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector2_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector2);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector2_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pimFD_BC_zoomin_sector2_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector2_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector2);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector2_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector2_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector2_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector2);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector2_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector2_1e_cut =
            new TH1D("Vx_pimFD_BC_sector2_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector2);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_BC_sector2_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector2_1e_cut =
            new TH1D("Vx_pimFD_AC_sector2_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector2);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_AC_sector2_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector2_1e_cut =
            new TH1D("Vy_pimFD_BC_sector2_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector2);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_BC_sector2_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector2_1e_cut =
            new TH1D("Vy_pimFD_AC_sector2_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector2);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_AC_sector2_1e_cut);

        TH1D *h_dVz_pimFD_BC_sector2_1e_cut =
            new TH1D("dVz_pimFD_BC_sector2_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector2);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_BC_sector2_1e_cut);
        TH1D *h_dVz_pimFD_AC_sector2_1e_cut =
            new TH1D("dVz_pimFD_AC_sector2_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector2);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_AC_sector2_1e_cut);

        TH1D *h_dVz_pimFD_BC_zoomin_sector2_1e_cut, *h_dVz_pimFD_AC_zoomin_sector2_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pimFD_BC_zoomin_sector2_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector2_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector2);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector2_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector2_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector2_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector2);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector2_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pimFD_BC_zoomin_sector2_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector2_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector2);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector2_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector2_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector2_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector2);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector2_1e_cut);
        }

        TH2D *h_dc_pimFD_hit_map_BC_sector2_1e_cut[4];  // 3 regions
        TH2D *h_dc_pimFD_hit_map_AC_sector2_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pimFD_hit_map_BC_sector2_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_BC_sector2_%d", i), Form("#pi^{-} DC hitmap in region %d (before #pi^{-} cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_BC_sector2_1e_cut[i]);
            h_dc_pimFD_hit_map_AC_sector2_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_AC_sector2_%d", i), Form("#pi^{-} DC hitmap in region %d (after #pi^{-} cuts, sector2);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_AC_sector2_1e_cut[i]);
        }

#pragma endregion

#pragma region FD piminus histograms - sector 3
        TH1D *h_Vz_pimFD_BC_sector3_1e_cut =
            new TH1D("Vz_pimFD_BC_sector3_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector3);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_BC_sector3_1e_cut);
        TH1D *h_Vz_pimFD_AC_sector3_1e_cut =
            new TH1D("Vz_pimFD_AC_sector3_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector3);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_AC_sector3_1e_cut);

        TH1D *h_Vz_pimFD_BC_zoomin_sector3_1e_cut, *h_Vz_pimFD_AC_zoomin_sector3_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pimFD_BC_zoomin_sector3_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector3_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector3);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector3_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector3_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector3_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector3);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector3_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pimFD_BC_zoomin_sector3_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector3_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector3);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector3_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector3_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector3_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector3);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector3_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector3_1e_cut =
            new TH1D("Vx_pimFD_BC_sector3_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector3);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_BC_sector3_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector3_1e_cut =
            new TH1D("Vx_pimFD_AC_sector3_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector3);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_AC_sector3_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector3_1e_cut =
            new TH1D("Vy_pimFD_BC_sector3_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector3);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_BC_sector3_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector3_1e_cut =
            new TH1D("Vy_pimFD_AC_sector3_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector3);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_AC_sector3_1e_cut);

        TH1D *h_dVz_pimFD_BC_sector3_1e_cut =
            new TH1D("dVz_pimFD_BC_sector3_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector3);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_BC_sector3_1e_cut);
        TH1D *h_dVz_pimFD_AC_sector3_1e_cut =
            new TH1D("dVz_pimFD_AC_sector3_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector3);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_AC_sector3_1e_cut);

        TH1D *h_dVz_pimFD_BC_zoomin_sector3_1e_cut, *h_dVz_pimFD_AC_zoomin_sector3_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pimFD_BC_zoomin_sector3_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector3_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector3);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector3_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector3_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector3_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector3);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector3_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pimFD_BC_zoomin_sector3_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector3_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector3);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector3_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector3_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector3_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector3);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector3_1e_cut);
        }

        TH2D *h_dc_pimFD_hit_map_BC_sector3_1e_cut[4];  // 3 regions
        TH2D *h_dc_pimFD_hit_map_AC_sector3_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pimFD_hit_map_BC_sector3_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_BC_sector3_%d", i), Form("#pi^{-} DC hitmap in region %d (before #pi^{-} cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_BC_sector3_1e_cut[i]);
            h_dc_pimFD_hit_map_AC_sector3_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_AC_sector3_%d", i), Form("#pi^{-} DC hitmap in region %d (after #pi^{-} cuts, sector3);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_AC_sector3_1e_cut[i]);
        }

#pragma endregion

#pragma region FD piminus histograms - sector 4
        TH1D *h_Vz_pimFD_BC_sector4_1e_cut =
            new TH1D("Vz_pimFD_BC_sector4_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector4);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_BC_sector4_1e_cut);
        TH1D *h_Vz_pimFD_AC_sector4_1e_cut =
            new TH1D("Vz_pimFD_AC_sector4_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector4);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_AC_sector4_1e_cut);

        TH1D *h_Vz_pimFD_BC_zoomin_sector4_1e_cut, *h_Vz_pimFD_AC_zoomin_sector4_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pimFD_BC_zoomin_sector4_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector4_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector4);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector4_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector4_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector4_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector4);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector4_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pimFD_BC_zoomin_sector4_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector4_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector4);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector4_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector4_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector4_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector4);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector4_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector4_1e_cut =
            new TH1D("Vx_pimFD_BC_sector4_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector4);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_BC_sector4_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector4_1e_cut =
            new TH1D("Vx_pimFD_AC_sector4_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector4);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_AC_sector4_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector4_1e_cut =
            new TH1D("Vy_pimFD_BC_sector4_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector4);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_BC_sector4_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector4_1e_cut =
            new TH1D("Vy_pimFD_AC_sector4_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector4);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_AC_sector4_1e_cut);

        TH1D *h_dVz_pimFD_BC_sector4_1e_cut =
            new TH1D("dVz_pimFD_BC_sector4_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector4);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_BC_sector4_1e_cut);
        TH1D *h_dVz_pimFD_AC_sector4_1e_cut =
            new TH1D("dVz_pimFD_AC_sector4_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector4);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_AC_sector4_1e_cut);

        TH1D *h_dVz_pimFD_BC_zoomin_sector4_1e_cut, *h_dVz_pimFD_AC_zoomin_sector4_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pimFD_BC_zoomin_sector4_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector4_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector4);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector4_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector4_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector4_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector4);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector4_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pimFD_BC_zoomin_sector4_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector4_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector4);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector4_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector4_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector4_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector4);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector4_1e_cut);
        }

        TH2D *h_dc_pimFD_hit_map_BC_sector4_1e_cut[4];  // 3 regions
        TH2D *h_dc_pimFD_hit_map_AC_sector4_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pimFD_hit_map_BC_sector4_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_BC_sector4_%d", i), Form("#pi^{-} DC hitmap in region %d (before #pi^{-} cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_BC_sector4_1e_cut[i]);
            h_dc_pimFD_hit_map_AC_sector4_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_AC_sector4_%d", i), Form("#pi^{-} DC hitmap in region %d (after #pi^{-} cuts, sector4);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_AC_sector4_1e_cut[i]);
        }

#pragma endregion

#pragma region FD piminus histograms - sector 5
        TH1D *h_Vz_pimFD_BC_sector5_1e_cut =
            new TH1D("Vz_pimFD_BC_sector5_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector5);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_BC_sector5_1e_cut);
        TH1D *h_Vz_pimFD_AC_sector5_1e_cut =
            new TH1D("Vz_pimFD_AC_sector5_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector5);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_AC_sector5_1e_cut);

        TH1D *h_Vz_pimFD_BC_zoomin_sector5_1e_cut, *h_Vz_pimFD_AC_zoomin_sector5_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pimFD_BC_zoomin_sector5_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector5_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector5);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector5_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector5_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector5_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector5);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector5_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pimFD_BC_zoomin_sector5_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector5_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector5);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector5_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector5_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector5_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector5);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector5_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector5_1e_cut =
            new TH1D("Vx_pimFD_BC_sector5_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector5);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_BC_sector5_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector5_1e_cut =
            new TH1D("Vx_pimFD_AC_sector5_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector5);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_AC_sector5_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector5_1e_cut =
            new TH1D("Vy_pimFD_BC_sector5_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector5);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_BC_sector5_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector5_1e_cut =
            new TH1D("Vy_pimFD_AC_sector5_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector5);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_AC_sector5_1e_cut);

        TH1D *h_dVz_pimFD_BC_sector5_1e_cut =
            new TH1D("dVz_pimFD_BC_sector5_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector5);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_BC_sector5_1e_cut);
        TH1D *h_dVz_pimFD_AC_sector5_1e_cut =
            new TH1D("dVz_pimFD_AC_sector5_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector5);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_AC_sector5_1e_cut);

        TH1D *h_dVz_pimFD_BC_zoomin_sector5_1e_cut, *h_dVz_pimFD_AC_zoomin_sector5_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pimFD_BC_zoomin_sector5_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector5_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector5);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector5_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector5_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector5_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector5);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector5_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pimFD_BC_zoomin_sector5_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector5_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector5);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector5_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector5_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector5_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector5);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector5_1e_cut);
        }

        TH2D *h_dc_pimFD_hit_map_BC_sector5_1e_cut[4];  // 3 regions
        TH2D *h_dc_pimFD_hit_map_AC_sector5_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pimFD_hit_map_BC_sector5_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_BC_sector5_%d", i), Form("#pi^{-} DC hitmap in region %d (before #pi^{-} cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_BC_sector5_1e_cut[i]);
            h_dc_pimFD_hit_map_AC_sector5_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_AC_sector5_%d", i), Form("#pi^{-} DC hitmap in region %d (after #pi^{-} cuts, sector5);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_AC_sector5_1e_cut[i]);
        }

#pragma endregion

#pragma region FD piminus histograms - sector 6
        TH1D *h_Vz_pimFD_BC_sector6_1e_cut =
            new TH1D("Vz_pimFD_BC_sector6_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector6);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_BC_sector6_1e_cut);
        TH1D *h_Vz_pimFD_AC_sector6_1e_cut =
            new TH1D("Vz_pimFD_AC_sector6_1e_cut", ("V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector6);V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimFD_AC_sector6_1e_cut);

        TH1D *h_Vz_pimFD_BC_zoomin_sector6_1e_cut, *h_Vz_pimFD_AC_zoomin_sector6_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pimFD_BC_zoomin_sector6_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector6_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector6);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector6_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector6_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector6_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector6);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector6_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pimFD_BC_zoomin_sector6_1e_cut =
                new TH1D("Vz_pimFD_BC_zoomin_sector6_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector6);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_BC_zoomin_sector6_1e_cut);
            h_Vz_pimFD_AC_zoomin_sector6_1e_cut =
                new TH1D("Vz_pimFD_AC_zoomin_sector6_1e_cut",
                         ("V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector6);V_{z}}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimFD_AC_zoomin_sector6_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector6_1e_cut =
            new TH1D("Vx_pimFD_BC_sector6_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector6);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_BC_sector6_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector6_1e_cut =
            new TH1D("Vx_pimFD_AC_sector6_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector6);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vx_pimFD_AC_sector6_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector6_1e_cut =
            new TH1D("Vy_pimFD_BC_sector6_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector6);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_BC_sector6_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector6_1e_cut =
            new TH1D("Vy_pimFD_AC_sector6_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector6);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -3, 3);
        HistoList.push_back(h_Vy_pimFD_AC_sector6_1e_cut);

        TH1D *h_dVz_pimFD_BC_sector6_1e_cut =
            new TH1D("dVz_pimFD_BC_sector6_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector6);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_BC_sector6_1e_cut);
        TH1D *h_dVz_pimFD_AC_sector6_1e_cut =
            new TH1D("dVz_pimFD_AC_sector6_1e_cut",
                     ("dV_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector6);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimFD_AC_sector6_1e_cut);

        TH1D *h_dVz_pimFD_BC_zoomin_sector6_1e_cut, *h_dVz_pimFD_AC_zoomin_sector6_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pimFD_BC_zoomin_sector6_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector6_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector6);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -8,
                -4);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector6_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector6_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector6_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector6);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector6_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pimFD_BC_zoomin_sector6_1e_cut = new TH1D(
                "dVz_pimFD_BC_zoomin_sector6_1e_cut",
                ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector6);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(), 75, -4,
                1);
            HistoList.push_back(h_dVz_pimFD_BC_zoomin_sector6_1e_cut);
            h_dVz_pimFD_AC_zoomin_sector6_1e_cut =
                new TH1D("dVz_pimFD_AC_zoomin_sector6_1e_cut",
                         ("dV_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector6);dV^{#pi^{-}FD}_{z}=V^{e}_{z}-V^{#pi^{-}FD}_{z} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_dVz_pimFD_AC_zoomin_sector6_1e_cut);
        }

        TH2D *h_dc_pimFD_hit_map_BC_sector6_1e_cut[4];  // 3 regions
        TH2D *h_dc_pimFD_hit_map_AC_sector6_1e_cut[4];  // 3 regions

        // DC hit maps
        for (int i = 1; i <= 3; i++) {
            h_dc_pimFD_hit_map_BC_sector6_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_BC_sector6_%d", i), Form("#pi^{-} DC hitmap in region %d (before #pi^{-} cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_BC_sector6_1e_cut[i]);
            h_dc_pimFD_hit_map_AC_sector6_1e_cut[i] =
                new TH2D(Form("dc_pimFD_hit_map_AC_sector6_%d", i), Form("#pi^{-} DC hitmap in region %d (after #pi^{-} cuts, sector6);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
            HistoList.push_back(h_dc_pimFD_hit_map_AC_sector6_1e_cut[i]);
        }

#pragma endregion

#pragma endregion

#pragma region CD piplus histograms
        TH1D *h_Vz_pipCD_BC_1e_cut = new TH1D("Vz_pipCD_BC_1e_cut", ("V_{z}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (before cut);V_{z}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipCD_BC_1e_cut);
        TH1D *h_Vz_pipCD_AC_1e_cut = new TH1D("Vz_pipCD_AC_1e_cut", ("V_{z}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (after cut);V_{z}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pipCD_AC_1e_cut);

        TH1D *h_Vz_pipCD_BC_zoomin_1e_cut, *h_Vz_pipCD_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pipCD_BC_zoomin_1e_cut = new TH1D(
                "Vz_pipCD_BC_zoomin_1e_cut", ("V_{z}^{#pi^{+}CD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);V_{z}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipCD_BC_zoomin_1e_cut);
            h_Vz_pipCD_AC_zoomin_1e_cut = new TH1D("Vz_pipCD_AC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{+}CD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);V_{z}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pipCD_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pipCD_BC_zoomin_1e_cut = new TH1D("Vz_pipCD_BC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{+}CD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);V_{z}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipCD_BC_zoomin_1e_cut);
            h_Vz_pipCD_AC_zoomin_1e_cut = new TH1D("Vz_pipCD_AC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{+}CD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);V_{z}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pipCD_AC_zoomin_1e_cut);
        }

        TH1D *h_Vx_pipCD_BC_1e_cut =
            new TH1D("Vx_pipCD_BC_1e_cut", ("V_{x}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (before cut);V_{x}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -0.75, 0.75);
        HistoList.push_back(h_Vx_pipCD_BC_1e_cut);
        TH1D *h_Vx_pipCD_AC_1e_cut =
            new TH1D("Vx_pipCD_AC_1e_cut", ("V_{x}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (after cut);V_{x}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -0.75, 0.75);
        HistoList.push_back(h_Vx_pipCD_AC_1e_cut);
        TH1D *h_Vy_pipCD_BC_1e_cut =
            new TH1D("Vy_pipCD_BC_1e_cut", ("V_{y}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (before cut);V_{y}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -0.75, 0.75);
        HistoList.push_back(h_Vy_pipCD_BC_1e_cut);
        TH1D *h_Vy_pipCD_AC_1e_cut =
            new TH1D("Vy_pipCD_AC_1e_cut", ("V_{y}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (after cut);V_{y}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -0.75, 0.75);
        HistoList.push_back(h_Vy_pipCD_AC_1e_cut);

        TH1D *h_dVz_pipCD_BC_1e_cut =
            new TH1D("dVz_pipCD_BC_1e_cut",
                     ("dV_{z}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts);dV^{#pi^{+}CD}_{z}=V^{e}_{z}-V^{#pi^{+}CD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pipCD_BC_1e_cut);
        TH1D *h_dVz_pipCD_AC_1e_cut =
            new TH1D("dVz_pipCD_AC_1e_cut", ("dV_{z}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts);dV^{#pi^{+}CD}_{z}=V^{e}_{z}-V^{#pi^{+}CD}_{z} [cm];Counts").c_str(),
                     75, -8, 8);
        HistoList.push_back(h_dVz_pipCD_AC_1e_cut);

        TH1D *h_dVz_pipCD_BC_zoomin_1e_cut, *h_dVz_pipCD_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pipCD_BC_zoomin_1e_cut = new TH1D(
                "dVz_pipCD_BC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{+}CD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);dV^{#pi^{+}CD}_{z}=V^{e}_{z}-V^{#pi^{+}CD}_{z} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_dVz_pipCD_BC_zoomin_1e_cut);
            h_dVz_pipCD_AC_zoomin_1e_cut = new TH1D(
                "dVz_pipCD_AC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{+}CD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);dV^{#pi^{+}CD}_{z}=V^{e}_{z}-V^{#pi^{+}CD}_{z} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_dVz_pipCD_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pipCD_BC_zoomin_1e_cut = new TH1D(
                "dVz_pipCD_BC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{+}CD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);dV^{#pi^{+}CD}_{z}=V^{e}_{z}-V^{#pi^{+}CD}_{z} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_dVz_pipCD_BC_zoomin_1e_cut);
            h_dVz_pipCD_AC_zoomin_1e_cut = new TH1D(
                "dVz_pipCD_AC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{+}CD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);dV^{#pi^{+}CD}_{z}=V^{e}_{z}-V^{#pi^{+}CD}_{z} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_dVz_pipCD_AC_zoomin_1e_cut);
        }

        TH1D *h_Chi2_pipCD_BC_1e_cut =
            new TH1D("Chi2_pipCD_BC_1e_cut", ("#chi^{2}_{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts);#chi^{2}_{#pi^{+}CD};Counts").c_str(), 75, -20, 20);
        HistoList.push_back(h_Chi2_pipCD_BC_1e_cut);
        TH1D *h_Chi2_pipCD_AC_1e_cut =
            new TH1D("Chi2_pipCD_AC_1e_cut", ("#chi^{2}_{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts);#chi^{2}_{#pi^{+}CD};Counts").c_str(), 75, -20, 20);
        HistoList.push_back(h_Chi2_pipCD_AC_1e_cut);
#pragma endregion

#pragma region CD piminus histograms
        TH1D *h_Vz_pimCD_BC_1e_cut = new TH1D("Vz_pimCD_BC_1e_cut", ("V_{z}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (before cut);V_{z}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimCD_BC_1e_cut);
        TH1D *h_Vz_pimCD_AC_1e_cut = new TH1D("Vz_pimCD_AC_1e_cut", ("V_{z}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (after cut);V_{z}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_pimCD_AC_1e_cut);

        TH1D *h_Vz_pimCD_BC_zoomin_1e_cut, *h_Vz_pimCD_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            h_Vz_pimCD_BC_zoomin_1e_cut = new TH1D(
                "Vz_pimCD_BC_zoomin_1e_cut", ("V_{z}^{#pi^{-}CD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts);V_{z}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimCD_BC_zoomin_1e_cut);
            h_Vz_pimCD_AC_zoomin_1e_cut = new TH1D("Vz_pimCD_AC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{-}CD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts);V_{z}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_Vz_pimCD_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            h_Vz_pimCD_BC_zoomin_1e_cut = new TH1D("Vz_pimCD_BC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{-}CD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts);V_{z}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimCD_BC_zoomin_1e_cut);
            h_Vz_pimCD_AC_zoomin_1e_cut = new TH1D("Vz_pimCD_AC_zoomin_1e_cut",
                                                   ("V_{z}^{#pi^{-}CD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts);V_{z}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_Vz_pimCD_AC_zoomin_1e_cut);
        }

        TH1D *h_Vx_pimCD_BC_1e_cut =
            new TH1D("Vx_pimCD_BC_1e_cut", ("V_{x}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (before cut);V_{x}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -0.75, 0.75);
        HistoList.push_back(h_Vx_pimCD_BC_1e_cut);
        TH1D *h_Vx_pimCD_AC_1e_cut =
            new TH1D("Vx_pimCD_AC_1e_cut", ("V_{x}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (after cut);V_{x}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -0.75, 0.75);
        HistoList.push_back(h_Vx_pimCD_AC_1e_cut);
        TH1D *h_Vy_pimCD_BC_1e_cut =
            new TH1D("Vy_pimCD_BC_1e_cut", ("V_{y}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (before cut);V_{y}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -0.75, 0.75);
        HistoList.push_back(h_Vy_pimCD_BC_1e_cut);
        TH1D *h_Vy_pimCD_AC_1e_cut =
            new TH1D("Vy_pimCD_AC_1e_cut", ("V_{y}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (after cut);V_{y}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -0.75, 0.75);
        HistoList.push_back(h_Vy_pimCD_AC_1e_cut);

        TH1D *h_dVz_pimCD_BC_1e_cut =
            new TH1D("dVz_pimCD_BC_1e_cut",
                     ("dV_{z}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts);dV^{#pi^{-}CD}_{z}=V^{e}_{z}-V^{#pi^{-}CD}_{z} [cm];Counts").c_str(), 75, -8, 8);
        HistoList.push_back(h_dVz_pimCD_BC_1e_cut);
        TH1D *h_dVz_pimCD_AC_1e_cut =
            new TH1D("dVz_pimCD_AC_1e_cut", ("dV_{z}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts);dV^{#pi^{-}CD}_{z}=V^{e}_{z}-V^{#pi^{-}CD}_{z} [cm];Counts").c_str(),
                     75, -8, 8);
        HistoList.push_back(h_dVz_pimCD_AC_1e_cut);

        TH1D *h_dVz_pimCD_BC_zoomin_1e_cut, *h_dVz_pimCD_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            h_dVz_pimCD_BC_zoomin_1e_cut = new TH1D(
                "dVz_pimCD_BC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{-}CD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts);dV^{#pi^{-}CD}_{z}=V^{e}_{z}-V^{#pi^{-}CD}_{z} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_dVz_pimCD_BC_zoomin_1e_cut);
            h_dVz_pimCD_AC_zoomin_1e_cut = new TH1D(
                "dVz_pimCD_AC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{-}CD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts);dV^{#pi^{-}CD}_{z}=V^{e}_{z}-V^{#pi^{-}CD}_{z} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_dVz_pimCD_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            h_dVz_pimCD_BC_zoomin_1e_cut = new TH1D(
                "dVz_pimCD_BC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{-}CD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts);dV^{#pi^{-}CD}_{z}=V^{e}_{z}-V^{#pi^{-}CD}_{z} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_dVz_pimCD_BC_zoomin_1e_cut);
            h_dVz_pimCD_AC_zoomin_1e_cut = new TH1D(
                "dVz_pimCD_AC_zoomin_1e_cut",
                ("dV_{z}^{#pi^{-}CD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts);dV^{#pi^{-}CD}_{z}=V^{e}_{z}-V^{#pi^{-}CD}_{z} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_dVz_pimCD_AC_zoomin_1e_cut);
        }

        TH1D *h_Chi2_pimCD_BC_1e_cut =
            new TH1D("Chi2_pimCD_BC_1e_cut", ("#chi^{2}_{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts);#chi^{2}_{#pi^{-}CD};Counts").c_str(), 75, -20, 20);
        HistoList.push_back(h_Chi2_pimCD_BC_1e_cut);
        TH1D *h_Chi2_pimCD_AC_1e_cut =
            new TH1D("Chi2_pimCD_AC_1e_cut", ("#chi^{2}_{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts);#chi^{2}_{#pi^{-}CD};Counts").c_str(), 75, -20, 20);
        HistoList.push_back(h_Chi2_pimCD_AC_1e_cut);
#pragma endregion

#pragma endregion

        clas12ana clasAna;

        // clasAna.readEcalSFPar((path_definitions::PathDefinitions.PIDCutsDirectory + "paramsSF_40Ca_x2.dat").c_str());
        // clasAna.setEcalSFCuts();

        // clasAna.readEcalPPar((path_definitions::PathDefinitions.PIDCutsDirectory + "paramsPI_40Ca_x2.dat").c_str());
        // // clasAna.setEcalPCuts();

        // clasAna.setEcalEdgeCuts();

        // clasAna.setNpheCuts();

        // clasAna.readInputParam(
        //     (path_definitions::PathDefinitions.PIDCutsDirectory + "Fitted_PID_Cuts_-_" + SampleName + ".par").c_str());  // load sample-appropreate cuts file from CutsDirectory
        // clasAna.setPidCuts();                                                                                            // making f_pidCuts = true

        // // clasAna.set_e_VertexCuts();  // making f_e_vertexCuts = true
        // // clasAna.setVertexCuts();  // making f_vertexCuts = true

        // clasAna.set_e_DCEdgeCuts();
        // clasAna.setDCEdgeCuts();

        // // clasAna.setVertexCorrCuts();                                                                                 // making f_corr_vertexCuts = true
        // // clasAna.setVertexCorrCutsLim(CutManager.dVz_cuts.GetLowerCut(), CutManager.dVz_cuts.GetUpperCut());          // setting dVz cuts (general)
        // // clasAna.setVertexCorrCutsLimFD(CutManager.dVz_cuts_FD.GetLowerCut(), CutManager.dVz_cuts_FD.GetUpperCut());  // setting dVz cuts (FD only)
        // // clasAna.setVertexCorrCutsLimCD(CutManager.dVz_cuts_CD.GetLowerCut(), CutManager.dVz_cuts_CD.GetUpperCut());  // setting dVz cuts (CD only)

        // clasAna.readInputParam(
        //     (path_definitions::PathDefinitions.NucleonCutsDirectory + "Nucleon_Cuts_-_" + SampleName + ".par").c_str());  // load sample-appropreate cuts file from CutsDirectory

        // clasAna.printParams();

        std::cout << "\n\nRunning on " << SampleName << " with " << Ebeam << " GeV beam energy\n\n";

#pragma region Loop over events
        int NumOfEvents = 0;
        int NumOfEvents_wAny_e_det = 0;
        int NumOfEvents_wOne_e_det = 0;

        int NumOfEvents_wAny_e = 0;
        int NumOfEvents_wOne_e = 0;

        while (chain.Next() == true) {
            // Display completed
            ++NumOfEvents;
            if ((NumOfEvents % 1000000) == 0) { std::cerr << "\n\n" << NumOfEvents / 1000000 << " million completed\n\n"; }
            if ((NumOfEvents % 100000) == 0) { std::cerr << "\n...\n"; }

            if ((ApplyLimiter && NumOfEvents > Limiter)) { break; }

            clasAna.Run(c12);
            auto electrons = clasAna.getByPid(11);
            auto protons = clasAna.getByPid(2212);
            auto piplus = clasAna.getByPid(211);
            auto piminus = clasAna.getByPid(-211);

            // get particles by type
            auto allParticles = c12->getDetParticles();
            auto electrons_det = c12->getByID(11);
            auto piplus_det = c12->getByID(211);
            auto piminus_det = c12->getByID(-211);

            if (electrons_det.size() > 0) { ++NumOfEvents_wAny_e_det; }
            if (electrons_det.size() == 1) { ++NumOfEvents_wOne_e_det; }
            if (electrons.size() > 0) { ++NumOfEvents_wAny_e; }
            if (electrons.size() == 1) { ++NumOfEvents_wOne_e; }

            if (electrons_det.size() != 1) { continue; }

            double weight = 1;

            double starttime = c12->event()->getStartTime();

            TVector3 P_b(0, 0, Ebeam);

            TVector3 reco_P_e;
            reco_P_e.SetMagThetaPhi(electrons_det[0]->getP(), electrons_det[0]->getTheta(), electrons_det[0]->getPhi());

            double vtz_e_det = electrons_det[0]->par()->getVz();
            // double vtz_e = electrons[0]->par()->getVz();

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
            //  (e,e') (reco)
            //  =======================================================================================================================================================================

            //  - Apply dVz cuts on both e and FD/CD pions ----------------------------------------------------------------------------------------------------------------------------

            vector<double> vertex_corr_cuts_cd = {-1.8, 3.1};  // electron vertex <-> particle vertex correlation cuts
            vector<double> vertex_corr_cuts_fd = {-3.5, 5.8};  // electron vertex <-> particle vertex correlation cuts

            bool Both_e_and_pipFD_passed_dVz_cuts = true, Both_e_and_pipCD_passed_dVz_cuts = true;
            bool Both_e_and_pimFD_passed_dVz_cuts = true, Both_e_and_pimCD_passed_dVz_cuts = true;

            for (int i = 0; i < piplus.size(); i++) {
                double temp_dVz_pip = electrons[0]->par()->getVz() - piplus[i]->par()->getVz();

                if ((piplus[i]->getRegion() == clas12::FD) && !(temp_dVz_pip > vertex_corr_cuts_fd.at(0) && temp_dVz_pip < vertex_corr_cuts_fd.at(1))) {
                    Both_e_and_pipFD_passed_dVz_cuts = false;
                } else if ((piplus[i]->getRegion() == clas12::CD) && !(temp_dVz_pip > vertex_corr_cuts_cd.at(0) && temp_dVz_pip < vertex_corr_cuts_cd.at(1))) {
                    Both_e_and_pipCD_passed_dVz_cuts = false;
                }
            }

            for (int i = 0; i < piminus.size(); i++) {
                double temp_dVz_pim = electrons[0]->par()->getVz() - piminus[i]->par()->getVz();

                if ((piminus[i]->getRegion() == clas12::FD) && !(temp_dVz_pim > vertex_corr_cuts_fd.at(0) && temp_dVz_pim < vertex_corr_cuts_fd.at(1))) {
                    Both_e_and_pimFD_passed_dVz_cuts = false;
                } else if ((piminus[i]->getRegion() == clas12::CD) && !(temp_dVz_pim > vertex_corr_cuts_cd.at(0) && temp_dVz_pim < vertex_corr_cuts_cd.at(1))) {
                    Both_e_and_pimCD_passed_dVz_cuts = false;
                }
            }

            bool Both_e_and_pions_passed_dVz_cuts = ((piplus.size() > 0) || (piminus.size() > 0));
            // (Both_e_and_pipFD_passed_dVz_cuts && Both_e_and_pipCD_passed_dVz_cuts && Both_e_and_pimFD_passed_dVz_cuts && Both_e_and_pimCD_passed_dVz_cuts);

            // skip event if e and pions do not pass dVz cuts
            if (!Both_e_and_pions_passed_dVz_cuts) { continue; }

            //  =======================================================================================================================================================================
            //  (e,e') (reco)
            //  =======================================================================================================================================================================

            //  - Electron cuts -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region Electrons BPID
            h_Vx_e_BC_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            h_Vy_e_BC_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            h_Vz_e_BC_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            h_Vz_e_BC_zoomin_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);

            reco_analysis_functions::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_1e_cut, weight);

            h_nphe_BC_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);

            h_Edep_PCAL_VS_EC_BC_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

            h_SF_VS_Edep_PCAL_BC_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
            h_SF_VS_P_e_BC_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

            h_SF_VS_Lv_BC_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_BC_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_BC_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

            h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);

            if (electrons_det[0]->getSector() == 1) {
                h_Vx_e_BC_sector1_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
                h_Vy_e_BC_sector1_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
                h_Vz_e_BC_sector1_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
                h_Vz_e_BC_zoomin_sector1_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector1_1e_cut, weight);

                h_nphe_BC_sector1_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_BC_sector1_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_BC_sector1_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_BC_sector1_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_BC_sector1_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_BC_sector1_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_BC_sector1_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector1_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons_det[0]->getSector() == 2) {
                h_Vx_e_BC_sector2_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
                h_Vy_e_BC_sector2_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
                h_Vz_e_BC_sector2_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
                h_Vz_e_BC_zoomin_sector2_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector2_1e_cut, weight);

                h_nphe_BC_sector2_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_BC_sector2_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_BC_sector2_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_BC_sector2_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_BC_sector2_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_BC_sector2_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_BC_sector2_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector2_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons_det[0]->getSector() == 3) {
                h_Vx_e_BC_sector3_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
                h_Vy_e_BC_sector3_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
                h_Vz_e_BC_sector3_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
                h_Vz_e_BC_zoomin_sector3_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector3_1e_cut, weight);

                h_nphe_BC_sector3_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_BC_sector3_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_BC_sector3_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_BC_sector3_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_BC_sector3_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_BC_sector3_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_BC_sector3_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector3_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons_det[0]->getSector() == 4) {
                h_Vx_e_BC_sector4_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
                h_Vy_e_BC_sector4_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
                h_Vz_e_BC_sector4_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
                h_Vz_e_BC_zoomin_sector4_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector4_1e_cut, weight);

                h_nphe_BC_sector4_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_BC_sector4_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_BC_sector4_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_BC_sector4_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_BC_sector4_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_BC_sector4_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_BC_sector4_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector4_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons_det[0]->getSector() == 5) {
                h_Vx_e_BC_sector5_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
                h_Vy_e_BC_sector5_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
                h_Vz_e_BC_sector5_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
                h_Vz_e_BC_zoomin_sector5_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector5_1e_cut, weight);

                h_nphe_BC_sector5_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);

                h_SF_VS_Edep_PCAL_BC_sector5_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_Edep_PCAL_VS_EC_BC_sector5_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_P_e_BC_sector5_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_BC_sector5_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_BC_sector5_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_BC_sector5_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector5_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons_det[0]->getSector() == 6) {
                h_Vx_e_BC_sector6_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
                h_Vy_e_BC_sector6_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
                h_Vz_e_BC_sector6_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
                h_Vz_e_BC_zoomin_sector6_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector6_1e_cut, weight);

                h_nphe_BC_sector6_1e_cut->Fill(electrons_det[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_BC_sector6_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_BC_sector6_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_BC_sector6_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_BC_sector6_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_BC_sector6_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_BC_sector6_1e_cut->Fill(electrons_det[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_BC_sector6_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            }

#pragma endregion

            if (electrons.size() != 1) { continue; }

            if (electrons[0]->par()->getBeta() > 1.2) { continue; }

            // // bool bad_Vz_e_CutCond = (electrons[0]->par()->getVz() < -4. || electrons[0]->par()->getVz() > -2.);
            // bool bad_DC_edge_CutCond = (!reco_analysis_functions::DCEdgeCuts(electrons[0]));
            // bool bad_nphe_CutCond = (electrons[0]->che(clas12::HTCC)->getNphe() <= 2);
            // bool bad_Edep_PCAL_CutCond = (Edep_PCAL <= 0.06);
            // bool bad_SF_CutCond = (EoP_e < 0.2 || EoP_e > 0.28);
            // bool bad_PCAL_edge_CutCond = (electrons[0]->cal(clas12::PCAL)->getLv() < 14. || electrons[0]->cal(clas12::PCAL)->getLw() < 14.);
            // bool bad_diag_CutCond = (!reco_analysis_functions::checkEcalDiagCuts(electrons[0]));

            // // if (bad_Vz_e_CutCond) { continue; }
            // if (bad_DC_edge_CutCond) { continue; }
            // if (bad_nphe_CutCond) { continue; }
            // if (bad_Edep_PCAL_CutCond) { continue; }
            // if (bad_SF_CutCond) { continue; }
            // if (bad_PCAL_edge_CutCond) { continue; }
            // if (bad_diag_CutCond) { continue; }

#pragma region Electrons APID
            h_Vx_e_AC_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
            h_Vy_e_AC_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
            h_Vz_e_AC_1e_cut->Fill(electrons[0]->par()->getVz(), weight);
            h_Vz_e_AC_zoomin_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

            reco_analysis_functions::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_1e_cut, weight);

            h_nphe_AC_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

            h_Edep_PCAL_VS_EC_AC_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

            h_SF_VS_Edep_PCAL_AC_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
            h_SF_VS_P_e_AC_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

            h_SF_VS_Lv_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

            h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);

            if (electrons[0]->getSector() == 1) {
                h_Vx_e_AC_sector1_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
                h_Vy_e_AC_sector1_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
                h_Vz_e_AC_sector1_1e_cut->Fill(electrons[0]->par()->getVz(), weight);
                h_Vz_e_AC_zoomin_sector1_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector1_1e_cut, weight);

                h_nphe_AC_sector1_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_AC_sector1_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_AC_sector1_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_AC_sector1_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_AC_sector1_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_AC_sector1_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_AC_sector1_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector1_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons[0]->getSector() == 2) {
                h_Vx_e_AC_sector2_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
                h_Vy_e_AC_sector2_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
                h_Vz_e_AC_sector2_1e_cut->Fill(electrons[0]->par()->getVz(), weight);
                h_Vz_e_AC_zoomin_sector2_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector2_1e_cut, weight);

                h_nphe_AC_sector2_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_AC_sector2_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_AC_sector2_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_AC_sector2_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_AC_sector2_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_AC_sector2_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_AC_sector2_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector2_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons[0]->getSector() == 3) {
                h_Vx_e_AC_sector3_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
                h_Vy_e_AC_sector3_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
                h_Vz_e_AC_sector3_1e_cut->Fill(electrons[0]->par()->getVz(), weight);
                h_Vz_e_AC_zoomin_sector3_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector3_1e_cut, weight);

                h_nphe_AC_sector3_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_AC_sector3_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_AC_sector3_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_AC_sector3_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_AC_sector3_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_AC_sector3_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_AC_sector3_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector3_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons[0]->getSector() == 4) {
                h_Vx_e_AC_sector4_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
                h_Vy_e_AC_sector4_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
                h_Vz_e_AC_sector4_1e_cut->Fill(electrons[0]->par()->getVz(), weight);
                h_Vz_e_AC_zoomin_sector4_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector4_1e_cut, weight);

                h_nphe_AC_sector4_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_AC_sector4_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_AC_sector4_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_AC_sector4_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_AC_sector4_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_AC_sector4_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_AC_sector4_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector4_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons[0]->getSector() == 5) {
                h_Vx_e_AC_sector5_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
                h_Vy_e_AC_sector5_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
                h_Vz_e_AC_sector5_1e_cut->Fill(electrons[0]->par()->getVz(), weight);
                h_Vz_e_AC_zoomin_sector5_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector5_1e_cut, weight);

                h_nphe_AC_sector5_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

                h_SF_VS_Edep_PCAL_AC_sector5_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
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
                h_Vz_e_AC_zoomin_sector6_1e_cut->Fill(electrons[0]->par()->getVz(), weight);

                reco_analysis_functions::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector6_1e_cut, weight);

                h_nphe_AC_sector6_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_AC_sector6_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_AC_sector6_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_AC_sector6_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_AC_sector6_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_AC_sector6_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_AC_sector6_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector6_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            }

#pragma endregion

            //  - Piplus cuts -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region piplus BPID
            for (int i = 0; i < piplus_det.size(); i++) {
                if (piplus_det[i]->getRegion() == FD) {
                    h_Vx_pipFD_BC_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                    h_Vy_pipFD_BC_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                    h_Vz_pipFD_BC_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                    h_Vz_pipFD_BC_zoomin_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                    h_dVz_pipFD_BC_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                    h_dVz_pipFD_BC_zoomin_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                    reco_analysis_functions::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_1e_cut, weight);

                    h_Chi2_pipFD_BC_1e_cut->Fill(piplus_det[i]->par()->getChi2Pid(), weight);

                    if (piplus_det[i]->getSector() == 1) {
                        h_Vx_pipFD_BC_sector1_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector1_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector1_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector1_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector1_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector1_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector1_1e_cut, weight);
                    } else if (piplus_det[i]->getSector() == 2) {
                        h_Vx_pipFD_BC_sector2_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector2_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector2_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector2_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector2_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector2_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector2_1e_cut, weight);
                    } else if (piplus_det[i]->getSector() == 3) {
                        h_Vx_pipFD_BC_sector3_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector3_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector3_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector3_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector3_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector3_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector3_1e_cut, weight);
                    } else if (piplus_det[i]->getSector() == 4) {
                        h_Vx_pipFD_BC_sector4_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector4_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector4_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector4_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector4_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector4_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector4_1e_cut, weight);
                    } else if (piplus_det[i]->getSector() == 5) {
                        h_Vx_pipFD_BC_sector5_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector5_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector5_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector5_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector5_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector5_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector5_1e_cut, weight);
                    } else if (piplus_det[i]->getSector() == 6) {
                        h_Vx_pipFD_BC_sector6_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector6_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector6_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector6_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector6_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector6_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector6_1e_cut, weight);
                    }
                } else if (piplus_det[i]->getRegion() == CD) {
                    h_Chi2_pipCD_BC_1e_cut->Fill(piplus_det[i]->par()->getChi2Pid(), weight);

                    h_Vx_pipCD_BC_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                    h_Vy_pipCD_BC_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                    h_Vz_pipCD_BC_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                    h_Vz_pipCD_BC_zoomin_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                    h_dVz_pipCD_BC_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                    h_dVz_pipCD_BC_zoomin_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                }
            }
#pragma endregion

#pragma region piplus APID
            for (int i = 0; i < piplus.size(); i++) {
                if (piplus[i]->getRegion() == FD) {
                    h_Vx_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                    h_Vy_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                    h_Vz_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                    h_Vz_pipFD_AC_zoomin_1e_cut->Fill(piplus[i]->par()->getVz(), weight);

                    h_dVz_pipFD_AC_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                    h_dVz_pipFD_AC_zoomin_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                    reco_analysis_functions::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_1e_cut, weight);

                    h_Chi2_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getChi2Pid(), weight);

                    if (piplus[i]->getSector() == 1) {
                        h_Vx_pipFD_AC_sector1_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                        h_Vy_pipFD_AC_sector1_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                        h_Vz_pipFD_AC_sector1_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                        h_Vz_pipFD_AC_zoomin_sector1_1e_cut->Fill(piplus[i]->par()->getVz(), weight);

                        h_dVz_pipFD_AC_sector1_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector1_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector1_1e_cut, weight);
                    } else if (piplus[i]->getSector() == 2) {
                        h_Vx_pipFD_AC_sector2_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                        h_Vy_pipFD_AC_sector2_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                        h_Vz_pipFD_AC_sector2_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                        h_Vz_pipFD_AC_zoomin_sector2_1e_cut->Fill(piplus[i]->par()->getVz(), weight);

                        h_dVz_pipFD_AC_sector2_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector2_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector2_1e_cut, weight);
                    } else if (piplus[i]->getSector() == 3) {
                        h_Vx_pipFD_AC_sector3_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                        h_Vy_pipFD_AC_sector3_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                        h_Vz_pipFD_AC_sector3_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                        h_Vz_pipFD_AC_zoomin_sector3_1e_cut->Fill(piplus[i]->par()->getVz(), weight);

                        h_dVz_pipFD_AC_sector3_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector3_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector3_1e_cut, weight);
                    } else if (piplus[i]->getSector() == 4) {
                        h_Vx_pipFD_AC_sector4_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                        h_Vy_pipFD_AC_sector4_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                        h_Vz_pipFD_AC_sector4_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                        h_Vz_pipFD_AC_zoomin_sector4_1e_cut->Fill(piplus[i]->par()->getVz(), weight);

                        h_dVz_pipFD_AC_sector4_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector4_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector4_1e_cut, weight);
                    } else if (piplus[i]->getSector() == 5) {
                        h_Vx_pipFD_AC_sector5_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                        h_Vy_pipFD_AC_sector5_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                        h_Vz_pipFD_AC_sector5_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                        h_Vz_pipFD_AC_zoomin_sector5_1e_cut->Fill(piplus[i]->par()->getVz(), weight);

                        h_dVz_pipFD_AC_sector5_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector5_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector5_1e_cut, weight);
                    } else if (piplus[i]->getSector() == 6) {
                        h_Vx_pipFD_AC_sector6_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                        h_Vy_pipFD_AC_sector6_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                        h_Vz_pipFD_AC_sector6_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                        h_Vz_pipFD_AC_zoomin_sector6_1e_cut->Fill(piplus[i]->par()->getVz(), weight);

                        h_dVz_pipFD_AC_sector6_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector6_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector6_1e_cut, weight);
                    }
                } else if (piplus[i]->getRegion() == CD) {
                    h_Chi2_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getChi2Pid(), weight);

                    h_Vx_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getVx(), weight);
                    h_Vy_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getVy(), weight);
                    h_Vz_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getVz(), weight);
                    h_Vz_pipCD_AC_zoomin_1e_cut->Fill(piplus[i]->par()->getVz(), weight);

                    h_dVz_pipCD_AC_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                    h_dVz_pipCD_AC_zoomin_1e_cut->Fill(-(piplus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                }
            }
#pragma endregion

            //  - Piminus cuts ------------------------------------------------------------------------------------------------------------------------------------------

            //  - Piplus cuts -------------------------------------------------------------------------------------------------------------------------------------------

#pragma region piminus BPID
            for (int i = 0; i < piminus_det.size(); i++) {
                if (piminus_det[i]->getRegion() == FD) {
                    h_Vx_pimFD_BC_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                    h_Vy_pimFD_BC_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                    h_Vz_pimFD_BC_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                    h_Vz_pimFD_BC_zoomin_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                    h_dVz_pimFD_BC_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                    h_dVz_pimFD_BC_zoomin_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                    reco_analysis_functions::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_1e_cut, weight);

                    h_Chi2_pimFD_BC_1e_cut->Fill(piminus_det[i]->par()->getChi2Pid(), weight);

                    if (piminus_det[i]->getSector() == 1) {
                        h_Vx_pimFD_BC_sector1_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector1_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector1_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector1_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector1_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector1_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector1_1e_cut, weight);
                    } else if (piminus_det[i]->getSector() == 2) {
                        h_Vx_pimFD_BC_sector2_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector2_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector2_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector2_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector2_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector2_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector2_1e_cut, weight);
                    } else if (piminus_det[i]->getSector() == 3) {
                        h_Vx_pimFD_BC_sector3_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector3_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector3_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector3_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector3_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector3_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector3_1e_cut, weight);
                    } else if (piminus_det[i]->getSector() == 4) {
                        h_Vx_pimFD_BC_sector4_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector4_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector4_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector4_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector4_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector4_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector4_1e_cut, weight);
                    } else if (piminus_det[i]->getSector() == 5) {
                        h_Vx_pimFD_BC_sector5_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector5_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector5_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector5_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector5_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector5_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector5_1e_cut, weight);
                    } else if (piminus_det[i]->getSector() == 6) {
                        h_Vx_pimFD_BC_sector6_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector6_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector6_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector6_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector6_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector6_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector6_1e_cut, weight);
                    }
                } else if (piminus_det[i]->getRegion() == CD) {
                    h_Chi2_pimCD_BC_1e_cut->Fill(piminus_det[i]->par()->getChi2Pid(), weight);

                    h_Vx_pimCD_BC_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                    h_Vy_pimCD_BC_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                    h_Vz_pimCD_BC_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                    h_Vz_pimCD_BC_zoomin_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                    h_dVz_pimCD_BC_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                    h_dVz_pimCD_BC_zoomin_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                }
            }
#pragma endregion

#pragma region piminus APID
            for (int i = 0; i < piminus.size(); i++) {
                if (piminus[i]->getRegion() == FD) {
                    h_Vx_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                    h_Vy_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                    h_Vz_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                    h_Vz_pimFD_AC_zoomin_1e_cut->Fill(piminus[i]->par()->getVz(), weight);

                    h_dVz_pimFD_AC_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                    h_dVz_pimFD_AC_zoomin_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                    reco_analysis_functions::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_1e_cut, weight);

                    h_Chi2_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getChi2Pid(), weight);

                    if (piminus[i]->getSector() == 1) {
                        h_Vx_pimFD_AC_sector1_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                        h_Vy_pimFD_AC_sector1_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                        h_Vz_pimFD_AC_sector1_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                        h_Vz_pimFD_AC_zoomin_sector1_1e_cut->Fill(piminus[i]->par()->getVz(), weight);

                        h_dVz_pimFD_AC_sector1_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector1_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector1_1e_cut, weight);
                    } else if (piminus[i]->getSector() == 2) {
                        h_Vx_pimFD_AC_sector2_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                        h_Vy_pimFD_AC_sector2_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                        h_Vz_pimFD_AC_sector2_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                        h_Vz_pimFD_AC_zoomin_sector2_1e_cut->Fill(piminus[i]->par()->getVz(), weight);

                        h_dVz_pimFD_AC_sector2_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector2_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector2_1e_cut, weight);
                    } else if (piminus[i]->getSector() == 3) {
                        h_Vx_pimFD_AC_sector3_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                        h_Vy_pimFD_AC_sector3_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                        h_Vz_pimFD_AC_sector3_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                        h_Vz_pimFD_AC_zoomin_sector3_1e_cut->Fill(piminus[i]->par()->getVz(), weight);

                        h_dVz_pimFD_AC_sector3_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector3_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector3_1e_cut, weight);
                    } else if (piminus[i]->getSector() == 4) {
                        h_Vx_pimFD_AC_sector4_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                        h_Vy_pimFD_AC_sector4_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                        h_Vz_pimFD_AC_sector4_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                        h_Vz_pimFD_AC_zoomin_sector4_1e_cut->Fill(piminus[i]->par()->getVz(), weight);

                        h_dVz_pimFD_AC_sector4_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector4_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector4_1e_cut, weight);
                    } else if (piminus[i]->getSector() == 5) {
                        h_Vx_pimFD_AC_sector5_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                        h_Vy_pimFD_AC_sector5_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                        h_Vz_pimFD_AC_sector5_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                        h_Vz_pimFD_AC_zoomin_sector5_1e_cut->Fill(piminus[i]->par()->getVz(), weight);

                        h_dVz_pimFD_AC_sector5_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector5_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector5_1e_cut, weight);
                    } else if (piminus[i]->getSector() == 6) {
                        h_Vx_pimFD_AC_sector6_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                        h_Vy_pimFD_AC_sector6_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                        h_Vz_pimFD_AC_sector6_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                        h_Vz_pimFD_AC_zoomin_sector6_1e_cut->Fill(piminus[i]->par()->getVz(), weight);

                        h_dVz_pimFD_AC_sector6_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector6_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        reco_analysis_functions::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector6_1e_cut, weight);
                    }
                } else if (piminus[i]->getRegion() == CD) {
                    h_Chi2_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getChi2Pid(), weight);

                    h_Vx_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getVx(), weight);
                    h_Vy_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getVy(), weight);
                    h_Vz_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getVz(), weight);
                    h_Vz_pimCD_AC_zoomin_1e_cut->Fill(piminus[i]->par()->getVz(), weight);

                    h_dVz_pimCD_AC_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                    h_dVz_pimCD_AC_zoomin_1e_cut->Fill(-(piminus[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                }
            }
#pragma endregion
        }

#pragma endregion

#pragma region nPlotting and saving histograms
        std::cout << "\n\nPlotting and saving histograms..." << "\n\n";

        /////////////////////////////////////////////////////
        // Organize histograms
        /////////////////////////////////////////////////////
        for (int i = 0; i < HistoList.size(); i++) {
            if (HistoList[i]->InheritsFrom("TH1D")) {
                HistoList[i]->Sumw2();
                HistoList[i]->SetMinimum(0);
                HistoList[i]->SetLineWidth(2);
                HistoList[i]->SetLineColor(kRed);
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

        std::string PDF_fileName = OutputDir + "/" + OutFolderName + ".pdf";
        char fileName[PDF_fileName.length()];
        sprintf(fileName, "%s[", PDF_fileName.c_str());
        myText->SaveAs(fileName);
        sprintf(fileName, "%s", PDF_fileName.c_str());

        /////////////////////////////////////
        // CND Neutron Information
        /////////////////////////////////////
        myText->cd();

        titles.DrawLatex(0.05, 0.9, "HipoLooper Output");
        text.DrawLatex(0.05, 0.75, ("Plots from (e,e') events in: #font[42]{" + CodeRun_status + "}").c_str());

        if (IsData) {
            text.DrawLatex(0.05, 0.7, ("InputFiles: #font[42]{" + InputFiles.at(sample) + "}").c_str());
        } else {
            text.DrawLatex(0.05, 0.7, ("BaseDir: #font[42]{" + BaseDir + "}").c_str());
            text.DrawLatex(0.05, 0.65, ("InputFiles: #font[42]{BaseDir + " + InputFiles.at(sample).substr(BaseDir.length()) + "}").c_str());
        }

        text.DrawLatex(0.05, 0.60, "Event counts:");
        text.DrawLatex(0.10, 0.55, ("Total #(events):            #font[42]{" + std::to_string(NumOfEvents) + "}").c_str());
        text.DrawLatex(0.10, 0.50, ("#(Events) with any e_det:  #font[42]{" + std::to_string(NumOfEvents_wAny_e_det) + "}").c_str());
        text.DrawLatex(0.10, 0.45,
                       ("#(Events) with one e_det:  #font[42]{" + std::to_string(NumOfEvents_wOne_e_det) + " (" +
                        basic_tools::ToStringWithPrecision((100 * NumOfEvents_wOne_e_det / NumOfEvents_wAny_e_det), 2) + "%)}")
                           .c_str());
        text.DrawLatex(0.10, 0.40, ("#(Events) with any e:       #font[42]{" + std::to_string(NumOfEvents_wAny_e) + "}").c_str());
        text.DrawLatex(0.10, 0.35,
                       ("#(Events) with one e:       #font[42]{" + std::to_string(NumOfEvents_wOne_e) + " (" +
                        basic_tools::ToStringWithPrecision((100 * NumOfEvents_wOne_e / NumOfEvents_wAny_e), 2) + "%)}")
                           .c_str());

        myText->Print(fileName, "pdf Title: Cover");
        // myText->Print(fileName, "pdf");
        myText->Clear();

        bool first_electron = true;
        bool first_electron_sector1 = true, first_electron_sector2 = true, first_electron_sector3 = true, first_electron_sector4 = true, first_electron_sector5 = true,
             first_electron_sector6 = true;

        bool first_piplusFD = true;
        bool first_piplusFD_sector1 = true, first_piplusFD_sector2 = true, first_piplusFD_sector3 = true, first_piplusFD_sector4 = true, first_piplusFD_sector5 = true,
             first_piplusFD_sector6 = true;

        bool first_piminusFD = true;
        bool first_piminusFD_sector1 = true, first_piminusFD_sector2 = true, first_piminusFD_sector3 = true, first_piminusFD_sector4 = true, first_piminusFD_sector5 = true,
             first_piminusFD_sector6 = true;

        bool first_piplusCD = true;
        bool first_piplusCD_sector1 = true, first_piplusCD_sector2 = true, first_piplusCD_sector3 = true, first_piplusCD_sector4 = true, first_piplusCD_sector5 = true,
             first_piplusCD_sector6 = true;

        bool first_piminusCD = true;
        bool first_piminusCD_sector1 = true, first_piminusCD_sector2 = true, first_piminusCD_sector3 = true, first_piminusCD_sector4 = true, first_piminusCD_sector5 = true,
             first_piminusCD_sector6 = true;

        int plot_counter = 2;

        double yOffset = 0.075;  // Offset for the y position of the text
        // double yOffset = 0.10;  // Offset for the y position of the text

        for (int i = 0; i < HistoList.size(); i++) {
            // Maps to hold first-time flags
            std::map<std::string, bool *> first_flags = {
                {"{e}", &first_electron}, {"{#pi^{+}FD}", &first_piplusFD}, {"{#pi^{-}FD}", &first_piminusFD}, {"{#pi^{+}CD}", &first_piplusCD}, {"{#pi^{-}CD}", &first_piminusCD}};

            std::map<std::string, std::string> particle_labels = {
                {"{e}", "e^{-}"}, {"{#pi^{+}FD}", "FD #pi^{+}"}, {"{#pi^{-}FD}", "FD #pi^{-}"}, {"{#pi^{+}CD}", "CD #pi^{+}"}, {"{#pi^{-}CD}", "CD #pi^{-}"}};

            // Maps of sector flags (assumes these variables already exist)
            std::map<std::string, std::map<int, bool *>> sector_flags = {{"{e}",
                                                                          {{1, &first_electron_sector1},
                                                                           {2, &first_electron_sector2},
                                                                           {3, &first_electron_sector3},
                                                                           {4, &first_electron_sector4},
                                                                           {5, &first_electron_sector5},
                                                                           {6, &first_electron_sector6}}},
                                                                         {"{#pi^{+}FD}",
                                                                          {{1, &first_piplusFD_sector1},
                                                                           {2, &first_piplusFD_sector2},
                                                                           {3, &first_piplusFD_sector3},
                                                                           {4, &first_piplusFD_sector4},
                                                                           {5, &first_piplusFD_sector5},
                                                                           {6, &first_piplusFD_sector6}}},
                                                                         {"{#pi^{-}FD}",
                                                                          {{1, &first_piminusFD_sector1},
                                                                           {2, &first_piminusFD_sector2},
                                                                           {3, &first_piminusFD_sector3},
                                                                           {4, &first_piminusFD_sector4},
                                                                           {5, &first_piminusFD_sector5},
                                                                           {6, &first_piminusFD_sector6}}},
                                                                         {"{#pi^{+}CD}",
                                                                          {{1, &first_piplusCD_sector1},
                                                                           {2, &first_piplusCD_sector2},
                                                                           {3, &first_piplusCD_sector3},
                                                                           {4, &first_piplusCD_sector4},
                                                                           {5, &first_piplusCD_sector5},
                                                                           {6, &first_piplusCD_sector6}}},
                                                                         {"{#pi^{-}CD}",
                                                                          {{1, &first_piminusCD_sector1},
                                                                           {2, &first_piminusCD_sector2},
                                                                           {3, &first_piminusCD_sector3},
                                                                           {4, &first_piminusCD_sector4},
                                                                           {5, &first_piminusCD_sector5},
                                                                           {6, &first_piminusCD_sector6}}}};

            std::string title = HistoList[i]->GetTitle();

            for (const auto &[particle_key, label] : particle_labels) {
                if (basic_tools::FindSubstring(title, particle_key)) {
                    myText->cd();
                    titles.SetTextAlign(22);  // Center text both horizontally and vertically

                    if (*first_flags[particle_key] && !basic_tools::FindSubstring(title, "sector")) {
                        std::string bookmark_title = label + " plots";
                        std::string sanitized_bookmark_title = histogram_functions::SanitizeForBookmark(bookmark_title);
                        titles.DrawLatex(0.5, 0.5, bookmark_title.c_str());
                        myText->Print(fileName, ("pdf Title:" + sanitized_bookmark_title).c_str());
                        myText->Clear();
                        *first_flags[particle_key] = false;
                        ++plot_counter;
                    } else {
                        for (int sector = 1; sector <= 6; ++sector) {
                            std::string sector_str = "sector" + std::to_string(sector);
                            std::string sector_title_str = "sector " + std::to_string(sector);

                            if (*sector_flags[particle_key][sector] && basic_tools::FindSubstring(title, sector_str)) {
                                std::string bookmark_title = label + " plots - " + sector_title_str;
                                // Compose hierarchical bookmark: parent>child (separation expects '>' for hierarchy)
                                std::string hierarchical_title = histogram_functions::SanitizeForBookmark(label + " plots") + ">" + histogram_functions::SanitizeForBookmark(bookmark_title);
                                titles.DrawLatex(0.5, 0.5, bookmark_title.c_str());
                                myText->Print(fileName, ("pdf Title:" + hierarchical_title).c_str());
                                myText->Clear();
                                *sector_flags[particle_key][sector] = false;
                                ++plot_counter;
                                break;
                            }
                        }
                    }

                    break;  // Stop checking other particles after match
                }
            }

            myCanvas->cd();

            myCanvas->cd()->SetGrid();
            myCanvas->cd()->SetBottomMargin(0.14), myCanvas->cd()->SetLeftMargin(0.16), myCanvas->cd()->SetRightMargin(0.12);

            HistoList[i]->GetYaxis()->SetTitleOffset(1.5);
            HistoList[i]->GetXaxis()->SetTitleOffset(1.1);

            // gPad->SetRightMargin(0.23);

            if (HistoList[i]->InheritsFrom("TH1D")) {
                HistoList[i]->Draw();

                if (basic_tools::FindSubstring(HistoList[i]->GetTitle(), "V_{z}^{") && !basic_tools::FindSubstring(HistoList[i]->GetTitle(), "dV_{z}^{")) {
                    gPad->Update();
                    TLine *speac_target_location_TLine;
                    double speac_target_location_value = 0.0;

                    if (target_status == "C12") {
                        speac_target_location_value = (2.5 - 3.0);
                    } else if (target_status == "Ar40") {
                        speac_target_location_value = (-2.5 - 3.0);
                    }

                    speac_target_location_TLine = new TLine(speac_target_location_value, 0., speac_target_location_value, gPad->GetFrame()->GetY2());
                    speac_target_location_TLine->SetLineColor(kBlue);
                    // speac_target_location_TLine->SetLineWidth(2);
                    speac_target_location_TLine->Draw("same");

                    TLine *measured_target_location_TLine;
                    double measured_target_location_value = HistoList[i]->GetBinCenter(HistoList[i]->GetMaximumBin());

                    measured_target_location_TLine = new TLine(measured_target_location_value, 0., measured_target_location_value, gPad->GetFrame()->GetY2());
                    measured_target_location_TLine->SetLineColor(kGreen + 1);
                    measured_target_location_TLine->SetLineWidth(3);
                    // measured_target_location_TLine->SetLineWidth(4);
                    measured_target_location_TLine->SetLineStyle(2);
                    measured_target_location_TLine->Draw("same");

                    auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25 - yOffset, gStyle->GetStatX() - 0.25, gStyle->GetStatY() - 0.375 - yOffset);
                    TLegendEntry *speac_target_location_TLine_entry =
                        Legend->AddEntry(speac_target_location_TLine, ("Spec. z pos. = " + basic_tools::ToStringWithPrecision(speac_target_location_value, 2) + " cm").c_str(), "l");
                    TLegendEntry *measured_target_location_TLine_entry =
                        Legend->AddEntry(measured_target_location_TLine, ("Meas. z pos. = " + basic_tools::ToStringWithPrecision(measured_target_location_value, 2) + " cm").c_str(), "l");

                    Legend->Draw("same");

                    auto ListOfFunctions = HistoList[i]->GetListOfFunctions();
                    ListOfFunctions->Add(speac_target_location_TLine);
                    ListOfFunctions->Add(measured_target_location_TLine);
                    ListOfFunctions->Add(Legend);
                }
            } else if (HistoList[i]->InheritsFrom("TH2D")) {
                HistoList[i]->Draw("colz");

                myCanvas->SetLogz(0);
                if (basic_tools::FindSubstring(HistoList[i]->GetName(), "PCAL") && !basic_tools::FindSubstring(HistoList[i]->GetName(), "sampling fraction")) { myCanvas->SetLogz(1); }

                if (HistoList[i]->GetEntries() != 0) {
                    gPad->Update();
                    TPaletteAxis *palette = (TPaletteAxis *)HistoList[i]->GetListOfFunctions()->FindObject("palette");
                    palette->SetY2NDC(0.55);
                    gPad->Modified();
                    gPad->Update();
                }
            }

            if (basic_tools::FindSubstring(HistoList[i]->GetTitle(), "V_{z}^{")) {
                std::string Individual_PDF_fileName = IndividualPlotsOutputDir + to_string(plot_counter) + "_" + HistoList[i]->GetName() + ".pdf";
                myCanvas->SaveAs(Individual_PDF_fileName.c_str());
                histogram_functions::FixPDFOrientation(Individual_PDF_fileName);
            }

            myCanvas->Print(fileName, "pdf");
            myCanvas->Clear();
            ++plot_counter;
        }

        sprintf(fileName, "%s]", PDF_fileName.c_str());
        myCanvas->Print(fileName, "pdf");

        histogram_functions::FixPDFOrientation(PDF_fileName);                                                             // Fix orientation
        histogram_functions::ReassignPDFBookmarks(basic_tools::GetCurrentDirectory() + "/", PDF_fileName, PDF_fileName);  // Reassign clean bookmarks

        histogram_functions::CompareHistograms({h_SF_VS_Edep_PCAL_BC_sector1_1e_cut, h_SF_VS_Edep_PCAL_BC_sector2_1e_cut, h_SF_VS_Edep_PCAL_BC_sector3_1e_cut,
                                                h_SF_VS_Edep_PCAL_BC_sector4_1e_cut, h_SF_VS_Edep_PCAL_BC_sector5_1e_cut, h_SF_VS_Edep_PCAL_BC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "01_SF_VS_Edep_PCAL_BC_BySector_1e_cut");
        histogram_functions::CompareHistograms({h_SF_VS_Edep_PCAL_AC_sector1_1e_cut, h_SF_VS_Edep_PCAL_AC_sector2_1e_cut, h_SF_VS_Edep_PCAL_AC_sector3_1e_cut,
                                                h_SF_VS_Edep_PCAL_AC_sector4_1e_cut, h_SF_VS_Edep_PCAL_AC_sector5_1e_cut, h_SF_VS_Edep_PCAL_AC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "02_SF_VS_Edep_PCAL_AC_BySector_1e_cut");

        histogram_functions::CompareHistograms({h_SF_VS_P_e_BC_sector1_1e_cut, h_SF_VS_P_e_BC_sector2_1e_cut, h_SF_VS_P_e_BC_sector3_1e_cut, h_SF_VS_P_e_BC_sector4_1e_cut,
                                                h_SF_VS_P_e_BC_sector5_1e_cut, h_SF_VS_P_e_BC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "03_SF_VS_P_e_BC_BySector_1e_cut");
        histogram_functions::CompareHistograms({h_SF_VS_P_e_AC_sector1_1e_cut, h_SF_VS_P_e_AC_sector2_1e_cut, h_SF_VS_P_e_AC_sector3_1e_cut, h_SF_VS_P_e_AC_sector4_1e_cut,
                                                h_SF_VS_P_e_AC_sector5_1e_cut, h_SF_VS_P_e_AC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "04_SF_VS_P_e_AC_BySector_1e_cut");

        histogram_functions::CompareHistograms(
            {h_Vz_e_BC_sector1_1e_cut, h_Vz_e_BC_sector2_1e_cut, h_Vz_e_BC_sector3_1e_cut, h_Vz_e_BC_sector4_1e_cut, h_Vz_e_BC_sector5_1e_cut, h_Vz_e_BC_sector6_1e_cut}, OutputDir,
            "Histogram_Comparisons", "05_Vz_e_BC_BySector_1e_cut");
        histogram_functions::CompareHistograms(
            {h_Vz_e_AC_sector1_1e_cut, h_Vz_e_AC_sector2_1e_cut, h_Vz_e_AC_sector3_1e_cut, h_Vz_e_AC_sector4_1e_cut, h_Vz_e_AC_sector5_1e_cut, h_Vz_e_AC_sector6_1e_cut}, OutputDir,
            "Histogram_Comparisons", "06_Vz_e_AC_BySector_1e_cut");

        histogram_functions::CompareHistograms({h_Vz_e_BC_zoomin_sector1_1e_cut, h_Vz_e_BC_zoomin_sector2_1e_cut, h_Vz_e_BC_zoomin_sector3_1e_cut, h_Vz_e_BC_zoomin_sector4_1e_cut,
                                                h_Vz_e_BC_zoomin_sector5_1e_cut, h_Vz_e_BC_zoomin_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "07_Vz_e_BC_zoomin_BySector_1e_cut");
        histogram_functions::CompareHistograms({h_Vz_e_AC_zoomin_sector1_1e_cut, h_Vz_e_AC_zoomin_sector2_1e_cut, h_Vz_e_AC_zoomin_sector3_1e_cut, h_Vz_e_AC_zoomin_sector4_1e_cut,
                                                h_Vz_e_AC_zoomin_sector5_1e_cut, h_Vz_e_AC_zoomin_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "08_Vz_e_AC_zoomin_BySector_1e_cut");

        histogram_functions::CompareHistograms({h_Vz_pipFD_BC_sector1_1e_cut, h_Vz_pipFD_BC_sector2_1e_cut, h_Vz_pipFD_BC_sector3_1e_cut, h_Vz_pipFD_BC_sector4_1e_cut,
                                                h_Vz_pipFD_BC_sector5_1e_cut, h_Vz_pipFD_BC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "09_Vz_pipFD_BC_BySector_1e_cut");
        histogram_functions::CompareHistograms({h_Vz_pipFD_AC_sector1_1e_cut, h_Vz_pipFD_AC_sector2_1e_cut, h_Vz_pipFD_AC_sector3_1e_cut, h_Vz_pipFD_AC_sector4_1e_cut,
                                                h_Vz_pipFD_AC_sector5_1e_cut, h_Vz_pipFD_AC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "10_Vz_pipFD_AC_BySector_1e_cut");

        histogram_functions::CompareHistograms({h_dVz_pipFD_BC_sector1_1e_cut, h_dVz_pipFD_BC_sector2_1e_cut, h_dVz_pipFD_BC_sector3_1e_cut, h_dVz_pipFD_BC_sector4_1e_cut,
                                                h_dVz_pipFD_BC_sector5_1e_cut, h_dVz_pipFD_BC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "11_dVz_pipFD_BC_BySector_1e_cut");
        histogram_functions::CompareHistograms({h_dVz_pipFD_AC_sector1_1e_cut, h_dVz_pipFD_AC_sector2_1e_cut, h_dVz_pipFD_AC_sector3_1e_cut, h_dVz_pipFD_AC_sector4_1e_cut,
                                                h_dVz_pipFD_AC_sector5_1e_cut, h_dVz_pipFD_AC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "12_dVz_pipFD_AC_BySector_1e_cut");

        histogram_functions::CompareHistograms({h_Vz_pipFD_BC_zoomin_sector1_1e_cut, h_Vz_pipFD_BC_zoomin_sector2_1e_cut, h_Vz_pipFD_BC_zoomin_sector3_1e_cut,
                                                h_Vz_pipFD_BC_zoomin_sector4_1e_cut, h_Vz_pipFD_BC_zoomin_sector5_1e_cut, h_Vz_pipFD_BC_zoomin_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "13_Vz_pipFD_BC_zoomin_BySector_1e_cut");
        histogram_functions::CompareHistograms({h_Vz_pipFD_AC_zoomin_sector1_1e_cut, h_Vz_pipFD_AC_zoomin_sector2_1e_cut, h_Vz_pipFD_AC_zoomin_sector3_1e_cut,
                                                h_Vz_pipFD_AC_zoomin_sector4_1e_cut, h_Vz_pipFD_AC_zoomin_sector5_1e_cut, h_Vz_pipFD_AC_zoomin_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "14_Vz_pipFD_AC_zoomin_BySector_1e_cut");

        histogram_functions::CompareHistograms({h_dVz_pipFD_BC_zoomin_sector1_1e_cut, h_dVz_pipFD_BC_zoomin_sector2_1e_cut, h_dVz_pipFD_BC_zoomin_sector3_1e_cut,
                                                h_dVz_pipFD_BC_zoomin_sector4_1e_cut, h_dVz_pipFD_BC_zoomin_sector5_1e_cut, h_dVz_pipFD_BC_zoomin_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "15_dVz_pipFD_BC_zoomin_BySector_1e_cut");
        histogram_functions::CompareHistograms({h_dVz_pipFD_AC_zoomin_sector1_1e_cut, h_dVz_pipFD_AC_zoomin_sector2_1e_cut, h_dVz_pipFD_AC_zoomin_sector3_1e_cut,
                                                h_dVz_pipFD_AC_zoomin_sector4_1e_cut, h_dVz_pipFD_AC_zoomin_sector5_1e_cut, h_dVz_pipFD_AC_zoomin_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "16_dVz_pipFD_AC_zoomin_BySector_1e_cut");

        histogram_functions::CompareHistograms({h_Vz_pimFD_BC_sector1_1e_cut, h_Vz_pimFD_BC_sector2_1e_cut, h_Vz_pimFD_BC_sector3_1e_cut, h_Vz_pimFD_BC_sector4_1e_cut,
                                                h_Vz_pimFD_BC_sector5_1e_cut, h_Vz_pimFD_BC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "17_Vz_pimFD_BC_BySector_1e_cut");
        histogram_functions::CompareHistograms({h_Vz_pimFD_AC_sector1_1e_cut, h_Vz_pimFD_AC_sector2_1e_cut, h_Vz_pimFD_AC_sector3_1e_cut, h_Vz_pimFD_AC_sector4_1e_cut,
                                                h_Vz_pimFD_AC_sector5_1e_cut, h_Vz_pimFD_AC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "18_Vz_pimFD_AC_BySector_1e_cut");

        histogram_functions::CompareHistograms({h_dVz_pimFD_BC_sector1_1e_cut, h_dVz_pimFD_BC_sector2_1e_cut, h_dVz_pimFD_BC_sector3_1e_cut, h_dVz_pimFD_BC_sector4_1e_cut,
                                                h_dVz_pimFD_BC_sector5_1e_cut, h_dVz_pimFD_BC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "19_dVz_pimFD_BC_BySector_1e_cut");
        histogram_functions::CompareHistograms({h_dVz_pimFD_AC_sector1_1e_cut, h_dVz_pimFD_AC_sector2_1e_cut, h_dVz_pimFD_AC_sector3_1e_cut, h_dVz_pimFD_AC_sector4_1e_cut,
                                                h_dVz_pimFD_AC_sector5_1e_cut, h_dVz_pimFD_AC_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "20_dVz_pimFD_AC_BySector_1e_cut");

        histogram_functions::CompareHistograms({h_Vz_pimFD_BC_zoomin_sector1_1e_cut, h_Vz_pimFD_BC_zoomin_sector2_1e_cut, h_Vz_pimFD_BC_zoomin_sector3_1e_cut,
                                                h_Vz_pimFD_BC_zoomin_sector4_1e_cut, h_Vz_pimFD_BC_zoomin_sector5_1e_cut, h_Vz_pimFD_BC_zoomin_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "21_Vz_pimFD_BC_zoomin_BySector_1e_cut");
        histogram_functions::CompareHistograms({h_Vz_pimFD_AC_zoomin_sector1_1e_cut, h_Vz_pimFD_AC_zoomin_sector2_1e_cut, h_Vz_pimFD_AC_zoomin_sector3_1e_cut,
                                                h_Vz_pimFD_AC_zoomin_sector4_1e_cut, h_Vz_pimFD_AC_zoomin_sector5_1e_cut, h_Vz_pimFD_AC_zoomin_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "22_Vz_pimFD_AC_zoomin_BySector_1e_cut");

        histogram_functions::CompareHistograms({h_dVz_pimFD_BC_zoomin_sector1_1e_cut, h_dVz_pimFD_BC_zoomin_sector2_1e_cut, h_dVz_pimFD_BC_zoomin_sector3_1e_cut,
                                                h_dVz_pimFD_BC_zoomin_sector4_1e_cut, h_dVz_pimFD_BC_zoomin_sector5_1e_cut, h_dVz_pimFD_BC_zoomin_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "23_dVz_pimFD_BC_zoomin_BySector_1e_cut");
        histogram_functions::CompareHistograms({h_dVz_pimFD_AC_zoomin_sector1_1e_cut, h_dVz_pimFD_AC_zoomin_sector2_1e_cut, h_dVz_pimFD_AC_zoomin_sector3_1e_cut,
                                                h_dVz_pimFD_AC_zoomin_sector4_1e_cut, h_dVz_pimFD_AC_zoomin_sector5_1e_cut, h_dVz_pimFD_AC_zoomin_sector6_1e_cut},
                                               OutputDir, "Histogram_Comparisons", "24_dVz_pimFD_AC_zoomin_BySector_1e_cut");

        histogram_functions::CompareHistograms({h_Vz_pipFD_BC_1e_cut, h_Vz_pimFD_BC_1e_cut}, OutputDir, "Histogram_Comparisons", "25_Vz_pions_FD_BC_1e_cut");
        histogram_functions::CompareHistograms({h_Vz_pipFD_AC_1e_cut, h_Vz_pimFD_AC_1e_cut}, OutputDir, "Histogram_Comparisons", "26_Vz_pions_FD_AC_1e_cut");

        histogram_functions::CompareHistograms({h_Vz_pipFD_BC_zoomin_1e_cut, h_Vz_pimFD_BC_zoomin_1e_cut}, OutputDir, "Histogram_Comparisons", "27_Vz_pions_FD_BC_zoomin_1e_cut");
        histogram_functions::CompareHistograms({h_Vz_pipFD_AC_zoomin_1e_cut, h_Vz_pimFD_AC_zoomin_1e_cut}, OutputDir, "Histogram_Comparisons", "28_Vz_pions_FD_AC_zoomin_1e_cut");

        histogram_functions::CompareHistograms({h_dVz_pipFD_BC_1e_cut, h_dVz_pimFD_BC_1e_cut}, OutputDir, "Histogram_Comparisons", "29_dVz_pions_FD_BC_1e_cut");
        histogram_functions::CompareHistograms({h_dVz_pipFD_AC_1e_cut, h_dVz_pimFD_AC_1e_cut}, OutputDir, "Histogram_Comparisons", "30_dVz_pions_FD_AC_1e_cut");

        histogram_functions::CompareHistograms({h_Vz_pipCD_BC_1e_cut, h_Vz_pimCD_BC_1e_cut}, OutputDir, "Histogram_Comparisons", "31_Vz_pions_CD_BC_1e_cut");
        histogram_functions::CompareHistograms({h_Vz_pipCD_AC_1e_cut, h_Vz_pimCD_AC_1e_cut}, OutputDir, "Histogram_Comparisons", "32_Vz_pions_CD_AC_1e_cut");

        histogram_functions::CompareHistograms({h_Vz_pipCD_BC_zoomin_1e_cut, h_Vz_pimCD_BC_zoomin_1e_cut}, OutputDir, "Histogram_Comparisons", "33_Vz_pions_CD_BC_zoomin_1e_cut");
        histogram_functions::CompareHistograms({h_Vz_pipCD_AC_zoomin_1e_cut, h_Vz_pimCD_AC_zoomin_1e_cut}, OutputDir, "Histogram_Comparisons", "34_Vz_pions_CD_AC_zoomin_1e_cut");

        histogram_functions::CompareHistograms({h_dVz_pipCD_BC_1e_cut, h_dVz_pimCD_BC_1e_cut}, OutputDir, "Histogram_Comparisons", "35_dVz_pions_CD_BC_1e_cut");
        histogram_functions::CompareHistograms({h_dVz_pipCD_AC_1e_cut, h_dVz_pimCD_AC_1e_cut}, OutputDir, "Histogram_Comparisons", "36_dVz_pions_CD_AC_1e_cut");

        outFile->cd();
        for (int i = 0; i < HistoList.size(); i++) { HistoList[i]->Write(); }
        outFile->Close();
#pragma endregion

        // Delete all ROOT objects whose class names start with TH (to prevent a memory leak):
        if (InputFiles.size() > 1) { gDirectory->Clear(); }

        cout << "\033[33m\n=============================================================\n\033[0m";
        cout << "\033[33m\n= HipoLooper summary                                         \n\033[0m";
        cout << "\033[33m\n=============================================================\n\033[0m";

        cout << "\033[33m\n\n- Input variables -------------------------------------------\n\033[0m";
        cout << "\033[33mInputFiles.at(sample):\033[0m " << InputFiles.at(sample) << endl;
        cout << "\033[33mnCodeRun_status:\033[0m       " << CodeRun_status << endl;
        cout << "\033[33mOutputDir:\033[0m             " << OutputDir << "\n\n";
    }
}
