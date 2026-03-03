//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef VARIABLE_CORRECTORS_H
#define VARIABLE_CORRECTORS_H

#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
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

namespace bt = basic_tools;

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
 *     Z_rec(φ) = Vz_true - A * cos(φ - φ_beam)
 * one can extract:
 *   - A             : amplitude ∝ beam offset magnitude
 *   - φ_beam        : direction of the beam offset
 *   - Vz_true       : average z-position of the target center
 *
 * Optionally, if a theta slice is provided, the fit is performed using:
 *     Z_rec(φ) = Vz_true - (r / tan(mean_theta)) * cos(φ - φ_beam)
 * which reflects the relation A = r / tan(theta) for transversely offset beams.
 *
 * The function returns a tuple with (Amplitude A or offset r, Beam direction φ_beam [deg],
 * Vz_true, and TGraphErrors* for visualization).
 *
 * @param Zrec_peaks   Vector of peak vertex positions per sector (size must be 6).
 * @param phi_peaks    Optional phi centers of the sectors [deg]. If omitted, estimated from SampleName.
 * @param theta_slice  Optional pair of theta limits in degrees (first < second). Used to fit with A = r / tan(theta).
 * @return std::tuple<double, double, double, TGraphErrors*> with (A or r, φ_beam, Vz_true, graph with fit and legend).
 */
std::tuple<double, double, double, TGraph *> FitVertexVsPhi(std::string Particle, std::string SampleName, const std::vector<double> &Zrec_peaks, const std::vector<double> &phi_peaks = {},
                                                            const std::pair<double, double> &theta_slice = {-1, -1}) {
    if (Zrec_peaks.size() != 6) { throw std::runtime_error("FitVertexVsPhi: expected 6 sector values (Zrec_peaks.size() != 6)"); }

    std::string sector_label[6] = {"Sector 1", "Sector 2", "Sector 3", "Sector 4", "Sector 5", "Sector 6"};

    double phi_deg[6];

    if (phi_peaks.size() == 6) {
        // Use provided phi_peaks directly
        for (int i = 0; i < 6; ++i) { phi_deg[i] = phi_peaks[i]; }
    } else if (phi_peaks.size() == 0) {
        // Default phi offset values based on the sample name
        double phi_e_offset = (bt::FindSubstring(SampleName, "2GeV")) ? 16.0 : (bt::FindSubstring(SampleName, "4GeV")) ? 7.0 : (bt::FindSubstring(SampleName, "6GeV")) ? 5.0 : 0.0;

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

    double minZ = *std::min_element(Zrec_peaks.begin(), Zrec_peaks.end());
    double maxZ = *std::max_element(Zrec_peaks.begin(), Zrec_peaks.end());

    bool useThetaSlice = (theta_slice.first > 0.0 && theta_slice.second > 0.0 && theta_slice.second > theta_slice.first);

    double mean_theta = 0.0;
    double mean_theta_rad = 0.0;
    if (useThetaSlice) {
        mean_theta = 0.5 * (theta_slice.first + theta_slice.second);  // mean in degrees
        // mean_theta is provided in degrees; convert to radians for trigonometric use
        mean_theta_rad = mean_theta * TMath::DegToRad();
    }

    // Use phi_deg directly as x-values
    TGraph *g = new TGraph(6, phi_deg, z_vals);

    if (useThetaSlice) {
        g->SetTitle(("V_{z,rec}^{" + Particle + "} peaks vs. #phi_{" + Particle + "} peaks for " + bt::ToStringWithPrecision(theta_slice.first) + "#circ #leq #theta_{" + Particle +
                     "} #leq " + bt::ToStringWithPrecision(theta_slice.second) + "#circ;#phi_{" + Particle + "} [#circ];V_{z,rec}^{" + Particle + "} peaks [cm]")
                        .c_str());
    } else {
        g->SetTitle(("V_{z,rec}^{" + Particle + "} peaks vs. #phi_{" + Particle + "} peaks;#phi_{" + Particle + "} peaks [#circ];V_{z,rec}^{" + Particle + "} peaks [cm]").c_str());
    }

    g->SetMarkerStyle(21);
    g->SetMarkerSize(1.5);
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();

    g->GetXaxis()->SetLimits(-180, 180);

    // double margin = 0.5 * std::abs(maxZ - minZ);
    double margin = 0.1 * std::abs(maxZ - minZ);
    g->GetYaxis()->SetRangeUser(minZ - margin, maxZ + 10 * margin);
    // g->GetYaxis()->SetRangeUser(minZ - margin, maxZ + margin);

    // if (bt::FindSubstring(SampleName, "2GeV")) {
    //     if (bt::FindSubstring(SampleName, "Ar40")) {
    //         g->GetYaxis()->SetRangeUser(-6.8, -5.9);
    //     }
    // } else if (bt::FindSubstring(SampleName, "4GeV")) {
    //     if (bt::FindSubstring(SampleName, "Ar40")) {
    //         g->GetYaxis()->SetRangeUser(-6.8, -5.9);
    //     }
    // } else if (bt::FindSubstring(SampleName, "6GeV")) {
    //     g->GetXaxis()->SetRangeUser(-180, 180);
    // }

    // Fit function: argument in degrees, convert to radians in the formula
    TF1 *fitFunc = nullptr;

    if (!useThetaSlice) {
        fitFunc = new TF1("fitFunc", "(-[0]) * cos((x - [1]) * TMath::DegToRad()) + [2]", -180, 180);
        fitFunc->SetParNames("Amplitude", "Phi_beam", "Vz_true");
    } else {
        // mean_theta is provided in degrees; convert to radians for trigonometric use
        fitFunc = new TF1("fitFunc", ([=](double *x, double *par) { return (-par[0] / tan(mean_theta_rad)) * cos((x[0] - par[1]) * TMath::DegToRad()) + par[2]; }), -180, 180, 3);
        fitFunc->SetParNames("r", "Phi_beam", "Vz_true");
    }

    double ampGuess = 0.5 * (maxZ - minZ);
    double meanGuess = 0.5 * (maxZ + minZ);
    fitFunc->SetParameters(ampGuess, 0.0, meanGuess);

    fitFunc->SetParLimits(0, 0, 9999);  // Set amplitude (or r) limits to be non-negative

    // double minPhi_beam = 30;
    // double maxPhi_beam = 45;
    // fitFunc->SetParLimits(1, minPhi_beam, maxPhi_beam);

    // double minVz_true = -5.6;
    // double maxVz_true = -5.4;
    // fitFunc->SetParLimits(2, minVz_true, maxVz_true);

    g->Fit(fitFunc);
    // g->Fit(fitFunc, "F");

    double A = fitFunc->GetParameter(0);
    double phi_beam = fitFunc->GetParameter(1);
    double Vz_true = fitFunc->GetParameter(2);

    if (useThetaSlice) {
        A = A / tan(mean_theta_rad);  // convert back from r to amplitude
    }

    // Converts phi_beam into range [-180^\circ, 180^\circ]
    phi_beam = fmod(phi_beam, 360.0);
    if (phi_beam < -180.0) {
        phi_beam += 360.0;
    } else if (phi_beam > 180.0) {
        phi_beam -= 360.0;
    }

    std::ostringstream legendText;

    if (useThetaSlice) {
        legendText << "V_{z,rec}^{" << Particle << "}(#phi_{" << Particle << "}) = V_{z,true}^{" << Particle << "} - #left[#frac{r #times cos(#phi_{" << Particle
                   << "} - #phi_{beam})}{tan(#theta_{" << Particle << "}^{average})}#right]_{#theta_{" << Particle << "}^{average} = " << bt::ToStringWithPrecision(mean_theta) << "#circ}";
    } else {
        legendText << "V_{z,rec}^{" << Particle << "}(#phi_{" << Particle << "}) = V_{z,true}^{" << Particle << "} - A*cos(#phi_{" << Particle << "} - #phi_{beam})";
    }

    TLegend *legend = new TLegend(0.18, 0.77, 0.74, 0.88);
    legend->AddEntry(fitFunc, legendText.str().c_str(), "l");
    legend->SetTextAlign(12);
    legend->SetTextSize(0.025);
    g->GetListOfFunctions()->Add(legend);

    TPaveText *FitParam1 = new TPaveText(0.18, 0.65, 0.40, 0.75, "NDC");
    TPaveText *FitParam2 = new TPaveText(0.40, 0.65, 0.60, 0.75, "NDC");

    for (auto *box : {FitParam1, FitParam2}) {
        box->SetBorderSize(1);
        box->SetFillColor(0);
        box->SetTextAlign(12);
        box->SetTextFont(42);
        box->SetTextSize(0.025);
    }

    double chi2_ndf = fitFunc->GetChisquare() / fitFunc->GetNDF();
    FitParam1->AddText(("#chi^{2}/NDF = " + bt::ToStringWithPrecision(chi2_ndf, 3)).c_str());

    if (useThetaSlice) {
        FitParam1->AddText(("r = " + bt::ToStringWithPrecision(A) + " cm").c_str());
    } else {
        FitParam1->AddText(("A = " + bt::ToStringWithPrecision(A) + " cm").c_str());
    }

    FitParam2->AddText(("#phi_{beam} = " + bt::ToStringWithPrecision(phi_beam) + "#circ (#in [-180#circ, 180#circ])").c_str());
    FitParam2->AddText(("V_{z,true}^{" + Particle + "} = " + bt::ToStringWithPrecision(Vz_true) + " cm").c_str());

    g->GetListOfFunctions()->Add(FitParam1);
    g->GetListOfFunctions()->Add(FitParam2);
    g->GetListOfFunctions()->Add(fitFunc);

    std::cout << std::endl;

    for (int i = 0; i < 6; ++i) {
        double x = phi_deg[i];
        double y = z_vals[i];

        std::ostringstream label;
        label << "#splitline{#font[62]{" << sector_label[i] << "}}{(" << bt::ToStringWithPrecision(x) << "#circ, " << bt::ToStringWithPrecision(y) << " cm)}";
        std::cout << label.str() << std::endl;

        TLatex *latex = new TLatex(x + 3, y + 0.075, label.str().c_str());  // offset to avoid overlap
        latex->SetTextFont(42);
        latex->SetTextSize(0.025);
        latex->SetTextAlign(12);
        g->GetListOfFunctions()->Add(latex);
    }

    std::cout << std::endl;

    return std::make_tuple(A, phi_beam, Vz_true, g);
}

};  // namespace variable_correctors

#endif  // VARIABLE_CORRECTORS_H
