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
    int version = 14;  // Version of the code
    std::string OutFolderName_prefix = "0a0" + basic_tools::ToStringWithPrecision(version, 0) + "_HipoLooper";
    std::string OutFolderName_ver_status = "_v" + basic_tools::ToStringWithPrecision(version, 0) + "_";

    // std::string General_status = "";
    // std::string General_status = "_NewCuts2_wReverseddVz_cuts_on_both_electrons_and_piplusOnly";
    // std::string General_status = "_NewCuts2_wReverseddVz_cuts_on_both_electrons_and_piminusOnly";
    // std::string General_status = "_NewCuts2_wReverseddVz_cuts_on_both_electrons_and_piplusFDOnly";
    // std::string General_status = "_NewCuts2_wReverseddVz_cuts_on_both_electrons_and_piminusFDOnly";
    // std::string General_status = "_NewCuts2_wReverseddVz_cuts_on_both_electrons_and_piplusCDOnly";
    // std::string General_status = "_NewCuts2_wReverseddVz_cuts_on_both_electrons_and_piminusCDOnly";
    // std::string General_status = "_NewCuts2_wReverseddVz_cuts";
    std::string General_status = "_NewCuts2_wdVz_cuts";
    // std::string General_status = "_NewCuts2_NOdVz_cuts";

    bool ApplyLimiter = true;
    // int Limiter = 10000000;  // 10M events (fo the data)
    int Limiter = 1000000;  // 100 files or 1M events (fo the data)
    // int Limiter = 100000;  // 10 files or 100K events (fo the data)
    // int Limiter = 10000;  // 1 file

    std::string BaseDir = "/lustre24/expphy/volatile/clas12/asportes/2N_Analysis_Reco_Samples/GENIE_Reco_Samples";

    std::vector<std::string> InputFiles;
    InputFiles.push_back("/Users/alon/Downloads/014_HipoLooper_v14_Ar40_data_2GeV_run_015672_NewCuts2_wdVz_cuts/014_HipoLooper_v14_Ar40_data_2GeV_run_015672_NewCuts2_wdVz_cuts.root");
    // InputFiles.push_back(
    //     "/Users/alon/Downloads/13_HipoLooper_v13/013_HipoLooper_v13_NewCuts2_wdVz_cuts/013_HipoLooper_v13_Ar40_data_2GeV_run_015672_NewCuts2_wReverseddVz_cuts/"
    //     "013_HipoLooper_v13_Ar40_data_2GeV_run_015672_NewCuts2_wReverseddVz_cuts.root");

    std::string SaveDirFolder = "/Users/alon/Downloads";

    for (auto sample = 0; sample < InputFiles.size(); ++sample) {
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

        std::string OutFolderName = OutFolderName_prefix + OutFolderName_ver_status + CodeRun_status + General_status;
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

        std::string SaveDirFoldersName = CodeRun_status + "_plots";
        const char *filename = InputFiles[sample].c_str();

        system(("rm -rf " + SaveDirFolder + "/" + SaveDirFoldersName).c_str());
        system(("mkdir -p " + SaveDirFolder + "/" + SaveDirFoldersName).c_str());

        std::cout << "\033[33m\n\n===========================================================================================\n\n";
        std::cout << "\033[33mCodeRun_status:    \t\033[0m" << CodeRun_status << "\n";
        std::cout << "\033[33mSaveDirFolder:     \t\033[0m" << SaveDirFolder << "\n";
        std::cout << "\033[33mSaveDirFoldersName:\t\033[0m" << SaveDirFoldersName << "\n";
        std::cout << "\033[33mfilename:          \t\033[0m" << filename << "\n";

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

        auto [h_Vz_e_AC_sector1_1e_cut, h_phi_e_AC_sector1_1e_cut] = loadVzAndPhiHistograms("sector1");
        auto [h_Vz_e_AC_sector2_1e_cut, h_phi_e_AC_sector2_1e_cut] = loadVzAndPhiHistograms("sector2");
        auto [h_Vz_e_AC_sector3_1e_cut, h_phi_e_AC_sector3_1e_cut] = loadVzAndPhiHistograms("sector3");
        auto [h_Vz_e_AC_sector4_1e_cut, h_phi_e_AC_sector4_1e_cut] = loadVzAndPhiHistograms("sector4");
        auto [h_Vz_e_AC_sector5_1e_cut, h_phi_e_AC_sector5_1e_cut] = loadVzAndPhiHistograms("sector5");
        auto [h_Vz_e_AC_sector6_1e_cut, h_phi_e_AC_sector6_1e_cut] = loadVzAndPhiHistograms("sector6");

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
                fitMin = -std::fabs(peakCenter * 1.05);
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

            fit->SetLineColor(kMagenta);

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

        std::vector<double> Vz_e_peaks_BySector = {peak_sector1, peak_sector2, peak_sector3, peak_sector4, peak_sector5, peak_sector6};
        // h_Vz_e_AC_sector1_1e_cut->GetBinCenter(h_Vz_e_AC_sector1_1e_cut->GetMaximumBin()), h_Vz_e_AC_sector2_1e_cut->GetBinCenter(h_Vz_e_AC_sector2_1e_cut->GetMaximumBin()),
        // // -6.5, h_Vz_e_AC_sector4_1e_cut->GetBinCenter(h_Vz_e_AC_sector4_1e_cut->GetMaximumBin()),
        // h_Vz_e_AC_sector3_1e_cut->GetBinCenter(h_Vz_e_AC_sector3_1e_cut->GetMaximumBin()), h_Vz_e_AC_sector4_1e_cut->GetBinCenter(h_Vz_e_AC_sector4_1e_cut->GetMaximumBin()),
        // h_Vz_e_AC_sector5_1e_cut->GetBinCenter(h_Vz_e_AC_sector5_1e_cut->GetMaximumBin()), h_Vz_e_AC_sector6_1e_cut->GetBinCenter(h_Vz_e_AC_sector6_1e_cut->GetMaximumBin())};
        auto [A, phi_beam, Z0, FittedParametersGraph] = variable_correctors::FitVertexVsPhi("e", Ebeam_status_1, Vz_e_peaks_BySector);

        FittedParametersGraph->GetXaxis()->CenterTitle();
        FittedParametersGraph->GetYaxis()->CenterTitle();

        HistoList.insert(HistoList.begin(), FittedParametersGraph);

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

            // for (const auto &[particle_key, label] : particle_labels) {
            //     if (basic_tools::FindSubstring(title, particle_key)) {
            //         myText->cd();
            //         titles.SetTextAlign(22);  // Center text both horizontally and vertically

            //         if (*first_flags[particle_key] && !basic_tools::FindSubstring(title, "sector")) {
            //             std::string bookmark_title = label + " plots";
            //             std::string sanitized_bookmark_title = histogram_functions::SanitizeForBookmark(bookmark_title);
            //             titles.DrawLatex(0.5, 0.5, bookmark_title.c_str());
            //             myText->Print(fileName, ("pdf Title:" + sanitized_bookmark_title).c_str());
            //             myText->Clear();
            //             *first_flags[particle_key] = false;
            //             ++plot_counter;
            //         } else {
            //             for (int sector = 1; sector <= 6; ++sector) {
            //                 std::string sector_str = "sector" + std::to_string(sector);
            //                 std::string sector_title_str = "sector " + std::to_string(sector);

            //                 if (*sector_flags[particle_key][sector] && basic_tools::FindSubstring(title, sector_str)) {
            //                     std::string bookmark_title = label + " plots - " + sector_title_str;
            //                     // Compose hierarchical bookmark: parent>child (separation expects '>' for hierarchy)
            //                     std::string hierarchical_title = histogram_functions::SanitizeForBookmark(label + " plots") + ">" +
            //                     histogram_functions::SanitizeForBookmark(bookmark_title); titles.DrawLatex(0.5, 0.5, bookmark_title.c_str()); myText->Print(fileName, ("pdf Title:" +
            //                     hierarchical_title).c_str()); myText->Clear(); *sector_flags[particle_key][sector] = false;
            //                     ++plot_counter;
            //                     break;
            //                 }
            //             }
            //         }

            //         break;  // Stop checking other particles after match
            //     }
            // }

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

                if (basic_tools::FindSubstring(h->GetTitle(), "V_{z}^{") && !basic_tools::FindSubstring(h->GetTitle(), "dV_{z}^{")) {
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