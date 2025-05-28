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

// Include CLAS12 libraries:
#include "framework/includes/clas12_include.h"

using namespace constants;

void HipoLooper() {
    std::cout << "\n\nInitiating HipoLooper.cpp\n";

    std::string BaseDir = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples";

    // Data samples:

    std::string InputFiles = "/cache/clas12/rg-m/production/pass1/2gev/C/dst/recon/015664/*.hipo";
    // std::string InputFiles = "/cache/clas12/rg-m/production/pass1/4gev/C/dst/recon/015778/*.hipo";

    // std::string InputFiles = "/cache/clas12/rg-m/production/pass1/2gev/Ar/dst/recon/015672/*.hipo";
    // std::string InputFiles = "/cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon/015743/*.hipo";
    // std::string InputFiles = "/cache/clas12/rg-m/production/pass1/6gev/Ar/dst/recon/015792/*.hipo";

    // Simulation samples:

    // std::string InputFiles = BaseDir + "/C12/G18_10a_00_000/2070MeV_Q2_0_02_Ar40_test/reconhipo/*.hipo";
    // std::string InputFiles = BaseDir + "/C12/G18_10a_00_000/4029MeV_Q2_0_25_Ar40_test/reconhipo/*.hipo";

    // std::string InputFiles = BaseDir + "/Ar40/G18_10a_00_000/2070MeV_Q2_0_02_Ar40_test/reconhipo/*.hipo";
    // std::string InputFiles = BaseDir + "/Ar40/G18_10a_00_000/4029MeV_Q2_0_25_Ar40_test/reconhipo/*.hipo";
    // std::string InputFiles = BaseDir + "/Ar40/G18_10a_00_000/5986MeV_Q2_0_40_Ar40_test/reconhipo/*.hipo";

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
    int Limiter = 1000000;  // 100 files
    // int Limiter = 100000;  // 10 files
    // int Limiter = 10000; // 1 file

    std::string OutFolderName_prefix = "03_HipoLooper";
    std::string OutFolderName_ver_status = "_v3_";
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

    /////////////////////////////////////
    // Prepare histograms
    /////////////////////////////////////
    vector<TH1 *> HistoList_electron_cuts;

    gStyle->SetTitleXSize(0.05);
    gStyle->SetTitleYSize(0.05);

    gStyle->SetTitleXOffset(0.8);
    gStyle->SetTitleYOffset(0.8);

    char temp_name[100];
    char temp_title[100];

    TH1D *h_Vz_e_BC_1e_cut =
        new TH1D("Vz_e_BC_1e_cut",
                 ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList_electron_cuts.push_back(h_Vz_e_BC_1e_cut);
    TH1D *h_Vz_e_AC_1e_cut =
        new TH1D("Vz_e_AC_1e_cut",
                 ("V_{z}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{z}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList_electron_cuts.push_back(h_Vz_e_AC_1e_cut);
    TH1D *h_Vx_e_BC_1e_cut =
        new TH1D("Vx_e_BC_1e_cut",
                 ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList_electron_cuts.push_back(h_Vx_e_BC_1e_cut);
    TH1D *h_Vx_e_AC_1e_cut =
        new TH1D("Vx_e_AC_1e_cut",
                 ("V_{x}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{x}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList_electron_cuts.push_back(h_Vx_e_AC_1e_cut);
    TH1D *h_Vy_e_BC_1e_cut =
        new TH1D("Vy_e_BC_1e_cut",
                 ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (before cut);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList_electron_cuts.push_back(h_Vy_e_BC_1e_cut);
    TH1D *h_Vy_e_AC_1e_cut =
        new TH1D("Vy_e_AC_1e_cut",
                 ("V_{y}^{e} in 1e cut - " + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + " (after cut);V_{y}^{e} [cm];Counts").c_str(), 75, -10, 5);
    HistoList_electron_cuts.push_back(h_Vy_e_AC_1e_cut);

    TH2D *h_dc_electron_hit_map_BC_1e_cut[4];  // 3 regions
    TH2D *h_dc_electron_hit_map_AC_1e_cut[4];  // 3 regions

    // DC hit maps
    for (int i = 1; i <= 3; i++) {
        h_dc_electron_hit_map_BC_1e_cut[i] = new TH2D(Form("dc_electron_hit_map_BC_%d", i), Form("DC hitmap in region %d (before cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_electron_cuts.push_back(h_dc_electron_hit_map_BC_1e_cut[i]);
        h_dc_electron_hit_map_AC_1e_cut[i] = new TH2D(Form("dc_electron_hit_map_AC_%d", i), Form("DC hitmap in region %d (after cuts);x [cm];y [cm]", i), 600, -300, 300, 600, -300, 300);
        HistoList_electron_cuts.push_back(h_dc_electron_hit_map_AC_1e_cut[i]);
    }

    TH1D *h_nphe_BC_1e_cut = new TH1D("nphe_BC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (before cut);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList_electron_cuts.push_back(h_nphe_BC_1e_cut);
    TH1D *h_nphe_AC_1e_cut = new TH1D("nphe_AC_1e_cut", "Number of photo-electrons in HTCC in 1e cut (after cut);Number of photo-electrons;Counts", 20, 0, 20);
    HistoList_electron_cuts.push_back(h_nphe_AC_1e_cut);

    TH2D *h_Edep_PCAL_VS_EC_BC_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_BC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (before cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0,
                 0.2, 100, 0, 0.3);
    HistoList_electron_cuts.push_back(h_Edep_PCAL_VS_EC_BC_1e_cut);
    TH2D *h_Edep_PCAL_VS_EC_AC_1e_cut =
        new TH2D("Edep_PCAL_VS_EC_AC_1e_cut", "E_{dep}^{PCAL} vs. E_{dep}^{EC} in 1e cut (after cut);E_{dep}^{PCAL} [GeV];E_{dep}^{EC} = E_{dep}^{ECIN} + E_{dep}^{ECOUT} [GeV]", 100, 0, 0.2,
                 100, 0, 0.3);
    HistoList_electron_cuts.push_back(h_Edep_PCAL_VS_EC_AC_1e_cut);

    TH2D *h_SF_VS_P_e_BC_1e_cut =
        new TH2D("SF_VS_P_e_BC_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (before cut);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_P_e_BC_1e_cut);
    TH2D *h_SF_VS_P_e_AC_1e_cut =
        new TH2D("SF_VS_P_e_AC_1e_cut", "Electron sampling fraction vs. P_{e} in 1e cut (after cut);P_{e} [GeV/c];Electron sampling fraction", 100, 0, Ebeam * 1.1, 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_P_e_AC_1e_cut);

    TH2D *h_SF_VS_Lv_BC_1e_cut = new TH2D("SF_VS_Lv_BC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (before cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lv_BC_1e_cut);
    TH2D *h_SF_VS_Lv_AC_1e_cut = new TH2D("SF_VS_Lv_AC_1e_cut", "Electron SF vs. PCAL V coor. in 1e cut (after cut);PCAL V coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lv_AC_1e_cut);

    TH2D *h_SF_VS_Lw_BC_1e_cut = new TH2D("SF_VS_Lw_BC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (before cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lw_BC_1e_cut);
    TH2D *h_SF_VS_Lw_AC_1e_cut = new TH2D("SF_VS_Lw_AC_1e_cut", "Electron SF vs. PCAL W coor. in 1e cut (after cut);PCAL W coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lw_AC_1e_cut);

    TH2D *h_SF_VS_Lu_BC_1e_cut = new TH2D("SF_VS_Lu_BC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (before cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lu_BC_1e_cut);
    TH2D *h_SF_VS_Lu_AC_1e_cut = new TH2D("SF_VS_Lu_AC_1e_cut", "Electron SF vs. PCAL U coor. in 1e cut (after cut);PCAL U coor. [cm];Electron SF", 100, 0, 60., 100, 0.16, 0.3);
    HistoList_electron_cuts.push_back(h_SF_VS_Lu_AC_1e_cut);

    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_BC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (before cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100, 0, 0.35);
    HistoList_electron_cuts.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut);
    TH2D *h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut =
        new TH2D("E_PCALoP_e_VS_E_PCALoP_e_AC", "E_{dep}^{PCAL}/P_{e} vs. E_{dep}^{ECIN}/P_{e} in 1e cut (after cut);E_{dep}^{PCAL}/P_{e};E_{dep}^{ECIN}/P_{e}", 100, 0, 0.3, 100, 0, 0.35);
    HistoList_electron_cuts.push_back(h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut);

    int counter = 0;

    while (chain.Next() == true) {
        // Display completed
        ++counter;
        if ((counter % 1000000) == 0) { std::cerr << "\n" << counter / 1000000 << " million completed"; }
        if ((counter % 100000) == 0) { std::cerr << "."; }

        if ((ApplyLimiter && counter > Limiter)) { break; }

        // get particles by type
        auto allParticles = c12->getDetParticles();
        auto electrons = c12->getByID(11);

        if (electrons.size() != 1) { continue; }

        double weight = 1;

        double starttime = c12->event()->getStartTime();

        TVector3 P_b(0, 0, Ebeam);

        TVector3 reco_P_e;
        reco_P_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());

        double vtz_e = electrons[0]->par()->getVz();

        TVector3 P_q = P_b - reco_P_e;

        double nu = Ebeam - reco_P_e.Mag();
        double QSq = P_q.Mag2() - (nu * nu);
        double xB = QSq / (2 * m_n * nu);
        double WSq = (m_n * m_n) - QSq + (2 * nu * m_n);
        double theta_e = reco_P_e.Theta() * 180 / M_PI;
        double EoP_e = (electrons[0]->cal(clas12::PCAL)->getEnergy() + electrons[0]->cal(ECIN)->getEnergy() + electrons[0]->cal(ECOUT)->getEnergy()) / reco_P_e.Mag();
        double E_PCALoP_e = electrons[0]->cal(clas12::PCAL)->getEnergy() / reco_P_e.Mag();
        double E_ECINoP_e = electrons[0]->cal(clas12::ECIN)->getEnergy() / reco_P_e.Mag();
        double Edep_PCAL = electrons[0]->cal(clas12::PCAL)->getEnergy();
        double Edep_EC = electrons[0]->cal(clas12::ECIN)->getEnergy() + electrons[0]->cal(clas12::ECOUT)->getEnergy();

        bool ElectronInPCAL = (electrons[0]->cal(clas12::PCAL)->getDetector() == 7);                                  // PCAL hit
        bool ElectronInECIN = (electrons[0]->cal(clas12::ECIN)->getDetector() == 7);                                  // ECIN hit
        bool ElectronInECOUT = (electrons[0]->cal(clas12::ECOUT)->getDetector() == 7);                                // ECOUT hit
        auto Electron_ECAL_detlayer = ElectronInPCAL ? clas12::PCAL : ElectronInECIN ? clas12::ECIN : clas12::ECOUT;  // find first layer of hit

        //  =======================================================================================================================================================================
        //  1e cut (reco)
        //  =======================================================================================================================================================================

        //  Electron PID cuts
        //  -----------------------------------------------------------------------------------------------------------------------------------------------------

        h_Vx_e_BC_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
        h_Vy_e_BC_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
        h_Vz_e_BC_1e_cut->Fill(electrons[0]->par()->getVz(), weight);
        // bool bad_Vz_e_CutCond = (electrons[0]->par()->getVz() < -4. || electrons[0]->par()->getVz() > -2.);
        // // bool bad_Vz_e_CutCond = (electrons[0]->par()->getVz() < -6. ||
        // // electrons[0]->par()->getVz() > 0.);
        // if (!bad_Vz_e_CutCond) {
        //     h_Vx_e_AC_1e_cut->Fill(electrons[0]->par()->getVx(), weight);
        //     h_Vy_e_AC_1e_cut->Fill(electrons[0]->par()->getVy(), weight);
        //     h_Vz_e_AC_1e_cut->Fill(electrons[0]->par()->getVz(), weight);
        // }

        // fillDCdebug(electrons[0], h_dc_electron_hit_map_BC_1e_cut, weight);
        // bool bad_DC_edge_CutCond = (!DCEdgeCuts(electrons[0]));
        // if (!bad_DC_edge_CutCond) { fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_1e_cut, weight); }

        h_nphe_BC_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);
        // bool bad_nphe_CutCond = (electrons[0]->che(clas12::HTCC)->getNphe() <= 2);
        // if (!bad_nphe_CutCond) { h_nphe_AC_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight); }

        h_Edep_PCAL_VS_EC_BC_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);
        // bool bad_Edep_PCAL_CutCond = (Edep_PCAL <= 0.06);
        // if (!bad_Edep_PCAL_CutCond) { h_Edep_PCAL_VS_EC_AC_1e_cut->Fill(Edep_PCAL, Edep_EC, weight); }

        h_SF_VS_P_e_BC_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);
        // bool bad_SF_CutCond = (EoP_e < 0.2 || EoP_e > 0.28);
        // if (!bad_SF_CutCond) { h_SF_VS_P_e_AC_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight); }

        h_SF_VS_Lv_BC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
        h_SF_VS_Lw_BC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
        h_SF_VS_Lu_BC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
        // bool bad_PCAL_edge_CutCond = (electrons[0]->cal(clas12::PCAL)->getLv() < 14. || electrons[0]->cal(clas12::PCAL)->getLw() < 14.);
        // if (!bad_PCAL_edge_CutCond) {
        //     h_SF_VS_Lv_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
        //     h_SF_VS_Lw_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
        //     h_SF_VS_Lu_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);
        // }

        h_E_PCALoP_e_VS_E_PCALoP_e_BC_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
        // bool bad_diag_CutCond = (!checkEcalDiagCuts(electrons[0]));
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
    }

    std::cout << "\n\nPlotting and saving histograms..." << "\n\n";

    /////////////////////////////////////////////////////
    // Organize histograms
    /////////////////////////////////////////////////////
    for (int i = 0; i < HistoList_electron_cuts.size(); i++) {
        if (HistoList_electron_cuts[i]->InheritsFrom("TH1D")) {
            HistoList_electron_cuts[i]->Sumw2();
            HistoList_electron_cuts[i]->SetMinimum(0);
            HistoList_electron_cuts[i]->SetLineWidth(2);
            HistoList_electron_cuts[i]->SetLineColor(kRed);
        }

        HistoList_electron_cuts[i]->GetXaxis()->CenterTitle();
        HistoList_electron_cuts[i]->GetYaxis()->CenterTitle();
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
    text.SetTextSize(0.025);

    gStyle->SetOptStat("ourmen");

    TCanvas *myCanvas_electron_cuts = new TCanvas("myPage_electron_cuts", "myPage_electron_cuts", pixelx, pixely);

    std::string electron_cuts_PDF_fileName = "/lustre24/expphy/volatile/clas12/asportes/Analysis_output/" + OutFolderName + "/electron_cuts.pdf";
    char fileName_electron_cuts[electron_cuts_PDF_fileName.length()];
    sprintf(fileName_electron_cuts, "%s[", electron_cuts_PDF_fileName.c_str());
    myText->SaveAs(fileName_electron_cuts);
    sprintf(fileName_electron_cuts, "%s", electron_cuts_PDF_fileName.c_str());

    /////////////////////////////////////
    // CND Neutron Information
    /////////////////////////////////////
    myText->cd();

    titles.DrawLatex(0.05, 0.9, ("#splitline{Plots from (e,e') events in}{" + target_status + sample_type_status + genie_tune_status + Ebeam_status + Run_status + "}").c_str());

    if (IsData) {
        text.DrawLatex(0.05, 0.7, ("InputFiles: " + InputFiles).c_str());
    } else {
        text.DrawLatex(0.05, 0.7, ("BaseDir: " + BaseDir).c_str());
        text.DrawLatex(0.05, 0.65, ("InputFiles: BaseDir + " + InputFiles.substr(BaseDir.length())).c_str());
    }

    myText->Print(fileName_electron_cuts, "pdf");
    myText->Clear();

    for (int i = 0; i < HistoList_electron_cuts.size(); i++) {
        myCanvas_electron_cuts->cd();

        myCanvas_electron_cuts->cd()->SetGrid();
        myCanvas_electron_cuts->cd()->SetBottomMargin(0.14), myCanvas_electron_cuts->cd()->SetLeftMargin(0.16), myCanvas_electron_cuts->cd()->SetRightMargin(0.12);

        HistoList_electron_cuts[i]->GetYaxis()->SetTitleOffset(1.5);
        HistoList_electron_cuts[i]->GetXaxis()->SetTitleOffset(1.1);

        gPad->SetRightMargin(0.23);

        if (HistoList_electron_cuts[i]->InheritsFrom("TH1D")) {
            HistoList_electron_cuts[i]->Draw();

            gPad->Update();
            TLine *target_location;
            double spec_target_location = 0.0;

            if (target_status == "C12") {
                spec_target_location = (2.5 - 3.0);
            } else if (target_status == "Ar40") {
                spec_target_location = (-2.5 - 3.0);
            }

            target_location = new TLine(spec_target_location, 0., spec_target_location, gPad->GetFrame()->GetY2());
            target_location->SetLineColor(kBlue);
            target_location->SetLineWidth(2);
            target_location->Draw("same");

            auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25, gStyle->GetStatX() - 0.25, gStyle->GetStatY() - 0.3);
            TLegendEntry *target_location_entry = Legend->AddEntry(target_location, ("Spec target pos. = " + basic_tools::ToStringWithPrecision(spec_target_location,2) + " cm").c_str(), "l");

            Legend->Draw("same");
        } else if (HistoList_electron_cuts[i]->InheritsFrom("TH2D")) {
            HistoList_electron_cuts[i]->Draw("colz");

            if (HistoList_electron_cuts[i]->GetEntries() != 0) {
                gPad->Update();
                TPaletteAxis *palette = (TPaletteAxis *)HistoList_electron_cuts[i]->GetListOfFunctions()->FindObject("palette");
                palette->SetY2NDC(0.475);
                gPad->Modified();
                gPad->Update();
            }
        }

        myCanvas_electron_cuts->Print(fileName_electron_cuts, "pdf");
        myCanvas_electron_cuts->Clear();
    }

    sprintf(fileName_electron_cuts, "%s]", electron_cuts_PDF_fileName.c_str());
    myCanvas_electron_cuts->Print(fileName_electron_cuts, "pdf");

    outFile->cd();
    for (int i = 0; i < HistoList_electron_cuts.size(); i++) { HistoList_electron_cuts[i]->Write(); }
    outFile->Close();
}
