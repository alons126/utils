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
// #include "framework/classes/hPlots/hsPlots.cpp"

// Include CLAS12 libraries:
#include "framework/includes/clas12_include.h"

using namespace constants;

namespace bt = basic_tools;
namespace am = analysis_math;
namespace raf = reco_analysis_functions;
namespace hf = histogram_functions;

void HipoLooper() {
    auto start = std::chrono::system_clock::now();  // Start counting running time

    std::cout << "\033[33m" << "\n\nInitiating HipoLooper.cpp...\n\n" << "\033[0m";

    int version = 22;  // Version of the code
    std::string OutFolderName_prefix = bt::ToStringWithPrecision(version, 0) + "_HipoLooper";
    std::string OutFolderName_ver_status = "_v" + bt::ToStringWithPrecision(version, 0) + "_";

    std::string General_status = "after_sampling_test_3";  // General status of the analysis
    // std::string General_status = "Ar40_test_2_full";  // General status of the analysis

    General_status = "__" + General_status;

    bool ApplyLimiter = true;
    // bool ApplyLimiter = true;
    int Limiter = 10000000;  // 10M events (fo the data)
    // int Limiter = 1000000;  // 100 files or 1M events (fo the data)
    // int Limiter = 100000;  // 10 files or 100K events (fo the data)
    // int Limiter = 10000;  // 1 file

    std::string BaseDir = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples";

    std::vector<std::string> InputFiles;

    // // Data samples:

    // InputFiles.push_back("/cache/clas12/rg-m/production/pass1/2gev/C/dst/recon/015664/*.hipo");
    // InputFiles.push_back("/cache/clas12/rg-m/production/pass1/4gev/C/dst/recon/015778/*.hipo");

    // InputFiles.push_back("/cache/clas12/rg-m/production/pass1/2gev/Ar/dst/recon/015672/*.hipo");
    InputFiles.push_back("/cache/clas12/rg-m/production/pass1/4gev/Ar/dst/recon/015743/*.hipo");
    // InputFiles.push_back("/cache/clas12/rg-m/production/pass1/6gev/Ar/dst/recon/015792/*.hipo");

    // // Simulation samples:

    // InputFiles.push_back(BaseDir + "/C12/G18_10a_00_000/2070MeV_Q2_0_02_Ar40_test_2/reconhipo/*.hipo");
    // InputFiles.push_back(BaseDir + "/C12/G18_10a_00_000/4029MeV_Q2_0_25_Ar40_test_2/reconhipo/*.hipo");
    // // InputFiles.push_back(BaseDir + "/C12/G18_10a_00_000/2070MeV_Q2_0_02_Ar40_test/reconhipo/*.hipo");
    // // InputFiles.push_back(BaseDir + "/C12/G18_10a_00_000/4029MeV_Q2_0_25_Ar40_test/reconhipo/*.hipo");

    // InputFiles.push_back(BaseDir + "/Ar40/G18_10a_00_000/2070MeV_Q2_0_02_Ar40_test_2/reconhipo/*.hipo");
    // InputFiles.push_back(BaseDir + "/Ar40/G18_10a_00_000/4029MeV_Q2_0_25_Ar40_test_2/reconhipo/*.hipo");
    // InputFiles.push_back(BaseDir + "/Ar40/G18_10a_00_000/5986MeV_Q2_0_40_Ar40_test_2/reconhipo/*.hipo");
    // // InputFiles.push_back(BaseDir + "/Ar40/G18_10a_00_000/2070MeV_Q2_0_02_Ar40_test/reconhipo/*.hipo");
    // // InputFiles.push_back(BaseDir + "/Ar40/G18_10a_00_000/4029MeV_Q2_0_25_Ar40_test/reconhipo/*.hipo");
    // // InputFiles.push_back(BaseDir + "/Ar40/G18_10a_00_000/5986MeV_Q2_0_40_Ar40_test/reconhipo/*.hipo");

    for (int sample = 0; sample < InputFiles.size(); sample++) {
#pragma region Setup and configuration
        bool IsData = bt::FindSubstring(InputFiles.at(sample), "cache");

        double Ebeam = (bt::FindSubstring(InputFiles.at(sample), "2070MeV") || bt::FindSubstring(InputFiles.at(sample), "2gev"))   ? 2.07052
                       : (bt::FindSubstring(InputFiles.at(sample), "4029MeV") || bt::FindSubstring(InputFiles.at(sample), "4gev")) ? 4.02962
                       : (bt::FindSubstring(InputFiles.at(sample), "5986MeV") || bt::FindSubstring(InputFiles.at(sample), "6gev")) ? 5.98636
                                                                                                                                   : 0.0;
        if (Ebeam == 0.0) {
            std::cerr << "\n\nError! Ebeam not found in InputFiles string! Aborting...\n\n";
            exit(1);
        }

        bool Is2GeV = (bt::FindSubstring(InputFiles.at(sample), "2070MeV") || bt::FindSubstring(InputFiles.at(sample), "2gev"));
        bool Is4GeV = (bt::FindSubstring(InputFiles.at(sample), "4029MeV") || bt::FindSubstring(InputFiles.at(sample), "4gev"));
        bool Is6GeV = (bt::FindSubstring(InputFiles.at(sample), "5986MeV") || bt::FindSubstring(InputFiles.at(sample), "6gev"));

        std::string target_status = (bt::FindSubstring(InputFiles.at(sample), "/C12/") || bt::FindSubstring(InputFiles.at(sample), "/C/"))     ? "C12"
                                    : (bt::FindSubstring(InputFiles.at(sample), "/Ar40/") || bt::FindSubstring(InputFiles.at(sample), "/Ar/")) ? "Ar40"
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

        std::string Run_status = bt::FindSubstring(InputFiles.at(sample), "015664")   ? "_run_015664"
                                 : bt::FindSubstring(InputFiles.at(sample), "015778") ? "_run_015778"
                                 : bt::FindSubstring(InputFiles.at(sample), "015672") ? "_run_015672"
                                 : bt::FindSubstring(InputFiles.at(sample), "015743") ? "_run_015743"
                                 : bt::FindSubstring(InputFiles.at(sample), "015792") ? "_run_015792"
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
        // TFile *outFile_ByThetaSlices = new TFile((OutputDir + "/" + OutFileName + "_ByThetaSlices.root").c_str(), "RECREATE");

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
        vector<TObject *> HistoList;
        vector<TObject *> HistoList_ByThetaSlices;

        gStyle->SetTitleXSize(0.05);
        gStyle->SetTitleYSize(0.05);

        gStyle->SetTitleXOffset(0.8);
        gStyle->SetTitleYOffset(0.8);

        char temp_name[100];
        char temp_title[100];

        std::vector<double> HistoList_zoomin_limits;

        if (target_status == "Ar40") {
            HistoList_zoomin_limits = {-8.0, -4.0};
        } else if (target_status == "C12") {
            HistoList_zoomin_limits = {-4.0, 1.0};
        }

        std::map<std::string, std::pair<double, double>> Beam_Coordinates;  // {Vx peak, Vy peak} in cm
        Beam_Coordinates["C12_data_2GeV_run_015664_e"] = {0.1629, 0.0560};
        Beam_Coordinates["C12_data_2GeV_run_015664_pipFD"] = {0.1749, 0.0985};
        Beam_Coordinates["C12_data_2GeV_run_015664_pimFD"] = {0.1707, 0.0997};

        Beam_Coordinates["C12_data_4GeV_run_015778_e"] = {0.1658, 0.1368};
        Beam_Coordinates["C12_data_4GeV_run_015778_pipFD"] = {0.1767, 0.1357};
        Beam_Coordinates["C12_data_4GeV_run_015778_pimFD"] = {0.1624, 0.1517};

        // Beam_Coordinates["Ar40_data_2GeV_run_015672_e"] = {0.1600, 0.1600};  // zoom-out peak
        // Beam_Coordinates["Ar40_data_2GeV_run_015672_pipFD"] = {0.1600, 0.1600};
        // Beam_Coordinates["Ar40_data_2GeV_run_015672_pimFD"] = {0.1600, 0.1600};
        // Beam_Coordinates["Ar40_data_2GeV_run_015672_e"] = {0.1825, 0.1691}; // mean
        // Beam_Coordinates["Ar40_data_2GeV_run_015672_pipFD"] = {0.1389, 0.06858};
        // Beam_Coordinates["Ar40_data_2GeV_run_015672_pimFD"] = {0.1102, 0.1984};
        Beam_Coordinates["Ar40_data_2GeV_run_015672_e"] = {0.1485, 0.1275}; // zoom-in peak fit
        Beam_Coordinates["Ar40_data_2GeV_run_015672_pipFD"] = {0.1560, 0.1201};
        Beam_Coordinates["Ar40_data_2GeV_run_015672_pimFD"] = {0.1444, 0.1260};

        Beam_Coordinates["Ar40_data_4GeV_run_015743_e"] = {0.1661, 0.1421};
        Beam_Coordinates["Ar40_data_4GeV_run_015743_pipFD"] = {0.1785, 0.1400};
        Beam_Coordinates["Ar40_data_4GeV_run_015743_pimFD"] = {0.1638, 0.1521};

        Beam_Coordinates["Ar40_data_6GeV_run_015792_e"] = {0.1564, 0.1469};
        Beam_Coordinates["Ar40_data_6GeV_run_015792_pipFD"] = {0.1667, 0.1388};
        Beam_Coordinates["Ar40_data_6GeV_run_015792_pimFD"] = {0.1455, 0.1524};

        // Lambda to compute r = sqrt(Vx² + Vy²)
        auto compute_r = [&CodeRun_status, &IsData](const std::map<std::string, std::pair<double, double>> &Beam_Coor, const std::string &particle) -> double {
            if (IsData) {
                std::string key = CodeRun_status + "_" + particle;

                if (Beam_Coor.count(key) == 0) {
                    std::cerr << "\n\nError! Beam_Coor does not contain key: " << key << ". Aborting...\n\n";
                    return std::numeric_limits<double>::quiet_NaN();
                }

                double Vx_peak = Beam_Coor.at(key).first;
                double Vy_peak = Beam_Coor.at(key).second;

                if (Vx_peak == 0. || Vy_peak == 0.) {
                    std::cerr << "\n\nError! Vx_peak is for " << particle << " zero! Aborting...\n\n";
                    std::cerr << "CodeRun_status: " << CodeRun_status << " Vx_peak: " << Vx_peak << " Vy_peak: " << Vy_peak << std::endl;
                }

                return std::sqrt(Vx_peak * Vx_peak + Vy_peak * Vy_peak);
            } else {
                return 0;
            }
        };

        // Lambda to compute phi_beam in radians, using atan2 for correct quadrant
        auto compute_phi_beam_rad = [&CodeRun_status, &IsData](const std::map<std::string, std::pair<double, double>> &Beam_Coor, const std::string &particle) -> double {
            if (IsData) {
                std::string key = CodeRun_status + "_" + particle;

                if (Beam_Coor.count(key) == 0) {
                    std::cerr << "\n\nError! Beam_Coor does not contain key: " << key << ". Aborting...\n\n";
                    return std::numeric_limits<double>::quiet_NaN();
                }

                double Vx_peak = Beam_Coor.at(key).first;
                double Vy_peak = Beam_Coor.at(key).second;

                if (Vx_peak == 0. || Vy_peak == 0.) {
                    std::cerr << "\n\nError! Vx_peak is for " << particle << " zero! Aborting...\n\n";
                    std::cerr << "CodeRun_status: " << CodeRun_status << " Vx_peak: " << Vx_peak << " Vy_peak: " << Vy_peak << std::endl;
                }

                // Adjusted phi_beam_rad from a coordinate system where phi_beam_rad is in the [0, 2pi] range to a system where it is in the [-pi, pi] range
                return std::atan2(Vy_peak, Vx_peak) - am::pi;
                // return std::atan2(Vy_peak, Vx_peak);
            } else {
                return 0;
            }
        };

        // Lambda to compute corrected Vz
        auto correct_Vz = [](double Vz_rec, double r, double theta_particle_rad, double phi_particle_rad, double phi_beam_rad) -> double {
            return Vz_rec + (r / std::tan(theta_particle_rad)) * std::cos(phi_particle_rad - phi_beam_rad);
        };

#pragma region electron histograms

#pragma region electron histograms - all sectors
        TH1D *h_Vz_e_BC_1e_cut = new TH1D("Vz_e_BC_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_BC_1e_cut);
        TH1D *h_Vz_e_AC_1e_cut = new TH1D("Vz_e_AC_1e_cut", ("V_{z}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts);V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_Vz_e_AC_1e_cut);

        // hsPlots h_Vz_e_AC_1e_cut_BySliceOf = hsPlots(theta_slices, hsPlots::TH1D_TYPE, HistoList_ByThetaSlices, "Vz_e_AC_1e_cut_BySliceOf",
        //                                              "V_{z}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts);V_{z}^{e} [cm];Counts", 75, -9, 2, 75, -9, 2, "#theta_{e}
        //                                              [#circ]");

        TH1D *h_Vz_e_BC_zoomin_1e_cut = new TH1D("Vz_e_BC_zoomin_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before e^{-} cuts);V_{z}^{e} [cm];Counts").c_str(), 75,
                                                 HistoList_zoomin_limits.at(0), HistoList_zoomin_limits.at(1));
        HistoList.push_back(h_Vz_e_BC_zoomin_1e_cut);
        TH1D *h_Vz_e_AC_zoomin_1e_cut = new TH1D("Vz_e_AC_zoomin_1e_cut", ("V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts);V_{z}^{e} [cm];Counts").c_str(), 75,
                                                 HistoList_zoomin_limits.at(0), HistoList_zoomin_limits.at(1));
        HistoList.push_back(h_Vz_e_AC_zoomin_1e_cut);

        // hsPlots h_Vz_e_AC_zoomin_1e_cut_BySliceOf =
        //     hsPlots(theta_slices, hsPlots::TH1D_TYPE, HistoList_ByThetaSlices, "Vz_e_AC_zoomin_1e_cut_BySliceOf",
        //             "V_{z}^{e} in (e,e') - zoomin - " + CodeRun_status + " (after e^{-} cuts);V_{z}^{e} [cm];Counts", 75, HistoList_zoomin_limits.at(0), HistoList_zoomin_limits.at(1),
        //             75, HistoList_zoomin_limits.at(0), HistoList_zoomin_limits.at(1), "#theta_{e} [#circ]");

        // TH1D *h_corrected_Vz_e_BC_1e_cut =
        //     new TH1D("corrected_Vz_e_BC_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_e_BC_1e_cut);
        TH1D *h_corrected_Vz_e_AC_1e_cut =
            new TH1D("corrected_Vz_e_AC_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_e_AC_1e_cut);

        TH1D *h_corrected_Vz_e_BC_zoomin_1e_cut, *h_corrected_Vz_e_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_e_BC_zoomin_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_1e_cut);
            h_corrected_Vz_e_AC_zoomin_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_1e_cut", ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);Corrected V_{z}^{e} [cm];Counts").c_str(),
                         75, -8, -4);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_e_BC_zoomin_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_1e_cut);
            h_corrected_Vz_e_AC_zoomin_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_1e_cut", ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);Corrected V_{z}^{e} [cm];Counts").c_str(),
                         75, -4, 1);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_1e_cut);
        }

        TH1D *h_Vx_e_BC_1e_cut = new TH1D("Vx_e_BC_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_BC_1e_cut);
        TH1D *h_Vx_e_AC_1e_cut = new TH1D("Vx_e_AC_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_AC_1e_cut);
        TH1D *h_Vy_e_BC_1e_cut = new TH1D("Vy_e_BC_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_BC_1e_cut);
        TH1D *h_Vy_e_AC_1e_cut = new TH1D("Vy_e_AC_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_AC_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_1e_cut = new TH2D(
            "Vz_VS_phi_e_AC_1e_cut", ("V_{z}^{e} vs. #phi_{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_1e_cut);

        TH2D *h_Vz_VS_theta_e_AC_1e_cut = new TH2D(
            "Vz_VS_theta_e_AC_1e_cut", ("V_{z}^{e} vs. #theta_{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts);#theta_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100, 0, 50, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_theta_e_AC_1e_cut);

        TH2D *h_corrected_Vz_VS_phi_e_AC_1e_cut = new TH2D(
            "corrected_Vz_VS_phi_e_AC_1e_cut", ("Corrected V_{z}^{e} vs. #phi_{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts);#phi_{e} [#circ];Corrected V_{z}^{e} [cm]").c_str(),
            100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_corrected_Vz_VS_phi_e_AC_1e_cut);

        TH2D *h_corrected_Vz_VS_theta_e_AC_1e_cut = new TH2D(
            "corrected_Vz_VS_theta_e_AC_1e_cut",
            ("Corrected V_{z}^{e} vs. #theta_{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts);#theta_{e} [#circ];Corrected V_{z}^{e} [cm]").c_str(), 100, 0, 50, 100, -13, 2);
        HistoList.push_back(h_corrected_Vz_VS_theta_e_AC_1e_cut);

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

        // TH1D *h_corrected_Vz_e_BC_sector1_1e_cut = new TH1D(
        //     "corrected_Vz_e_BC_sector1_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector1);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9,
        //     2);
        // HistoList.push_back(h_corrected_Vz_e_BC_sector1_1e_cut);
        TH1D *h_corrected_Vz_e_AC_sector1_1e_cut = new TH1D(
            "corrected_Vz_e_AC_sector1_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector1);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_e_AC_sector1_1e_cut);

        TH1D *h_corrected_Vz_e_BC_zoomin_sector1_1e_cut, *h_corrected_Vz_e_AC_zoomin_sector1_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_e_BC_zoomin_sector1_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector1_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector1);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector1_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector1_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector1_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector1);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector1_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_e_BC_zoomin_sector1_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector1_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector1);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector1_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector1_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector1_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector1);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector1_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector1_1e_cut =
            new TH1D("Vx_e_BC_sector1_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector1);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_BC_sector1_1e_cut);
        TH1D *h_Vx_e_AC_sector1_1e_cut =
            new TH1D("Vx_e_AC_sector1_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector1);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_AC_sector1_1e_cut);
        TH1D *h_Vy_e_BC_sector1_1e_cut =
            new TH1D("Vy_e_BC_sector1_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector1);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_BC_sector1_1e_cut);
        TH1D *h_Vy_e_AC_sector1_1e_cut =
            new TH1D("Vy_e_AC_sector1_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector1);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_AC_sector1_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_sector1_1e_cut =
            new TH2D("Vz_VS_phi_e_AC_sector1_1e_cut", ("V_{z}^{e} vs. #phi_{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector1);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100,
                     -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_sector1_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_zoomin_sector1_1e_cut =
            new TH2D("Vz_VS_phi_e_AC_zoomin_sector1_1e_cut",
                     ("V_{z}^{e} vs. #phi_{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector1);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100, -10, 70, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_zoomin_sector1_1e_cut);

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

        // TH1D *h_corrected_Vz_e_BC_sector2_1e_cut = new TH1D(
        //     "corrected_Vz_e_BC_sector2_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector2);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9,
        //     2);
        // HistoList.push_back(h_corrected_Vz_e_BC_sector2_1e_cut);
        TH1D *h_corrected_Vz_e_AC_sector2_1e_cut = new TH1D(
            "corrected_Vz_e_AC_sector2_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector2);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_e_AC_sector2_1e_cut);

        TH1D *h_corrected_Vz_e_BC_zoomin_sector2_1e_cut, *h_corrected_Vz_e_AC_zoomin_sector2_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_e_BC_zoomin_sector2_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector2_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector2);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector2_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector2_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector2_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector2);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector2_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_e_BC_zoomin_sector2_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector2_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector2);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector2_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector2_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector2_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector2);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector2_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector2_1e_cut =
            new TH1D("Vx_e_BC_sector2_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector2);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_BC_sector2_1e_cut);
        TH1D *h_Vx_e_AC_sector2_1e_cut =
            new TH1D("Vx_e_AC_sector2_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector2);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_AC_sector2_1e_cut);
        TH1D *h_Vy_e_BC_sector2_1e_cut =
            new TH1D("Vy_e_BC_sector2_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector2);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_BC_sector2_1e_cut);
        TH1D *h_Vy_e_AC_sector2_1e_cut =
            new TH1D("Vy_e_AC_sector2_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector2);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_AC_sector2_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_sector2_1e_cut =
            new TH2D("Vz_VS_phi_e_AC_sector2_1e_cut", ("V_{z}^{e} vs. #phi_{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector2);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100,
                     -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_sector2_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_zoomin_sector2_1e_cut =
            new TH2D("Vz_VS_phi_e_AC_zoomin_sector2_1e_cut",
                     ("V_{z}^{e} vs. #phi_{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector2);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100, 50, 130, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_zoomin_sector2_1e_cut);

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

        // TH1D *h_corrected_Vz_e_BC_sector3_1e_cut = new TH1D(
        //     "corrected_Vz_e_BC_sector3_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector3);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9,
        //     2);
        // HistoList.push_back(h_corrected_Vz_e_BC_sector3_1e_cut);
        TH1D *h_corrected_Vz_e_AC_sector3_1e_cut = new TH1D(
            "corrected_Vz_e_AC_sector3_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector3);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_e_AC_sector3_1e_cut);

        TH1D *h_corrected_Vz_e_BC_zoomin_sector3_1e_cut, *h_corrected_Vz_e_AC_zoomin_sector3_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_e_BC_zoomin_sector3_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector3_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector3);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector3_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector3_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector3_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector3);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector3_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_e_BC_zoomin_sector3_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector3_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector3);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector3_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector3_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector3_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector3);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector3_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector3_1e_cut =
            new TH1D("Vx_e_BC_sector3_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector3);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_BC_sector3_1e_cut);
        TH1D *h_Vx_e_AC_sector3_1e_cut =
            new TH1D("Vx_e_AC_sector3_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector3);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_AC_sector3_1e_cut);
        TH1D *h_Vy_e_BC_sector3_1e_cut =
            new TH1D("Vy_e_BC_sector3_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector3);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_BC_sector3_1e_cut);
        TH1D *h_Vy_e_AC_sector3_1e_cut =
            new TH1D("Vy_e_AC_sector3_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector3);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_AC_sector3_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_sector3_1e_cut =
            new TH2D("Vz_VS_phi_e_AC_sector3_1e_cut", ("V_{z}^{e} vs. #phi_{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector3);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100,
                     -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_sector3_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_zoomin_sector3_1e_cut =
            new TH2D("Vz_VS_phi_e_AC_zoomin_sector3_1e_cut",
                     ("V_{z}^{e} vs. #phi_{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector3);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100, 110, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_zoomin_sector3_1e_cut);

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

        // TH1D *h_corrected_Vz_e_BC_sector4_1e_cut = new TH1D(
        //     "corrected_Vz_e_BC_sector4_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector4);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9,
        //     2);
        // HistoList.push_back(h_corrected_Vz_e_BC_sector4_1e_cut);
        TH1D *h_corrected_Vz_e_AC_sector4_1e_cut = new TH1D(
            "corrected_Vz_e_AC_sector4_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector4);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_e_AC_sector4_1e_cut);

        TH1D *h_corrected_Vz_e_BC_zoomin_sector4_1e_cut, *h_corrected_Vz_e_AC_zoomin_sector4_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_e_BC_zoomin_sector4_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector4_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector4);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector4_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector4_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector4_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector4);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector4_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_e_BC_zoomin_sector4_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector4_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector4);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector4_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector4_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector4_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector4);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector4_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector4_1e_cut =
            new TH1D("Vx_e_BC_sector4_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector4);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_BC_sector4_1e_cut);
        TH1D *h_Vx_e_AC_sector4_1e_cut =
            new TH1D("Vx_e_AC_sector4_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector4);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_AC_sector4_1e_cut);
        TH1D *h_Vy_e_BC_sector4_1e_cut =
            new TH1D("Vy_e_BC_sector4_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector4);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_BC_sector4_1e_cut);
        TH1D *h_Vy_e_AC_sector4_1e_cut =
            new TH1D("Vy_e_AC_sector4_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector4);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_AC_sector4_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_sector4_1e_cut =
            new TH2D("Vz_VS_phi_e_AC_sector4_1e_cut", ("V_{z}^{e} vs. #phi_{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector4);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100,
                     -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_sector4_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_zoomin_sector4_1e_cut = new TH2D(
            "Vz_VS_phi_e_AC_zoomin_sector4_1e_cut",
            ("V_{z}^{e} vs. #phi_{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector4);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100, -180, -110, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_zoomin_sector4_1e_cut);

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

        // TH1D *h_corrected_Vz_e_BC_sector5_1e_cut = new TH1D(
        //     "corrected_Vz_e_BC_sector5_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector5);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9,
        //     2);
        // HistoList.push_back(h_corrected_Vz_e_BC_sector5_1e_cut);
        TH1D *h_corrected_Vz_e_AC_sector5_1e_cut = new TH1D(
            "corrected_Vz_e_AC_sector5_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector5);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_e_AC_sector5_1e_cut);

        TH1D *h_corrected_Vz_e_BC_zoomin_sector5_1e_cut, *h_corrected_Vz_e_AC_zoomin_sector5_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_e_BC_zoomin_sector5_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector5_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector5);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector5_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector5_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector5_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector5);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector5_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_e_BC_zoomin_sector5_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector5_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector5);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector5_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector5_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector5_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector5);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector5_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector5_1e_cut =
            new TH1D("Vx_e_BC_sector5_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector5);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_BC_sector5_1e_cut);
        TH1D *h_Vx_e_AC_sector5_1e_cut =
            new TH1D("Vx_e_AC_sector5_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector5);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_AC_sector5_1e_cut);
        TH1D *h_Vy_e_BC_sector5_1e_cut =
            new TH1D("Vy_e_BC_sector5_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector5);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_BC_sector5_1e_cut);
        TH1D *h_Vy_e_AC_sector5_1e_cut =
            new TH1D("Vy_e_AC_sector5_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector5);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_AC_sector5_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_sector5_1e_cut =
            new TH2D("Vz_VS_phi_e_AC_sector5_1e_cut", ("V_{z}^{e} vs. #phi_{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector5);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100,
                     -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_sector5_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_zoomin_sector5_1e_cut = new TH2D(
            "Vz_VS_phi_e_AC_zoomin_sector5_1e_cut",
            ("V_{z}^{e} vs. #phi_{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector5);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100, -130, -50, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_zoomin_sector5_1e_cut);

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

        // TH1D *h_corrected_Vz_e_BC_sector6_1e_cut = new TH1D(
        //     "corrected_Vz_e_BC_sector6_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector6);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9,
        //     2);
        // HistoList.push_back(h_corrected_Vz_e_BC_sector6_1e_cut);
        TH1D *h_corrected_Vz_e_AC_sector6_1e_cut = new TH1D(
            "corrected_Vz_e_AC_sector6_1e_cut", ("Corrected V_{z}^{e} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector6);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_e_AC_sector6_1e_cut);

        TH1D *h_corrected_Vz_e_BC_zoomin_sector6_1e_cut, *h_corrected_Vz_e_AC_zoomin_sector6_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_e_BC_zoomin_sector6_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector6_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector6);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector6_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector6_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector6_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector6);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector6_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_e_BC_zoomin_sector6_1e_cut =
            //     new TH1D("corrected_Vz_e_BC_zoomin_sector6_1e_cut",
            //              ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector6);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_e_BC_zoomin_sector6_1e_cut);
            h_corrected_Vz_e_AC_zoomin_sector6_1e_cut =
                new TH1D("corrected_Vz_e_AC_zoomin_sector6_1e_cut",
                         ("Corrected V_{z}^{e} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector6);Corrected V_{z}^{e} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_e_AC_zoomin_sector6_1e_cut);
        }

        TH1D *h_Vx_e_BC_sector6_1e_cut =
            new TH1D("Vx_e_BC_sector6_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector6);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_BC_sector6_1e_cut);
        TH1D *h_Vx_e_AC_sector6_1e_cut =
            new TH1D("Vx_e_AC_sector6_1e_cut", ("V_{x}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector6);V_{x}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_e_AC_sector6_1e_cut);
        TH1D *h_Vy_e_BC_sector6_1e_cut =
            new TH1D("Vy_e_BC_sector6_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (before e^{-} cuts, sector6);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_BC_sector6_1e_cut);
        TH1D *h_Vy_e_AC_sector6_1e_cut =
            new TH1D("Vy_e_AC_sector6_1e_cut", ("V_{y}^{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector6);V_{y}^{e} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_e_AC_sector6_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_sector6_1e_cut =
            new TH2D("Vz_VS_phi_e_AC_sector6_1e_cut", ("V_{z}^{e} vs. #phi_{e} in (e,e') - " + CodeRun_status + " (after e^{-} cuts, sector6);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100,
                     -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_sector6_1e_cut);

        TH2D *h_Vz_VS_phi_e_AC_zoomin_sector6_1e_cut =
            new TH2D("Vz_VS_phi_e_AC_zoomin_sector6_1e_cut",
                     ("V_{z}^{e} vs. #phi_{e} in (e,e') - zoom-in - " + CodeRun_status + " (after e^{-} cuts, sector6);#phi_{e} [#circ];V_{z}^{e} [cm]").c_str(), 100, -70, 10, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_e_AC_zoomin_sector6_1e_cut);

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

        // TH1D *h_corrected_Vz_pipFD_BC_1e_cut =
        //     new TH1D("corrected_Vz_pipFD_BC_1e_cut", ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts);Corrected V_{z}^{#pi^{+}FD}
        //     [cm];Counts").c_str(),
        //              75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pipFD_BC_1e_cut);
        TH1D *h_corrected_Vz_pipFD_AC_1e_cut =
            new TH1D("corrected_Vz_pipFD_AC_1e_cut", ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(),
                     75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pipFD_AC_1e_cut);

        TH1D *h_corrected_Vz_pipFD_BC_zoomin_1e_cut, *h_corrected_Vz_pipFD_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pipFD_BC_zoomin_1e_cut =
            //     new TH1D("corrected_Vz_pipFD_BC_zoomin_1e_cut",
            //              ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_1e_cut =
                new TH1D("corrected_Vz_pipFD_AC_zoomin_1e_cut",
                         ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pipFD_BC_zoomin_1e_cut =
            //     new TH1D("corrected_Vz_pipFD_BC_zoomin_1e_cut",
            //              ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_1e_cut =
                new TH1D("corrected_Vz_pipFD_AC_zoomin_1e_cut",
                         ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_1e_cut =
            new TH1D("Vx_pipFD_BC_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_BC_1e_cut);
        TH1D *h_Vx_pipFD_AC_1e_cut =
            new TH1D("Vx_pipFD_AC_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_AC_1e_cut);
        TH1D *h_Vy_pipFD_BC_1e_cut =
            new TH1D("Vy_pipFD_BC_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pipFD_BC_1e_cut);
        TH1D *h_Vy_pipFD_AC_1e_cut =
            new TH1D("Vy_pipFD_AC_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pipFD_AC_1e_cut =
            new TH2D("Vz_VS_phi_pipFD_AC_1e_cut",
                     ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+}FD cuts);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(), 100, -180,
                     180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_1e_cut);

        TH2D *h_Vz_VS_theta_pipFD_AC_1e_cut =
            new TH2D("Vz_VS_theta_pipFD_AC_1e_cut",
                     ("V_{z}^{#pi^{+}FD} vs. #theta_{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+}FD cuts);#theta_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(), 100, 0,
                     50, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_theta_pipFD_AC_1e_cut);

        TH2D *h_corrected_Vz_VS_phi_pipFD_AC_1e_cut = new TH2D(
            "corrected_Vz_VS_phi_pipFD_AC_1e_cut",
            ("Corrected V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+}FD cuts);#phi_{#pi^{+}FD} [#circ];Corrected V_{z}^{#pi^{+}FD} [cm]").c_str(),
            100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_corrected_Vz_VS_phi_pipFD_AC_1e_cut);

        TH2D *h_corrected_Vz_VS_theta_pipFD_AC_1e_cut = new TH2D(
            "corrected_Vz_VS_theta_pipFD_AC_1e_cut",
            ("Corrected V_{z}^{#pi^{+}FD} vs. #theta_{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+}FD cuts);#theta_{#pi^{+}FD} [#circ];Corrected V_{z}^{#pi^{+}FD} [cm]")
                .c_str(),
            100, 0, 50, 100, -13, 2);
        HistoList.push_back(h_corrected_Vz_VS_theta_pipFD_AC_1e_cut);

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

        // TH1D *h_corrected_Vz_pipFD_BC_sector1_1e_cut =
        //     new TH1D("corrected_Vz_pipFD_BC_sector1_1e_cut",
        //              ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector1);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pipFD_BC_sector1_1e_cut);
        TH1D *h_corrected_Vz_pipFD_AC_sector1_1e_cut =
            new TH1D("corrected_Vz_pipFD_AC_sector1_1e_cut",
                     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector1);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pipFD_AC_sector1_1e_cut);

        TH1D *h_corrected_Vz_pipFD_BC_zoomin_sector1_1e_cut, *h_corrected_Vz_pipFD_AC_zoomin_sector1_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector1_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector1_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector1);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector1_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector1_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector1_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector1);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector1_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector1_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector1_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector1);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector1_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector1_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector1_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector1);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector1_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_sector1_1e_cut =
            new TH1D("Vx_pipFD_BC_sector1_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector1);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_BC_sector1_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector1_1e_cut =
            new TH1D("Vx_pipFD_AC_sector1_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector1);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_AC_sector1_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector1_1e_cut =
            new TH1D("Vy_pipFD_BC_sector1_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector1);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pipFD_BC_sector1_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector1_1e_cut =
            new TH1D("Vy_pipFD_AC_sector1_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector1);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pipFD_AC_sector1_1e_cut =
            new TH2D("Vz_VS_phi_pipFD_AC_sector1_1e_cut",
                     ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+}FD cuts, sector1);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_sector1_1e_cut);

        TH2D *h_Vz_VS_phi_pipFD_AC_zoomin_sector1_1e_cut = new TH2D(
            "Vz_VS_phi_pipFD_AC_zoomin_sector1_1e_cut",
            ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+}FD cuts, sector1);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
            100, -10, 70, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_zoomin_sector1_1e_cut);

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

        // TH1D *h_corrected_Vz_pipFD_BC_sector2_1e_cut =
        //     new TH1D("corrected_Vz_pipFD_BC_sector2_1e_cut",
        //              ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector2);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pipFD_BC_sector2_1e_cut);
        TH1D *h_corrected_Vz_pipFD_AC_sector2_1e_cut =
            new TH1D("corrected_Vz_pipFD_AC_sector2_1e_cut",
                     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector2);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pipFD_AC_sector2_1e_cut);

        TH1D *h_corrected_Vz_pipFD_BC_zoomin_sector2_1e_cut, *h_corrected_Vz_pipFD_AC_zoomin_sector2_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector2_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector2_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector2);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector2_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector2_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector2_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector2);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector2_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector2_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector2_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector2);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector2_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector2_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector2_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector2);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector2_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_sector2_1e_cut =
            new TH1D("Vx_pipFD_BC_sector2_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector2);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_BC_sector2_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector2_1e_cut =
            new TH1D("Vx_pipFD_AC_sector2_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector2);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_AC_sector2_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector2_1e_cut =
            new TH1D("Vy_pipFD_BC_sector2_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector2);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pipFD_BC_sector2_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector2_1e_cut =
            new TH1D("Vy_pipFD_AC_sector2_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector2);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pipFD_AC_sector2_1e_cut =
            new TH2D("Vz_VS_phi_pipFD_AC_sector2_1e_cut",
                     ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+}FD cuts, sector2);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_sector2_1e_cut);

        TH2D *h_Vz_VS_phi_pipFD_AC_zoomin_sector2_1e_cut = new TH2D(
            "Vz_VS_phi_pipFD_AC_zoomin_sector2_1e_cut",
            ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+}FD cuts, sector2);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
            100, 50, 130, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_zoomin_sector2_1e_cut);

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

        // TH1D *h_corrected_Vz_pipFD_BC_sector3_1e_cut =
        //     new TH1D("corrected_Vz_pipFD_BC_sector3_1e_cut",
        //              ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector3);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pipFD_BC_sector3_1e_cut);
        TH1D *h_corrected_Vz_pipFD_AC_sector3_1e_cut =
            new TH1D("corrected_Vz_pipFD_AC_sector3_1e_cut",
                     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector3);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pipFD_AC_sector3_1e_cut);

        TH1D *h_corrected_Vz_pipFD_BC_zoomin_sector3_1e_cut, *h_corrected_Vz_pipFD_AC_zoomin_sector3_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector3_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector3_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector3);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector3_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector3_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector3_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector3);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector3_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector3_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector3_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector3);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector3_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector3_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector3_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector3);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector3_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_sector3_1e_cut =
            new TH1D("Vx_pipFD_BC_sector3_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector3);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_BC_sector3_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector3_1e_cut =
            new TH1D("Vx_pipFD_AC_sector3_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector3);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_AC_sector3_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector3_1e_cut =
            new TH1D("Vy_pipFD_BC_sector3_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector3);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pipFD_BC_sector3_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector3_1e_cut =
            new TH1D("Vy_pipFD_AC_sector3_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector3);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pipFD_AC_sector3_1e_cut =
            new TH2D("Vz_VS_phi_pipFD_AC_sector3_1e_cut",
                     ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+}FD cuts, sector3);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_sector3_1e_cut);

        TH2D *h_Vz_VS_phi_pipFD_AC_zoomin_sector3_1e_cut = new TH2D(
            "Vz_VS_phi_pipFD_AC_zoomin_sector3_1e_cut",
            ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+}FD cuts, sector3);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
            100, 110, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_zoomin_sector3_1e_cut);

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

        // TH1D *h_corrected_Vz_pipFD_BC_sector4_1e_cut =
        //     new TH1D("corrected_Vz_pipFD_BC_sector4_1e_cut",
        //              ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector4);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pipFD_BC_sector4_1e_cut);
        TH1D *h_corrected_Vz_pipFD_AC_sector4_1e_cut =
            new TH1D("corrected_Vz_pipFD_AC_sector4_1e_cut",
                     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector4);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pipFD_AC_sector4_1e_cut);

        TH1D *h_corrected_Vz_pipFD_BC_zoomin_sector4_1e_cut, *h_corrected_Vz_pipFD_AC_zoomin_sector4_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector4_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector4_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector4);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector4_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector4_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector4_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector4);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector4_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector4_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector4_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector4);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector4_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector4_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector4_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector4);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector4_1e_cut);
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
            new TH1D("Vx_pipFD_BC_sector4_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector4);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_BC_sector4_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector4_1e_cut =
            new TH1D("Vx_pipFD_AC_sector4_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector4);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_AC_sector4_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector4_1e_cut =
            new TH1D("Vy_pipFD_BC_sector4_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector4);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pipFD_BC_sector4_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector4_1e_cut =
            new TH1D("Vy_pipFD_AC_sector4_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector4);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pipFD_AC_sector4_1e_cut =
            new TH2D("Vz_VS_phi_pipFD_AC_sector4_1e_cut",
                     ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+}FD cuts, sector4);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_sector4_1e_cut);

        TH2D *h_Vz_VS_phi_pipFD_AC_zoomin_sector4_1e_cut = new TH2D(
            "Vz_VS_phi_pipFD_AC_zoomin_sector4_1e_cut",
            ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+}FD cuts, sector4);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
            100, -180, -110, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_zoomin_sector4_1e_cut);

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

        // TH1D *h_corrected_Vz_pipFD_BC_sector5_1e_cut =
        //     new TH1D("corrected_Vz_pipFD_BC_sector5_1e_cut",
        //              ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector5);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pipFD_BC_sector5_1e_cut);
        TH1D *h_corrected_Vz_pipFD_AC_sector5_1e_cut =
            new TH1D("corrected_Vz_pipFD_AC_sector5_1e_cut",
                     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector5);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pipFD_AC_sector5_1e_cut);

        TH1D *h_corrected_Vz_pipFD_BC_zoomin_sector5_1e_cut, *h_corrected_Vz_pipFD_AC_zoomin_sector5_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector5_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector5_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector5);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector5_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector5_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector5_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector5);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector5_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector5_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector5_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector5);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector5_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector5_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector5_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector5);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector5_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_sector5_1e_cut =
            new TH1D("Vx_pipFD_BC_sector5_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector5);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_BC_sector5_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector5_1e_cut =
            new TH1D("Vx_pipFD_AC_sector5_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector5);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_AC_sector5_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector5_1e_cut =
            new TH1D("Vy_pipFD_BC_sector5_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector5);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pipFD_BC_sector5_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector5_1e_cut =
            new TH1D("Vy_pipFD_AC_sector5_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector5);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pipFD_AC_sector5_1e_cut =
            new TH2D("Vz_VS_phi_pipFD_AC_sector5_1e_cut",
                     ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+}FD cuts, sector5);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_sector5_1e_cut);

        TH2D *h_Vz_VS_phi_pipFD_AC_zoomin_sector5_1e_cut = new TH2D(
            "Vz_VS_phi_pipFD_AC_zoomin_sector5_1e_cut",
            ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+}FD cuts, sector5);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
            100, -130, -50, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_zoomin_sector5_1e_cut);

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

        // TH1D *h_corrected_Vz_pipFD_BC_sector6_1e_cut =
        //     new TH1D("corrected_Vz_pipFD_BC_sector6_1e_cut",
        //              ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector6);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pipFD_BC_sector6_1e_cut);
        TH1D *h_corrected_Vz_pipFD_AC_sector6_1e_cut =
            new TH1D("corrected_Vz_pipFD_AC_sector6_1e_cut",
                     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector6);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pipFD_AC_sector6_1e_cut);

        TH1D *h_corrected_Vz_pipFD_BC_zoomin_sector6_1e_cut, *h_corrected_Vz_pipFD_AC_zoomin_sector6_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector6_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector6_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector6);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector6_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector6_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector6_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector6);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector6_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pipFD_BC_zoomin_sector6_1e_cut = new TH1D(
            //     "corrected_Vz_pipFD_BC_zoomin_sector6_1e_cut",
            //     ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{+} cuts, sector6);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pipFD_BC_zoomin_sector6_1e_cut);
            h_corrected_Vz_pipFD_AC_zoomin_sector6_1e_cut = new TH1D(
                "corrected_Vz_pipFD_AC_zoomin_sector6_1e_cut",
                ("Corrected V_{z}^{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+} cuts, sector6);Corrected V_{z}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pipFD_AC_zoomin_sector6_1e_cut);
        }

        TH1D *h_Vx_pipFD_BC_sector6_1e_cut =
            new TH1D("Vx_pipFD_BC_sector6_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector6);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_BC_sector6_1e_cut);
        TH1D *h_Vx_pipFD_AC_sector6_1e_cut =
            new TH1D("Vx_pipFD_AC_sector6_1e_cut", ("V_{x}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector6);V_{x}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pipFD_AC_sector6_1e_cut);
        TH1D *h_Vy_pipFD_BC_sector6_1e_cut =
            new TH1D("Vy_pipFD_BC_sector6_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (before #pi^{+} cuts, sector6);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pipFD_BC_sector6_1e_cut);
        TH1D *h_Vy_pipFD_AC_sector6_1e_cut =
            new TH1D("Vy_pipFD_AC_sector6_1e_cut", ("V_{y}^{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+} cuts, sector6);V_{y}^{#pi^{+}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pipFD_AC_sector6_1e_cut =
            new TH2D("Vz_VS_phi_pipFD_AC_sector6_1e_cut",
                     ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - " + CodeRun_status + " (after #pi^{+}FD cuts, sector6);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_sector6_1e_cut);

        TH2D *h_Vz_VS_phi_pipFD_AC_zoomin_sector6_1e_cut = new TH2D(
            "Vz_VS_phi_pipFD_AC_zoomin_sector6_1e_cut",
            ("V_{z}^{#pi^{+}FD} vs. #phi_{#pi^{+}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{+}FD cuts, sector6);#phi_{#pi^{+}FD} [#circ];V_{z}^{#pi^{+}FD} [cm]").c_str(),
            100, -70, 10, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pipFD_AC_zoomin_sector6_1e_cut);

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

        // TH1D *h_corrected_Vz_pimFD_BC_1e_cut =
        //     new TH1D("corrected_Vz_pimFD_BC_1e_cut", ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts);Corrected V_{z}^{#pi^{-}FD}
        //     [cm];Counts").c_str(),
        //              75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pimFD_BC_1e_cut);
        TH1D *h_corrected_Vz_pimFD_AC_1e_cut =
            new TH1D("corrected_Vz_pimFD_AC_1e_cut", ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(),
                     75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pimFD_AC_1e_cut);

        TH1D *h_corrected_Vz_pimFD_BC_zoomin_1e_cut, *h_corrected_Vz_pimFD_AC_zoomin_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pimFD_BC_zoomin_1e_cut =
            //     new TH1D("corrected_Vz_pimFD_BC_zoomin_1e_cut",
            //              ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_1e_cut =
                new TH1D("corrected_Vz_pimFD_AC_zoomin_1e_cut",
                         ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pimFD_BC_zoomin_1e_cut =
            //     new TH1D("corrected_Vz_pimFD_BC_zoomin_1e_cut",
            //              ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_1e_cut =
                new TH1D("corrected_Vz_pimFD_AC_zoomin_1e_cut",
                         ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_1e_cut =
            new TH1D("Vx_pimFD_BC_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_BC_1e_cut);
        TH1D *h_Vx_pimFD_AC_1e_cut =
            new TH1D("Vx_pimFD_AC_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_AC_1e_cut);
        TH1D *h_Vy_pimFD_BC_1e_cut =
            new TH1D("Vy_pimFD_BC_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pimFD_BC_1e_cut);
        TH1D *h_Vy_pimFD_AC_1e_cut =
            new TH1D("Vy_pimFD_AC_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pimFD_AC_1e_cut =
            new TH2D("Vz_VS_phi_pimFD_AC_1e_cut",
                     ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-}FD cuts);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(), 100, -180,
                     180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_1e_cut);

        TH2D *h_Vz_VS_theta_pimFD_AC_1e_cut =
            new TH2D("Vz_VS_theta_pimFD_AC_1e_cut",
                     ("V_{z}^{#pi^{-}FD} vs. #theta_{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-}FD cuts);#theta_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(), 100, 0,
                     50, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_theta_pimFD_AC_1e_cut);

        TH2D *h_corrected_Vz_VS_phi_pimFD_AC_1e_cut = new TH2D(
            "corrected_Vz_VS_phi_pimFD_AC_1e_cut",
            ("Corrected V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-}FD cuts);#phi_{#pi^{-}FD} [#circ];Corrected V_{z}^{#pi^{-}FD} [cm]").c_str(),
            100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_corrected_Vz_VS_phi_pimFD_AC_1e_cut);

        TH2D *h_corrected_Vz_VS_theta_pimFD_AC_1e_cut = new TH2D(
            "corrected_Vz_VS_theta_pimFD_AC_1e_cut",
            ("Corrected V_{z}^{#pi^{-}FD} vs. #theta_{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-}FD cuts);#theta_{#pi^{-}FD} [#circ];Corrected V_{z}^{#pi^{-}FD} [cm]")
                .c_str(),
            100, 0, 50, 100, -13, 2);
        HistoList.push_back(h_corrected_Vz_VS_theta_pimFD_AC_1e_cut);

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

        // TH1D *h_corrected_Vz_pimFD_BC_sector1_1e_cut =
        //     new TH1D("corrected_Vz_pimFD_BC_sector1_1e_cut",
        //              ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector1);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pimFD_BC_sector1_1e_cut);
        TH1D *h_corrected_Vz_pimFD_AC_sector1_1e_cut =
            new TH1D("corrected_Vz_pimFD_AC_sector1_1e_cut",
                     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector1);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pimFD_AC_sector1_1e_cut);

        TH1D *h_corrected_Vz_pimFD_BC_zoomin_sector1_1e_cut, *h_corrected_Vz_pimFD_AC_zoomin_sector1_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector1_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector1_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector1);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector1_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector1_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector1_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector1);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector1_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector1_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector1_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector1);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector1_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector1_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector1_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector1);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector1_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector1_1e_cut =
            new TH1D("Vx_pimFD_BC_sector1_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector1);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_BC_sector1_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector1_1e_cut =
            new TH1D("Vx_pimFD_AC_sector1_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector1);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_AC_sector1_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector1_1e_cut =
            new TH1D("Vy_pimFD_BC_sector1_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector1);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pimFD_BC_sector1_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector1_1e_cut =
            new TH1D("Vy_pimFD_AC_sector1_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector1);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pimFD_AC_sector1_1e_cut =
            new TH2D("Vz_VS_phi_pimFD_AC_sector1_1e_cut",
                     ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-}FD cuts, sector1);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_sector1_1e_cut);

        TH2D *h_Vz_VS_phi_pimFD_AC_zoomin_sector1_1e_cut = new TH2D(
            "Vz_VS_phi_pimFD_AC_zoomin_sector1_1e_cut",
            ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-}FD cuts, sector1);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
            100, -10, 70, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_zoomin_sector1_1e_cut);

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

        // TH1D *h_corrected_Vz_pimFD_BC_sector2_1e_cut =
        //     new TH1D("corrected_Vz_pimFD_BC_sector2_1e_cut",
        //              ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector2);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pimFD_BC_sector2_1e_cut);
        TH1D *h_corrected_Vz_pimFD_AC_sector2_1e_cut =
            new TH1D("corrected_Vz_pimFD_AC_sector2_1e_cut",
                     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector2);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pimFD_AC_sector2_1e_cut);

        TH1D *h_corrected_Vz_pimFD_BC_zoomin_sector2_1e_cut, *h_corrected_Vz_pimFD_AC_zoomin_sector2_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector2_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector2_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector2);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector2_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector2_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector2_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector2);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector2_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector2_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector2_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector2);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector2_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector2_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector2_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector2);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector2_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector2_1e_cut =
            new TH1D("Vx_pimFD_BC_sector2_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector2);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_BC_sector2_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector2_1e_cut =
            new TH1D("Vx_pimFD_AC_sector2_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector2);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_AC_sector2_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector2_1e_cut =
            new TH1D("Vy_pimFD_BC_sector2_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector2);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pimFD_BC_sector2_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector2_1e_cut =
            new TH1D("Vy_pimFD_AC_sector2_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector2);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pimFD_AC_sector2_1e_cut =
            new TH2D("Vz_VS_phi_pimFD_AC_sector2_1e_cut",
                     ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-}FD cuts, sector2);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_sector2_1e_cut);

        TH2D *h_Vz_VS_phi_pimFD_AC_zoomin_sector2_1e_cut = new TH2D(
            "Vz_VS_phi_pimFD_AC_zoomin_sector2_1e_cut",
            ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-}FD cuts, sector2);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
            100, 50, 130, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_zoomin_sector2_1e_cut);

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

        // TH1D *h_corrected_Vz_pimFD_BC_sector3_1e_cut =
        //     new TH1D("corrected_Vz_pimFD_BC_sector3_1e_cut",
        //              ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector3);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pimFD_BC_sector3_1e_cut);
        TH1D *h_corrected_Vz_pimFD_AC_sector3_1e_cut =
            new TH1D("corrected_Vz_pimFD_AC_sector3_1e_cut",
                     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector3);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pimFD_AC_sector3_1e_cut);

        TH1D *h_corrected_Vz_pimFD_BC_zoomin_sector3_1e_cut, *h_corrected_Vz_pimFD_AC_zoomin_sector3_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector3_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector3_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector3);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector3_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector3_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector3_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector3);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector3_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector3_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector3_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector3);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector3_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector3_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector3_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector3);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector3_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector3_1e_cut =
            new TH1D("Vx_pimFD_BC_sector3_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector3);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_BC_sector3_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector3_1e_cut =
            new TH1D("Vx_pimFD_AC_sector3_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector3);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_AC_sector3_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector3_1e_cut =
            new TH1D("Vy_pimFD_BC_sector3_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector3);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pimFD_BC_sector3_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector3_1e_cut =
            new TH1D("Vy_pimFD_AC_sector3_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector3);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pimFD_AC_sector3_1e_cut =
            new TH2D("Vz_VS_phi_pimFD_AC_sector3_1e_cut",
                     ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-}FD cuts, sector3);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_sector3_1e_cut);

        TH2D *h_Vz_VS_phi_pimFD_AC_zoomin_sector3_1e_cut = new TH2D(
            "Vz_VS_phi_pimFD_AC_zoomin_sector3_1e_cut",
            ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-}FD cuts, sector3);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
            100, 130, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_zoomin_sector3_1e_cut);

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

        // TH1D *h_corrected_Vz_pimFD_BC_sector4_1e_cut =
        //     new TH1D("corrected_Vz_pimFD_BC_sector4_1e_cut",
        //              ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector4);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pimFD_BC_sector4_1e_cut);
        TH1D *h_corrected_Vz_pimFD_AC_sector4_1e_cut =
            new TH1D("corrected_Vz_pimFD_AC_sector4_1e_cut",
                     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector4);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pimFD_AC_sector4_1e_cut);

        TH1D *h_corrected_Vz_pimFD_BC_zoomin_sector4_1e_cut, *h_corrected_Vz_pimFD_AC_zoomin_sector4_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector4_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector4_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector4);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector4_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector4_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector4_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector4);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector4_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector4_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector4_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector4);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector4_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector4_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector4_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector4);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector4_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector4_1e_cut =
            new TH1D("Vx_pimFD_BC_sector4_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector4);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_BC_sector4_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector4_1e_cut =
            new TH1D("Vx_pimFD_AC_sector4_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector4);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_AC_sector4_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector4_1e_cut =
            new TH1D("Vy_pimFD_BC_sector4_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector4);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pimFD_BC_sector4_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector4_1e_cut =
            new TH1D("Vy_pimFD_AC_sector4_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector4);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pimFD_AC_sector4_1e_cut =
            new TH2D("Vz_VS_phi_pimFD_AC_sector4_1e_cut",
                     ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-}FD cuts, sector4);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_sector4_1e_cut);

        TH2D *h_Vz_VS_phi_pimFD_AC_zoomin_sector4_1e_cut = new TH2D(
            "Vz_VS_phi_pimFD_AC_zoomin_sector4_1e_cut",
            ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-}FD cuts, sector4);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
            100, -180, -110, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_zoomin_sector4_1e_cut);

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

        // TH1D *h_corrected_Vz_pimFD_BC_sector5_1e_cut =
        //     new TH1D("corrected_Vz_pimFD_BC_sector5_1e_cut",
        //              ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector5);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pimFD_BC_sector5_1e_cut);
        TH1D *h_corrected_Vz_pimFD_AC_sector5_1e_cut =
            new TH1D("corrected_Vz_pimFD_AC_sector5_1e_cut",
                     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector5);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pimFD_AC_sector5_1e_cut);

        TH1D *h_corrected_Vz_pimFD_BC_zoomin_sector5_1e_cut, *h_corrected_Vz_pimFD_AC_zoomin_sector5_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector5_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector5_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector5);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector5_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector5_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector5_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector5);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector5_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector5_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector5_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector5);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector5_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector5_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector5_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector5);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector5_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector5_1e_cut =
            new TH1D("Vx_pimFD_BC_sector5_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector5);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_BC_sector5_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector5_1e_cut =
            new TH1D("Vx_pimFD_AC_sector5_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector5);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_AC_sector5_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector5_1e_cut =
            new TH1D("Vy_pimFD_BC_sector5_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector5);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pimFD_BC_sector5_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector5_1e_cut =
            new TH1D("Vy_pimFD_AC_sector5_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector5);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pimFD_AC_sector5_1e_cut =
            new TH2D("Vz_VS_phi_pimFD_AC_sector5_1e_cut",
                     ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-}FD cuts, sector5);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_sector5_1e_cut);

        TH2D *h_Vz_VS_phi_pimFD_AC_zoomin_sector5_1e_cut = new TH2D(
            "Vz_VS_phi_pimFD_AC_zoomin_sector5_1e_cut",
            ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-}FD cuts, sector5);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
            100, -130, -50, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_zoomin_sector5_1e_cut);

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

        // TH1D *h_corrected_Vz_pimFD_BC_sector6_1e_cut =
        //     new TH1D("corrected_Vz_pimFD_BC_sector6_1e_cut",
        //              ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector6);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        // HistoList.push_back(h_corrected_Vz_pimFD_BC_sector6_1e_cut);
        TH1D *h_corrected_Vz_pimFD_AC_sector6_1e_cut =
            new TH1D("corrected_Vz_pimFD_AC_sector6_1e_cut",
                     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector6);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -9, 2);
        HistoList.push_back(h_corrected_Vz_pimFD_AC_sector6_1e_cut);

        TH1D *h_corrected_Vz_pimFD_BC_zoomin_sector6_1e_cut, *h_corrected_Vz_pimFD_AC_zoomin_sector6_1e_cut;

        if (target_status == "Ar40") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector6_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector6_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector6);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector6_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector6_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector6_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector6);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -8, -4);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector6_1e_cut);
        } else if (target_status == "C12") {
            // h_corrected_Vz_pimFD_BC_zoomin_sector6_1e_cut = new TH1D(
            //     "corrected_Vz_pimFD_BC_zoomin_sector6_1e_cut",
            //     ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (before #pi^{-} cuts, sector6);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            // HistoList.push_back(h_corrected_Vz_pimFD_BC_zoomin_sector6_1e_cut);
            h_corrected_Vz_pimFD_AC_zoomin_sector6_1e_cut = new TH1D(
                "corrected_Vz_pimFD_AC_zoomin_sector6_1e_cut",
                ("Corrected V_{z}^{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-} cuts, sector6);Corrected V_{z}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -4, 1);
            HistoList.push_back(h_corrected_Vz_pimFD_AC_zoomin_sector6_1e_cut);
        }

        TH1D *h_Vx_pimFD_BC_sector6_1e_cut =
            new TH1D("Vx_pimFD_BC_sector6_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector6);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_BC_sector6_1e_cut);
        TH1D *h_Vx_pimFD_AC_sector6_1e_cut =
            new TH1D("Vx_pimFD_AC_sector6_1e_cut", ("V_{x}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector6);V_{x}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vx_pimFD_AC_sector6_1e_cut);
        TH1D *h_Vy_pimFD_BC_sector6_1e_cut =
            new TH1D("Vy_pimFD_BC_sector6_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (before #pi^{-} cuts, sector6);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
        HistoList.push_back(h_Vy_pimFD_BC_sector6_1e_cut);
        TH1D *h_Vy_pimFD_AC_sector6_1e_cut =
            new TH1D("Vy_pimFD_AC_sector6_1e_cut", ("V_{y}^{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-} cuts, sector6);V_{y}^{#pi^{-}FD} [cm];Counts").c_str(), 75, -1, 1);
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

        TH2D *h_Vz_VS_phi_pimFD_AC_sector6_1e_cut =
            new TH2D("Vz_VS_phi_pimFD_AC_sector6_1e_cut",
                     ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - " + CodeRun_status + " (after #pi^{-}FD cuts, sector6);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
                     100, -180, 180, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_sector6_1e_cut);

        TH2D *h_Vz_VS_phi_pimFD_AC_zoomin_sector6_1e_cut = new TH2D(
            "Vz_VS_phi_pimFD_AC_zoomin_sector6_1e_cut",
            ("V_{z}^{#pi^{-}FD} vs. #phi_{#pi^{-}FD} in (e,e') - zoom-in - " + CodeRun_status + " (after #pi^{-}FD cuts, sector6);#phi_{#pi^{-}FD} [#circ];V_{z}^{#pi^{-}FD} [cm]").c_str(),
            100, -70, 10, 100, -13, 2);
        HistoList.push_back(h_Vz_VS_phi_pimFD_AC_zoomin_sector6_1e_cut);

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
            new TH1D("Vx_pipCD_BC_1e_cut", ("V_{x}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (before cut);V_{x}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -0.4, 0.4);
        HistoList.push_back(h_Vx_pipCD_BC_1e_cut);
        TH1D *h_Vx_pipCD_AC_1e_cut =
            new TH1D("Vx_pipCD_AC_1e_cut", ("V_{x}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (after cut);V_{x}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -0.4, 0.4);
        HistoList.push_back(h_Vx_pipCD_AC_1e_cut);
        TH1D *h_Vy_pipCD_BC_1e_cut =
            new TH1D("Vy_pipCD_BC_1e_cut", ("V_{y}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (before cut);V_{y}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -0.4, 0.4);
        HistoList.push_back(h_Vy_pipCD_BC_1e_cut);
        TH1D *h_Vy_pipCD_AC_1e_cut =
            new TH1D("Vy_pipCD_AC_1e_cut", ("V_{y}^{#pi^{+}CD} in (e,e') - " + CodeRun_status + " (after cut);V_{y}^{#pi^{+}CD} [cm];Counts").c_str(), 75, -0.4, 0.4);
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
            new TH1D("Vx_pimCD_BC_1e_cut", ("V_{x}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (before cut);V_{x}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -0.4, 0.4);
        HistoList.push_back(h_Vx_pimCD_BC_1e_cut);
        TH1D *h_Vx_pimCD_AC_1e_cut =
            new TH1D("Vx_pimCD_AC_1e_cut", ("V_{x}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (after cut);V_{x}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -0.4, 0.4);
        HistoList.push_back(h_Vx_pimCD_AC_1e_cut);
        TH1D *h_Vy_pimCD_BC_1e_cut =
            new TH1D("Vy_pimCD_BC_1e_cut", ("V_{y}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (before cut);V_{y}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -0.4, 0.4);
        HistoList.push_back(h_Vy_pimCD_BC_1e_cut);
        TH1D *h_Vy_pimCD_AC_1e_cut =
            new TH1D("Vy_pimCD_AC_1e_cut", ("V_{y}^{#pi^{-}CD} in (e,e') - " + CodeRun_status + " (after cut);V_{y}^{#pi^{-}CD} [cm];Counts").c_str(), 75, -0.4, 0.4);
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

        clasAna.setVertexCorrCuts(true);  // making f_corr_vertexCuts = true

        // Set vertex correlation cuts for 2 GeV samples
        if (Is2GeV) {
            clasAna.setVertexCorrCuts_FD(-6, 4);
            clasAna.setVertexCorrCuts_CD(-5, 3);
        }

        std::cout << "\033[33m" << "\n\nRunning on " << SampleName << " with " << Ebeam << " GeV beam energy\n\n" << "\033[0m";

#pragma region Loop over events
        int NumOfEvents = 0;
        int NumOfEvents_wAny_e_det = 0, NumOfEvents_wOne_e_det = 0;
        int NumOfEvents_wAny_e = 0, NumOfEvents_wOne_e = 0;

        while (chain.Next() == true) {
            // cout << "\033[32m" << "\n\nProcessing event " << NumOfEvents + 1 << "\n\033[0m";

            // Display completed
            ++NumOfEvents;
            if ((NumOfEvents % 1000000) == 0) { std::cerr << "\033[33m" << "\n\n" << NumOfEvents / 1000000 << " million completed\n\n" << "\033[0m"; }
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

            //  - Electron cuts -----------------------------------------------------------------------------------------------------------------------------------------

#pragma region Electrons BPID
            h_Vx_e_BC_1e_cut->Fill(electrons_det[0]->par()->getVx(), weight);
            h_Vy_e_BC_1e_cut->Fill(electrons_det[0]->par()->getVy(), weight);
            h_Vz_e_BC_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);
            h_Vz_e_BC_zoomin_1e_cut->Fill(electrons_det[0]->par()->getVz(), weight);

            raf::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_1e_cut, weight);

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

                raf::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector1_1e_cut, weight);

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

                raf::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector2_1e_cut, weight);

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

                raf::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector3_1e_cut, weight);

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

                raf::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector4_1e_cut, weight);

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

                raf::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector5_1e_cut, weight);

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

                raf::fillDCdebug(electrons_det[0], h_dc_electron_hit_map_BC_sector6_1e_cut, weight);

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

#pragma region Electrons APID
            double Vx_e = electrons[0]->par()->getVx();
            double Vy_e = electrons[0]->par()->getVy();
            double Vz_e = electrons[0]->par()->getVz();

            auto r_e = compute_r(Beam_Coordinates, "e");
            auto phi_beam_e_rad = compute_phi_beam_rad(Beam_Coordinates, "e");
            auto corrected_Vz_e = correct_Vz(Vz_e, r_e, electrons[0]->getTheta(), electrons[0]->getPhi(), phi_beam_e_rad);

            h_Vx_e_AC_1e_cut->Fill(Vx_e, weight);
            h_Vy_e_AC_1e_cut->Fill(Vy_e, weight);
            h_Vz_e_AC_1e_cut->Fill(Vz_e, weight);
            h_Vz_e_AC_zoomin_1e_cut->Fill(Vz_e, weight);

            h_corrected_Vz_e_AC_1e_cut->Fill(corrected_Vz_e, weight);
            h_corrected_Vz_e_AC_zoomin_1e_cut->Fill(corrected_Vz_e, weight);

            // h_Vz_e_AC_1e_cut_BySliceOf.Fill(electrons[0]->getTheta() * 180 / am::pi, Vz_e, 0, weight);
            // h_Vz_e_AC_zoomin_1e_cut_BySliceOf.Fill(electrons[0]->getTheta() * 180 / am::pi, Vz_e, 0, weight);

            h_Vz_VS_phi_e_AC_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);
            h_Vz_VS_theta_e_AC_1e_cut->Fill(electrons[0]->getTheta() * 180 / am::pi, Vz_e, weight);

            h_corrected_Vz_VS_phi_e_AC_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, corrected_Vz_e, weight);
            h_corrected_Vz_VS_theta_e_AC_1e_cut->Fill(electrons[0]->getTheta() * 180 / am::pi, corrected_Vz_e, weight);

            raf::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_1e_cut, weight);

            h_nphe_AC_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

            h_Edep_PCAL_VS_EC_AC_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

            h_SF_VS_Edep_PCAL_AC_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
            h_SF_VS_P_e_AC_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

            h_SF_VS_Lv_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
            h_SF_VS_Lw_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
            h_SF_VS_Lu_AC_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

            h_E_PCALoP_e_VS_E_PCALoP_e_AC_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);

            if (electrons[0]->getSector() == 1) {
                h_Vx_e_AC_sector1_1e_cut->Fill(Vx_e, weight);
                h_Vy_e_AC_sector1_1e_cut->Fill(Vy_e, weight);
                h_Vz_e_AC_sector1_1e_cut->Fill(Vz_e, weight);
                h_Vz_e_AC_zoomin_sector1_1e_cut->Fill(Vz_e, weight);

                h_corrected_Vz_e_AC_sector1_1e_cut->Fill(corrected_Vz_e, weight);
                h_corrected_Vz_e_AC_zoomin_sector1_1e_cut->Fill(corrected_Vz_e, weight);

                h_Vz_VS_phi_e_AC_sector1_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);
                h_Vz_VS_phi_e_AC_zoomin_sector1_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);

                raf::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector1_1e_cut, weight);

                h_nphe_AC_sector1_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_AC_sector1_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_AC_sector1_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_AC_sector1_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_AC_sector1_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_AC_sector1_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_AC_sector1_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector1_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons[0]->getSector() == 2) {
                h_Vx_e_AC_sector2_1e_cut->Fill(Vx_e, weight);
                h_Vy_e_AC_sector2_1e_cut->Fill(Vy_e, weight);
                h_Vz_e_AC_sector2_1e_cut->Fill(Vz_e, weight);
                h_Vz_e_AC_zoomin_sector2_1e_cut->Fill(Vz_e, weight);

                h_corrected_Vz_e_AC_sector2_1e_cut->Fill(corrected_Vz_e, weight);
                h_corrected_Vz_e_AC_zoomin_sector2_1e_cut->Fill(corrected_Vz_e, weight);

                h_Vz_VS_phi_e_AC_sector2_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);
                h_Vz_VS_phi_e_AC_zoomin_sector2_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);

                raf::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector2_1e_cut, weight);

                h_nphe_AC_sector2_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_AC_sector2_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_AC_sector2_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_AC_sector2_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_AC_sector2_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_AC_sector2_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_AC_sector2_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector2_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons[0]->getSector() == 3) {
                h_Vx_e_AC_sector3_1e_cut->Fill(Vx_e, weight);
                h_Vy_e_AC_sector3_1e_cut->Fill(Vy_e, weight);
                h_Vz_e_AC_sector3_1e_cut->Fill(Vz_e, weight);
                h_Vz_e_AC_zoomin_sector3_1e_cut->Fill(Vz_e, weight);

                h_corrected_Vz_e_AC_sector3_1e_cut->Fill(corrected_Vz_e, weight);
                h_corrected_Vz_e_AC_zoomin_sector3_1e_cut->Fill(corrected_Vz_e, weight);

                h_Vz_VS_phi_e_AC_sector3_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);
                h_Vz_VS_phi_e_AC_zoomin_sector3_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);

                raf::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector3_1e_cut, weight);

                h_nphe_AC_sector3_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_AC_sector3_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_AC_sector3_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_AC_sector3_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_AC_sector3_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_AC_sector3_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_AC_sector3_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector3_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons[0]->getSector() == 4) {
                h_Vx_e_AC_sector4_1e_cut->Fill(Vx_e, weight);
                h_Vy_e_AC_sector4_1e_cut->Fill(Vy_e, weight);
                h_Vz_e_AC_sector4_1e_cut->Fill(Vz_e, weight);
                h_Vz_e_AC_zoomin_sector4_1e_cut->Fill(Vz_e, weight);

                h_corrected_Vz_e_AC_sector4_1e_cut->Fill(corrected_Vz_e, weight);
                h_corrected_Vz_e_AC_zoomin_sector4_1e_cut->Fill(corrected_Vz_e, weight);

                h_Vz_VS_phi_e_AC_sector4_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);
                h_Vz_VS_phi_e_AC_zoomin_sector4_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);

                raf::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector4_1e_cut, weight);

                h_nphe_AC_sector4_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

                h_Edep_PCAL_VS_EC_AC_sector4_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_Edep_PCAL_AC_sector4_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_SF_VS_P_e_AC_sector4_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_AC_sector4_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_AC_sector4_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_AC_sector4_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector4_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons[0]->getSector() == 5) {
                h_Vx_e_AC_sector5_1e_cut->Fill(Vx_e, weight);
                h_Vy_e_AC_sector5_1e_cut->Fill(Vy_e, weight);
                h_Vz_e_AC_sector5_1e_cut->Fill(Vz_e, weight);
                h_Vz_e_AC_zoomin_sector5_1e_cut->Fill(Vz_e, weight);

                h_corrected_Vz_e_AC_sector5_1e_cut->Fill(corrected_Vz_e, weight);
                h_corrected_Vz_e_AC_zoomin_sector5_1e_cut->Fill(corrected_Vz_e, weight);

                h_Vz_VS_phi_e_AC_sector5_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);
                h_Vz_VS_phi_e_AC_zoomin_sector5_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);

                raf::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector5_1e_cut, weight);

                h_nphe_AC_sector5_1e_cut->Fill(electrons[0]->che(clas12::HTCC)->getNphe(), weight);

                h_SF_VS_Edep_PCAL_AC_sector5_1e_cut->Fill(Edep_PCAL, EoP_e, weight);
                h_Edep_PCAL_VS_EC_AC_sector5_1e_cut->Fill(Edep_PCAL, Edep_EC, weight);

                h_SF_VS_P_e_AC_sector5_1e_cut->Fill(reco_P_e.Mag(), EoP_e, weight);

                h_SF_VS_Lv_AC_sector5_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLv(), EoP_e, weight);
                h_SF_VS_Lw_AC_sector5_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLw(), EoP_e, weight);
                h_SF_VS_Lu_AC_sector5_1e_cut->Fill(electrons[0]->cal(clas12::PCAL)->getLu(), EoP_e, weight);

                h_E_PCALoP_e_VS_E_PCALoP_e_AC_sector5_1e_cut->Fill(E_PCALoP_e, E_ECINoP_e, weight);
            } else if (electrons[0]->getSector() == 6) {
                h_Vx_e_AC_sector6_1e_cut->Fill(Vx_e, weight);
                h_Vy_e_AC_sector6_1e_cut->Fill(Vy_e, weight);
                h_Vz_e_AC_sector6_1e_cut->Fill(Vz_e, weight);
                h_Vz_e_AC_zoomin_sector6_1e_cut->Fill(Vz_e, weight);

                h_corrected_Vz_e_AC_sector6_1e_cut->Fill(corrected_Vz_e, weight);
                h_corrected_Vz_e_AC_zoomin_sector6_1e_cut->Fill(corrected_Vz_e, weight);

                h_Vz_VS_phi_e_AC_sector6_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);
                h_Vz_VS_phi_e_AC_zoomin_sector6_1e_cut->Fill(electrons[0]->getPhi() * 180 / am::pi, Vz_e, weight);

                raf::fillDCdebug(electrons[0], h_dc_electron_hit_map_AC_sector6_1e_cut, weight);

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

                    raf::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_1e_cut, weight);

                    h_Chi2_pipFD_BC_1e_cut->Fill(piplus_det[i]->par()->getChi2Pid(), weight);

                    if (piplus_det[i]->getSector() == 1) {
                        h_Vx_pipFD_BC_sector1_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector1_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector1_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector1_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector1_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector1_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector1_1e_cut, weight);
                    } else if (piplus_det[i]->getSector() == 2) {
                        h_Vx_pipFD_BC_sector2_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector2_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector2_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector2_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector2_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector2_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector2_1e_cut, weight);
                    } else if (piplus_det[i]->getSector() == 3) {
                        h_Vx_pipFD_BC_sector3_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector3_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector3_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector3_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector3_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector3_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector3_1e_cut, weight);
                    } else if (piplus_det[i]->getSector() == 4) {
                        h_Vx_pipFD_BC_sector4_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector4_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector4_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector4_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector4_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector4_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector4_1e_cut, weight);
                    } else if (piplus_det[i]->getSector() == 5) {
                        h_Vx_pipFD_BC_sector5_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector5_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector5_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector5_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector5_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector5_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector5_1e_cut, weight);
                    } else if (piplus_det[i]->getSector() == 6) {
                        h_Vx_pipFD_BC_sector6_1e_cut->Fill(piplus_det[i]->par()->getVx(), weight);
                        h_Vy_pipFD_BC_sector6_1e_cut->Fill(piplus_det[i]->par()->getVy(), weight);
                        h_Vz_pipFD_BC_sector6_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);
                        h_Vz_pipFD_BC_zoomin_sector6_1e_cut->Fill(piplus_det[i]->par()->getVz(), weight);

                        h_dVz_pipFD_BC_sector6_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_BC_zoomin_sector6_1e_cut->Fill(-(piplus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piplus_det[i], h_dc_pipFD_hit_map_BC_sector6_1e_cut, weight);
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
                double Vx_pip = piplus[i]->par()->getVx();
                double Vy_pip = piplus[i]->par()->getVy();
                double Vz_pip = piplus[i]->par()->getVz();

                if (piplus[i]->getRegion() == FD) {
                    auto r_pip = compute_r(Beam_Coordinates, "pipFD");
                    auto phi_beam_pip_rad = compute_phi_beam_rad(Beam_Coordinates, "pipFD");
                    auto corrected_Vz_pip = correct_Vz(Vz_pip, r_pip, piplus[i]->getTheta(), piplus[i]->getPhi(), phi_beam_pip_rad);

                    // std::cout << "Vz_pip: " << Vz_pip << ", corrected_Vz_pip: " << corrected_Vz_pip << std::endl;
                    // std::cout << "r: " << r << ", phi_beam_rad: " << phi_beam_rad << std::endl;
                    // std::cout << std::endl;

                    h_Vx_pipFD_AC_1e_cut->Fill(Vx_pip, weight);
                    h_Vy_pipFD_AC_1e_cut->Fill(Vy_pip, weight);
                    h_Vz_pipFD_AC_1e_cut->Fill(Vz_pip, weight);
                    h_Vz_pipFD_AC_zoomin_1e_cut->Fill(Vz_pip, weight);

                    h_corrected_Vz_pipFD_AC_1e_cut->Fill(corrected_Vz_pip, weight);
                    h_corrected_Vz_pipFD_AC_zoomin_1e_cut->Fill(corrected_Vz_pip, weight);

                    h_dVz_pipFD_AC_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);
                    h_dVz_pipFD_AC_zoomin_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);

                    h_Vz_VS_phi_pipFD_AC_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);
                    h_Vz_VS_theta_pipFD_AC_1e_cut->Fill(piplus[i]->getTheta() * 180 / am::pi, Vz_pip, weight);

                    h_corrected_Vz_VS_phi_pipFD_AC_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, corrected_Vz_pip, weight);
                    h_corrected_Vz_VS_theta_pipFD_AC_1e_cut->Fill(piplus[i]->getTheta() * 180 / am::pi, corrected_Vz_pip, weight);

                    raf::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_1e_cut, weight);

                    h_Chi2_pipFD_AC_1e_cut->Fill(piplus[i]->par()->getChi2Pid(), weight);

                    if (piplus[i]->getSector() == 1) {
                        h_Vx_pipFD_AC_sector1_1e_cut->Fill(Vx_pip, weight);
                        h_Vy_pipFD_AC_sector1_1e_cut->Fill(Vy_pip, weight);
                        h_Vz_pipFD_AC_sector1_1e_cut->Fill(Vz_pip, weight);
                        h_Vz_pipFD_AC_zoomin_sector1_1e_cut->Fill(Vz_pip, weight);

                        h_corrected_Vz_pipFD_AC_sector1_1e_cut->Fill(corrected_Vz_pip, weight);
                        h_corrected_Vz_pipFD_AC_zoomin_sector1_1e_cut->Fill(corrected_Vz_pip, weight);

                        h_dVz_pipFD_AC_sector1_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector1_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pipFD_AC_sector1_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);
                        h_Vz_VS_phi_pipFD_AC_zoomin_sector1_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);

                        raf::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector1_1e_cut, weight);
                    } else if (piplus[i]->getSector() == 2) {
                        h_Vx_pipFD_AC_sector2_1e_cut->Fill(Vx_pip, weight);
                        h_Vy_pipFD_AC_sector2_1e_cut->Fill(Vy_pip, weight);
                        h_Vz_pipFD_AC_sector2_1e_cut->Fill(Vz_pip, weight);
                        h_Vz_pipFD_AC_zoomin_sector2_1e_cut->Fill(Vz_pip, weight);

                        h_corrected_Vz_pipFD_AC_sector2_1e_cut->Fill(corrected_Vz_pip, weight);
                        h_corrected_Vz_pipFD_AC_zoomin_sector2_1e_cut->Fill(corrected_Vz_pip, weight);

                        h_dVz_pipFD_AC_sector2_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector2_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pipFD_AC_sector2_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);
                        h_Vz_VS_phi_pipFD_AC_zoomin_sector2_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);

                        raf::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector2_1e_cut, weight);
                    } else if (piplus[i]->getSector() == 3) {
                        h_Vx_pipFD_AC_sector3_1e_cut->Fill(Vx_pip, weight);
                        h_Vy_pipFD_AC_sector3_1e_cut->Fill(Vy_pip, weight);
                        h_Vz_pipFD_AC_sector3_1e_cut->Fill(Vz_pip, weight);
                        h_Vz_pipFD_AC_zoomin_sector3_1e_cut->Fill(Vz_pip, weight);

                        h_corrected_Vz_pipFD_AC_sector3_1e_cut->Fill(corrected_Vz_pip, weight);
                        h_corrected_Vz_pipFD_AC_zoomin_sector3_1e_cut->Fill(corrected_Vz_pip, weight);

                        h_dVz_pipFD_AC_sector3_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector3_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pipFD_AC_sector3_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);
                        h_Vz_VS_phi_pipFD_AC_zoomin_sector3_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);

                        raf::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector3_1e_cut, weight);
                    } else if (piplus[i]->getSector() == 4) {
                        h_Vx_pipFD_AC_sector4_1e_cut->Fill(Vx_pip, weight);
                        h_Vy_pipFD_AC_sector4_1e_cut->Fill(Vy_pip, weight);
                        h_Vz_pipFD_AC_sector4_1e_cut->Fill(Vz_pip, weight);
                        h_Vz_pipFD_AC_zoomin_sector4_1e_cut->Fill(Vz_pip, weight);

                        h_corrected_Vz_pipFD_AC_sector4_1e_cut->Fill(corrected_Vz_pip, weight);
                        h_corrected_Vz_pipFD_AC_zoomin_sector4_1e_cut->Fill(corrected_Vz_pip, weight);

                        h_dVz_pipFD_AC_sector4_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector4_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pipFD_AC_sector4_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);
                        h_Vz_VS_phi_pipFD_AC_zoomin_sector4_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);

                        raf::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector4_1e_cut, weight);
                    } else if (piplus[i]->getSector() == 5) {
                        h_Vx_pipFD_AC_sector5_1e_cut->Fill(Vx_pip, weight);
                        h_Vy_pipFD_AC_sector5_1e_cut->Fill(Vy_pip, weight);
                        h_Vz_pipFD_AC_sector5_1e_cut->Fill(Vz_pip, weight);
                        h_Vz_pipFD_AC_zoomin_sector5_1e_cut->Fill(Vz_pip, weight);

                        h_corrected_Vz_pipFD_AC_sector5_1e_cut->Fill(corrected_Vz_pip, weight);
                        h_corrected_Vz_pipFD_AC_zoomin_sector5_1e_cut->Fill(corrected_Vz_pip, weight);

                        h_dVz_pipFD_AC_sector5_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector5_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pipFD_AC_sector5_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);
                        h_Vz_VS_phi_pipFD_AC_zoomin_sector5_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);

                        raf::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector5_1e_cut, weight);
                    } else if (piplus[i]->getSector() == 6) {
                        h_Vx_pipFD_AC_sector6_1e_cut->Fill(Vx_pip, weight);
                        h_Vy_pipFD_AC_sector6_1e_cut->Fill(Vy_pip, weight);
                        h_Vz_pipFD_AC_sector6_1e_cut->Fill(Vz_pip, weight);
                        h_Vz_pipFD_AC_zoomin_sector6_1e_cut->Fill(Vz_pip, weight);

                        h_corrected_Vz_pipFD_AC_sector6_1e_cut->Fill(corrected_Vz_pip, weight);
                        h_corrected_Vz_pipFD_AC_zoomin_sector6_1e_cut->Fill(corrected_Vz_pip, weight);

                        h_dVz_pipFD_AC_sector6_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);
                        h_dVz_pipFD_AC_zoomin_sector6_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pipFD_AC_sector6_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);
                        h_Vz_VS_phi_pipFD_AC_zoomin_sector6_1e_cut->Fill(piplus[i]->getPhi() * 180 / am::pi, Vz_pip, weight);

                        raf::fillDCdebug(piplus[i], h_dc_pipFD_hit_map_AC_sector6_1e_cut, weight);
                    }
                } else if (piplus[i]->getRegion() == CD) {
                    h_Chi2_pipCD_AC_1e_cut->Fill(piplus[i]->par()->getChi2Pid(), weight);

                    h_Vx_pipCD_AC_1e_cut->Fill(Vx_pip, weight);
                    h_Vy_pipCD_AC_1e_cut->Fill(Vy_pip, weight);
                    h_Vz_pipCD_AC_1e_cut->Fill(Vz_pip, weight);
                    h_Vz_pipCD_AC_zoomin_1e_cut->Fill(Vz_pip, weight);

                    h_dVz_pipCD_AC_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);
                    h_dVz_pipCD_AC_zoomin_1e_cut->Fill(-(Vz_pip - electrons[0]->par()->getVz()), weight);
                }
            }
#pragma endregion

            //  - Piminus cuts ------------------------------------------------------------------------------------------------------------------------------------------

#pragma region piminus BPID
            for (int i = 0; i < piminus_det.size(); i++) {
                if (piminus_det[i]->getRegion() == FD) {
                    h_Vx_pimFD_BC_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                    h_Vy_pimFD_BC_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                    h_Vz_pimFD_BC_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                    h_Vz_pimFD_BC_zoomin_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                    h_dVz_pimFD_BC_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                    h_dVz_pimFD_BC_zoomin_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                    raf::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_1e_cut, weight);

                    h_Chi2_pimFD_BC_1e_cut->Fill(piminus_det[i]->par()->getChi2Pid(), weight);

                    if (piminus_det[i]->getSector() == 1) {
                        h_Vx_pimFD_BC_sector1_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector1_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector1_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector1_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector1_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector1_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector1_1e_cut, weight);
                    } else if (piminus_det[i]->getSector() == 2) {
                        h_Vx_pimFD_BC_sector2_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector2_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector2_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector2_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector2_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector2_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector2_1e_cut, weight);
                    } else if (piminus_det[i]->getSector() == 3) {
                        h_Vx_pimFD_BC_sector3_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector3_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector3_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector3_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector3_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector3_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector3_1e_cut, weight);
                    } else if (piminus_det[i]->getSector() == 4) {
                        h_Vx_pimFD_BC_sector4_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector4_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector4_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector4_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector4_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector4_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector4_1e_cut, weight);
                    } else if (piminus_det[i]->getSector() == 5) {
                        h_Vx_pimFD_BC_sector5_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector5_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector5_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector5_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector5_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector5_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector5_1e_cut, weight);
                    } else if (piminus_det[i]->getSector() == 6) {
                        h_Vx_pimFD_BC_sector6_1e_cut->Fill(piminus_det[i]->par()->getVx(), weight);
                        h_Vy_pimFD_BC_sector6_1e_cut->Fill(piminus_det[i]->par()->getVy(), weight);
                        h_Vz_pimFD_BC_sector6_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);
                        h_Vz_pimFD_BC_zoomin_sector6_1e_cut->Fill(piminus_det[i]->par()->getVz(), weight);

                        h_dVz_pimFD_BC_sector6_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_BC_zoomin_sector6_1e_cut->Fill(-(piminus_det[i]->par()->getVz() - electrons[0]->par()->getVz()), weight);

                        raf::fillDCdebug(piminus_det[i], h_dc_pimFD_hit_map_BC_sector6_1e_cut, weight);
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
                double Vx_pim = piminus[i]->par()->getVx();
                double Vy_pim = piminus[i]->par()->getVy();
                double Vz_pim = piminus[i]->par()->getVz();

                if (piminus[i]->getRegion() == FD) {
                    auto r_pim = compute_r(Beam_Coordinates, "pimFD");
                    auto phi_beam_pim_rad = compute_phi_beam_rad(Beam_Coordinates, "pimFD");
                    auto corrected_Vz_pim = correct_Vz(Vz_pim, r_pim, piminus[i]->getTheta(), piminus[i]->getPhi(), phi_beam_pim_rad);

                    h_Vx_pimFD_AC_1e_cut->Fill(Vx_pim, weight);
                    h_Vy_pimFD_AC_1e_cut->Fill(Vy_pim, weight);
                    h_Vz_pimFD_AC_1e_cut->Fill(Vz_pim, weight);
                    h_Vz_pimFD_AC_zoomin_1e_cut->Fill(Vz_pim, weight);

                    h_corrected_Vz_pimFD_AC_1e_cut->Fill(corrected_Vz_pim, weight);
                    h_corrected_Vz_pimFD_AC_zoomin_1e_cut->Fill(corrected_Vz_pim, weight);

                    h_dVz_pimFD_AC_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);
                    h_dVz_pimFD_AC_zoomin_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);

                    h_Vz_VS_phi_pimFD_AC_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);
                    h_Vz_VS_theta_pimFD_AC_1e_cut->Fill(piminus[i]->getTheta() * 180 / am::pi, Vz_pim, weight);

                    h_corrected_Vz_VS_phi_pimFD_AC_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, corrected_Vz_pim, weight);
                    h_corrected_Vz_VS_theta_pimFD_AC_1e_cut->Fill(piminus[i]->getTheta() * 180 / am::pi, corrected_Vz_pim, weight);

                    raf::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_1e_cut, weight);

                    h_Chi2_pimFD_AC_1e_cut->Fill(piminus[i]->par()->getChi2Pid(), weight);

                    if (piminus[i]->getSector() == 1) {
                        h_Vx_pimFD_AC_sector1_1e_cut->Fill(Vx_pim, weight);
                        h_Vy_pimFD_AC_sector1_1e_cut->Fill(Vy_pim, weight);
                        h_Vz_pimFD_AC_sector1_1e_cut->Fill(Vz_pim, weight);
                        h_Vz_pimFD_AC_zoomin_sector1_1e_cut->Fill(Vz_pim, weight);

                        h_corrected_Vz_pimFD_AC_sector1_1e_cut->Fill(corrected_Vz_pim, weight);
                        h_corrected_Vz_pimFD_AC_zoomin_sector1_1e_cut->Fill(corrected_Vz_pim, weight);

                        h_dVz_pimFD_AC_sector1_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector1_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pimFD_AC_sector1_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);
                        h_Vz_VS_phi_pimFD_AC_zoomin_sector1_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);

                        raf::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector1_1e_cut, weight);
                    } else if (piminus[i]->getSector() == 2) {
                        h_Vx_pimFD_AC_sector2_1e_cut->Fill(Vx_pim, weight);
                        h_Vy_pimFD_AC_sector2_1e_cut->Fill(Vy_pim, weight);
                        h_Vz_pimFD_AC_sector2_1e_cut->Fill(Vz_pim, weight);
                        h_Vz_pimFD_AC_zoomin_sector2_1e_cut->Fill(Vz_pim, weight);

                        h_corrected_Vz_pimFD_AC_sector2_1e_cut->Fill(corrected_Vz_pim, weight);
                        h_corrected_Vz_pimFD_AC_zoomin_sector2_1e_cut->Fill(corrected_Vz_pim, weight);

                        h_dVz_pimFD_AC_sector2_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector2_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pimFD_AC_sector2_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);
                        h_Vz_VS_phi_pimFD_AC_zoomin_sector2_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);

                        raf::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector2_1e_cut, weight);
                    } else if (piminus[i]->getSector() == 3) {
                        h_Vx_pimFD_AC_sector3_1e_cut->Fill(Vx_pim, weight);
                        h_Vy_pimFD_AC_sector3_1e_cut->Fill(Vy_pim, weight);
                        h_Vz_pimFD_AC_sector3_1e_cut->Fill(Vz_pim, weight);
                        h_Vz_pimFD_AC_zoomin_sector3_1e_cut->Fill(Vz_pim, weight);

                        h_corrected_Vz_pimFD_AC_sector3_1e_cut->Fill(corrected_Vz_pim, weight);
                        h_corrected_Vz_pimFD_AC_zoomin_sector3_1e_cut->Fill(corrected_Vz_pim, weight);

                        h_dVz_pimFD_AC_sector3_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector3_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pimFD_AC_sector3_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);
                        h_Vz_VS_phi_pimFD_AC_zoomin_sector3_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);

                        raf::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector3_1e_cut, weight);
                    } else if (piminus[i]->getSector() == 4) {
                        h_Vx_pimFD_AC_sector4_1e_cut->Fill(Vx_pim, weight);
                        h_Vy_pimFD_AC_sector4_1e_cut->Fill(Vy_pim, weight);
                        h_Vz_pimFD_AC_sector4_1e_cut->Fill(Vz_pim, weight);
                        h_Vz_pimFD_AC_zoomin_sector4_1e_cut->Fill(Vz_pim, weight);

                        h_corrected_Vz_pimFD_AC_sector4_1e_cut->Fill(corrected_Vz_pim, weight);
                        h_corrected_Vz_pimFD_AC_zoomin_sector4_1e_cut->Fill(corrected_Vz_pim, weight);

                        h_dVz_pimFD_AC_sector4_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector4_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pimFD_AC_sector4_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);
                        h_Vz_VS_phi_pimFD_AC_zoomin_sector4_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);

                        raf::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector4_1e_cut, weight);
                    } else if (piminus[i]->getSector() == 5) {
                        h_Vx_pimFD_AC_sector5_1e_cut->Fill(Vx_pim, weight);
                        h_Vy_pimFD_AC_sector5_1e_cut->Fill(Vy_pim, weight);
                        h_Vz_pimFD_AC_sector5_1e_cut->Fill(Vz_pim, weight);
                        h_Vz_pimFD_AC_zoomin_sector5_1e_cut->Fill(Vz_pim, weight);

                        h_corrected_Vz_pimFD_AC_sector5_1e_cut->Fill(corrected_Vz_pim, weight);
                        h_corrected_Vz_pimFD_AC_zoomin_sector5_1e_cut->Fill(corrected_Vz_pim, weight);

                        h_dVz_pimFD_AC_sector5_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector5_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pimFD_AC_sector5_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);
                        h_Vz_VS_phi_pimFD_AC_zoomin_sector5_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);

                        raf::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector5_1e_cut, weight);
                    } else if (piminus[i]->getSector() == 6) {
                        h_Vx_pimFD_AC_sector6_1e_cut->Fill(Vx_pim, weight);
                        h_Vy_pimFD_AC_sector6_1e_cut->Fill(Vy_pim, weight);
                        h_Vz_pimFD_AC_sector6_1e_cut->Fill(Vz_pim, weight);
                        h_Vz_pimFD_AC_zoomin_sector6_1e_cut->Fill(Vz_pim, weight);

                        h_corrected_Vz_pimFD_AC_sector6_1e_cut->Fill(corrected_Vz_pim, weight);
                        h_corrected_Vz_pimFD_AC_zoomin_sector6_1e_cut->Fill(corrected_Vz_pim, weight);

                        h_dVz_pimFD_AC_sector6_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);
                        h_dVz_pimFD_AC_zoomin_sector6_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);

                        h_Vz_VS_phi_pimFD_AC_sector6_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);
                        h_Vz_VS_phi_pimFD_AC_zoomin_sector6_1e_cut->Fill(piminus[i]->getPhi() * 180 / am::pi, Vz_pim, weight);

                        raf::fillDCdebug(piminus[i], h_dc_pimFD_hit_map_AC_sector6_1e_cut, weight);
                    }
                } else if (piminus[i]->getRegion() == CD) {
                    h_Chi2_pimCD_AC_1e_cut->Fill(piminus[i]->par()->getChi2Pid(), weight);

                    h_Vx_pimCD_AC_1e_cut->Fill(Vx_pim, weight);
                    h_Vy_pimCD_AC_1e_cut->Fill(Vy_pim, weight);
                    h_Vz_pimCD_AC_1e_cut->Fill(Vz_pim, weight);
                    h_Vz_pimCD_AC_zoomin_1e_cut->Fill(Vz_pim, weight);

                    h_dVz_pimCD_AC_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);
                    h_dVz_pimCD_AC_zoomin_1e_cut->Fill(-(Vz_pim - electrons[0]->par()->getVz()), weight);
                }
            }
#pragma endregion
        }

#pragma endregion

#pragma region nExtracting Vz correction parameters
        std::cout << "\033[33m" << "\n\nExtracting Vz correction parameters..." << "\n\n" << "\033[0m";

        /////////////////////////////////////////////////////
        // Extracting Vz correction parameters
        /////////////////////////////////////////////////////

        auto fit_peak_gaussian = [](TH1D *hist, std::vector<double> fitLimits = {}) -> double {
            double fitMin, fitMax;

            if (hist->GetEntries() == 0) {
                std::cerr << "Histogram is empty. Returning NaN." << std::endl;
                return std::numeric_limits<double>::quiet_NaN();
            }

            if (fitLimits.size() == 0) {
                // If no limits are provided, use the histogram's peak center
                double peakCenter = hist->GetBinCenter(hist->GetMaximumBin());
                fitMin = -std::fabs(peakCenter * 1.1);
                fitMax = -std::fabs(peakCenter * 0.9);
            } else if (fitLimits.size() == 2) {
                fitMin = fitLimits[0];
                fitMax = fitLimits[1];
            } else {
                std::cerr << "Error: fitLimits must contain exactly two elements." << std::endl;
                return std::numeric_limits<double>::quiet_NaN();
            }

            TF1 *fit = new TF1("fit", "gaus", fitMin, fitMax);
            hist->Fit(fit, "RQ");  // R = use range, Q = quiet

            fit->SetLineColor(kViolet);
            // fit->SetLineColor(kMagenta);

            hist->GetListOfFunctions()->Clear();
            hist->GetListOfFunctions()->Add(fit);  // Add fit to the histogram's function list

            return fit->GetParameter(1);  // Return fitted mean
        };

        // Usage:
        double peak_sector1 = fit_peak_gaussian(h_Vz_e_AC_sector1_1e_cut);
        double peak_sector2 = fit_peak_gaussian(h_Vz_e_AC_sector2_1e_cut);
        double peak_sector3 = fit_peak_gaussian(h_Vz_e_AC_sector3_1e_cut);
        double peak_sector4 = fit_peak_gaussian(h_Vz_e_AC_sector4_1e_cut);
        double peak_sector5 = fit_peak_gaussian(h_Vz_e_AC_sector5_1e_cut);
        double peak_sector6 = fit_peak_gaussian(h_Vz_e_AC_sector6_1e_cut);

        std::vector<double> Vz_e_peaks_BySector = {
            h_Vz_e_AC_sector1_1e_cut->GetBinCenter(h_Vz_e_AC_sector1_1e_cut->GetMaximumBin()), h_Vz_e_AC_sector2_1e_cut->GetBinCenter(h_Vz_e_AC_sector2_1e_cut->GetMaximumBin()),
            h_Vz_e_AC_sector3_1e_cut->GetBinCenter(h_Vz_e_AC_sector3_1e_cut->GetMaximumBin()), h_Vz_e_AC_sector4_1e_cut->GetBinCenter(h_Vz_e_AC_sector4_1e_cut->GetMaximumBin()),
            h_Vz_e_AC_sector5_1e_cut->GetBinCenter(h_Vz_e_AC_sector5_1e_cut->GetMaximumBin()), h_Vz_e_AC_sector6_1e_cut->GetBinCenter(h_Vz_e_AC_sector6_1e_cut->GetMaximumBin())};
        auto [A, phi_beam, Z0, FittedParametersGraph] = am::FitVertexVsPhi("e", Ebeam_status_1, Vz_e_peaks_BySector);
#pragma endregion

#pragma region Plotting and saving histograms
        std::cout << "\033[33m" << "\n\nPlotting and saving histograms..." << "\n\n" << "\033[0m";

        /////////////////////////////////////////////////////
        // Organize histograms
        /////////////////////////////////////////////////////
        int insert_index = 0;

        for (int i = 0; i < HistoList.size(); i++) {
            if (HistoList[i]->InheritsFrom("TH1")) {
                auto *h1 = (TH1 *)HistoList[i];
                h1->Sumw2();
                h1->SetMinimum(0);
                h1->SetLineWidth(2);
                h1->SetLineColor(kRed);
            }

            if (HistoList[i]->InheritsFrom("TH1") || HistoList[i]->InheritsFrom("TH2")) {
                auto *h = (TH1 *)HistoList[i];
                h->GetXaxis()->CenterTitle();
                h->GetYaxis()->CenterTitle();
            } else if (HistoList[i]->InheritsFrom("TGraph")) {
                auto *g = (TGraph *)HistoList[i];
                g->GetXaxis()->CenterTitle();
                g->GetYaxis()->CenterTitle();
            }

            if (std::string(HistoList[i]->GetName()) == "Vz_VS_phi_e_AC_1e_cut") { insert_index = i + 1; }
        }

        HistoList.insert(HistoList.begin() + insert_index, FittedParametersGraph);
        FittedParametersGraph->GetXaxis()->CenterTitle();
        FittedParametersGraph->GetYaxis()->CenterTitle();

        for (int i = 0; i < HistoList_ByThetaSlices.size(); i++) {
            if (HistoList_ByThetaSlices[i]->InheritsFrom("TH1")) {
                auto *h1 = (TH1 *)HistoList_ByThetaSlices[i];
                h1->Sumw2();
                h1->SetMinimum(0);
                h1->SetLineWidth(2);
                h1->SetLineColor(kRed);
            }

            if (HistoList_ByThetaSlices[i]->InheritsFrom("TH1") || HistoList_ByThetaSlices[i]->InheritsFrom("TH2")) {
                auto *h = (TH1 *)HistoList_ByThetaSlices[i];
                h->GetXaxis()->CenterTitle();
                h->GetYaxis()->CenterTitle();
            } else if (HistoList_ByThetaSlices[i]->InheritsFrom("TGraph")) {
                auto *g = (TGraph *)HistoList_ByThetaSlices[i];
                g->GetXaxis()->CenterTitle();
                g->GetYaxis()->CenterTitle();
            }

            // if (std::string(HistoList_ByThetaSlices[i]->GetName()) == "Vz_VS_phi_e_AC_1e_cut") { insert_index = i + 1; }
        }

        /////////////////////////////////////////////////////
        // Now create the output PDFs
        /////////////////////////////////////////////////////
        auto GeneratePDFOutput = [&](std::string TempOutputDir, std::string TempOutFolderName, std::string TempBaseDir, std::vector<std::string> TempInputFiles, int Tempsample,
                                     std::vector<TObject *> TempHistoList, int TempNumOfEvents, int TempNumOfEvents_wAny_e_det, int TempNumOfEvents_wOne_e_det, int TempNumOfEvents_wAny_e,
                                     int TempNumOfEvents_wOne_e, std::string TempCodeRun_status, bool TempIsData, std::string Temptarget_status) {
            int pixelx = 1980;
            int pixely = 1530;
            TCanvas *myText = new TCanvas("myText", "myText", pixelx, pixely);
            TLatex titles;
            TLatex text;
            titles.SetTextSize(0.1);
            text.SetTextSize(0.03);

            gStyle->SetOptStat("ourmen");

            TCanvas *myCanvas = new TCanvas("myPage", "myPage", pixelx, pixely);

            std::string PDF_fileName = TempOutputDir + "/" + TempOutFolderName + ".pdf";
            char fileName[PDF_fileName.length()];
            sprintf(fileName, "%s[", PDF_fileName.c_str());
            myText->SaveAs(fileName);
            sprintf(fileName, "%s", PDF_fileName.c_str());

            // First page:
            myText->cd();
            titles.DrawLatex(0.05, 0.9, "HipoLooper Output");
            text.DrawLatex(0.05, 0.80, "This output is for the Ar40 implementation in GEMC");
            text.DrawLatex(0.05, 0.70, ("Plots from (e,e') events in: #font[42]{" + TempCodeRun_status + "}").c_str());

            if (TempIsData) {
                text.DrawLatex(0.05, 0.65, ("TempInputFiles: #font[42]{" + TempInputFiles.at(Tempsample) + "}").c_str());
                text.DrawLatex(0.05, 0.60, "TempOutFolderName:");
                text.DrawLatex(0.10, 0.55, ("#font[42]{" + TempOutFolderName + "}").c_str());
            } else {
                text.DrawLatex(0.05, 0.65, ("TempBaseDir: #font[42]{" + TempBaseDir + "}").c_str());
                text.DrawLatex(0.05, 0.60, ("TempInputFiles: #font[42]{TempBaseDir + " + TempInputFiles.at(Tempsample).substr(TempBaseDir.length()) + "}").c_str());
                text.DrawLatex(0.05, 0.55, "TempOutFolderName:");
                text.DrawLatex(0.10, 0.50, ("#font[42]{" + TempOutFolderName + "}").c_str());
            }

            if (ApplyLimiter) {
                text.DrawLatex(0.05, 0.45, ("Event counts (ApplyLimiter = " + bt::BoolToString(ApplyLimiter) + ", Limiter = " + bt::ToStringWithPrecision(Limiter, 0) + "):").c_str());
            } else {
                text.DrawLatex(0.05, 0.45, ("Event counts (ApplyLimiter = " + bt::BoolToString(ApplyLimiter) + "):").c_str());
            }

            text.DrawLatex(0.10, 0.40, ("Total #(events):            #font[42]{" + std::to_string(TempNumOfEvents) + "}").c_str());
            text.DrawLatex(0.10, 0.35, ("#(Events) with any e_det:  #font[42]{" + std::to_string(TempNumOfEvents_wAny_e_det) + "}").c_str());
            text.DrawLatex(0.10, 0.30,
                           ("#(Events) with one e_det:  #font[42]{" + std::to_string(TempNumOfEvents_wOne_e_det) + " (" +
                            bt::ToStringWithPrecision((100 * TempNumOfEvents_wOne_e_det / TempNumOfEvents_wAny_e_det), 2) + "%)}")
                               .c_str());
            text.DrawLatex(0.10, 0.25, ("#(Events) with any e:       #font[42]{" + std::to_string(TempNumOfEvents_wAny_e) + "}").c_str());
            text.DrawLatex(0.10, 0.20,
                           ("#(Events) with one e:       #font[42]{" + std::to_string(TempNumOfEvents_wOne_e) + " (" +
                            bt::ToStringWithPrecision((100 * TempNumOfEvents_wOne_e / TempNumOfEvents_wAny_e), 2) + "%)}")
                               .c_str());

            myText->Print(fileName, "pdf Title: Cover");
            myText->Clear();

            // Second page:
            titles.SetTextSize(0.075);
            titles.DrawLatex(0.05, 0.90, "Beam position parameters for corrected Vz");
            text.DrawLatex(0.05, 0.70, "e^{-}:");
            text.DrawLatex(0.10, 0.65,
                           ("Cartesian: #font[42]{(V_{x}^{e},V_{y}^{e}) = (" + bt::ToStringWithPrecision(Beam_Coordinates.at(CodeRun_status + "_e").first) + " cm, " +
                            bt::ToStringWithPrecision(Beam_Coordinates.at(CodeRun_status + "_e").second) + " cm)}")
                               .c_str());
            text.DrawLatex(0.10, 0.60,
                           ("Polar: #font[42]{(r_{e}, #phi_{beam}^{e}) = (" + bt::ToStringWithPrecision(compute_r(Beam_Coordinates, "e")) + " cm, " +
                            bt::ToStringWithPrecision(compute_phi_beam_rad(Beam_Coordinates, "e") * 180 / am::pi) + "#circ)}")
                               .c_str());
            text.DrawLatex(0.05, 0.55, "#pi^{+}FD:");
            text.DrawLatex(0.10, 0.50,
                           ("Cartesian: #font[42]{(V_{x}^{#pi^{+}FD},V_{y}^{#pi^{+}FD}) = (" + bt::ToStringWithPrecision(Beam_Coordinates.at(CodeRun_status + "_pipFD").first) + " cm, " +
                            bt::ToStringWithPrecision(Beam_Coordinates.at(CodeRun_status + "_pipFD").second) + " cm)}")
                               .c_str());
            text.DrawLatex(0.10, 0.45,
                           ("Polar: #font[42]{(r_{#pi^{+}FD}, #phi_{beam}^{#pi^{+}FD}) = (" + bt::ToStringWithPrecision(compute_r(Beam_Coordinates, "pipFD")) + " cm, " +
                            bt::ToStringWithPrecision(compute_phi_beam_rad(Beam_Coordinates, "pipFD") * 180 / am::pi) + "#circ)}")
                               .c_str());
            text.DrawLatex(0.05, 0.40, "#pi^{-}FD:");
            text.DrawLatex(0.10, 0.35,
                           ("Cartesian: #font[42]{(V_{x}^{#pi^{-}FD},V_{y}^{#pi^{-}FD}) = (" + bt::ToStringWithPrecision(Beam_Coordinates.at(CodeRun_status + "_pimFD").first) + " cm, " +
                            bt::ToStringWithPrecision(Beam_Coordinates.at(CodeRun_status + "_pimFD").second) + " cm)}")
                               .c_str());
            text.DrawLatex(0.10, 0.30,
                           ("Polar: #font[42]{(r_{#pi^{-}FD}, #phi_{beam}^{#pi^{-}FD}) = (" + bt::ToStringWithPrecision(compute_r(Beam_Coordinates, "pimFD")) + " cm, " +
                            bt::ToStringWithPrecision(compute_phi_beam_rad(Beam_Coordinates, "pimFD") * 180 / am::pi) + "#circ)}")
                               .c_str());

            myText->Print(fileName, "pdf Title: Parameters");
            myText->Clear();

            // Remaining flags, loops, and drawing logic as-is
            // (You may optionally break this block further into helper lambdas)
            // ...
            // (Paste all remaining code from first_electron through ReassignPDFBookmarks here)

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

            for (int i = 0; i < TempHistoList.size(); i++) {
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

                std::string title = TempHistoList[i]->GetTitle();

                for (const auto &[particle_key, label] : particle_labels) {
                    if (bt::FindSubstring(title, particle_key)) {
                        myText->cd();
                        titles.SetTextAlign(22);  // Center text both horizontally and vertically

                        if (*first_flags[particle_key] && !bt::FindSubstring(title, "sector")) {
                            std::string bookmark_title = label + " plots";
                            std::string sanitized_bookmark_title = hf::SanitizeForBookmark(bookmark_title);
                            titles.DrawLatex(0.5, 0.5, bookmark_title.c_str());
                            myText->Print(fileName, ("pdf Title:" + sanitized_bookmark_title).c_str());
                            myText->Clear();
                            *first_flags[particle_key] = false;
                            ++plot_counter;
                        } else {
                            for (int sector = 1; sector <= 6; ++sector) {
                                std::string sector_str = "sector" + std::to_string(sector);
                                std::string sector_title_str = "sector " + std::to_string(sector);

                                if (*sector_flags[particle_key][sector] && bt::FindSubstring(title, sector_str)) {
                                    std::string bookmark_title = label + " plots - " + sector_title_str;
                                    // Compose hierarchical bookmark: parent>child (separation expects '>' for hierarchy)
                                    std::string hierarchical_title = hf::SanitizeForBookmark(label + " plots") + ">" + hf::SanitizeForBookmark(bookmark_title);
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

                if (TempHistoList[i]->InheritsFrom("TH1") || TempHistoList[i]->InheritsFrom("TH2")) {
                    auto *h = (TH1 *)TempHistoList[i];
                    h->GetYaxis()->SetTitleOffset(1.5);
                    h->GetXaxis()->SetTitleOffset(1.1);
                } else if (TempHistoList[i]->InheritsFrom("TGraph")) {
                    auto *g = (TGraph *)TempHistoList[i];
                    g->SetTitle((std::string(g->GetTitle()) + std::string(" - " + TempCodeRun_status)).c_str());  // Ensure title has x and y labels

                    g->GetYaxis()->SetTitleOffset(1.5);
                    g->GetXaxis()->SetTitleOffset(1.1);
                }

                if (TempHistoList[i]->InheritsFrom("TH1D")) {
                    auto *h = (TH1D *)TempHistoList[i];

                    h->Draw();

                    if (bt::FindSubstring(h->GetTitle(), "Corrected V_{z}^{") || (bt::FindSubstring(h->GetTitle(), "V_{z}^{") && !bt::FindSubstring(h->GetTitle(), "dV_{z}^{"))) {
                        gPad->Update();
                        TLine *speac_target_location_TLine;
                        double speac_target_location_value = 0.0;

                        if (Temptarget_status == "C12") {
                            speac_target_location_value = (2.5 - 3.0);
                        } else if (Temptarget_status == "Ar40") {
                            speac_target_location_value = (-2.5 - 3.0);
                        }

                        speac_target_location_TLine = new TLine(speac_target_location_value, 0., speac_target_location_value, gPad->GetFrame()->GetY2());
                        speac_target_location_TLine->SetLineColor(kBlue);
                        speac_target_location_TLine->Draw("same");

                        TLine *measured_target_location_TLine;
                        double measured_target_location_value = fit_peak_gaussian(h);

                        measured_target_location_TLine = new TLine(measured_target_location_value, 0., measured_target_location_value, gPad->GetFrame()->GetY2());
                        measured_target_location_TLine->SetLineColor(kGreen + 1);
                        measured_target_location_TLine->SetLineWidth(3);
                        measured_target_location_TLine->SetLineStyle(2);
                        measured_target_location_TLine->Draw("same");

                        auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25 - yOffset, gStyle->GetStatX() - 0.25, gStyle->GetStatY() - 0.375 - yOffset);
                        TLegendEntry *speac_target_location_TLine_entry =
                            Legend->AddEntry(speac_target_location_TLine, ("Spec. z pos. = " + bt::ToStringWithPrecision(speac_target_location_value, 2) + " cm").c_str(), "l");
                        TLegendEntry *measured_target_location_TLine_entry =
                            Legend->AddEntry(measured_target_location_TLine, ("Meas. z pos. = " + bt::ToStringWithPrecision(measured_target_location_value, 2) + " cm").c_str(), "l");

                        Legend->Draw("same");

                        auto ListOfFunctions = h->GetListOfFunctions();
                        ListOfFunctions->Add(speac_target_location_TLine);
                        ListOfFunctions->Add(measured_target_location_TLine);
                        ListOfFunctions->Add(Legend);
                    } else if ((bt::FindSubstring(h->GetTitle(), "V_{x}^{") || bt::FindSubstring(h->GetTitle(), "V_{y}^{")) && bt::FindSubstring(h->GetTitle(), "after")) {
                        gPad->Update();

                        std::vector<double> fitLimits = {h->GetBinCenter(h->GetMaximumBin()) * 0.7, h->GetBinCenter(h->GetMaximumBin()) * 1.3};

                        TLine *measured_target_location_TLine;
                        // double measured_target_location_value = h->GetBinCenter(h->GetMaximumBin());
                        double measured_target_location_value = fit_peak_gaussian(h, fitLimits);
                        // double measured_target_location_value = fit_peak_gaussian(h);

                        measured_target_location_TLine = new TLine(measured_target_location_value, 0., measured_target_location_value, gPad->GetFrame()->GetY2());
                        measured_target_location_TLine->SetLineColor(kGreen + 1);
                        measured_target_location_TLine->SetLineWidth(3);
                        measured_target_location_TLine->SetLineStyle(2);
                        measured_target_location_TLine->Draw("same");

                        auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25 - yOffset, gStyle->GetStatX() - 0.25, gStyle->GetStatY() - 0.325 - yOffset);
                        // auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25 - yOffset, gStyle->GetStatX() - 0.25, gStyle->GetStatY() - 0.375 - yOffset);
                        TLegendEntry *measured_target_location_TLine_entry;

                        if (bt::FindSubstring(h->GetTitle(), "V_{x}^{")) {
                            measured_target_location_TLine_entry =
                                Legend->AddEntry(measured_target_location_TLine, ("Meas. x pos. = " + bt::ToStringWithPrecision(measured_target_location_value, 4) + " cm").c_str(), "l");
                            // Legend->AddEntry(measured_target_location_TLine, ("Meas. x pos. = " + bt::ToStringWithPrecision(measured_target_location_value, 2) + "
                            // cm").c_str(), "l");
                        } else if (bt::FindSubstring(h->GetTitle(), "V_{y}^{")) {
                            measured_target_location_TLine_entry =
                                Legend->AddEntry(measured_target_location_TLine, ("Meas. y pos. = " + bt::ToStringWithPrecision(measured_target_location_value, 4) + " cm").c_str(), "l");
                            // Legend->AddEntry(measured_target_location_TLine, ("Meas. y pos. = " + bt::ToStringWithPrecision(measured_target_location_value, 2) + "
                            // cm").c_str(), "l");
                        }

                        Legend->Draw("same");

                        auto ListOfFunctions = h->GetListOfFunctions();
                        ListOfFunctions->Add(measured_target_location_TLine);
                        ListOfFunctions->Add(Legend);
                    }
                } else if (TempHistoList[i]->InheritsFrom("TH2D")) {
                    auto *h = (TH2D *)TempHistoList[i];

                    h->Draw("colz");

                    myCanvas->SetLogz(0);
                    if (bt::FindSubstring(h->GetName(), "PCAL") && !bt::FindSubstring(h->GetName(), "sampling fraction")) { myCanvas->SetLogz(1); }

                    if (h->GetEntries() != 0) {
                        gPad->Update();
                        TPaletteAxis *palette = (TPaletteAxis *)h->GetListOfFunctions()->FindObject("palette");
                        palette->SetY2NDC(0.55);
                        gPad->Modified();
                        gPad->Update();
                    }
                } else if (TempHistoList[i]->InheritsFrom("TGraph")) {
                    ((TGraph *)TempHistoList[i])->Draw("ap");
                }

                if (bt::FindSubstring(TempHistoList[i]->GetTitle(), "V_{z}^{") && !bt::FindSubstring(TempHistoList[i]->GetTitle(), "sector")) {
                    std::string Individual_PDF_fileName = IndividualPlotsOutputDir + to_string(plot_counter) + "_" + TempHistoList[i]->GetName() + ".pdf";
                    myCanvas->SaveAs(Individual_PDF_fileName.c_str());
                    hf::FixPDFOrientation(Individual_PDF_fileName);
                }

                myCanvas->Print(fileName, "pdf");
                myCanvas->Clear();
                ++plot_counter;
            }

            sprintf(fileName, "%s]", PDF_fileName.c_str());
            myCanvas->Print(fileName, "pdf");

            hf::FixPDFOrientation(PDF_fileName);                                                    // Fix orientation
            hf::ReassignPDFBookmarks(bt::GetCurrentDirectory() + "/", PDF_fileName, PDF_fileName);  // Reassign clean bookmarks
        };

        GeneratePDFOutput(OutputDir, OutFolderName, BaseDir, InputFiles, sample, HistoList, NumOfEvents, NumOfEvents_wAny_e_det, NumOfEvents_wOne_e_det, NumOfEvents_wAny_e,
                          NumOfEvents_wOne_e, CodeRun_status, IsData, target_status);

        int ComparisonNumber = 0;

        ++ComparisonNumber;
        hf::CompareHistograms({h_SF_VS_Edep_PCAL_BC_sector1_1e_cut, h_SF_VS_Edep_PCAL_BC_sector2_1e_cut, h_SF_VS_Edep_PCAL_BC_sector3_1e_cut, h_SF_VS_Edep_PCAL_BC_sector4_1e_cut,
                               h_SF_VS_Edep_PCAL_BC_sector5_1e_cut, h_SF_VS_Edep_PCAL_BC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "SF_VS_Edep_PCAL_BC_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_SF_VS_Edep_PCAL_AC_sector1_1e_cut, h_SF_VS_Edep_PCAL_AC_sector2_1e_cut, h_SF_VS_Edep_PCAL_AC_sector3_1e_cut, h_SF_VS_Edep_PCAL_AC_sector4_1e_cut,
                               h_SF_VS_Edep_PCAL_AC_sector5_1e_cut, h_SF_VS_Edep_PCAL_AC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "SF_VS_Edep_PCAL_AC_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_SF_VS_P_e_BC_sector1_1e_cut, h_SF_VS_P_e_BC_sector2_1e_cut, h_SF_VS_P_e_BC_sector3_1e_cut, h_SF_VS_P_e_BC_sector4_1e_cut, h_SF_VS_P_e_BC_sector5_1e_cut,
                               h_SF_VS_P_e_BC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "SF_VS_P_e_BC_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_SF_VS_P_e_AC_sector1_1e_cut, h_SF_VS_P_e_AC_sector2_1e_cut, h_SF_VS_P_e_AC_sector3_1e_cut, h_SF_VS_P_e_AC_sector4_1e_cut, h_SF_VS_P_e_AC_sector5_1e_cut,
                               h_SF_VS_P_e_AC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "SF_VS_P_e_AC_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_e_BC_sector1_1e_cut, h_Vz_e_BC_sector2_1e_cut, h_Vz_e_BC_sector3_1e_cut, h_Vz_e_BC_sector4_1e_cut, h_Vz_e_BC_sector5_1e_cut, h_Vz_e_BC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_e_BC_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_e_AC_sector1_1e_cut, h_Vz_e_AC_sector2_1e_cut, h_Vz_e_AC_sector3_1e_cut, h_Vz_e_AC_sector4_1e_cut, h_Vz_e_AC_sector5_1e_cut, h_Vz_e_AC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_e_AC_BySector_1e_cut");

        // ++ComparisonNumber;
        // hf::CompareHistograms({h_corrected_Vz_e_BC_sector1_1e_cut, h_corrected_Vz_e_BC_sector2_1e_cut, h_corrected_Vz_e_BC_sector3_1e_cut, h_corrected_Vz_e_BC_sector4_1e_cut,
        //                        h_corrected_Vz_e_BC_sector5_1e_cut, h_corrected_Vz_e_BC_sector6_1e_cut},
        //                       OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_e_BC_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_corrected_Vz_e_AC_sector1_1e_cut, h_corrected_Vz_e_AC_sector2_1e_cut, h_corrected_Vz_e_AC_sector3_1e_cut, h_corrected_Vz_e_AC_sector4_1e_cut,
                               h_corrected_Vz_e_AC_sector5_1e_cut, h_corrected_Vz_e_AC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_e_AC_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_e_BC_zoomin_sector1_1e_cut, h_Vz_e_BC_zoomin_sector2_1e_cut, h_Vz_e_BC_zoomin_sector3_1e_cut, h_Vz_e_BC_zoomin_sector4_1e_cut,
                               h_Vz_e_BC_zoomin_sector5_1e_cut, h_Vz_e_BC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_e_BC_zoomin_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_e_AC_zoomin_sector1_1e_cut, h_Vz_e_AC_zoomin_sector2_1e_cut, h_Vz_e_AC_zoomin_sector3_1e_cut, h_Vz_e_AC_zoomin_sector4_1e_cut,
                               h_Vz_e_AC_zoomin_sector5_1e_cut, h_Vz_e_AC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_e_AC_zoomin_BySector_1e_cut");

        // ++ComparisonNumber;
        // hf::CompareHistograms({h_corrected_Vz_e_BC_zoomin_sector1_1e_cut, h_corrected_Vz_e_BC_zoomin_sector2_1e_cut, h_corrected_Vz_e_BC_zoomin_sector3_1e_cut,
        //                        h_corrected_Vz_e_BC_zoomin_sector4_1e_cut, h_corrected_Vz_e_BC_zoomin_sector5_1e_cut, h_corrected_Vz_e_BC_zoomin_sector6_1e_cut},
        //                       OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_e_BC_zoomin_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_corrected_Vz_e_AC_zoomin_sector1_1e_cut, h_corrected_Vz_e_AC_zoomin_sector2_1e_cut, h_corrected_Vz_e_AC_zoomin_sector3_1e_cut,
                               h_corrected_Vz_e_AC_zoomin_sector4_1e_cut, h_corrected_Vz_e_AC_zoomin_sector5_1e_cut, h_corrected_Vz_e_AC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_e_AC_zoomin_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipFD_BC_sector1_1e_cut, h_Vz_pipFD_BC_sector2_1e_cut, h_Vz_pipFD_BC_sector3_1e_cut, h_Vz_pipFD_BC_sector4_1e_cut, h_Vz_pipFD_BC_sector5_1e_cut,
                               h_Vz_pipFD_BC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pipFD_BC_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipFD_AC_sector1_1e_cut, h_Vz_pipFD_AC_sector2_1e_cut, h_Vz_pipFD_AC_sector3_1e_cut, h_Vz_pipFD_AC_sector4_1e_cut, h_Vz_pipFD_AC_sector5_1e_cut,
                               h_Vz_pipFD_AC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pipFD_AC_BySector_1e_cut");

        // ++ComparisonNumber;
        // hf::CompareHistograms({h_corrected_Vz_pipFD_BC_sector1_1e_cut, h_corrected_Vz_pipFD_BC_sector2_1e_cut, h_corrected_Vz_pipFD_BC_sector3_1e_cut,
        // h_corrected_Vz_pipFD_BC_sector4_1e_cut,
        //                        h_corrected_Vz_pipFD_BC_sector5_1e_cut, h_corrected_Vz_pipFD_BC_sector6_1e_cut},
        //                       OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_pipFD_BC_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_corrected_Vz_pipFD_AC_sector1_1e_cut, h_corrected_Vz_pipFD_AC_sector2_1e_cut, h_corrected_Vz_pipFD_AC_sector3_1e_cut, h_corrected_Vz_pipFD_AC_sector4_1e_cut,
                               h_corrected_Vz_pipFD_AC_sector5_1e_cut, h_corrected_Vz_pipFD_AC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_pipFD_AC_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pipFD_BC_sector1_1e_cut, h_dVz_pipFD_BC_sector2_1e_cut, h_dVz_pipFD_BC_sector3_1e_cut, h_dVz_pipFD_BC_sector4_1e_cut, h_dVz_pipFD_BC_sector5_1e_cut,
                               h_dVz_pipFD_BC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pipFD_BC_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pipFD_AC_sector1_1e_cut, h_dVz_pipFD_AC_sector2_1e_cut, h_dVz_pipFD_AC_sector3_1e_cut, h_dVz_pipFD_AC_sector4_1e_cut, h_dVz_pipFD_AC_sector5_1e_cut,
                               h_dVz_pipFD_AC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pipFD_AC_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipFD_BC_zoomin_sector1_1e_cut, h_Vz_pipFD_BC_zoomin_sector2_1e_cut, h_Vz_pipFD_BC_zoomin_sector3_1e_cut, h_Vz_pipFD_BC_zoomin_sector4_1e_cut,
                               h_Vz_pipFD_BC_zoomin_sector5_1e_cut, h_Vz_pipFD_BC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pipFD_BC_zoomin_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipFD_AC_zoomin_sector1_1e_cut, h_Vz_pipFD_AC_zoomin_sector2_1e_cut, h_Vz_pipFD_AC_zoomin_sector3_1e_cut, h_Vz_pipFD_AC_zoomin_sector4_1e_cut,
                               h_Vz_pipFD_AC_zoomin_sector5_1e_cut, h_Vz_pipFD_AC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pipFD_AC_zoomin_BySector_1e_cut");

        // ++ComparisonNumber;
        // hf::CompareHistograms({h_corrected_Vz_pipFD_BC_zoomin_sector1_1e_cut, h_corrected_Vz_pipFD_BC_zoomin_sector2_1e_cut, h_corrected_Vz_pipFD_BC_zoomin_sector3_1e_cut,
        //                        h_corrected_Vz_pipFD_BC_zoomin_sector4_1e_cut, h_corrected_Vz_pipFD_BC_zoomin_sector5_1e_cut, h_corrected_Vz_pipFD_BC_zoomin_sector6_1e_cut},
        //                       OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_pipFD_BC_zoomin_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_corrected_Vz_pipFD_AC_zoomin_sector1_1e_cut, h_corrected_Vz_pipFD_AC_zoomin_sector2_1e_cut, h_corrected_Vz_pipFD_AC_zoomin_sector3_1e_cut,
                               h_corrected_Vz_pipFD_AC_zoomin_sector4_1e_cut, h_corrected_Vz_pipFD_AC_zoomin_sector5_1e_cut, h_corrected_Vz_pipFD_AC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_pipFD_AC_zoomin_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pipFD_BC_zoomin_sector1_1e_cut, h_dVz_pipFD_BC_zoomin_sector2_1e_cut, h_dVz_pipFD_BC_zoomin_sector3_1e_cut, h_dVz_pipFD_BC_zoomin_sector4_1e_cut,
                               h_dVz_pipFD_BC_zoomin_sector5_1e_cut, h_dVz_pipFD_BC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pipFD_BC_zoomin_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pipFD_AC_zoomin_sector1_1e_cut, h_dVz_pipFD_AC_zoomin_sector2_1e_cut, h_dVz_pipFD_AC_zoomin_sector3_1e_cut, h_dVz_pipFD_AC_zoomin_sector4_1e_cut,
                               h_dVz_pipFD_AC_zoomin_sector5_1e_cut, h_dVz_pipFD_AC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pipFD_AC_zoomin_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pimFD_BC_sector1_1e_cut, h_Vz_pimFD_BC_sector2_1e_cut, h_Vz_pimFD_BC_sector3_1e_cut, h_Vz_pimFD_BC_sector4_1e_cut, h_Vz_pimFD_BC_sector5_1e_cut,
                               h_Vz_pimFD_BC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pimFD_BC_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pimFD_AC_sector1_1e_cut, h_Vz_pimFD_AC_sector2_1e_cut, h_Vz_pimFD_AC_sector3_1e_cut, h_Vz_pimFD_AC_sector4_1e_cut, h_Vz_pimFD_AC_sector5_1e_cut,
                               h_Vz_pimFD_AC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pimFD_AC_BySector_1e_cut");

        // ++ComparisonNumber;
        // hf::CompareHistograms({h_corrected_Vz_pimFD_BC_sector1_1e_cut, h_corrected_Vz_pimFD_BC_sector2_1e_cut, h_corrected_Vz_pimFD_BC_sector3_1e_cut,
        // h_corrected_Vz_pimFD_BC_sector4_1e_cut,
        //                        h_corrected_Vz_pimFD_BC_sector5_1e_cut, h_corrected_Vz_pimFD_BC_sector6_1e_cut},
        //                       OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_pimFD_BC_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_corrected_Vz_pimFD_AC_sector1_1e_cut, h_corrected_Vz_pimFD_AC_sector2_1e_cut, h_corrected_Vz_pimFD_AC_sector3_1e_cut, h_corrected_Vz_pimFD_AC_sector4_1e_cut,
                               h_corrected_Vz_pimFD_AC_sector5_1e_cut, h_corrected_Vz_pimFD_AC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_pimFD_AC_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pimFD_BC_sector1_1e_cut, h_dVz_pimFD_BC_sector2_1e_cut, h_dVz_pimFD_BC_sector3_1e_cut, h_dVz_pimFD_BC_sector4_1e_cut, h_dVz_pimFD_BC_sector5_1e_cut,
                               h_dVz_pimFD_BC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pimFD_BC_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pimFD_AC_sector1_1e_cut, h_dVz_pimFD_AC_sector2_1e_cut, h_dVz_pimFD_AC_sector3_1e_cut, h_dVz_pimFD_AC_sector4_1e_cut, h_dVz_pimFD_AC_sector5_1e_cut,
                               h_dVz_pimFD_AC_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pimFD_AC_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pimFD_BC_zoomin_sector1_1e_cut, h_Vz_pimFD_BC_zoomin_sector2_1e_cut, h_Vz_pimFD_BC_zoomin_sector3_1e_cut, h_Vz_pimFD_BC_zoomin_sector4_1e_cut,
                               h_Vz_pimFD_BC_zoomin_sector5_1e_cut, h_Vz_pimFD_BC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pimFD_BC_zoomin_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pimFD_AC_zoomin_sector1_1e_cut, h_Vz_pimFD_AC_zoomin_sector2_1e_cut, h_Vz_pimFD_AC_zoomin_sector3_1e_cut, h_Vz_pimFD_AC_zoomin_sector4_1e_cut,
                               h_Vz_pimFD_AC_zoomin_sector5_1e_cut, h_Vz_pimFD_AC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pimFD_AC_zoomin_BySector_1e_cut");

        // ++ComparisonNumber;
        // hf::CompareHistograms({h_corrected_Vz_pimFD_BC_zoomin_sector1_1e_cut, h_corrected_Vz_pimFD_BC_zoomin_sector2_1e_cut, h_corrected_Vz_pimFD_BC_zoomin_sector3_1e_cut,
        //                        h_corrected_Vz_pimFD_BC_zoomin_sector4_1e_cut, h_corrected_Vz_pimFD_BC_zoomin_sector5_1e_cut, h_corrected_Vz_pimFD_BC_zoomin_sector6_1e_cut},
        //                       OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_pimFD_BC_zoomin_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_corrected_Vz_pimFD_AC_zoomin_sector1_1e_cut, h_corrected_Vz_pimFD_AC_zoomin_sector2_1e_cut, h_corrected_Vz_pimFD_AC_zoomin_sector3_1e_cut,
                               h_corrected_Vz_pimFD_AC_zoomin_sector4_1e_cut, h_corrected_Vz_pimFD_AC_zoomin_sector5_1e_cut, h_corrected_Vz_pimFD_AC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "corrected_Vz_pimFD_AC_zoomin_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pimFD_BC_zoomin_sector1_1e_cut, h_dVz_pimFD_BC_zoomin_sector2_1e_cut, h_dVz_pimFD_BC_zoomin_sector3_1e_cut, h_dVz_pimFD_BC_zoomin_sector4_1e_cut,
                               h_dVz_pimFD_BC_zoomin_sector5_1e_cut, h_dVz_pimFD_BC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pimFD_BC_zoomin_BySector_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pimFD_AC_zoomin_sector1_1e_cut, h_dVz_pimFD_AC_zoomin_sector2_1e_cut, h_dVz_pimFD_AC_zoomin_sector3_1e_cut, h_dVz_pimFD_AC_zoomin_sector4_1e_cut,
                               h_dVz_pimFD_AC_zoomin_sector5_1e_cut, h_dVz_pimFD_AC_zoomin_sector6_1e_cut},
                              OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pimFD_AC_zoomin_BySector_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipFD_BC_1e_cut, h_Vz_pimFD_BC_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pions_FD_BC_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipFD_AC_1e_cut, h_Vz_pimFD_AC_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pions_FD_AC_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipFD_BC_zoomin_1e_cut, h_Vz_pimFD_BC_zoomin_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pions_FD_BC_zoomin_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipFD_AC_zoomin_1e_cut, h_Vz_pimFD_AC_zoomin_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pions_FD_AC_zoomin_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pipFD_BC_1e_cut, h_dVz_pimFD_BC_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pions_FD_BC_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pipFD_AC_1e_cut, h_dVz_pimFD_AC_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pions_FD_AC_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipCD_BC_1e_cut, h_Vz_pimCD_BC_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pions_CD_BC_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipCD_AC_1e_cut, h_Vz_pimCD_AC_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pions_CD_AC_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipCD_BC_zoomin_1e_cut, h_Vz_pimCD_BC_zoomin_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pions_CD_BC_zoomin_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_Vz_pipCD_AC_zoomin_1e_cut, h_Vz_pimCD_AC_zoomin_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "Vz_pions_CD_AC_zoomin_1e_cut");

        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pipCD_BC_1e_cut, h_dVz_pimCD_BC_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pions_CD_BC_1e_cut");
        ++ComparisonNumber;
        hf::CompareHistograms({h_dVz_pipCD_AC_1e_cut, h_dVz_pimCD_AC_1e_cut}, OutputDir, "Histogram_Comparisons",
                              bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + "DeltaVz_pions_CD_AC_1e_cut");

        outFile->cd();
        for (int i = 0; i < HistoList.size(); i++) { HistoList[i]->Write(); }
        outFile->Close();

#pragma endregion

        // Delete all ROOT objects whose class names start with TH (to prevent a memory leak):
        if (InputFiles.size() > 1) { gDirectory->Clear(); }

        std::cout << "\033[33m" << "\n=============================================================\n" << "\033[0m";
        std::cout << "\033[33m" << "= HipoLooper summary                                         \n" << "\033[0m";
        std::cout << "\033[33m" << "=============================================================\n" << "\033[0m";

        std::cout << "\033[33m" << "\n- Input variables -------------------------------------------\n" << "\033[0m";
        std::cout << "\033[33m" << "InputFiles.at(sample):\033[0m " << InputFiles.at(sample) << endl;
        std::cout << "\033[33m" << "nCodeRun_status:\033[0m       " << CodeRun_status << endl;
        std::cout << "\033[33m" << "OutputDir:\033[0m             " << OutputDir << "\n\n";
    }
    // }

    auto end = std::chrono::system_clock::now();
    auto elapsed_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    double elapsed_time_minutes = elapsed_time_seconds.count() / 60;

    if (elapsed_time_seconds.count() < 60) {
        std::cout << "\033[33m" << "Running time:" << "\033[0m" << "\t\t" << elapsed_time_seconds.count() << " seconds\n\n";
    } else {
        std::cout << "\033[33m" << "Running time:" << "\033[0m" << "\t\t" << bt::ToStringWithPrecision(elapsed_time_minutes, 3) << " minutes\n\n";
    }
}
