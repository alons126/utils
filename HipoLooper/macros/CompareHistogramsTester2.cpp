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

// using namespace std;

namespace bt = basic_tools;
// namespace am = analysis_math;
// namespace raf = reco_analysis_functions;
namespace hf = histogram_functions;
namespace vc = variable_correctors;

// CompareHistogramsTester ------------------------------------------------------------------

void CompareHistogramsTester2() {
    int version = 25;  // Version of the code
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

    // InputFiles.push_back(
    //     "/Users/alon/Downloads/24_HipoLooper_v24/24_HipoLooper_v24_Ar40_data_2GeV_run_015672__after_sampling_test_8_full/"
    //     "24_HipoLooper_v24_Ar40_data_2GeV_run_015672__after_sampling_test_8_full.root");
    // InputFiles.push_back(
    //     "/Users/alon/Downloads/24_HipoLooper_v24/24_HipoLooper_v24_Ar40_data_4GeV_run_015743__after_sampling_test_8_full/"
    //     "24_HipoLooper_v24_Ar40_data_4GeV_run_015743__after_sampling_test_8_full.root");
    // InputFiles.push_back(
    //     "/Users/alon/Downloads/24_HipoLooper_v24/24_HipoLooper_v24_Ar40_data_6GeV_run_015792__after_sampling_test_8_full/"
    //     "24_HipoLooper_v24_Ar40_data_6GeV_run_015792__after_sampling_test_8_full.root");
    // InputFiles.push_back(
    //     "/Users/alon/Downloads/24_HipoLooper_v24/24_HipoLooper_v24_C12_data_2GeV_run_015664__after_sampling_test_8_full/"
    //     "24_HipoLooper_v24_C12_data_2GeV_run_015664__after_sampling_test_8_full.root");
    // InputFiles.push_back(
    //     "/Users/alon/Downloads/24_HipoLooper_v24/24_HipoLooper_v24_C12_data_4GeV_run_015778__after_sampling_test_8_full/"
    //     "24_HipoLooper_v24_C12_data_4GeV_run_015778__after_sampling_test_8_full.root");

    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_2GeV_run_015664_10_to_15_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_2GeV_run_015664_10_to_15_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_2GeV_run_015664_15_to_20_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_2GeV_run_015664_15_to_20_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_2GeV_run_015664_20_to_25_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_2GeV_run_015664_20_to_25_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_2GeV_run_015664_25_to_30_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_2GeV_run_015664_25_to_30_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_2GeV_run_015664_30_to_35_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_2GeV_run_015664_30_to_35_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_2GeV_run_015664_35_to_40_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_2GeV_run_015664_35_to_40_deg_lastB4thetaSliceFit.root");

    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_4GeV_run_015778_10_to_15_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_4GeV_run_015778_10_to_15_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_4GeV_run_015778_15_to_20_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_4GeV_run_015778_15_to_20_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_4GeV_run_015778_20_to_25_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_4GeV_run_015778_20_to_25_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_4GeV_run_015778_25_to_30_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_4GeV_run_015778_25_to_30_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_4GeV_run_015778_30_to_35_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_4GeV_run_015778_30_to_35_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_C12_data_4GeV_run_015778_35_to_40_deg_lastB4thetaSliceFit/017_HipoLooper_v17_C12_data_4GeV_run_015778_35_to_40_deg_lastB4thetaSliceFit.root");

    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_2GeV_run_015672_10_to_15_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_2GeV_run_015672_10_to_15_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_2GeV_run_015672_15_to_20_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_2GeV_run_015672_15_to_20_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_2GeV_run_015672_20_to_25_deg_lastB4thetaSliceFit/"
    //     "017_HipoLooper_v17_Ar40_data_2GeV_run_015672_20_to_25_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_2GeV_run_015672_25_to_30_deg_lastB4thetaSliceFit/"
    //     "017_HipoLooper_v17_Ar40_data_2GeV_run_015672_25_to_30_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_2GeV_run_015672_30_to_35_deg_lastB4thetaSliceFit/"
    //     "017_HipoLooper_v17_Ar40_data_2GeV_run_015672_30_to_35_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_2GeV_run_015672_35_to_40_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_2GeV_run_015672_35_to_40_deg_lastB4thetaSliceFit.root");

    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_10_to_15_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_10_to_15_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_15_to_20_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_15_to_20_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_20_to_25_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_20_to_25_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_25_to_30_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_25_to_30_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_30_to_35_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_30_to_35_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_35_to_40_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_4GeV_run_015743_35_to_40_deg_lastB4thetaSliceFit.root");

    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_6GeV_run_015792_10_to_15_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_6GeV_run_015792_10_to_15_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_6GeV_run_015792_15_to_20_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_6GeV_run_015792_15_to_20_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_6GeV_run_015792_20_to_25_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_6GeV_run_015792_20_to_25_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_6GeV_run_015792_25_to_30_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_6GeV_run_015792_25_to_30_deg_lastB4thetaSliceFit.root");
    // InputFiles.push_back(
    //     "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
    //     "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_6GeV_run_015792_30_to_35_deg_lastB4thetaSliceFit/017_HipoLooper_v17_Ar40_data_6GeV_run_015792_30_to_35_deg_lastB4thetaSliceFit.root");
    InputFiles.push_back(
        "/Users/alon/Code runs/utils/HipoLooper (Ar40 "
        "imp)/17_HipoLooper_v17/017_HipoLooper_v17_Ar40_data_6GeV_run_015792_35_to_40_deg_lastB4thetaSliceFit/"
        "017_HipoLooper_v17_Ar40_data_6GeV_run_015792_35_to_40_deg_lastB4thetaSliceFit.root");

    // InputFiles.push_back("/Users/alon/Downloads/26_HipoLooper_v26_Ar40_data_4GeV_run_015743__reformat_test/26_HipoLooper_v26_Ar40_data_4GeV_run_015743__reformat_test.root");

    std::string SaveDirFolder = "/Users/alon/Downloads";

    for (auto sample = 0; sample < InputFiles.size(); ++sample) {
        std::string General_status = "";

        // Extract version from filename and append to General_status
        std::string version_suffix = "";
        std::string version_tag = "_v";
        size_t version_pos = InputFiles.at(sample).find(version_tag);
        if (version_pos != std::string::npos) {
            size_t version_start = version_pos + version_tag.size();
            size_t version_end = InputFiles.at(sample).find_first_not_of("0123456789", version_start);
            std::string version_number = InputFiles.at(sample).substr(version_start, version_end - version_start);
            version_suffix = "_v" + version_number;
        }
        // General_status = version_suffix + General_status;
        // General_status = General_status + version_suffix;
        // std::string General_status = General_status + version_suffix;

        std::pair<double, double> theta_slice;

        if (basic_tools::FindSubstring(InputFiles.at(sample), "_to_")) {
            std::string path = InputFiles.at(sample);
            std::string filename = path.substr(path.find_last_of("/\\") + 1);  // isolate filename

            size_t to_pos = filename.find("_to_");

            if (to_pos != std::string::npos && to_pos >= 2) {
                size_t slice_start = to_pos - 2;  // start two characters before "_to_" to get the range start
                size_t slice_end = filename.find(".root");
                if (slice_end != std::string::npos && slice_end > slice_start) {
                    General_status = version_suffix + General_status + "_" + filename.substr(slice_start, slice_end - slice_start);
                }
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
            General_status = version_suffix + General_status;
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

        std::string OutFolderName = OutFolderName_prefix + OutFolderName_ver_status_2 + CodeRun_status + General_status;
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
        std::cout << "\033[33mGeneral_status:    \t\033[0m" << General_status << "\n";

        if (basic_tools::FindSubstring(InputFiles.at(sample), "_to_")) {
            std::cout << "\033[33mtheta_slice:           \t\033[0m {" << theta_slice.first << ", " << theta_slice.second << "}\n";
        }

        std::cout << "\n";

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

            if (!h2) {
                std::cerr << "Error: Failed to load " << name << "." << std::endl;
                return nullptr;
                exit(0);
            }

            std::string projName = basic_tools::ReplaceSubstring(name, "Vz_VS_", "");
            auto h_phi = h2->ProjectionX(projName.c_str());
            h_phi->SetTitle(basic_tools::ReplaceSubstring(h_phi->GetTitle(), "V_{z}^{e} vs. #phi_{e}", "#phi_{e}").c_str());

            if (!h_phi) {
                std::cerr << "Error: Projection of " << name << " failed." << std::endl;
                return nullptr;
                exit(0);
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

        // auto h_Vz_e_AC_1e_cut = (TH1D *)load("Vz_e_AC_1e_cut", "TH1D");
        // auto h_Vz_e_AC_zoomin_1e_cut = (TH1D *)load("Vz_e_AC_zoomin_1e_cut", "TH1D");
        // auto h_Vx_e_AC_1e_cut = (TH1D *)load("Vx_e_AC_1e_cut", "TH1D");
        // auto h_Vy_e_AC_1e_cut = (TH1D *)load("Vy_e_AC_1e_cut", "TH1D");
        // auto h_Vz_VS_phi_e_AC_1e_cut = (TH2D *)load("Vz_VS_phi_e_AC_1e_cut", "TH2D");
        // auto h_Vz_e_AC_sector1_1e_cut = (TH1D *)load("Vz_e_AC_sector1_1e_cut", "TH1D");
        // auto [h_Vz_e_AC_zoomin_sector1_1e_cut, h_phi_e_AC_sector1_1e_cut] = loadVzAndPhiHistograms("sector1", "e");
        // auto h_Vz_e_AC_sector2_1e_cut = (TH1D *)load("Vz_e_AC_sector2_1e_cut", "TH1D");
        // auto [h_Vz_e_AC_zoomin_sector2_1e_cut, h_phi_e_AC_sector2_1e_cut] = loadVzAndPhiHistograms("sector2", "e");
        // auto h_Vz_e_AC_sector3_1e_cut = (TH1D *)load("Vz_e_AC_sector3_1e_cut", "TH1D");
        // auto [h_Vz_e_AC_zoomin_sector3_1e_cut, h_phi_e_AC_sector3_1e_cut] = loadVzAndPhiHistograms("sector3", "e");
        // auto h_Vz_e_AC_sector4_1e_cut = (TH1D *)load("Vz_e_AC_sector4_1e_cut", "TH1D");
        // auto [h_Vz_e_AC_zoomin_sector4_1e_cut, h_phi_e_AC_sector4_1e_cut] = loadVzAndPhiHistograms("sector4", "e");
        // auto h_Vz_e_AC_sector5_1e_cut = (TH1D *)load("Vz_e_AC_sector5_1e_cut", "TH1D");
        // auto [h_Vz_e_AC_zoomin_sector5_1e_cut, h_phi_e_AC_sector5_1e_cut] = loadVzAndPhiHistograms("sector5", "e");
        // auto h_Vz_e_AC_sector6_1e_cut = (TH1D *)load("Vz_e_AC_sector6_1e_cut", "TH1D");
        // auto [h_Vz_e_AC_zoomin_sector6_1e_cut, h_phi_e_AC_sector6_1e_cut] = loadVzAndPhiHistograms("sector6", "e");
        // // auto h_corrected_Vz_e_AC_1e_cut = (TH1D *)load("corrected_Vz_e_AC_1e_cut", "TH1D");
        // // h_corrected_Vz_e_AC_1e_cut->SetName("Vz_e_AC_corrected_1e_cut");
        // // auto h_corrected_Vz_e_AC_sector1_1e_cut = (TH1D *)load("corrected_Vz_e_AC_sector1_1e_cut", "TH1D");
        // // h_corrected_Vz_e_AC_sector1_1e_cut->SetName("Vz_e_AC_corrected_sector1_1e_cut");
        // // auto h_corrected_Vz_e_AC_sector2_1e_cut = (TH1D *)load("corrected_Vz_e_AC_sector2_1e_cut", "TH1D");
        // // h_corrected_Vz_e_AC_sector2_1e_cut->SetName("Vz_e_AC_corrected_sector2_1e_cut");
        // // auto h_corrected_Vz_e_AC_sector3_1e_cut = (TH1D *)load("corrected_Vz_e_AC_sector3_1e_cut", "TH1D");
        // // h_corrected_Vz_e_AC_sector3_1e_cut->SetName("Vz_e_AC_corrected_sector3_1e_cut");
        // // auto h_corrected_Vz_e_AC_sector4_1e_cut = (TH1D *)load("corrected_Vz_e_AC_sector4_1e_cut", "TH1D");
        // // h_corrected_Vz_e_AC_sector4_1e_cut->SetName("Vz_e_AC_corrected_sector4_1e_cut");
        // // auto h_corrected_Vz_e_AC_sector5_1e_cut = (TH1D *)load("corrected_Vz_e_AC_sector5_1e_cut", "TH1D");
        // // h_corrected_Vz_e_AC_sector5_1e_cut->SetName("Vz_e_AC_corrected_sector5_1e_cut");
        // // auto h_corrected_Vz_e_AC_sector6_1e_cut = (TH1D *)load("corrected_Vz_e_AC_sector6_1e_cut", "TH1D");
        // // h_corrected_Vz_e_AC_sector6_1e_cut->SetName("Vz_e_AC_corrected_sector6_1e_cut");

        auto h_Vz_pipFD_AC_1e_cut = (TH1D *)load("Vz_pipFD_AC_1e_cut", "TH1D");
        auto h_Vz_pipFD_AC_zoomin_1e_cut = (TH1D *)load("Vz_pipFD_AC_zoomin_1e_cut", "TH1D");
        auto h_Vx_pipFD_AC_1e_cut = (TH1D *)load("Vx_pipFD_AC_1e_cut", "TH1D");
        auto h_Vy_pipFD_AC_1e_cut = (TH1D *)load("Vy_pipFD_AC_1e_cut", "TH1D");
        auto h_Vz_VS_phi_pipFD_AC_1e_cut = (TH2D *)load("Vz_VS_phi_pipFD_AC_1e_cut", "TH2D");
        auto h_Vz_pipFD_AC_sector1_1e_cut = (TH1D *)load("Vz_pipFD_AC_sector1_1e_cut", "TH1D");
        auto [h_Vz_pipFD_AC_zoomin_sector1_1e_cut, h_phi_pipFD_AC_sector1_1e_cut] = loadVzAndPhiHistograms("sector1", "pipFD");
        auto h_Vz_pipFD_AC_sector2_1e_cut = (TH1D *)load("Vz_pipFD_AC_sector2_1e_cut", "TH1D");
        auto [h_Vz_pipFD_AC_zoomin_sector2_1e_cut, h_phi_pipFD_AC_sector2_1e_cut] = loadVzAndPhiHistograms("sector2", "pipFD");
        auto h_Vz_pipFD_AC_sector3_1e_cut = (TH1D *)load("Vz_pipFD_AC_sector3_1e_cut", "TH1D");
        auto [h_Vz_pipFD_AC_zoomin_sector3_1e_cut, h_phi_pipFD_AC_sector3_1e_cut] = loadVzAndPhiHistograms("sector3", "pipFD");
        auto h_Vz_pipFD_AC_sector4_1e_cut = (TH1D *)load("Vz_pipFD_AC_sector4_1e_cut", "TH1D");
        auto [h_Vz_pipFD_AC_zoomin_sector4_1e_cut, h_phi_pipFD_AC_sector4_1e_cut] = loadVzAndPhiHistograms("sector4", "pipFD");
        auto h_Vz_pipFD_AC_sector5_1e_cut = (TH1D *)load("Vz_pipFD_AC_sector5_1e_cut", "TH1D");
        auto [h_Vz_pipFD_AC_zoomin_sector5_1e_cut, h_phi_pipFD_AC_sector5_1e_cut] = loadVzAndPhiHistograms("sector5", "pipFD");
        auto h_Vz_pipFD_AC_sector6_1e_cut = (TH1D *)load("Vz_pipFD_AC_sector6_1e_cut", "TH1D");
        auto [h_Vz_pipFD_AC_zoomin_sector6_1e_cut, h_phi_pipFD_AC_sector6_1e_cut] = loadVzAndPhiHistograms("sector6", "pipFD");
        // auto h_corrected_Vz_pipFD_AC_1e_cut = (TH1D *)load("corrected_Vz_pipFD_AC_1e_cut", "TH1D");
        // h_corrected_Vz_pipFD_AC_1e_cut->SetName("Vz_pipFD_AC_corrected_1e_cut");
        // auto h_corrected_Vz_pipFD_AC_sector1_1e_cut = (TH1D *)load("corrected_Vz_pipFD_AC_sector1_1e_cut", "TH1D");
        // h_corrected_Vz_pipFD_AC_sector1_1e_cut->SetName("Vz_pipFD_AC_corrected_sector1_1e_cut");
        // auto h_corrected_Vz_pipFD_AC_sector2_1e_cut = (TH1D *)load("corrected_Vz_pipFD_AC_sector2_1e_cut", "TH1D");
        // h_corrected_Vz_pipFD_AC_sector2_1e_cut->SetName("Vz_pipFD_AC_corrected_sector2_1e_cut");
        // auto h_corrected_Vz_pipFD_AC_sector3_1e_cut = (TH1D *)load("corrected_Vz_pipFD_AC_sector3_1e_cut", "TH1D");
        // h_corrected_Vz_pipFD_AC_sector3_1e_cut->SetName("Vz_pipFD_AC_corrected_sector3_1e_cut");
        // auto h_corrected_Vz_pipFD_AC_sector4_1e_cut = (TH1D *)load("corrected_Vz_pipFD_AC_sector4_1e_cut", "TH1D");
        // h_corrected_Vz_pipFD_AC_sector4_1e_cut->SetName("Vz_pipFD_AC_corrected_sector4_1e_cut");
        // auto h_corrected_Vz_pipFD_AC_sector5_1e_cut = (TH1D *)load("corrected_Vz_pipFD_AC_sector5_1e_cut", "TH1D");
        // h_corrected_Vz_pipFD_AC_sector5_1e_cut->SetName("Vz_pipFD_AC_corrected_sector5_1e_cut");
        // auto h_corrected_Vz_pipFD_AC_sector6_1e_cut = (TH1D *)load("corrected_Vz_pipFD_AC_sector6_1e_cut", "TH1D");
        // h_corrected_Vz_pipFD_AC_sector6_1e_cut->SetName("Vz_pipFD_AC_corrected_sector6_1e_cut");

        auto h_Vz_pimFD_AC_1e_cut = (TH1D *)load("Vz_pimFD_AC_1e_cut", "TH1D");
        auto h_Vz_pimFD_AC_zoomin_1e_cut = (TH1D *)load("Vz_pimFD_AC_zoomin_1e_cut", "TH1D");
        auto h_Vx_pimFD_AC_1e_cut = (TH1D *)load("Vx_pimFD_AC_1e_cut", "TH1D");
        auto h_Vy_pimFD_AC_1e_cut = (TH1D *)load("Vy_pimFD_AC_1e_cut", "TH1D");
        // auto h_Vz_VS_phi_pimFD_AC_1e_cut = (TH2D *)load("Vz_VS_phi_pimFD_AC_1e_cut", "TH2D");
        auto h_Vz_pimFD_AC_sector1_1e_cut = (TH1D *)load("Vz_pimFD_AC_sector1_1e_cut", "TH1D");
        auto [h_Vz_pimFD_AC_zoomin_sector1_1e_cut, h_phi_pimFD_AC_sector1_1e_cut] = loadVzAndPhiHistograms("sector1", "pimFD");
        auto h_Vz_pimFD_AC_sector2_1e_cut = (TH1D *)load("Vz_pimFD_AC_sector2_1e_cut", "TH1D");
        auto [h_Vz_pimFD_AC_zoomin_sector2_1e_cut, h_phi_pimFD_AC_sector2_1e_cut] = loadVzAndPhiHistograms("sector2", "pimFD");
        auto h_Vz_pimFD_AC_sector3_1e_cut = (TH1D *)load("Vz_pimFD_AC_sector3_1e_cut", "TH1D");
        auto [h_Vz_pimFD_AC_zoomin_sector3_1e_cut, h_phi_pimFD_AC_sector3_1e_cut] = loadVzAndPhiHistograms("sector3", "pimFD");
        auto h_Vz_pimFD_AC_sector4_1e_cut = (TH1D *)load("Vz_pimFD_AC_sector4_1e_cut", "TH1D");
        auto [h_Vz_pimFD_AC_zoomin_sector4_1e_cut, h_phi_pimFD_AC_sector4_1e_cut] = loadVzAndPhiHistograms("sector4", "pimFD");
        auto h_Vz_pimFD_AC_sector5_1e_cut = (TH1D *)load("Vz_pimFD_AC_sector5_1e_cut", "TH1D");
        auto [h_Vz_pimFD_AC_zoomin_sector5_1e_cut, h_phi_pimFD_AC_sector5_1e_cut] = loadVzAndPhiHistograms("sector5", "pimFD");
        auto h_Vz_pimFD_AC_sector6_1e_cut = (TH1D *)load("Vz_pimFD_AC_sector6_1e_cut", "TH1D");
        auto [h_Vz_pimFD_AC_zoomin_sector6_1e_cut, h_phi_pimFD_AC_sector6_1e_cut] = loadVzAndPhiHistograms("sector6", "pimFD");
        // auto h_corrected_Vz_pimFD_AC_1e_cut = (TH1D *)load("corrected_Vz_pimFD_AC_1e_cut", "TH1D");
        // h_corrected_Vz_pimFD_AC_1e_cut->SetName("Vz_pimFD_AC_corrected_1e_cut");
        // auto h_corrected_Vz_pimFD_AC_sector1_1e_cut = (TH1D *)load("corrected_Vz_pimFD_AC_sector1_1e_cut", "TH1D");
        // h_corrected_Vz_pimFD_AC_sector1_1e_cut->SetName("Vz_pimFD_AC_corrected_sector1_1e_cut");
        // auto h_corrected_Vz_pimFD_AC_sector2_1e_cut = (TH1D *)load("corrected_Vz_pimFD_AC_sector2_1e_cut", "TH1D");
        // h_corrected_Vz_pimFD_AC_sector2_1e_cut->SetName("Vz_pimFD_AC_corrected_sector2_1e_cut");
        // auto h_corrected_Vz_pimFD_AC_sector3_1e_cut = (TH1D *)load("corrected_Vz_pimFD_AC_sector3_1e_cut", "TH1D");
        // h_corrected_Vz_pimFD_AC_sector3_1e_cut->SetName("Vz_pimFD_AC_corrected_sector3_1e_cut");
        // auto h_corrected_Vz_pimFD_AC_sector4_1e_cut = (TH1D *)load("corrected_Vz_pimFD_AC_sector4_1e_cut", "TH1D");
        // h_corrected_Vz_pimFD_AC_sector4_1e_cut->SetName("Vz_pimFD_AC_corrected_sector4_1e_cut");
        // auto h_corrected_Vz_pimFD_AC_sector5_1e_cut = (TH1D *)load("corrected_Vz_pimFD_AC_sector5_1e_cut", "TH1D");
        // h_corrected_Vz_pimFD_AC_sector5_1e_cut->SetName("Vz_pimFD_AC_corrected_sector5_1e_cut");
        // auto h_corrected_Vz_pimFD_AC_sector6_1e_cut = (TH1D *)load("corrected_Vz_pimFD_AC_sector6_1e_cut", "TH1D");
        // h_corrected_Vz_pimFD_AC_sector6_1e_cut->SetName("Vz_pimFD_AC_corrected_sector6_1e_cut");

        auto h_Vz_pipCD_AC_1e_cut = (TH1D *)load("Vz_pipCD_AC_1e_cut", "TH1D");
        auto h_Vz_pipCD_AC_zoomin_1e_cut = (TH1D *)load("Vz_pipCD_AC_zoomin_1e_cut", "TH1D");

        auto h_Vz_pimCD_AC_1e_cut = (TH1D *)load("Vz_pimCD_AC_1e_cut", "TH1D");
        auto h_Vz_pimCD_AC_zoomin_1e_cut = (TH1D *)load("Vz_pimCD_AC_zoomin_1e_cut", "TH1D");

        /////////////////////////////////////////////////////
        // Extracting Vz correction parameters
        /////////////////////////////////////////////////////

        // auto fit_peak_gaussian = [&](TH1D *hist, std::vector<double> fitLimits = {}) -> double {
        //     double fitMin, fitMax;

        //     if (hist->GetEntries() == 0) {
        //         std::cerr << "Histogram is empty. Returning NaN." << std::endl;
        //         return std::numeric_limits<double>::quiet_NaN();
        //     }

        //     if (fitLimits.size() == 0) {
        //         // If no limits are provided, use the histogram's peak center
        //         double peakCenter = hist->GetBinCenter(hist->GetMaximumBin());

        //         if (bt::FindSubstring(hist->GetName(), "_e_") || bt::FindSubstring(hist->GetName(), "_pipCD_") || bt::FindSubstring(hist->GetName(), "_pimCD_")) {
        //             if (peakCenter < 0) {
        //                 // If peak is negative, set limits accordingly
        //                 fitMin = -std::fabs(peakCenter * 1.1);
        //                 fitMax = -std::fabs(peakCenter * 0.9);
        //             } else {
        //                 // If peak is positive, set limits accordingly
        //                 fitMin = std::fabs(peakCenter * 0.9);
        //                 fitMax = std::fabs(peakCenter * 1.1);
        //             }
        //         } else {
        //             if (Ebeam_status_1 == "2GeV") {
        //                 if (peakCenter < 0) {
        //                     // If peak is negative, set limits accordingly
        //                     fitMin = -std::fabs(peakCenter * 1.4);
        //                     fitMax = -std::fabs(peakCenter * 0.6);
        //                 } else {
        //                     // If peak is positive, set limits accordingly
        //                     fitMin = std::fabs(peakCenter * 0.6);
        //                     fitMax = std::fabs(peakCenter * 1.4);
        //                 }
        //             } else {
        //                 if (peakCenter < 0) {
        //                     // If peak is negative, set limits accordingly
        //                     fitMin = -std::fabs(peakCenter * 1.2);
        //                     fitMax = -std::fabs(peakCenter * 0.8);
        //                 } else {
        //                     // If peak is positive, set limits accordingly
        //                     fitMin = std::fabs(peakCenter * 0.8);
        //                     fitMax = std::fabs(peakCenter * 1.2);
        //                 }
        //             }
        //         }
        //     } else if (fitLimits.size() == 2) {
        //         fitMin = fitLimits[0];
        //         fitMax = fitLimits[1];
        //     } else {
        //         std::cerr << "Error: fitLimits must contain exactly two elements." << std::endl;
        //         return std::numeric_limits<double>::quiet_NaN();
        //     }

        //     TF1 *fit = new TF1("fit", "gaus", fitMin, fitMax);
        //     hist->Fit(fit, "RQ");  // R = use range, Q = quiet

        //     fit->SetLineColor(kViolet);
        //     // fit->SetLineColor(kMagenta);

        //     hist->GetListOfFunctions()->Clear();
        //     hist->GetListOfFunctions()->Add(fit);  // Add fit to the histogram's function list

        //     return fit->GetParameter(1);  // Return fitted mean
        // };

        // // Helper lambda to extract peak centers from histograms
        // auto get_peak_centers = [&](std::vector<TH1D *> hists, bool fit) {
        //     std::vector<double> centers;
        //     for (auto *h : hists) { centers.push_back(fit ? vc::FitPeakToGaussian(h) : h->GetBinCenter(h->GetMaximumBin())); }
        //     return centers;
        // };

        // // Helper lambda to extract and fit peaks and return fit results
        // auto extract_and_fit = [&](const std::string &label, const std::string &Ebeam, const std::vector<TH1D *> &Vz_hists, const std::vector<TH1D *> &phi_hists, bool fit_Vz,
        //                            bool fit_phi) -> std::tuple<double, double, double, TGraph *> {
        //     auto Vz_peaks = get_peak_centers(Vz_hists, fit_Vz);
        //     auto phi_peaks = get_peak_centers(phi_hists, fit_phi);
        //     return vc::FitVertexVsPhi(label, Ebeam, Vz_peaks, phi_peaks, theta_slice);
        // };

        // // Usage:
        // auto [A_e, phi_beam_e, Z0_e, FittedParametersGraph_e] = extract_and_fit(
        //     "e", Ebeam_status_1,
        //     {h_Vz_e_AC_zoomin_sector1_1e_cut, h_Vz_e_AC_zoomin_sector2_1e_cut, h_Vz_e_AC_zoomin_sector3_1e_cut, h_Vz_e_AC_zoomin_sector4_1e_cut, h_Vz_e_AC_zoomin_sector5_1e_cut,
        //      h_Vz_e_AC_zoomin_sector6_1e_cut},
        //     {h_phi_e_AC_sector1_1e_cut, h_phi_e_AC_sector2_1e_cut, h_phi_e_AC_sector3_1e_cut, h_phi_e_AC_sector4_1e_cut, h_phi_e_AC_sector5_1e_cut, h_phi_e_AC_sector6_1e_cut}, true,
        //     false);

        // auto [A_pipFD, phi_beam_pipFD, Z0_pipFD, FittedParametersGraph_pipFD] =
        //     extract_and_fit("#pi^{+}FD", Ebeam_status_1,
        //                     {h_Vz_pipFD_AC_zoomin_sector1_1e_cut, h_Vz_pipFD_AC_zoomin_sector2_1e_cut, h_Vz_pipFD_AC_zoomin_sector3_1e_cut, h_Vz_pipFD_AC_zoomin_sector4_1e_cut,
        //                      h_Vz_pipFD_AC_zoomin_sector5_1e_cut, h_Vz_pipFD_AC_zoomin_sector6_1e_cut},
        //                     {h_phi_pipFD_AC_sector1_1e_cut, h_phi_pipFD_AC_sector2_1e_cut, h_phi_pipFD_AC_sector3_1e_cut, h_phi_pipFD_AC_sector4_1e_cut, h_phi_pipFD_AC_sector5_1e_cut,
        //                      h_phi_pipFD_AC_sector6_1e_cut},
        //                     true, true);
        auto [A_pipFD, phi_beam_pipFD, Z0_pipFD, FittedParametersGraph_pipFD] =
            vc::FitVertexVsPhi("#pi^{+}FD", SampleName, h_Vz_VS_phi_pipFD_AC_1e_cut,
                               {h_Vz_pipFD_AC_zoomin_sector1_1e_cut, h_Vz_pipFD_AC_zoomin_sector2_1e_cut, h_Vz_pipFD_AC_zoomin_sector3_1e_cut, h_Vz_pipFD_AC_zoomin_sector4_1e_cut,
                                h_Vz_pipFD_AC_zoomin_sector5_1e_cut, h_Vz_pipFD_AC_zoomin_sector6_1e_cut},
                               {h_phi_pipFD_AC_sector1_1e_cut, h_phi_pipFD_AC_sector2_1e_cut, h_phi_pipFD_AC_sector3_1e_cut, h_phi_pipFD_AC_sector4_1e_cut, h_phi_pipFD_AC_sector5_1e_cut,
                                h_phi_pipFD_AC_sector6_1e_cut},
                               theta_slice);

        // auto [A_pimFD, phi_beam_pimFD, Z0_pimFD, FittedParametersGraph_pimFD] =
        //     extract_and_fit("#pi^{-}FD", Ebeam_status_1, h_Vz_VS_phi_pimFD_AC_1e_cut,
        //                     {h_Vz_pimFD_AC_zoomin_sector1_1e_cut, h_Vz_pimFD_AC_zoomin_sector2_1e_cut, h_Vz_pimFD_AC_zoomin_sector3_1e_cut, h_Vz_pimFD_AC_zoomin_sector4_1e_cut,
        //                      h_Vz_pimFD_AC_zoomin_sector5_1e_cut, h_Vz_pimFD_AC_zoomin_sector6_1e_cut},
        //                     {h_phi_pimFD_AC_sector1_1e_cut, h_phi_pimFD_AC_sector2_1e_cut, h_phi_pimFD_AC_sector3_1e_cut, h_phi_pimFD_AC_sector4_1e_cut, h_phi_pimFD_AC_sector5_1e_cut,
        //                      h_phi_pimFD_AC_sector6_1e_cut},
        //                     true, true);

        // Helper lambda for TH1 styling
        auto style_th1 = [](TH1 *h) {
            h->Sumw2();
            h->SetMinimum(0);
            h->SetLineWidth(2);
            h->SetLineColor(kRed);
        };

        // Helper lambda for centering axis titles
        auto center_titles = [](TObject *obj) {
            if (obj->InheritsFrom("TH1") || obj->InheritsFrom("TH2")) {
                auto *h = (TH1 *)obj;
                h->GetXaxis()->CenterTitle();
                h->GetYaxis()->CenterTitle();
            } else if (obj->InheritsFrom("TGraph")) {
                auto *g = (TGraph *)obj;
                g->GetXaxis()->CenterTitle();
                g->GetYaxis()->CenterTitle();
            }
        };

        int insert_index_e = 0;
        int insert_index_pipFD = 0;
        int insert_index_pimFD = 0;

        for (int i = 0; i < HistoList.size(); i++) {
            if (HistoList[i]->InheritsFrom("TH1")) {
                auto *h1 = (TH1 *)HistoList[i];
                if (!h1->GetSumw2N()) { h1->Sumw2(); }

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
            } else if (std::string(HistoList[i]->GetName()) == "Vz_pipFD_AC_zoomin_1e_cut") {
                insert_index_pipFD = i + 1;
            } else if (std::string(HistoList[i]->GetName()) == "Vz_pimFD_AC_zoomin_1e_cut") {
                insert_index_pimFD = i + 1;
            }
        }

        // // FittedParametersGraph_e->GetXaxis()->CenterTitle();
        // // FittedParametersGraph_e->GetYaxis()->CenterTitle();
        FittedParametersGraph_pipFD->GetXaxis()->CenterTitle();
        FittedParametersGraph_pipFD->GetYaxis()->CenterTitle();
        // FittedParametersGraph_pimFD->GetXaxis()->CenterTitle();
        // FittedParametersGraph_pimFD->GetYaxis()->CenterTitle();

        // // HistoList.insert(HistoList.begin() + insert_index_e, FittedParametersGraph_e);
        HistoList.insert(HistoList.begin() + insert_index_pipFD, FittedParametersGraph_pipFD);
        // HistoList.insert(HistoList.begin() + insert_index_pimFD, FittedParametersGraph_pimFD);

        // int insert_index_e = 0, insert_index_pipFD = 0, insert_index_pimFD = 0;

        // for (size_t i = 0; i < HistoList.size(); i++) {
        //     if (HistoList[i]->InheritsFrom("TH1")) { style_th1((TH1 *)HistoList[i]); }

        //     center_titles(HistoList[i]);

        //     std::string name = HistoList[i]->GetName();
        //     if (name == "Vz_VS_phi_e_AC_1e_cut")
        //         insert_index_e = i + 1;
        //     else if (name == "Vz_VS_phi_pipFD_AC_1e_cut")
        //         insert_index_pipFD = i + 1;
        //     else if (name == "Vz_VS_phi_pimFD_AC_1e_cut")
        //         insert_index_pimFD = i + 1;
        // }

        // std::vector<std::pair<int, TGraph *>> graph_inserts = {
        //     {insert_index_e, FittedParametersGraph_e}, {insert_index_pipFD, FittedParametersGraph_pipFD}, {insert_index_pimFD, FittedParametersGraph_pimFD}};

        // for (const auto &[index, graph] : graph_inserts) {
        //     HistoList.insert(HistoList.begin() + index, graph);
        //     center_titles(graph);
        // }

        // for (size_t i = 0; i < HistoList.size(); i++) {
        //     TH1 *h = (TH1 *)HistoList[i];
        //     std::cout << h->GetName() << "\n";
        // }

        // exit(1);

        // for (auto *obj : HistoList_ByThetaSlices) {
        //     if (obj->InheritsFrom("TH1")) { style_th1((TH1 *)obj); }
        //     center_titles(obj);
        // }

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
        myText->cd();

        titles.DrawLatex(0.05, 0.9, "HipoLooper Output");
        text.DrawLatex(0.05, 0.75, ("Plots from (e,e') events in: #font[42]{" + CodeRun_status + "}").c_str());

        if (IsData) {
            std::string input_str = InputFiles.at(sample);
            const size_t max_length_per_line = 80;
            const int max_lines = 5;

            std::vector<std::string> lines;
            size_t start = 0;
            while (start < input_str.length() && lines.size() < max_lines) {
                size_t end = start + max_length_per_line;
                if (end >= input_str.length()) {
                    lines.push_back(input_str.substr(start));
                    break;
                }

                size_t break_pos = input_str.find_last_of("/\\", end);
                if (break_pos == std::string::npos || break_pos <= start) break_pos = end;
                lines.push_back(input_str.substr(start, break_pos - start));
                start = break_pos;
            }

            text.DrawLatex(0.05, 0.70, "InputFile:");
            for (int i = 0; i < lines.size(); ++i) { text.DrawLatex(0.05, 0.66 - 0.04 * i, ("#font[42]{" + lines[i] + "}").c_str()); }
        } else {
            text.DrawLatex(0.05, 0.7, ("BaseDir: #font[42]{" + BaseDir + "}").c_str());
            text.DrawLatex(0.05, 0.65, ("InputFiles: #font[42]{BaseDir + " + InputFiles.at(sample).substr(BaseDir.length()) + "}").c_str());
        }

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

        myText->Print(fileName, "pdf Title: Cover");
        // myText->Print(fileName, "pdf");
        myText->Clear();

        std::map<std::string, bool> first_flags_scalar = {{"{e}", true}, {"{#pi^{+}FD}", true}, {"{#pi^{-}FD}", true}, {"{#pi^{+}CD}", true}, {"{#pi^{-}CD}", true}};
        std::map<std::string, std::array<bool, 6>> first_flags_sector;

        for (auto &[particle, _] : first_flags_scalar) { first_flags_sector[particle] = {true, true, true, true, true, true}; }

        std::map<std::string, bool *> first_flags;

        for (auto &[particle, flag] : first_flags_scalar) { first_flags[particle] = &flag; }

        std::map<std::string, std::map<int, bool *>> sector_flags;

        for (auto &[particle, sector_array] : first_flags_sector) {
            for (int sec = 0; sec < 6; ++sec) { sector_flags[particle][sec + 1] = &sector_array[sec]; }
        }

        std::map<std::string, std::string> particle_labels = {
            {"{e}", "e^{-}"}, {"{#pi^{+}FD}", "FD #pi^{+}"}, {"{#pi^{-}FD}", "FD #pi^{-}"}, {"{#pi^{+}CD}", "CD #pi^{+}"}, {"{#pi^{-}CD}", "CD #pi^{-}"}};

        int plot_counter = 2;
        double yOffset = 0.075;  // Offset for the y position of the text
        std::string current_slice_id;

        for (int i = 0; i < HistoList.size(); i++) {
            std::string title = HistoList[i]->GetTitle();
            std::string name = HistoList[i]->GetName();

            std::regex slice_re(R"(BySliceOfTheta_slice_from_(\d+\.\d+)_to_(\d+\.\d+))");
            std::smatch match;
            bool Is_hsPlot = std::regex_search(name, match, slice_re);
            std::string slice_id = Is_hsPlot ? "BySliceOfTheta_slice_from_" + match[1].str() + "_to_" + match[2].str() : "";

            // Reset first_flags once per new slice_id, outside the particle loop
            if (Is_hsPlot && slice_id != current_slice_id) {
                current_slice_id = slice_id;
                for (auto &[key, flag] : first_flags) { *flag = true; }
            }

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

                    auto [peak, err] = vc::FitPeakToGaussian(h);

                    TLine *speac_target_location_TLine;
                    double speac_target_location_value = 0.0;

                    if (target_status == "C12") {
                        speac_target_location_value = (2.5 - 3.0);
                    } else if (target_status == "Ar40") {
                        speac_target_location_value = (-2.5 - 3.0);
                    }

                    speac_target_location_TLine = new TLine(speac_target_location_value, 0., speac_target_location_value, gPad->GetFrame()->GetY2());
                    speac_target_location_TLine->SetLineColor(kBlue);
                    speac_target_location_TLine->Draw("same");

                    TLine *measured_target_location_TLine;
                    // double measured_target_location_value = h->GetBinCenter(h->GetMaximumBin());
                    double measured_target_location_value = peak;

                    measured_target_location_TLine = new TLine(measured_target_location_value, 0., measured_target_location_value, gPad->GetFrame()->GetY2());
                    measured_target_location_TLine->SetLineColor(kGreen + 1);
                    measured_target_location_TLine->SetLineWidth(3);
                    // measured_target_location_TLine->SetLineWidth(4);
                    measured_target_location_TLine->SetLineStyle(2);
                    measured_target_location_TLine->Draw("same");

                    auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25 - yOffset, gStyle->GetStatX() - 0.25 - 0.1, gStyle->GetStatY() - 0.375 - yOffset);
                    TLegendEntry *speac_target_location_TLine_entry =
                        Legend->AddEntry(speac_target_location_TLine, ("Spec. z pos. = " + basic_tools::ToStringWithPrecision(speac_target_location_value, 2) + " cm").c_str(), "l");
                    TLegendEntry *measured_target_location_TLine_entry = Legend->AddEntry(
                        measured_target_location_TLine,
                        ("Meas. z pos. = " + basic_tools::ToStringWithPrecision(measured_target_location_value, 2) + " #pm " + basic_tools::ToStringWithPrecision(err, 2) + " cm").c_str(),
                        "l");

                    Legend->Draw("same");

                    auto ListOfFunctions = h->GetListOfFunctions();
                    ListOfFunctions->Add(speac_target_location_TLine);
                    ListOfFunctions->Add(measured_target_location_TLine);
                    ListOfFunctions->Add(Legend);
                } else if (basic_tools::FindSubstring(h->GetName(), "phi_")) {
                    gPad->Update();

                    auto [peak, err] = vc::FitPeakToGaussian(h);

                    TLine *measured_target_location_TLine;
                    // double measured_target_location_value = h->GetBinCenter(h->GetMaximumBin());
                    double measured_target_location_value = peak;

                    measured_target_location_TLine = new TLine(measured_target_location_value, 0., measured_target_location_value, gPad->GetFrame()->GetY2());
                    measured_target_location_TLine->SetLineColor(kGreen + 1);
                    measured_target_location_TLine->SetLineWidth(3);
                    measured_target_location_TLine->SetLineStyle(2);
                    measured_target_location_TLine->Draw("same");

                    auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25 - yOffset, gStyle->GetStatX() - 0.25 - 0.1, gStyle->GetStatY() - 0.325 - yOffset);
                    TLegendEntry *measured_target_location_TLine_entry = Legend->AddEntry(
                        measured_target_location_TLine,
                        ("phi_e peak = " + basic_tools::ToStringWithPrecision(measured_target_location_value, 2) + " #pm " + basic_tools::ToStringWithPrecision(err, 2) + "#circ").c_str(),
                        "l");

                    Legend->Draw("same");

                    auto ListOfFunctions = h->GetListOfFunctions();
                    ListOfFunctions->Add(measured_target_location_TLine);
                    ListOfFunctions->Add(Legend);
                } else if ((bt::FindSubstring(h->GetTitle(), "V_{x}^{") || bt::FindSubstring(h->GetTitle(), "V_{y}^{")) && bt::FindSubstring(h->GetTitle(), "after")) {
                    gPad->Update();

                    std::vector<double> fitLimits = {h->GetBinCenter(h->GetMaximumBin()) * 0.7, h->GetBinCenter(h->GetMaximumBin()) * 1.3};

                    auto [peak, err] = vc::FitPeakToGaussian(h, fitLimits);

                    TLine *measured_target_location_TLine;
                    // double measured_target_location_value = h->GetBinCenter(h->GetMaximumBin());
                    double measured_target_location_value = peak;

                    measured_target_location_TLine = new TLine(measured_target_location_value, 0., measured_target_location_value, gPad->GetFrame()->GetY2());
                    measured_target_location_TLine->SetLineColor(kGreen + 1);
                    measured_target_location_TLine->SetLineWidth(3);
                    measured_target_location_TLine->SetLineStyle(2);
                    measured_target_location_TLine->Draw("same");

                    auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.25 - yOffset, gStyle->GetStatX() - 0.25 - 0.1, gStyle->GetStatY() - 0.325 - yOffset);
                    TLegendEntry *measured_target_location_TLine_entry;

                    if (bt::FindSubstring(h->GetTitle(), "V_{x}^{")) {
                        measured_target_location_TLine_entry = Legend->AddEntry(
                            measured_target_location_TLine,
                            ("Meas. x pos. = " + bt::ToStringWithPrecision(measured_target_location_value, 4) + " #pm " + basic_tools::ToStringWithPrecision(err, 4) + " cm").c_str(), "l");
                    } else if (bt::FindSubstring(h->GetTitle(), "V_{y}^{")) {
                        measured_target_location_TLine_entry = Legend->AddEntry(
                            measured_target_location_TLine,
                            ("Meas. y pos. = " + bt::ToStringWithPrecision(measured_target_location_value, 4) + " #pm " + basic_tools::ToStringWithPrecision(err, 4) + " cm").c_str(), "l");
                    }

                    Legend->Draw("same");

                    auto ListOfFunctions = h->GetListOfFunctions();
                    ListOfFunctions->Add(measured_target_location_TLine);
                    ListOfFunctions->Add(Legend);
                }
            } else if (HistoList[i]->InheritsFrom("TH2D")) {
                auto *h = (TH2D *)HistoList[i];

                h->Draw("colz");

                if (Is_hsPlot) {
                    TPaveStats *stats = (TPaveStats *)((TH1 *)HistoList[i])->FindObject("stats");
                    if (stats) {
                        stats->SetX1NDC(stats->GetX1NDC());
                        stats->SetY1NDC(stats->GetY1NDC() - 0.05);
                        stats->SetX2NDC(stats->GetX2NDC());
                        stats->SetY2NDC(stats->GetY2NDC() - 0.05);
                        gPad->Modified();
                        gPad->Update();
                    }
                }

                myCanvas->SetLogz(0);
                if (bt::FindSubstring(h->GetName(), "PCAL") && !bt::FindSubstring(h->GetName(), "sampling fraction")) { myCanvas->SetLogz(1); }

                if (h->GetEntries() != 0) {
                    gPad->Update();
                    TPaletteAxis *palette = (TPaletteAxis *)h->GetListOfFunctions()->FindObject("palette");
                    if (Is_hsPlot) {
                        palette->SetY2NDC(0.50);
                    } else {
                        palette->SetY2NDC(0.55);
                    }
                    gPad->Modified();
                    gPad->Update();
                }

                TList *funcs = h->GetListOfFunctions();
                std::vector<TGraphErrors *> graphs_to_draw;

                for (int j = 0; j < funcs->GetSize(); ++j) {
                    TObject *obj = funcs->At(j);
                    if (obj && obj->InheritsFrom("TGraphErrors")) {
                        auto *g = (TGraphErrors *)obj;
                        graphs_to_draw.push_back(g);
                    }
                }

                // Remove after iteration to avoid iterator invalidation
                for (auto *g : graphs_to_draw) {
                    funcs->Remove(g);
                    g->SetMarkerStyle(21);
                    g->SetMarkerSize(1.5);
                    g->SetLineWidth(2);
                    g->Draw("P same");  // Show points + error bars
                }
            } else if (HistoList[i]->InheritsFrom("TGraph")) {
                ((TGraph *)HistoList[i])->Draw("ap");
                // ((TGraph *)HistoList[i])->Draw("APL");

                // TLegend *Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.10 - yOffset, gStyle->GetStatX() - 0.25, gStyle->GetStatY() - 0.20 - yOffset);
                // Legend->AddEntry(HistoList[i], HistoList[i]->GetTitle(), "l");
                // Legend->Draw("same");

                // HistoList[i]->GetListOfFunctions()->Add(Legend);
            }

            if ((basic_tools::FindSubstring(HistoList[i]->GetTitle(), "V_{z}^{") && !basic_tools::FindSubstring(HistoList[i]->GetTitle(), "sector")) ||
                basic_tools::FindSubstring(HistoList[i]->GetTitle(), "Corrected V_{z}^{")) {
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

        hf::FixPDFOrientation(PDF_fileName);                                                    // Fix orientation
        hf::ReassignPDFBookmarks(bt::GetCurrentDirectory() + "/", PDF_fileName, PDF_fileName);  // Reassign clean bookmarks

        // Helper lambda for using the hf::CompareHistograms function
        int ComparisonNumber = 0;
        auto compare = [&](const std::vector<TObject *> &hists, const std::string &tag) {
            ++ComparisonNumber;
            hf::CompareHistograms(hists, OutputDir, "Histogram_Comparisons", bt::ToStringWithPrecision(ComparisonNumber, 0) + "_" + tag);
        };

        // compare({h_Vz_e_AC_sector1_1e_cut, h_Vz_e_AC_sector2_1e_cut, h_Vz_e_AC_sector3_1e_cut, h_Vz_e_AC_sector4_1e_cut, h_Vz_e_AC_sector5_1e_cut, h_Vz_e_AC_sector6_1e_cut},
        //         "Vz_e_AC_BySector_1e_cut");
        // // compare({h_corrected_Vz_e_AC_sector1_1e_cut, h_corrected_Vz_e_AC_sector2_1e_cut, h_corrected_Vz_e_AC_sector3_1e_cut, h_corrected_Vz_e_AC_sector4_1e_cut,
        // //          h_corrected_Vz_e_AC_sector5_1e_cut, h_corrected_Vz_e_AC_sector6_1e_cut},
        // //         "corrected_Vz_e_AC_BySector_1e_cut");

        compare({h_Vz_pipFD_AC_sector1_1e_cut, h_Vz_pipFD_AC_sector2_1e_cut, h_Vz_pipFD_AC_sector3_1e_cut, h_Vz_pipFD_AC_sector4_1e_cut, h_Vz_pipFD_AC_sector5_1e_cut,
                 h_Vz_pipFD_AC_sector6_1e_cut},
                "Vz_pipFD_AC_BySector_1e_cut");
        // compare({h_corrected_Vz_pipFD_AC_sector1_1e_cut, h_corrected_Vz_pipFD_AC_sector2_1e_cut, h_corrected_Vz_pipFD_AC_sector3_1e_cut, h_corrected_Vz_pipFD_AC_sector4_1e_cut,
        //          h_corrected_Vz_pipFD_AC_sector5_1e_cut, h_corrected_Vz_pipFD_AC_sector6_1e_cut},
        //         "corrected_Vz_pipFD_AC_BySector_1e_cut");

        compare({h_Vz_pimFD_AC_sector1_1e_cut, h_Vz_pimFD_AC_sector2_1e_cut, h_Vz_pimFD_AC_sector3_1e_cut, h_Vz_pimFD_AC_sector4_1e_cut, h_Vz_pimFD_AC_sector5_1e_cut,
                 h_Vz_pimFD_AC_sector6_1e_cut},
                "Vz_pimFD_AC_BySector_1e_cut");
        // compare({h_corrected_Vz_pimFD_AC_sector1_1e_cut, h_corrected_Vz_pimFD_AC_sector2_1e_cut, h_corrected_Vz_pimFD_AC_sector3_1e_cut, h_corrected_Vz_pimFD_AC_sector4_1e_cut,
        //          h_corrected_Vz_pimFD_AC_sector5_1e_cut, h_corrected_Vz_pimFD_AC_sector6_1e_cut},
        //         "corrected_Vz_pimFD_AC_BySector_1e_cut");

        compare({h_Vz_pipFD_AC_1e_cut, h_Vz_pimFD_AC_1e_cut}, "Vz_pions_FD_AC_1e_cut");
        // compare({h_corrected_Vz_pipFD_AC_1e_cut, h_corrected_Vz_pimFD_AC_1e_cut}, "corrected_Vz_pions_FD_AC_1e_cut");

        compare({h_Vz_pipCD_AC_1e_cut, h_Vz_pimCD_AC_1e_cut}, "Vz_pions_CD_AC_1e_cut");

        // // Clean up
        // for (TObject *obj : HistoList) { delete obj; }

        // file->Close();
        delete file;

        // // Delete all ROOT objects whose class names start with TH (to prevent a memory leak):
        // if (InputFiles.size() > 1) { gDirectory->Clear(); }
    }
}