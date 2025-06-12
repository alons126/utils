//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef ANALYSIS_MATH_H
#define ANALYSIS_MATH_H

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
#include "poly_solver.cpp"

namespace analysis_math {
using namespace poly_solver;

// Mathematical constants -----------------------------------------------------------------------------------------------------------------------------------------------

const double pi = M_PI;

// RadToDeg function ----------------------------------------------------------------------------------------------------------------------------------------------------

double RadToDeg(const double &rad) { return rad * 180. / pi; }

// DegToRad function ----------------------------------------------------------------------------------------------------------------------------------------------------

double DegToRad(const double &deg) { return deg * pi / 180.; }

// CalcTheta_rad function -----------------------------------------------------------------------------------------------------------------------------------------------

double CalcTheta_rad(const double &x, const double &y, const double &z) { return acos(z / sqrt(x * x + y * y + z * z)); }

// CalcTheta_deg function -----------------------------------------------------------------------------------------------------------------------------------------------

double CalcTheta_deg(const double &x, const double &y, const double &z) { return RadToDeg(CalcTheta_rad(x, y, z)); }

// CalcPhi_rad function -------------------------------------------------------------------------------------------------------------------------------------------------

double CalcPhi_rad(const double &x, const double &y) { return atan2(y, x); }

// CalcPhi_deg function -------------------------------------------------------------------------------------------------------------------------------------------------

double CalcPhi_deg(const double &x, const double &y, const double &z) { return RadToDeg(CalcPhi_rad(x, y)); }

// RadCalc function ------------------------------------------------------------------------------------------------------------------------------------------------------

inline double RadCalc(double x, double y, double z) { return sqrt(x * x + y * y + z * z); }

// GetPi0MomTh function -------------------------------------------------------------------------------------------------------------------------------------------------

double GetPi0MomTh(const double &ph_mom_th) {
    if (std::abs(ph_mom_th) == 9999) { return -9999; }
    double pi0_mom_th = std::sqrt(4 * ph_mom_th * ph_mom_th - constants::m_pizero * constants::m_pizero);
    return pi0_mom_th;
}

// CalcdTheta function --------------------------------------------------------------------------------------------------------------------------------------------------

double CalcdTheta(double dThetaTemp) { return std::abs(dThetaTemp); }

// CalcdPhi function ----------------------------------------------------------------------------------------------------------------------------------------------------

double CalcdPhi1(double dPhiTemp) {
    double dPhi = (dPhiTemp > 180.0) ? (dPhiTemp - 360.0) : (dPhiTemp < -180.0) ? (dPhiTemp + 360.0) : dPhiTemp;

    // if (dPhiTemp > 180.0) {
    //     dPhi = dPhiTemp - 360.0;
    // } else if (dPhiTemp < -180.0) {
    //     dPhi = dPhiTemp + 360.0;
    // } else {
    //     dPhi = dPhiTemp;
    // }

    return dPhi;
}

// GetPhi_e function ----------------------------------------------------------------------------------------------------------------------------------------------------

double GetPhi_e(TString OutPutFolder, double phi_N) {
    double phi_e_offset = 0.;  // Electron phi_e offset due to the solenoid field

    std::string OutPutFolder0(OutPutFolder.Data());

    if (basic_tools::FindSubstring(OutPutFolder0, "2070MeV")) {
        phi_e_offset = 16.;
    } else if (basic_tools::FindSubstring(OutPutFolder0, "4029MeV")) {
        phi_e_offset = 7.;
    } else if (basic_tools::FindSubstring(OutPutFolder0, "5986MeV")) {
        phi_e_offset = 5.;
    }

    std::vector<double> possible_angles = {-120, -60, 0, 60, 120, 180};

    // Calculate the target angle (180 degrees away from phi_N)
    double target_angle = CalcdPhi1(phi_N + 180);

    // Find the closest possible angle
    double closest_angle = possible_angles[0];
    double min_diff = std::abs(CalcdPhi1(target_angle - closest_angle));

    for (double angle : possible_angles) {
        double diff = std::abs(CalcdPhi1(target_angle - angle));
        if (diff < min_diff) {
            min_diff = diff;
            closest_angle = angle;
        }
    }

    return closest_angle + phi_e_offset;
    // return closest_angle;
}

// GetBinFromAng function -----------------------------------------------------------------------------------------------------------------------------------------------

int GetBinFromAng(double Angle, double AngleBins, double AngleMin, double AngleMax, bool printOut = false, const std::string &AngleType = "") {
    int Bin = 0;
    //    int Bin = -1;

    double Delta = (AngleMax - AngleMin) / AngleBins, min, max;

    for (int i = 0; i < AngleBins; i++) {
        double min_i, max_i;

        min_i = AngleMin + i * Delta, max_i = min_i + Delta;
        min = min_i, max = max_i;

        if (Angle >= min_i && Angle <= max_i) {
            if (printOut) {
                cout << "\n\n\nAngleType = " << AngleType << "\n";
                cout << "Angle = " << Angle << "\n";
                cout << "max_i = " << max_i << "\n";
                cout << "min_i = " << min_i << "\n";
                cout << "i = " << i << "\n";
                cout << "Delta = " << Delta << "\n\n\n";
            }

            Bin = i;
            break;
        }
    }  // end of loop over electrons vector

    return Bin;
}

// CalcdPhi function (CLAS12 extention) ---------------------------------------------------------------------------------------------------------------------------------

double CalcdPhi2(region_part_ptr proton1, region_part_ptr proton2) {
    if (proton1->getRegion() == proton2->getRegion()) {
        std::cerr << "\n\nCalcdPhi2: protons are in the same region (" << proton1->getRegion() << " & " << proton2->getRegion() << ")! Aborting...\n";
        exit(1);
    }

    double Phi_pFD_deg = 0.0, Phi_pCD_deg = 0.0;

    if (proton1->getRegion() == FD && proton2->getRegion() == CD) {
        Phi_pFD_deg = RadToDeg(proton1->getPhi());
        Phi_pCD_deg = RadToDeg(proton2->getPhi());
    } else if (proton1->getRegion() == CD && proton2->getRegion() == FD) {
        Phi_pFD_deg = RadToDeg(proton2->getPhi());
        Phi_pCD_deg = RadToDeg(proton1->getPhi());
    }

    return CalcdPhi1(Phi_pFD_deg - Phi_pCD_deg);
}

// TLKinCutsCheck function (CLAS12 extention) ---------------------------------------------------------------------------------------------------------------------------

/* TLKinCutsCheck for a general vector of particles */
bool TLKinCutsCheck(const std::unique_ptr<clas12::clas12reader> &c12, bool apply_kinematical_cuts, const vector<int> &FD_nucleon, const DSCuts &FD_nucleon_theta_cut,
                    const DSCuts &FD_nucleon_momentum_cut) {
    auto mcpbank = c12->mcparts();

    if (!apply_kinematical_cuts) {
        return true;
    } else {
        for (int i : FD_nucleon) {
            mcpbank->setEntry(i);

            int particlePDGtmp = mcpbank->getPid();

            double Particle_TL_Momentum = analysis_math::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
            double Particle_TL_Theta = acos((mcpbank->getPz()) / analysis_math::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
            double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

            bool Pass_FD_nucleon_theta_cuts = ((Particle_TL_Theta >= FD_nucleon_theta_cut.GetLowerCutConst()) && (Particle_TL_Theta <= FD_nucleon_theta_cut.GetUpperCutConst()));
            bool Pass_FD_nucleon_momentum_cuts =
                ((Particle_TL_Momentum >= FD_nucleon_momentum_cut.GetLowerCutConst()) && (Particle_TL_Momentum <= FD_nucleon_momentum_cut.GetUpperCutConst()));

            if (!(Pass_FD_nucleon_theta_cuts && Pass_FD_nucleon_momentum_cuts)) { return false; }
        }

        return true;
    }
}

// TLKinCutsCheck function (CLAS12 extention) ---------------------------------------------------------------------------------------------------------------------------

/* TLKinCutsCheck for leading FD neutrons */
bool TLKinCutsCheck(const std::unique_ptr<clas12::clas12reader> &c12, bool apply_kinematical_cuts, const int TL_IDed_neutrons_FD_mom_max, const DSCuts &FD_nucleon_theta_cut,
                    const DSCuts &FD_nucleon_momentum_cut) {
    auto mcpbank = c12->mcparts();

    if (!apply_kinematical_cuts) {
        return true;
    } else {
        if (TL_IDed_neutrons_FD_mom_max == -1) {
            return false;
        } else {
            mcpbank->setEntry(TL_IDed_neutrons_FD_mom_max);

            double Particle_TL_Momentum = analysis_math::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
            double Particle_TL_Theta = acos((mcpbank->getPz()) / analysis_math::RadCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
            double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

            bool Pass_FD_nucleon_momentum_cuts =
                ((Particle_TL_Momentum >= FD_nucleon_momentum_cut.GetLowerCutConst()) && (Particle_TL_Momentum <= FD_nucleon_momentum_cut.GetUpperCutConst()));
            bool Pass_FD_nucleon_theta_cuts = ((Particle_TL_Theta >= FD_nucleon_theta_cut.GetLowerCutConst()) && (Particle_TL_Theta <= FD_nucleon_theta_cut.GetUpperCutConst()));

            if (Pass_FD_nucleon_theta_cuts && Pass_FD_nucleon_momentum_cuts) {
                return true;
            } else {
                return false;
            }
        }
    }
}

// FitVertexVsPhi function ----------------------------------------------------------------------------------------------------------------------------------------------

/**
 * @brief Fits the average reconstructed vertex positions from 6 CLAS12 sectors
 *        to a sinusoidal function to extract the beam offset direction and magnitude.
 *
 * Motivation:
 * In CLAS12, if the beam is transversely offset relative to the nominal target axis,
 * the reconstructed z-vertex depends on the azimuthal angle of the detected particle.
 * This introduces a sector-dependent bias in Z_rec that follows a cosine dependence.
 * By fitting the peak Z_rec values across the 6 sectors to the form:
 *     Z_rec(φ) = A * cos(φ - φ_beam) + Z0
 * one can extract:
 *   - A        : amplitude ∝ beam offset magnitude
 *   - φ_beam   : direction of the beam offset
 *   - Z0       : average z-position of the target center
 *
 * The function returns a tuple with (Amplitude A, Beam direction φ_beam [rad], Z0, and TGraph* for visualization).
 *
 * @param Zrec_peaks Vector of peak vertex positions per sector (size must be 6).
 * @return std::tuple<double, double, double, TGraph*> with (A, φ_beam, Z0, graph with fit and legend).
 */
std::tuple<double, double, double, TGraph *> FitVertexVsPhi(const std::vector<double> &Zrec_peaks, std::string Particle) {
    if (Zrec_peaks.size() != 6) { throw std::runtime_error("FitVertexVsPhi: expected 6 sector values (Zrec_peaks.size() != 6)"); }

    double phi_deg[6] = {30, 90, 150, 210, 270, 330};
    double phi_rad[6];
    double z_vals[6];

    for (int i = 0; i < 6; ++i) {
        phi_rad[i] = phi_deg[i] * TMath::DegToRad();
        z_vals[i] = Zrec_peaks[i];
    }

    TGraph *g = new TGraph(6, phi_deg, z_vals);
    // TGraph *g = new TGraph(6, phi_rad, z_vals);
    g->SetTitle(("Average Z^{" + Particle + "}_{rec} vs. #phi_{" + Particle + "};#phi_{" + Particle + "}[#circ];Average Z^{" + Particle + "}_{rec} [cm]").c_str());
    g->SetMarkerStyle(20);
    g->SetMarkerSize(1.2);
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();

    TF1 *fitFunc = new TF1("fitFunc", "[0]*cos(x - [1]) + [2]", 0, 2 * TMath::Pi());
    fitFunc->SetParNames("Amplitude", "Phi_beam", "Z0");

    double maxZ = *std::max_element(Zrec_peaks.begin(), Zrec_peaks.end());
    double minZ = *std::min_element(Zrec_peaks.begin(), Zrec_peaks.end());
    double ampGuess = 0.5 * (maxZ - minZ);
    double meanGuess = 0.5 * (maxZ + minZ);

    fitFunc->SetParameters(ampGuess, 0.0, meanGuess);

    g->Fit(fitFunc, "Q");

    double A = fitFunc->GetParameter(0);
    double phi_beam = fitFunc->GetParameter(1);
    double Z0 = fitFunc->GetParameter(2);

    g->GetListOfFunctions()->Add(fitFunc);

    std::ostringstream legendText;
    legendText << "f(#phi) = " << std::fixed << std::setprecision(3) << A << " cos(#phi - " << phi_beam << ") + " << Z0;

    TLegend *legend = new TLegend(0.15, 0.8, 0.55, 0.88);
    legend->AddEntry(fitFunc, legendText.str().c_str(), "l");
    g->GetListOfFunctions()->Add(legend);

    return std::make_tuple(A, phi_beam, Z0, g);
}

};  // namespace analysis_math

#endif  // ANALYSIS_MATH_H
