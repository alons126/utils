//
// Created by Alon Sportes on 30/04/2025.
//

#include <TApplication.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TDatabasePDG.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1D.h>
#include <TH2.h>
#include <TH2D.h>
#include <THStack.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TObject.h>
#include <TPad.h>
#include <TPaletteAxis.h>
#include <TPaveText.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>

#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

// Include libraries:
#include "../framework/namespaces/general_utilities/analysis_math/variable_correctors.h"
#include "../framework/namespaces/general_utilities/histogram_functions.h"

// To run: root -q -b /Users/alon/Projects/utils/HipoLooper/macros/CompareHistogramsTester2.cpp

using namespace std;

// CompareHistogramsTester ------------------------------------------------------------------

void CompareHistogramsTester2() {
    int version = 16;  // Version of the code
    std::string OutFolderName_prefix = "ReRun_HipoLooper";
    // std::string OutFolderName_prefix = "ReRun" + basic_tools::ToStringWithPrecision(version, 0) + "_HipoLooper";
    std::string OutFolderName_ver_status_1 = "_v" + basic_tools::ToStringWithPrecision(version, 0);
    std::string OutFolderName_ver_status_2 = OutFolderName_ver_status_1 + "_";
    // std::string OutFolderName_ver_status = "_v" + basic_tools::ToStringWithPrecision(version, 0) + "_";

    std::string General_status = "";
    // std::string General_status = "RegTest";
    // std::string General_status = "Theta_e_test";

    std::string BaseDir = "/Users/alon/Code runs/utils/HipoLooper (Ar40 imp)/" + basic_tools::ToStringWithPrecision(version, 0) + "_HipoLooper" + OutFolderName_ver_status_1 + "/";

    std::vector<std::string> InputFiles;

    // InputFiles.push_back(BaseDir + "015_HipoLooper_v15_C12_data_2GeV_run_015664/015_HipoLooper_v15_C12_data_2GeV_run_015664.root");
    // InputFiles.push_back(BaseDir + "015_HipoLooper_v15_C12_data_4GeV_run_015778/015_HipoLooper_v15_C12_data_4GeV_run_015778.root");

    // InputFiles.push_back(BaseDir + "015_HipoLooper_v15_Ar40_data_2GeV_run_015672/015_HipoLooper_v15_Ar40_data_2GeV_run_015672.root");
    // InputFiles.push_back(BaseDir + "015_HipoLooper_v15_Ar40_data_4GeV_run_015743/015_HipoLooper_v15_Ar40_data_4GeV_run_015743.root");
    // InputFiles.push_back(BaseDir + "015_HipoLooper_v15_Ar40_data_6GeV_run_015792/015_HipoLooper_v15_Ar40_data_6GeV_run_015792.root");

    // InputFiles.push_back("/Users/alon/Downloads/016_HipoLooper_v16_Ar40_data_2GeV_run_015672RegTest/016_HipoLooper_v16_Ar40_data_2GeV_run_015672RegTest.root");
    // InputFiles.push_back("/Users/alon/Downloads/016_HipoLooper_v16_Ar40_data_2GeV_run_015672Theta_e_test/016_HipoLooper_v16_Ar40_data_2GeV_run_015672Theta_e_test.root");

    InputFiles.push_back(
        "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
        "imp)/16_HipoLooper_v16/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_5_to_10_theta_slices/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_5_to_10_theta_slices.root");
    InputFiles.push_back(
        "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
        "imp)/16_HipoLooper_v16/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_10_to_15_theta_slices/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_10_to_15_theta_slices.root");
    InputFiles.push_back(
        "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
        "imp)/16_HipoLooper_v16/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_15_to_20_theta_slices/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_15_to_20_theta_slices.root");
    InputFiles.push_back(
        "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
        "imp)/16_HipoLooper_v16/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_20_to_25_theta_slices/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_20_to_25_theta_slices.root");
    InputFiles.push_back(
        "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
        "imp)/16_HipoLooper_v16/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_25_to_30_theta_slices/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_25_to_30_theta_slices.root");
    InputFiles.push_back(
        "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
        "imp)/16_HipoLooper_v16/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_30_to_35_theta_slices/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_30_to_35_theta_slices.root");
    InputFiles.push_back(
        "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
        "imp)/16_HipoLooper_v16/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_35_to_40_theta_slices/016_HipoLooper_v16_Ar40_data_2GeV_run_015672_35_to_40_theta_slices.root");

    std::string SaveDirFolder = "/Users/alon/Downloads";

    for (auto sample = 0; sample < InputFiles.size(); ++sample) {
        std::string TempGeneral_status = "";
        std::pair<double, double> theta_slice;

        if (basic_tools::FindSubstring(InputFiles.at(sample), "_to_")) {
            std::string path = InputFiles.at(sample);
            std::string filename = path.substr(path.find_last_of("/\\") + 1);  // isolate filename

            size_t to_pos = filename.find("_to_");

            if (to_pos != std::string::npos && to_pos >= 2) {
                size_t slice_start = to_pos - 2;  // start two characters before "_to_" to get the range start
                size_t slice_end = filename.find(".root");
                if (slice_end != std::string::npos && slice_end > slice_start) { TempGeneral_status = General_status + "_" + filename.substr(slice_start, slice_end - slice_start); }
            }

            if (to_pos != std::string::npos && to_pos >= 1) {
                // Find start of llim
                size_t llim_start = filename.rfind('_', to_pos - 1);
                if (llim_start != std::string::npos) {
                    std::string llim_str = filename.substr(llim_start + 1, to_pos - llim_start - 1);
                    size_t ulim_end = filename.find('_', to_pos + 4);  // after "_to_"
                    std::string ulim_str = filename.substr(to_pos + 4, ulim_end - (to_pos + 4));
                    double theta_slice_llim = std::stod(llim_str);
                    double theta_slice_ulim = std::stod(ulim_str);
                    theta_slice = {theta_slice_llim, theta_slice_ulim};
                }
            }
        } else {
            TempGeneral_status = General_status;
        }

        bool IsData = basic_tools::FindSubstring(InputFiles.at(sample), "data");

        bool Is2GeV = (basic_tools::FindSubstring(InputFiles.at(sample), "2070MeV") || basic_tools::FindSubstring(InputFiles.at(sample), "2gev") ||
                       basic_tools::FindSubstring(InputFiles.at(sample), "2GeV"));
        bool Is4GeV = (basic_tools::FindSubstring(InputFiles.at(sample), "4029MeV") || basic_tools::FindSubstring(InputFiles.at(sample), "4gev") ||
                       basic_tools::FindSubstring(InputFiles.at(sample), "4GeV"));
        bool Is6GeV = (basic_tools::FindSubstring(InputFiles.at(sample), "5986MeV") || basic_tools::FindSubstring(InputFiles.at(sample), "6gev") ||
                       basic_tools::FindSubstring(InputFiles.at(sample), "6GeV"));

        double Ebeam = Is2GeV ? 2.07052 : Is4GeV ? 4.02962 : Is6GeV ? 5.98636 : 0.0;

        if (Ebeam == 0.0) {
            std::cerr << "\n\nError! Ebeam = 0 not found in InputFiles string! Aborting...\n\n";
            exit(1);
        }

        std::string target_status = (basic_tools::FindSubstring(InputFiles.at(sample), "C12") || basic_tools::FindSubstring(InputFiles.at(sample), "/C/"))     ? "C12"
                                    : (basic_tools::FindSubstring(InputFiles.at(sample), "Ar40") || basic_tools::FindSubstring(InputFiles.at(sample), "/Ar/")) ? "Ar40"
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

        std::string OutFolderName = OutFolderName_prefix + OutFolderName_ver_status_2 + CodeRun_status + TempGeneral_status;
        std::string OutFileName = OutFolderName;

        const std::string OutputDir = SaveDirFolder + "/" + OutFolderName;
        system(("rm -rf " + OutputDir).c_str());
        system(("mkdir -p " + OutputDir).c_str());

        const std::string IndividualPlotsOutputDir = OutputDir + "/Individual_plots/";
        system(("rm -rf " + IndividualPlotsOutputDir).c_str());
        system(("mkdir -p " + IndividualPlotsOutputDir).c_str());

        TFile *outFile = new TFile((OutputDir + "/" + OutFileName + ".root").c_str(), "RECREATE");

        std::string SampleName = target_status + sample_type_status + Ebeam_status_2 + Run_status;

        std::string SampleType = (IsData) ? "Data" : "sim";
        std::string SampleBeamE = Ebeam_status_2;

        const char *filename = InputFiles[sample].c_str();

        std::cout << "\033[33m\n\n===========================================================================================\n\n";
        std::cout << "\033[33mCodeRun_status:        \t\033[0m" << CodeRun_status << "\n";
        std::cout << "\033[33mSaveDirFolder:         \t\033[0m" << SaveDirFolder << "\n";
        std::cout << "\033[33mOutputDir:             \t\033[0m" << OutputDir << "\n";
        std::cout << "\033[33mfilename:              \t\033[0m" << filename << "\n";
        std::cout << "\033[33mGeneral_status:        \t\033[0m" << General_status << "\n";
        std::cout << "\033[33mTempGeneral_status:    \t\033[0m" << TempGeneral_status << "\n";

        if (basic_tools::FindSubstring(InputFiles.at(sample), "_to_")) {
            std::cout << "\033[33mtheta_slice:           \t\033[0m {" << theta_slice.first << ", " << theta_slice.second << "}\n";
        }

        TFile *file = new TFile(filename);
        std::vector<TObject *> HistoList;

        // Load histograms and register them for deletion
        auto load = [&](const char *name, const std::string &cls) -> TObject * {
            TObject *obj = histogram_functions::FindHistogram(file, name, cls);
            HistoList.push_back(obj);
            return obj;
        };

        // Project TH2D onto phi (X) axis and register result
        auto project = [&](const char *name) -> TH1D * {
            auto h2 = (TH2D *)load(name, "TH2D");
            if (!h2) return nullptr;

            std::string projName = basic_tools::ReplaceSubstring(name, "Vz_VS_", "");
            auto h_phi = h2->ProjectionX(projName.c_str());
            h_phi->SetTitle(basic_tools::ReplaceSubstring(h_phi->GetTitle(), "V_{z}^{e} vs. #phi_{e}", "#phi_{e}").c_str());

            if (!h_phi) {
                std::cerr << "Error: Projection of " << name << " failed." << std::endl;
                return nullptr;
            }

            HistoList.push_back(h_phi);
            return h_phi;
        };

        auto loadVzAndPhiHistograms = [&](const std::string &sector_tag, const std::string &particle = "e") -> std::pair<TH1D *, TH1D *> {
            std::string Vz_name = "Vz_" + particle + "_AC_zoomin_" + sector_tag + "_1e_cut";
            std::string Vz_vs_phi_name = "Vz_VS_phi_" + particle + "_AC_" + sector_tag + "_1e_cut";

            TH1D *vz_hist = (TH1D *)load(Vz_name.c_str(), "TH1D");
            TH1D *phi_hist = project(Vz_vs_phi_name.c_str());

            return std::make_pair(vz_hist, phi_hist);
        };

        auto h_Vz_e_AC_zoomin_1e_cut = (TH1D *)load("Vz_e_AC_zoomin_1e_cut", "TH1D");
        auto [h_Vz_e_AC_sector1_1e_cut, h_phi_e_AC_sector1_1e_cut] = loadVzAndPhiHistograms("sector1", "e");
        auto [h_Vz_e_AC_sector2_1e_cut, h_phi_e_AC_sector2_1e_cut] = loadVzAndPhiHistograms("sector2", "e");
        auto [h_Vz_e_AC_sector3_1e_cut, h_phi_e_AC_sector3_1e_cut] = loadVzAndPhiHistograms("sector3", "e");
        auto [h_Vz_e_AC_sector4_1e_cut, h_phi_e_AC_sector4_1e_cut] = loadVzAndPhiHistograms("sector4", "e");
        auto [h_Vz_e_AC_sector5_1e_cut, h_phi_e_AC_sector5_1e_cut] = loadVzAndPhiHistograms("sector5", "e");
        auto [h_Vz_e_AC_sector6_1e_cut, h_phi_e_AC_sector6_1e_cut] = loadVzAndPhiHistograms("sector6", "e");

        auto h_Vz_pipFD_AC_zoomin_1e_cut = (TH1D *)load("Vz_pipFD_AC_zoomin_1e_cut", "TH1D");
        // auto h_Vz_pipFD_AC_zoomin_sector1_1e_cut = (TH1D *)load("Vz_pipFD_AC_zoomin_sector1_1e_cut", "TH1D");
        // auto h_Vz_pipFD_AC_zoomin_sector2_1e_cut = (TH1D *)load("Vz_pipFD_AC_zoomin_sector2_1e_cut", "TH1D");
        // auto h_Vz_pipFD_AC_zoomin_sector3_1e_cut = (TH1D *)load("Vz_pipFD_AC_zoomin_sector3_1e_cut", "TH1D");
        // auto h_Vz_pipFD_AC_zoomin_sector4_1e_cut = (TH1D *)load("Vz_pipFD_AC_zoomin_sector4_1e_cut", "TH1D");
        // auto h_Vz_pipFD_AC_zoomin_sector5_1e_cut = (TH1D *)load("Vz_pipFD_AC_zoomin_sector5_1e_cut", "TH1D");
        // auto h_Vz_pipFD_AC_zoomin_sector6_1e_cut = (TH1D *)load("Vz_pipFD_AC_zoomin_sector6_1e_cut", "TH1D");
        auto [h_Vz_pipFD_AC_sector1_1e_cut, h_phi_pipFD_AC_sector1_1e_cut] = loadVzAndPhiHistograms("sector1", "pipFD");
        auto [h_Vz_pipFD_AC_sector2_1e_cut, h_phi_pipFD_AC_sector2_1e_cut] = loadVzAndPhiHistograms("sector2", "pipFD");
        auto [h_Vz_pipFD_AC_sector3_1e_cut, h_phi_pipFD_AC_sector3_1e_cut] = loadVzAndPhiHistograms("sector3", "pipFD");
        auto [h_Vz_pipFD_AC_sector4_1e_cut, h_phi_pipFD_AC_sector4_1e_cut] = loadVzAndPhiHistograms("sector4", "pipFD");
        auto [h_Vz_pipFD_AC_sector5_1e_cut, h_phi_pipFD_AC_sector5_1e_cut] = loadVzAndPhiHistograms("sector5", "pipFD");
        auto [h_Vz_pipFD_AC_sector6_1e_cut, h_phi_pipFD_AC_sector6_1e_cut] = loadVzAndPhiHistograms("sector6", "pipFD");

        auto h_Vz_pimFD_AC_zoomin_1e_cut = (TH1D *)load("Vz_pimFD_AC_zoomin_1e_cut", "TH1D");
        auto h_Vz_pimFD_AC_zoomin_sector1_1e_cut = (TH1D *)load("Vz_pimFD_AC_zoomin_sector1_1e_cut", "TH1D");
        auto h_Vz_pimFD_AC_zoomin_sector2_1e_cut = (TH1D *)load("Vz_pimFD_AC_zoomin_sector2_1e_cut", "TH1D");
        auto h_Vz_pimFD_AC_zoomin_sector3_1e_cut = (TH1D *)load("Vz_pimFD_AC_zoomin_sector3_1e_cut", "TH1D");
        auto h_Vz_pimFD_AC_zoomin_sector4_1e_cut = (TH1D *)load("Vz_pimFD_AC_zoomin_sector4_1e_cut", "TH1D");
        auto h_Vz_pimFD_AC_zoomin_sector5_1e_cut = (TH1D *)load("Vz_pimFD_AC_zoomin_sector5_1e_cut", "TH1D");
        auto h_Vz_pimFD_AC_zoomin_sector6_1e_cut = (TH1D *)load("Vz_pimFD_AC_zoomin_sector6_1e_cut", "TH1D");
        // auto [h_Vz_pimFD_AC_sector1_1e_cut, h_phi_pimFD_AC_sector1_1e_cut] = loadVzAndPhiHistograms("sector1", "pimFD");
        // auto [h_Vz_pimFD_AC_sector2_1e_cut, h_phi_pimFD_AC_sector2_1e_cut] = loadVzAndPhiHistograms("sector2", "pimFD");
        // auto [h_Vz_pimFD_AC_sector3_1e_cut, h_phi_pimFD_AC_sector3_1e_cut] = loadVzAndPhiHistograms("sector3", "pimFD");
        // auto [h_Vz_pimFD_AC_sector4_1e_cut, h_phi_pimFD_AC_sector4_1e_cut] = loadVzAndPhiHistograms("sector4", "pimFD");
        // auto [h_Vz_pimFD_AC_sector5_1e_cut, h_phi_pimFD_AC_sector5_1e_cut] = loadVzAndPhiHistograms("sector5", "pimFD");
        // auto [h_Vz_pimFD_AC_sector6_1e_cut, h_phi_pimFD_AC_sector6_1e_cut] = loadVzAndPhiHistograms("sector6", "pimFD");

        auto h_Vz_pipCD_AC_zoomin_1e_cut = (TH1D *)load("Vz_pipCD_AC_zoomin_1e_cut", "TH1D");
        auto h_Vz_pimCD_AC_zoomin_1e_cut = (TH1D *)load("Vz_pimCD_AC_zoomin_1e_cut", "TH1D");

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

                if (peakCenter < 0) {
                    // If peak is negative, set limits accordingly
                    fitMin = -std::fabs(peakCenter * 1.1);
                    fitMax = -std::fabs(peakCenter * 0.9);
                } else {
                    // If peak is positive, set limits accordingly
                    fitMin = std::fabs(peakCenter * 0.9);
                    fitMax = std::fabs(peakCenter * 1.1);
                }

                // fitMin = -std::fabs(peakCenter * 1.1);
                // fitMax = -std::fabs(peakCenter * 0.9);
                // fitMin = -std::fabs(peakCenter * 1.2);
                // fitMax = -std::fabs(peakCenter * 0.8);
                // fitMin = -std::fabs(peakCenter * 1.1);
                // fitMax = -std::fabs(peakCenter * 0.9);
                // // fitMin = -std::fabs(peakCenter * 1.2);
                // // fitMax = -std::fabs(peakCenter * 0.8);
            } else if (fitLimits.size() == 2) {
                fitMin = fitLimits[0];
                fitMax = fitLimits[1];
            } else {
                std::cerr << "Error: fitLimits must contain exactly two elements." << std::endl;
                return std::numeric_limits<double>::quiet_NaN();
            }

            TF1 *fit = new TF1("fit", "gaus", fitMin, fitMax);
            hist->Fit(fit, "RQ");  // R = use range, Q = quiet

            fit->SetLineColor(kMagenta);

            hist->GetListOfFunctions()->Clear();
            hist->GetListOfFunctions()->Add(fit);  // Add fit to the histogram's function list

            return fit->GetParameter(1);  // Return fitted mean
        };

        // auto approx_phi_peak_location = [](TH1D *hist) -> double {
        //     measured_target_location_TLine = new TLine(hist->GetBinCenter(hist->GetMaximumBin()), 0., hist->GetBinCenter(hist->GetMaximumBin()), gPad->GetFrame()->GetY2());
        //     measured_target_location_TLine->SetLineColor(kGreen + 1);
        //     measured_target_location_TLine->SetLineWidth(3);
        //     // measured_target_location_TLine->SetLineWidth(4);
        //     measured_target_location_TLine->SetLineStyle(2);
        //     // measured_target_location_TLine->Draw("same");

        //     auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25 - yOffset, gStyle->GetStatX() - 0.25, gStyle->GetStatY() - 0.375 - yOffset);
        //     // TLegendEntry *speac_target_location_TLine_entry =
        //         // Legend->AddEntry(speac_target_location_TLine, ("Spec. z pos. = " + basic_tools::ToStringWithPrecision(speac_target_location_value, 2) + " cm").c_str(), "l");
        //     TLegendEntry *measured_target_location_TLine_entry =
        //         Legend->AddEntry(measured_target_location_TLine, ("Meas. z pos. = " + basic_tools::ToStringWithPrecision(measured_target_location_value, 2) + " cm").c_str(), "l");

        //     Legend->Draw("same");

        //     return fit->GetParameter(1);  // Return fitted mean
        // };

        // Usage:
        std::vector<double> Vz_e_peaks_BySector = {fit_peak_gaussian(h_Vz_e_AC_sector1_1e_cut), fit_peak_gaussian(h_Vz_e_AC_sector2_1e_cut), fit_peak_gaussian(h_Vz_e_AC_sector3_1e_cut),
                                                   fit_peak_gaussian(h_Vz_e_AC_sector4_1e_cut), fit_peak_gaussian(h_Vz_e_AC_sector5_1e_cut), fit_peak_gaussian(h_Vz_e_AC_sector6_1e_cut)};
        std::vector<double> phi_e_peaks_BySector = {
            // fit_peak_gaussian(h_phi_e_AC_sector1_1e_cut), fit_peak_gaussian(h_phi_e_AC_sector2_1e_cut), fit_peak_gaussian(h_phi_e_AC_sector3_1e_cut),
            // fit_peak_gaussian(h_phi_e_AC_sector4_1e_cut), fit_peak_gaussian(h_phi_e_AC_sector5_1e_cut), fit_peak_gaussian(h_phi_e_AC_sector6_1e_cut)};
            h_phi_e_AC_sector1_1e_cut->GetBinCenter(h_phi_e_AC_sector1_1e_cut->GetMaximumBin()), h_phi_e_AC_sector2_1e_cut->GetBinCenter(h_phi_e_AC_sector2_1e_cut->GetMaximumBin()),
            h_phi_e_AC_sector3_1e_cut->GetBinCenter(h_phi_e_AC_sector3_1e_cut->GetMaximumBin()), h_phi_e_AC_sector4_1e_cut->GetBinCenter(h_phi_e_AC_sector4_1e_cut->GetMaximumBin()),
            h_phi_e_AC_sector5_1e_cut->GetBinCenter(h_phi_e_AC_sector5_1e_cut->GetMaximumBin()), h_phi_e_AC_sector6_1e_cut->GetBinCenter(h_phi_e_AC_sector6_1e_cut->GetMaximumBin())};
        auto [A_e, phi_beam_e, Z0_e, FittedParametersGraph_e] = variable_correctors::FitVertexVsPhi("e", Ebeam_status_1, Vz_e_peaks_BySector, phi_e_peaks_BySector, theta_slice);

        std::vector<double> Vz_pipFD_peaks_BySector = {fit_peak_gaussian(h_Vz_pipFD_AC_sector1_1e_cut), fit_peak_gaussian(h_Vz_pipFD_AC_sector2_1e_cut),
                                                       fit_peak_gaussian(h_Vz_pipFD_AC_sector3_1e_cut), fit_peak_gaussian(h_Vz_pipFD_AC_sector4_1e_cut),
                                                       fit_peak_gaussian(h_Vz_pipFD_AC_sector5_1e_cut), fit_peak_gaussian(h_Vz_pipFD_AC_sector6_1e_cut)};
        std::vector<double> phi_pipFD_peaks_BySector = {fit_peak_gaussian(h_phi_pipFD_AC_sector1_1e_cut), fit_peak_gaussian(h_phi_pipFD_AC_sector2_1e_cut),
                                                        fit_peak_gaussian(h_phi_pipFD_AC_sector3_1e_cut), fit_peak_gaussian(h_phi_pipFD_AC_sector4_1e_cut),
                                                        fit_peak_gaussian(h_phi_pipFD_AC_sector5_1e_cut), fit_peak_gaussian(h_phi_pipFD_AC_sector6_1e_cut)};
        // h_phi_pipFD_AC_sector1_1e_cut->GetBinCenter(h_phi_pipFD_AC_sector1_1e_cut->GetMaximumBin()),
        // h_phi_pipFD_AC_sector2_1e_cut->GetBinCenter(h_phi_pipFD_AC_sector2_1e_cut->GetMaximumBin()),
        // h_phi_pipFD_AC_sector3_1e_cut->GetBinCenter(h_phi_pipFD_AC_sector3_1e_cut->GetMaximumBin()),
        // h_phi_pipFD_AC_sector4_1e_cut->GetBinCenter(h_phi_pipFD_AC_sector4_1e_cut->GetMaximumBin()),
        // h_phi_pipFD_AC_sector5_1e_cut->GetBinCenter(h_phi_pipFD_AC_sector5_1e_cut->GetMaximumBin()),
        // h_phi_pipFD_AC_sector6_1e_cut->GetBinCenter(h_phi_pipFD_AC_sector6_1e_cut->GetMaximumBin())};
        auto [A_pipFD, phi_beam_pipFD, Z0_pipFD, FittedParametersGraph_pipFD] =
            variable_correctors::FitVertexVsPhi("#pi^{+}FD", Ebeam_status_1, Vz_pipFD_peaks_BySector, phi_pipFD_peaks_BySector, theta_slice);

        int insert_index_e = 0;
        int insert_index_pipFD = 0;

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

            if (std::string(HistoList[i]->GetName()) == "Vz_e_AC_zoomin_1e_cut") {
                insert_index_e = i;
                // insert_index_e = i + 1;
            } else if (std::string(HistoList[i]->GetName()) == "Vz_pipFD_AC_zoomin_1e_cut") {
                insert_index_pipFD = i;
                // insert_index_pipFD = i + 1;
            }
        }

        FittedParametersGraph_e->GetXaxis()->CenterTitle();
        FittedParametersGraph_e->GetYaxis()->CenterTitle();
        FittedParametersGraph_pipFD->GetXaxis()->CenterTitle();
        FittedParametersGraph_pipFD->GetYaxis()->CenterTitle();

        HistoList.insert(HistoList.begin() + insert_index_e, FittedParametersGraph_e);
        HistoList.insert(HistoList.begin() + insert_index_pipFD, FittedParametersGraph_pipFD);

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

        // /////////////////////////////////////
        // // CND Neutron Information
        // /////////////////////////////////////
        // myText->cd();

        // titles.DrawLatex(0.05, 0.9, "HipoLooper Output");
        // text.DrawLatex(0.05, 0.75, ("Plots from (e,e') events in: #font[42]{" + CodeRun_status + "}").c_str());

        // if (IsData) {
        //     text.DrawLatex(0.05, 0.7, ("InputFiles: #font[42]{" + InputFiles.at(sample) + "}").c_str());
        // } else {
        //     text.DrawLatex(0.05, 0.7, ("BaseDir: #font[42]{" + BaseDir + "}").c_str());
        //     text.DrawLatex(0.05, 0.65, ("InputFiles: #font[42]{BaseDir + " + InputFiles.at(sample).substr(BaseDir.length()) + "}").c_str());
        // }

        // text.DrawLatex(0.05, 0.60, "Event counts:");
        // text.DrawLatex(0.10, 0.55, ("Total #(events):            #font[42]{" + std::to_string(NumOfEvents) + "}").c_str());
        // text.DrawLatex(0.10, 0.50, ("#(Events) with any e_det:  #font[42]{" + std::to_string(NumOfEvents_wAny_e_det) + "}").c_str());
        // text.DrawLatex(0.10, 0.45,
        //                ("#(Events) with one e_det:  #font[42]{" + std::to_string(NumOfEvents_wOne_e_det) + " (" +
        //                 basic_tools::ToStringWithPrecision((100 * NumOfEvents_wOne_e_det / NumOfEvents_wAny_e_det), 2) + "%)}")
        //                    .c_str());
        // text.DrawLatex(0.10, 0.40, ("#(Events) with any e:       #font[42]{" + std::to_string(NumOfEvents_wAny_e) + "}").c_str());
        // text.DrawLatex(0.10, 0.35,
        //                ("#(Events) with one e:       #font[42]{" + std::to_string(NumOfEvents_wOne_e) + " (" +
        //                 basic_tools::ToStringWithPrecision((100 * NumOfEvents_wOne_e / NumOfEvents_wAny_e), 2) + "%)}")
        //                    .c_str());

        // myText->Print(fileName, "pdf Title: Cover");
        // // myText->Print(fileName, "pdf");
        // myText->Clear();

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

            if (HistoList[i]->InheritsFrom("TH1") || HistoList[i]->InheritsFrom("TH2")) {
                auto *h = (TH1 *)HistoList[i];
                h->GetYaxis()->SetTitleOffset(1.5);
                h->GetXaxis()->SetTitleOffset(1.1);
            } else if (HistoList[i]->InheritsFrom("TGraph")) {
                auto *g = (TGraph *)HistoList[i];
                g->SetTitle((std::string(g->GetTitle()) + std::string(" - " + CodeRun_status)).c_str());  // Ensure title has x and y labels

                g->GetYaxis()->SetTitleOffset(1.5);
                g->GetXaxis()->SetTitleOffset(1.1);
            }

            // gPad->SetRightMargin(0.23);

            if (HistoList[i]->InheritsFrom("TH1D")) {
                auto *h = (TH1D *)HistoList[i];

                h->Draw();

                if (basic_tools::FindSubstring(h->GetTitle(), "V_{z}^{") && !basic_tools::FindSubstring(h->GetTitle(), "dV_{z}^{") && !basic_tools::FindSubstring(h->GetTitle(), "phi")) {
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
                    // double measured_target_location_value = h->GetBinCenter(h->GetMaximumBin());
                    double measured_target_location_value = fit_peak_gaussian(h);

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

                    auto ListOfFunctions = h->GetListOfFunctions();
                    ListOfFunctions->Add(speac_target_location_TLine);
                    ListOfFunctions->Add(measured_target_location_TLine);
                    ListOfFunctions->Add(Legend);
                } else if (basic_tools::FindSubstring(h->GetName(), "phi_")) {
                    gPad->Update();

                    TLine *measured_target_location_TLine;
                    // double measured_target_location_value = h->GetBinCenter(h->GetMaximumBin());
                    double measured_target_location_value = fit_peak_gaussian(h);

                    measured_target_location_TLine = new TLine(measured_target_location_value, 0., measured_target_location_value, gPad->GetFrame()->GetY2());
                    measured_target_location_TLine->SetLineColor(kGreen + 1);
                    measured_target_location_TLine->SetLineWidth(3);
                    // measured_target_location_TLine->SetLineWidth(4);
                    measured_target_location_TLine->SetLineStyle(2);
                    measured_target_location_TLine->Draw("same");

                    auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25 - yOffset, gStyle->GetStatX() - 0.25, gStyle->GetStatY() - 0.325 - yOffset);
                    TLegendEntry *measured_target_location_TLine_entry =
                        Legend->AddEntry(measured_target_location_TLine, ("phi_e peak = " + basic_tools::ToStringWithPrecision(measured_target_location_value, 2) + "#circ").c_str(), "l");

                    Legend->Draw("same");

                    auto ListOfFunctions = h->GetListOfFunctions();
                    ListOfFunctions->Add(measured_target_location_TLine);
                    ListOfFunctions->Add(Legend);
                }
            } else if (HistoList[i]->InheritsFrom("TH2D")) {
                auto *h = (TH2D *)HistoList[i];

                h->Draw("colz");

                myCanvas->SetLogz(0);
                if (basic_tools::FindSubstring(h->GetName(), "PCAL") && !basic_tools::FindSubstring(h->GetName(), "sampling fraction")) { myCanvas->SetLogz(1); }

                if (h->GetEntries() != 0) {
                    gPad->Update();
                    TPaletteAxis *palette = (TPaletteAxis *)h->GetListOfFunctions()->FindObject("palette");
                    palette->SetY2NDC(0.55);
                    gPad->Modified();
                    gPad->Update();
                }
            } else if (HistoList[i]->InheritsFrom("TGraph")) {
                ((TGraph *)HistoList[i])->Draw("ap");
                // ((TGraph *)HistoList[i])->Draw("APL");

                // TLegend *Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.10 - yOffset, gStyle->GetStatX() - 0.25, gStyle->GetStatY() - 0.20 - yOffset);
                // Legend->AddEntry(HistoList[i], HistoList[i]->GetTitle(), "l");
                // Legend->Draw("same");

                // HistoList[i]->GetListOfFunctions()->Add(Legend);
            }

            if (basic_tools::FindSubstring(HistoList[i]->GetTitle(), "V_{z}^{") && !basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector")) {
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

        // Clean up
        for (TObject *obj : HistoList) { delete obj; }

        file->Close();
        delete file;
    }
}