//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef VARIABLE_CORRECTORS_H
#define VARIABLE_CORRECTORS_H

#include <TF1.h>
#include <TGraph.h>
#include <TMath.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>  // for std::tuple
#include <vector>

#include "../basic_tools.h"
#include "../constants.h"

// TODO: move into fitters namespace

namespace variable_correctors {

/**
 * @brief Fits the average reconstructed vertex positions from 6 CLAS12 sectors
 *        to a sinusoidal function to extract the beam offset direction and magnitude.
 *
 * Motivation:
 * In CLAS12, if the beam is transversely offset relative to the nominal target axis,
 * the reconstructed z-vertex depends on the azimuthal angle of the detected particle.
 * This introduces a sector-dependent bias in Z_rec that follows a cosine dependence.
 * By fitting the peak Z_rec values across the 6 sectors to the form:
 *     Z_rec(φ) = A * cos(φ - φ_beam) + Vz_0
 * one can extract:
 *   - A        : amplitude ∝ beam offset magnitude
 *   - φ_beam   : direction of the beam offset
 *   - Vz_0       : average z-position of the target center
 *
 * The function returns a tuple with (Amplitude A, Beam direction φ_beam [rad], Vz_0, and TGraph* for visualization).
 *
 * @param Zrec_peaks Vector of peak vertex positions per sector (size must be 6).
 * @return std::tuple<double, double, double, TGraph*> with (A, φ_beam, Vz_0, graph with fit and legend).
 */
std::tuple<double, double, double, TGraph *> FitVertexVsPhi(std::string Particle, std::string SampleName, const std::vector<double> &Zrec_peaks, const std::vector<double> &phi_peaks = {}) {
    if (Zrec_peaks.size() != 6) { throw std::runtime_error("FitVertexVsPhi: expected 6 sector values (Zrec_peaks.size() != 6)"); }

    std::string sector_lable[6] = {"Sector 1", "Sector 2", "Sector 3", "Sector 4", "Sector 5", "Sector 6"};

    double phi_deg[6];

    if (phi_peaks.size() == 6) {
        // Use provided phi_peaks directly
        for (int i = 0; i < 6; ++i) { phi_deg[i] = phi_peaks[i]; }
    } else if (phi_peaks.size() == 0) {
        // Default phi offset values based on the sample name
        double phi_e_offset = (basic_tools::FindSubstring(SampleName, "2GeV"))   ? 16.0
                              : (basic_tools::FindSubstring(SampleName, "4GeV")) ? 7.0
                              : (basic_tools::FindSubstring(SampleName, "6GeV")) ? 5.0
                                                                                 : 0.0;

        // Default phi values for each sector, adjusted by the phi_e_offset
        double default_phi_deg[6] = {30 - phi_e_offset,    // Sector 1
                                     90 - phi_e_offset,    // Sector 2
                                     150 - phi_e_offset,   // Sector 3
                                     -150 - phi_e_offset,  // Sector 4
                                     -90 - phi_e_offset,   // Sector 5
                                     -30 - phi_e_offset};  // Sector 6

        for (int i = 0; i < 6; ++i) { phi_deg[i] = default_phi_deg[i]; }
    } else if (phi_peaks.size() != 6 && phi_peaks.size() != 0) {
        throw std::runtime_error("FitVertexVsPhi: expected 6 sector values (phi_peaks.size() != 6)");
    }

    double z_vals[6];

    for (int i = 0; i < 6; ++i) { z_vals[i] = Zrec_peaks[i]; }

    // Use phi_deg directly as x-values
    TGraph *g = new TGraph(6, phi_deg, z_vals);
    g->SetTitle(("Rec V_{z}^{" + Particle + "} peaks vs. #phi_{" + Particle + "};#phi_{" + Particle + "} [#circ];Rec V_{z}^{" + Particle + "} peaks [cm]").c_str());
    g->SetMarkerStyle(21);
    g->SetMarkerSize(1.5);
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();

    g->GetXaxis()->SetLimits(-180, 180);
    g->GetYaxis()->SetRangeUser(*std::min_element(Zrec_peaks.begin(), Zrec_peaks.end()) * 1.02, *std::max_element(Zrec_peaks.begin(), Zrec_peaks.end()) * 0.9);

    // if (basic_tools::FindSubstring(SampleName, "2GeV")) {
    //     if (basic_tools::FindSubstring(SampleName, "Ar40")) {
    //         g->GetYaxis()->SetRangeUser(-6.8, -5.9);
    //     }
    // } else if (basic_tools::FindSubstring(SampleName, "4GeV")) {
    //     if (basic_tools::FindSubstring(SampleName, "Ar40")) {
    //         g->GetYaxis()->SetRangeUser(-6.8, -5.9);
    //     }
    // } else if (basic_tools::FindSubstring(SampleName, "6GeV")) {
    //     g->GetXaxis()->SetRangeUser(-180, 180);
    // }

    // Fit function: argument in degrees, convert to radians in the formula
    TF1 *fitFunc = new TF1("fitFunc", "[0]*cos((x - [1]) * TMath::DegToRad()) + [2]", -180, 180);
    fitFunc->SetParNames("Amplitude", "Phi_beam", "Vz_0");
    fitFunc->SetLineWidth(3);
    fitFunc->SetLineColor(kRed);

    double maxZ = *std::max_element(Zrec_peaks.begin(), Zrec_peaks.end());
    double minZ = *std::min_element(Zrec_peaks.begin(), Zrec_peaks.end());
    double ampGuess = 0.5 * (maxZ - minZ);
    double meanGuess = 0.5 * (maxZ + minZ);

    fitFunc->SetParameters(ampGuess, 0.0, meanGuess);

    // double maxAmp = -6.0;
    // double minAmp = -6.55;
    // fitFunc->SetParLimits(0, 0.5 * (maxAmp - minAmp) * 0.9, 0.5 * (maxAmp - minAmp) * 1.1);

    // g->Fit(fitFunc);
    g->Fit(fitFunc, "F");

    double A = fitFunc->GetParameter(0);
    double phi_beam = fitFunc->GetParameter(1);
    double Vz_0 = fitFunc->GetParameter(2);

    phi_beam = fmod(phi_beam, 360.0);
    if (phi_beam < -180.0) {
        phi_beam += 360.0;
    } else if (phi_beam > 180.0) {
        phi_beam -= 360.0;
    }

    std::ostringstream legendText;
    legendText << "V_{z}^{" << Particle << "}(#phi_{" << Particle << "}) = A*cos(#phi_{" << Particle << "} - #phi_{beam}) + V_{z,0}";

    TLegend *legend = new TLegend(0.18, 0.81, 0.59, 0.88);
    legend->AddEntry(fitFunc, legendText.str().c_str(), "l");
    legend->SetTextSize(0.03);
    g->GetListOfFunctions()->Add(legend);

    TPaveText *FitParam1 = new TPaveText(0.18, 0.68, 0.35, 0.78, "NDC");
    TPaveText *FitParam2 = new TPaveText(0.35, 0.68, 0.51, 0.78, "NDC");

    for (auto *box : {FitParam1, FitParam2}) {
        box->SetBorderSize(1);
        box->SetFillColor(0);
        box->SetTextAlign(12);
        box->SetTextFont(42);
        box->SetTextSize(0.03);
    }

    FitParam1->AddText(("Fit #chi^{2} = " + basic_tools::ToStringWithPrecision(fitFunc->GetChisquare())).c_str());
    FitParam1->AddText(("A = " + basic_tools::ToStringWithPrecision(A) + " cm").c_str());

    FitParam2->AddText(("#phi_{beam} = " + basic_tools::ToStringWithPrecision(phi_beam) + "#circ").c_str());
    FitParam2->AddText(("V_{z,0} = " + basic_tools::ToStringWithPrecision(Vz_0) + " cm").c_str());

    g->GetListOfFunctions()->Add(FitParam1);
    g->GetListOfFunctions()->Add(FitParam2);
    g->GetListOfFunctions()->Add(fitFunc);

    std::cout << std::endl;

    for (int i = 0; i < 6; ++i) {
        double x = phi_deg[i];
        double y = z_vals[i];

        std::ostringstream label;
        label << "#splitline{#font[62]{" << sector_lable[i] << "}}{(" << basic_tools::ToStringWithPrecision(x) << "#circ, " << basic_tools::ToStringWithPrecision(y) << " cm)}";
        std::cout << label.str() << std::endl;

        TLatex *latex = new TLatex(x + 2 - 10, y + 0.075, label.str().c_str());  // offset to avoid overlap
        latex->SetTextFont(42);
        latex->SetTextSize(0.025);
        latex->SetTextAlign(12);
        g->GetListOfFunctions()->Add(latex);
    }

    std::cout << std::endl;

    return std::make_tuple(A, phi_beam, Vz_0, g);
}

};  // namespace variable_correctors

#endif  // VARIABLE_CORRECTORS_H
