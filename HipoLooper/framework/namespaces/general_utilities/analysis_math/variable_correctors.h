//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef VARIABLE_CORRECTORS_H
#define VARIABLE_CORRECTORS_H

#include <TF1.h>
#include <TGraph.h>
#include <TMath.h>

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
std::tuple<double, double, double, TGraph *> FitVertexVsPhi(const std::vector<double> &Zrec_peaks, std::string Particle, std::string Ebeam_status) {
    if (Zrec_peaks.size() != 6) { throw std::runtime_error("FitVertexVsPhi: expected 6 sector values (Zrec_peaks.size() != 6)"); }

    double phi_e_offset = 0;

    if (basic_tools::FindSubstring(Ebeam_status, "2GeV")) {
        phi_e_offset = 16.;
    } else if (basic_tools::FindSubstring(Ebeam_status, "4GeV")) {
        phi_e_offset = 7.;
    } else if (basic_tools::FindSubstring(Ebeam_status, "6GeV")) {
        phi_e_offset = 5.;
    }

    double phi_deg[6] = {-150 - phi_e_offset, -90 - phi_e_offset, -30 - phi_e_offset, 30 - phi_e_offset, 90 - phi_e_offset, 150 - phi_e_offset};
    double z_vals[6];

    for (int i = 0; i < 6; ++i) { z_vals[i] = Zrec_peaks[i]; }

    // Use phi_deg directly as x-values
    TGraph *g = new TGraph(6, phi_deg, z_vals);
    g->GetXaxis()->SetLimits(-180, 180);
    g->GetYaxis()->SetRangeUser(-7, -5.5);
    // g->GetYaxis()->SetLimits(-180, (*std::max_element(Zrec_peaks.begin(), Zrec_peaks.end())) * 1.2);
    g->SetTitle(("Rec V_{z}^{" + Particle + "} peaks vs. #phi_{" + Particle + "};#phi_{" + Particle + "}[#circ];Rec V_{z}^{" + Particle + "} peaks [cm]").c_str());
    g->SetMarkerStyle(21);
    g->SetMarkerSize(1.5);
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();

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

    double maxAmp = -5.9;
    double minAmp = -6.6;
    fitFunc->SetParLimits(0, 0.5 * (maxAmp - minAmp) * 0.9, 0.5 * (maxAmp - minAmp) * 1.1);

    g->Fit(fitFunc);
    // g->Fit(fitFunc, "Q");

    double A = fitFunc->GetParameter(0);
    double phi_beam = fitFunc->GetParameter(1);
    double Vz_0 = fitFunc->GetParameter(2);

    std::ostringstream legendText;
    legendText << "V_{z}^{" << Particle << "}(#phi_{" << Particle << "}) = A*cos(#phi_{" << Particle << "} - #phi_{beam}) + V_{z,0}";

    TLegend *legend = new TLegend(0.18, 0.8, 0.55, 0.88);
    // TLegend *legend = new TLegend(0.18, 0.8, 0.6, 0.88);
    legend->AddEntry(fitFunc, legendText.str().c_str(), "l");
    legend->SetTextSize(0.03);
    g->GetListOfFunctions()->Add(legend);

    TPaveText *FitParam1 = new TPaveText(0.18, 0.68, 0.34, 0.78, "NDC");
    TPaveText *FitParam2 = new TPaveText(0.34, 0.68, 0.50, 0.78, "NDC");

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

    return std::make_tuple(A, phi_beam, Vz_0, g);
}

};  // namespace variable_correctors

#endif  // VARIABLE_CORRECTORS_H
