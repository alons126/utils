//
// Created by Alon Sportes on 16/03/2025.
//

#ifndef RECO_ANALYSIS_FUNCTIONS_H
#define RECO_ANALYSIS_FUNCTIONS_H

#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TString.h>
#include <TStyle.h>
#include <TTree.h>
#include <TVector3.h>

#include <cmath>
#include <iostream>
#include <tuple>
#include <vector>

#include "analysis_math/analysis_math.h"
#include "constants.h"
#include "lists.h"

// #include "HipoChain.h"
// #include "clas12reader.h"

#include "../../includes/clas12_include.h"

// using namespace clas12;
namespace am = analysis_math;

namespace reco_analysis_functions {

// ======================================================================================================================================================================
// General utilities for reconstruction-level analysis
// ======================================================================================================================================================================

// GetFDNeutronP function -----------------------------------------------------------------------------------------------------------------------------------------------

/* This is the old function used to calculate and obtain FD neutron momentum */
double GetFDNeutronP(clas12::region_part_ptr& Neutron, bool apply_nucleon_cuts) {
    double Momentum;

    if (apply_nucleon_cuts) {
#pragma region /* Get for ver. neutrons & calculate for 'photons' */
        int ParticlePDG = Neutron->par()->getPid();

        if (ParticlePDG == 2112) {
            /* Momentum for neutrons - get from file. */
            Momentum = Neutron->getP();

            //        #pragma region /* how neutron momentum is calculated in clas12 */
            //        TVector3 P_n;
            //        P_n.SetMagThetaPhi(Neutron->getP(), Neutron->getTheta(), Neutron->getPhi());
            //        double E_n = sqrt( constants::m_n *  constants::m_n + P_n.Mag2());
            //        double Path_n = Neutron->getPath();
            //
            //        /* NOTE: Beta = Beta_from_Momentum */
            //        double Beta_n = Neutron->par()->getBeta();
            //        double Beta_n_from_Momentum = P_n.Mag() / E_n;
            //
            //        /* NOTE: Time_from_Momentum = Time_from_Beta_n */
            //        double Time_from_Momentum = Path_n / (c * Beta_n_from_Momentum);
            //        double Time_from_Beta_n = Path_n / (c * Beta_n);
            //
            //        double Velocity_n = Path_n / Time_from_Beta_n;
            //        double Gamma_n = 1 / sqrt(1 - (Velocity_n * Velocity_n) / (constants::c * constants::c));
            //        double Momentum_n =  constants::m_n * Beta_n * Gamma_n; // how neutron momentum is calculated in clas12
            //        #pragma endregion

        } else if (ParticlePDG == 22) {
            /* Momentum for "photons" - calculate. */

            /* NOTE: all of these give the same path:
             * Taking dv_nhit = (Neutron->cal(detlayer)->getX() - Neutron->par()->getVx(), X <-> Y,  X <-> Z) and dv_nhit.Mag().
             * Neutron->cal(detlayer)->getPath().
             * Neutron->getPath(). */

            /* NOTE: Neutron->cal(detlayer)->getTime() = Neutron->getTime() */

            double Path_ph = Neutron->getPath();
            double Time_ph = Neutron->getTime();  // bad
                                                  //        double Velocity = Path_ph / Time_ph;
            double Beta_ph = Neutron->par()->getBeta();
            double Time_ph_from_Beta_ph = Path_ph / (constants::c * Beta_ph);
            double Velocity_ph = Path_ph / Time_ph_from_Beta_ph;
            //        double Gamma_ph = 1 / sqrt(1 - (Velocity_ph * Velocity_ph) / (constants::c * constants::c));
            double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));

            Momentum = constants::m_n * Beta_ph * Gamma_ph;
        }
#pragma endregion

    } else {
#pragma region /* Get momentum for both neutrons and photons from file */
        Momentum = Neutron->par()->getP();
#pragma endregion
    }

    return Momentum;
}

// CheckForNeutralFDECALHits function -----------------------------------------------------------------------------------------------------------------------------------

void CheckForNeutralFDECALHits(bool& ParticleInPCAL, bool& ParticleInECIN, bool& ParticleInECOUT, short& NeutralFD_ECAL_detlayer, clas12::region_part_ptr& NeutralFD) {
    /*
     * check where did the particle hit.
     * no hit - we'll get 0
     * we have a hit - we'll get a 7 (7 is the ID of the calorimeter).
     * Can also be done by checking deposited energy (double comparison), yet this method is better (int comparison)
     */
    ParticleInPCAL = (NeutralFD->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
    ParticleInECIN = (NeutralFD->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
    ParticleInECOUT = (NeutralFD->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

    // Determine first layer of hit
    NeutralFD_ECAL_detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;  // find first layer of hit
}

// CheckForECALHits functions -------------------------------------------------------------------------------------------------------------------------------------------

void CheckForECALHits(bool& ParticleInPCAL, bool& ParticleInECIN, bool& ParticleInECOUT, short& NeutralFD_ECAL_detlayer, vector<clas12::region_part_ptr>& allParticles, const int& i) {
    CheckForNeutralFDECALHits(ParticleInPCAL, ParticleInECIN, ParticleInECOUT, NeutralFD_ECAL_detlayer, allParticles[i]);
}

std::tuple<bool, bool, bool, short> CheckForECALHits(clas12::region_part_ptr ParticleFD) {
    bool ParticleInPCAL = (ParticleFD->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
    bool ParticleInECIN = (ParticleFD->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
    bool ParticleInECOUT = (ParticleFD->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

    // Determine first layer of hit
    short ParticleFD_ECAL_detlayer = ParticleInPCAL ? clas12::PCAL : ParticleInECIN ? clas12::ECIN : clas12::ECOUT;

    return {ParticleInPCAL, ParticleInECIN, ParticleInECOUT, ParticleFD_ECAL_detlayer};
}

// CheckForFTOFHits functions -------------------------------------------------------------------------------------------------------------------------------------------

std::tuple<bool, bool, bool> CheckForFTOFHits(clas12::region_part_ptr ParticleFD) {
    bool ParticleInFTOF1A = (ParticleFD->cal(clas12::FTOF1A)->getDetector() == 12);  // FTOF1A hit
    bool ParticleInFTOF1B = (ParticleFD->cal(clas12::FTOF1B)->getDetector() == 12);  // FTOF1B hit
    bool ParticleInFTOF2 = (ParticleFD->cal(clas12::FTOF2)->getDetector() == 12);    // FTOF2 hit

    return {ParticleInFTOF1A, ParticleInFTOF1B, ParticleInFTOF2};
}

// CalcPathnFD function -------------------------------------------------------------------------------------------------------------------------------------------------

double CalcPathnFD(clas12::region_part_ptr NeutronFD, clas12::region_part_ptr electron, const bool& Recalc_Path_nFD = true) {
    double reco_Path_nFD;

    auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, detlayer] = CheckForECALHits(NeutronFD);

    if (Recalc_Path_nFD) {
        TVector3 v_nvtx_3v;  // Neutron's vertex location
        v_nvtx_3v.SetXYZ(electron->par()->getVx(), electron->par()->getVy(), electron->par()->getVz());
        // v_nvtx_3v.SetXYZ(NeutronFD->par()->getVx(), NeutronFD->par()->getVy(), NeutronFD->par()->getVz());

        TVector3 v_hit_3v;  // Neutron's hit location in CND
        v_hit_3v.SetXYZ(NeutronFD->cal(detlayer)->getX(), NeutronFD->cal(detlayer)->getY(), NeutronFD->cal(detlayer)->getZ());

        TVector3 v_path_3v = v_hit_3v - v_nvtx_3v;  // Direct calculation of neutron's path (in vector form)

        reco_Path_nFD = v_path_3v.Mag();
    } else {
        reco_Path_nFD = NeutronFD->getPath();
    }

    return reco_Path_nFD;
}

// CalcToFnFD function --------------------------------------------------------------------------------------------------------------------------------------------------

double CalcToFnFD(clas12::region_part_ptr NeutronFD, double starttime = 9999, const bool& Recalc_ToF_nFD = true) {
    double reco_ToF_nFD;

    auto [ParticleInPCAL, ParticleInECIN, ParticleInECOUT, detlayer] = CheckForECALHits(NeutronFD);

    if (Recalc_ToF_nFD) {
        reco_ToF_nFD = NeutronFD->cal(detlayer)->getTime() - starttime;
    } else {
        reco_ToF_nFD = NeutronFD->getTime();
    }

    return reco_ToF_nFD;
}

// CalcPnFD function ----------------------------------------------------------------------------------------------------------------------------------------------------

double CalcPnFD(clas12::region_part_ptr NeutronFD, clas12::region_part_ptr electron, double starttime = 9999, const bool& Recalc_P_nFD = true) {
    double Momentum;

    // if (Recalc_P_nFD) {
    if (Recalc_P_nFD && (NeutronFD->par()->getPid() == 22)) {
        double reco_Path_nFD = CalcPathnFD(NeutronFD, electron, Recalc_P_nFD);
        double reco_ToF_nFD = CalcToFnFD(NeutronFD, starttime, Recalc_P_nFD);
        double reco_Beta_nFD = Recalc_P_nFD ? reco_Path_nFD / (reco_ToF_nFD * constants::c) : NeutronFD->par()->getBeta();
        double reco_Gamma_nFD = 1 / sqrt(1 - (reco_Beta_nFD * reco_Beta_nFD));

        Momentum = constants::m_n * reco_Beta_nFD * reco_Gamma_nFD;
    } else {
        Momentum = NeutronFD->getP();
    }

    return Momentum;
}

// checkEcalDiagCuts function -------------------------------------------------------------------------------------------------------------------------------------------

bool checkEcalDiagCuts(clas12::region_part_ptr electrons) {
    double ecal_diag_cut = 0.2;  // diagonal cut on SF

    double mom = electrons->par()->getP();
    // true if inside cut
    if (electrons->par()->getPid() == 11) {
        if ((electrons->cal(clas12::PCAL)->getEnergy() + electrons->cal(clas12::ECIN)->getEnergy()) / mom > ecal_diag_cut && mom > 4.5) {
            return true;
        } else if (mom <= 4.5) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

// DCEdgeCuts function --------------------------------------------------------------------------------------------------------------------------------------------------

bool DCEdgeCuts(clas12::region_part_ptr p) {
    std::vector<double> dc_edge_cut_el = {4.5, 3.5, 7.5};  // units cm; {region1, region2, region3} cuts for electrons INBENDING
    std::vector<double> dc_edge_cut_ptr = {2.5, 3, 10.5};  // units cm; {region1, region2, region3} cuts for protons  OUTBENDING

    // true if inside cut
    // cut all charged particles
    if (p->par()->getCharge() != 0) {
        auto traj_index_1 = p->traj(DC, 6)->getIndex();   // layer 1
        auto traj_index_2 = p->traj(DC, 18)->getIndex();  // layer 2
        auto traj_index_3 = p->traj(DC, 36)->getIndex();  // layer 3

        auto traj_edge_1 = p->traj(DC, 6)->getFloat("edge", traj_index_1);
        auto traj_edge_2 = p->traj(DC, 18)->getFloat("edge", traj_index_2);
        auto traj_edge_3 = p->traj(DC, 36)->getFloat("edge", traj_index_3);

        // PUT DC EDGE CUTS IN PARAMETER FILE

        // electron DC cuts
        if (p->par()->getCharge() < 0 && (dc_edge_cut_el.size() == 3 && traj_edge_1 > dc_edge_cut_el[0] && traj_edge_2 > dc_edge_cut_el[1] && traj_edge_3 > dc_edge_cut_el[2])) {
            return true;
        }
        // proton DC cuts
        else if (p->par()->getCharge() > 0 && (dc_edge_cut_ptr.size() == 3 && traj_edge_1 > dc_edge_cut_ptr[0] && traj_edge_2 > dc_edge_cut_ptr[1] && traj_edge_3 > dc_edge_cut_ptr[2])) {
            return true;
        } else {
            return false;
        }
    } else {
        return true;
    }
}

// fillDCdebug function -------------------------------------------------------------------------------------------------------------------------------------------------

void fillDCdebug(clas12::region_part_ptr p, TH2D** h, double weight) {
    //  if(p->par()->getPid() == 11)
    //    {
    h[1]->Fill(p->traj(DC, 6)->getX(), p->traj(DC, 6)->getY(), weight);
    h[2]->Fill(p->traj(DC, 18)->getX(), p->traj(DC, 18)->getY(), weight);
    h[3]->Fill(p->traj(DC, 36)->getX(), p->traj(DC, 36)->getY(), weight);
    //    }
}

// ======================================================================================================================================================================
// Helper functions for nFD vetoes
// ======================================================================================================================================================================

// Calc_Sol_dPhi function -----------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Calc_Sol_dPhi function */
/**
 * @brief Compute the solenoidal Larmor rotation angle Δφ for the electron track.
 *
 * Given an electron track, the magnetic field B (Tesla), and effective path length L (meters),
 * this function calculates the azimuthal deflection angle due to the solenoidal field.
 *
 * Formula:
 *   Δφ = (0.299792458 * q * B * L) / (2 * p)
 * where q is the track charge in units of e, p is the momentum magnitude in GeV/c.
 *
 * The factor 0.299792458 converts (T·m) into GeV/c, and the 1/2 factor is the solenoid coupling.
 * If B(z) is non-uniform, replace B*L with the integral ∫Bz dz.
 *
 * @param electrons Vector of clas12::region_part_ptr, first element should be the electron track.
 * @param B Magnetic field strength in Tesla.
 * @param L Effective solenoid length in meters.
 * @return Computed Δφ in radians. Returns 0.0 if inputs are invalid.
 */
double Calc_Sol_dPhi(const vector<clas12::region_part_ptr>& electrons, const double& B, const double& L) {
    // Sanity checks to avoid UB / division by zero
    if (electrons.empty() || !electrons[0]) {
#ifdef DEBUG_VETO
        std::cerr << "[Calc_Sol_dPhi] electrons empty or null ptr at [0]\n";
#endif
        return 0.0;
    }
    const auto* ep = electrons[0]->par();
    if (!ep) {
#ifdef DEBUG_VETO
        std::cerr << "[Calc_Sol_dPhi] electrons[0]->par() is null\n";
#endif
        return 0.0;
    }
    /* Charged particle offset in phi due to a solenoidal field.
     *
     * Rotation angle (Larmor rotation) over length L is:
     *   Δφ = (0.299792458 * q * ∫B·dz) / (2 * p)    [radians]
     * with p in GeV/c, B in Tesla, L in meters, q in units of |e|.
     * The 0.299792458 is the GeV/c↔(T·m) conversion; the 1/2 is the solenoid coupling.
     *
     * Notes:
     * - Use the particle momentum magnitude p, not Pz (paraxial p≈pz, but p is safer).
     * - Sign comes from q * B (right-hand rule). If your coord system twists the other way,
     *   flip the sign when adding to phi.
     * - If B(z) isn’t uniform, replace B*L with the path integral ∫Bz dz.
     */

    constexpr double kTmmGeV = 0.299792458;  // 1 T·m ≈ 0.299792458 GeV/c

    const double q = static_cast<double>(ep->getCharge());  // e.g., electron: -1
    const double p = ep->getP();                            // GeV/c (total)
    if (!(p > 0.0) || !std::isfinite(p)) {
#ifdef DEBUG_VETO
        std::cerr << "[Calc_Sol_dPhi] non-physical p=" << p << "\n";
#endif
        return 0.0;
    }

    // rotation angle in radians (includes sign via q*B)
    double BL_int = B * L;                                 // replace with integral if you have B(z)
    double Sol_dPhi = (kTmmGeV * q * BL_int) / (2.0 * p);  // radians

    return Sol_dPhi;
}

#pragma endregion

// Get_shifted_e_unit_v_NeutVeto function -------------------------------------------------------------------------------------------------------------------------------

#pragma region /* Get_shifted_e_unit_v_NeutVeto */
/**
 * @brief Build the expected charged hit position for the neutral‑veto check.
 *
 * This function estimates where the associated charged particle (typically the
 * electron) would appear in the ECAL, after accounting for solenoidal field
 * rotation Δφ. The rotated hit position (shifted_e_unit_v) is placed on the same radius
 * as the neutral hit (v_nhit) but with the electron’s θ and φ+Δφ.
 *
 * Workflow:
 *  - Verify that v_nhit has a valid, non‑zero magnitude; otherwise return (0,0,0).
 *  - Compute Δφ from B, L, and the electron momentum using Calc_Sol_dPhi().
 *  - Use SetMagThetaPhi() with |v_nhit|, electron θ, and φ+Δφ to build shifted_e_unit_v.
 *
 * Robustness:
 *  - Returns a zero vector on invalid v_nhit input.
 *  - Emits debug messages if compiled with -DDEBUG_VETO.
 *
 * @param electrons Vector of clas12::region_part_ptr, with the first element assumed to be the electron.
 * @param v_nhit Neutral hit position in ECAL coordinates.
 * @param B Magnetic field strength in Tesla.
 * @param L The effective distance the particle travels along the solenoid axis (the z-direction)
 * @return Rotated charged‑hit vector shifted_e_unit_v.
 */
TVector3 Get_shifted_e_unit_v_NeutVeto(const vector<clas12::region_part_ptr>& electrons, const TVector3& v_nhit, const double& B, const double& L) {
    if (v_nhit.Mag() == 0.0 || !std::isfinite(v_nhit.Mag())) {
#ifdef DEBUG_VETO
        std::cerr << "[Get_shifted_e_unit_v_NeutVeto] v_nhit has zero/non-finite magnitude, returning (0,0,0)\n";
#endif
        return TVector3(0, 0, 0);
    }
    TVector3 shifted_e_unit_v;

    double Sol_dPhi = Calc_Sol_dPhi(electrons, B, L);  // radians

    shifted_e_unit_v.SetMagThetaPhi(v_nhit.Mag(), electrons[0]->getTheta(),
                                    electrons[0]->getPhi() + Sol_dPhi  // add sign-consistent rotation
    );

    return shifted_e_unit_v;
}

#pragma endregion

// ======================================================================================================================================================================
// nFD veto functions
// ======================================================================================================================================================================

// NeutronECAL_Cut_Veto function ----------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* NeutronECAL_Cut_Veto function */
/**
 * @brief Legacy ECAL neutral selection + charged-proximity veto for an FD neutral candidate.
 *
 * Context:
 *  This is the older ECAL-based veto used in the MSc analysis (copied from the historical
 *  ParticleID implementation). It assumes a *single neutral candidate* at `allParticles[index]`.
 *  In this project it has been superseded by `ChargedECALveto(...)`, but is kept here for
 *  reference and cross-checks.
 *
 * What it does (high-level):
 *  1) Build the beam 3-vector and the reconstructed electron 3-momentum.
 *  2) Compute the 3-momentum transfer q = p_b - p_e (used only for optional, historically-used
 *     angular cuts).
 *  3) Require the candidate to be neutral (charge == 0).
 *  4) Determine which ECAL layer the candidate hit first (PCAL/ECIN/ECOUT), and build the neutral
 *     hit position vector v_nhit at that layer.
 *  5) Apply a set of neutral-quality veto criteria (see below).
 *  6) Loop over all reconstructed charged particles and veto the neutral if any charged particle
 *     has an ECAL hit (or ECAL trajectory projection) within `veto_cut` of v_nhit.
 *
 * Veto criteria (each explained + motivation):
 *  - Neutrality requirement (charge == 0):
 *      This function is intended to select neutrals. A charged candidate is rejected immediately.
 *
 *  - Beta sanity (beta >= 0):
 *      Negative beta is non-physical and typically indicates a malformed time/path association.
 *      Such candidates are rejected.
 *
 *  - Theta sanity (theta_n >= 1 deg):
 *      Reject candidates with theta close to 0. These often arise from bad reconstruction
 *      (e.g. default angles) and can destabilize geometry-based comparisons.
 *
 *  - ECAL-layer requirement (IC || OC):
 *      Require the neutral to have a hit in ECIN or ECOUT. This enforces the historical
 *      'neutron-like' definition used here and removes candidates that only appear in PCAL.
 *
 *  - PCAL veto (PC == false):
 *      If the candidate has a PCAL hit, reject it. Photons commonly deposit in PCAL; in this
 *      legacy selection, PCAL activity is treated as photon-like contamination.
 *
 *  - Charged proximity veto (|v_nhit - v_chit| < veto_cut):
 *      If any charged particle is spatially close to the neutral ECAL hit, veto the neutral.
 *      Motivation: suppress cases where a charged shower seeds or overlaps the neutral cluster.
 *      Charged position is taken from the *same* ECAL layer when available; otherwise the charged
 *      particle's ECAL trajectory projection is used.
 *
 * Notes:
 *  - The historically-present angular cuts on theta_nq and theta_q are currently commented out.
 *    If re-enabled, they would enforce additional kinematic/topology constraints.
 *
 * @param allParticles Reconstructed event particles (neutral candidate at `index`; scanned for charged activity).
 * @param electrons Reconstructed electrons (uses electrons[0]).
 * @param beamE Beam energy (GeV) used to build the beam 3-vector.
 * @param index Index of the neutral candidate in `allParticles`.
 * @param veto_cut Maximum allowed 3D separation between the neutral hit and any charged hit/projection.
 * @return True if the neutral passes all criteria and survives the charged proximity veto; false otherwise.
 */
bool NeutronECAL_Cut_Veto(vector<clas12::region_part_ptr>& allParticles, vector<clas12::region_part_ptr>& electrons, const double& beamE, const int& index, const double& veto_cut) {
    TVector3 p_b(0, 0, beamE); /* Beam 3-vector: (0,0,Ebeam) in GeV/c */

    // Reconstructed electron 3-momentum (magnitude + angles from the reconstruction banks)
    TVector3 p_e;
    p_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());

    // 3-momentum transfer q = p_b - p_e (kept for historical angular cuts; currently not used for veto decisions)
    TVector3 p_q = p_b - p_e;

    // Candidate must be neutral for this veto logic to be meaningful
    if (allParticles[index]->par()->getCharge() != 0) { return false; }

    // Build a unit vector that carries the neutral candidate angles (theta, phi)
    TVector3 p_n_Angles;
    p_n_Angles.SetMagThetaPhi(1.0, allParticles[index]->getTheta(), allParticles[index]->getPhi());  // unit vector; angles define the neutral direction

    // Determine which ECAL sampling layers were hit by the neutral candidate.
    // getDetector()==7 indicates an ECAL hit (calorimeter detector id). Using int comparisons avoids fragile double-energy checks.
    const bool PC = (allParticles[index]->cal(clas12::PCAL)->getDetector() == 7);   // PCAL hit
    const bool IC = (allParticles[index]->cal(clas12::ECIN)->getDetector() == 7);   // ECIN hit
    const bool OC = (allParticles[index]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

    // Earliest hit layer (used to define the neutral hit position v_nhit)
    const auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT;

    // Neutral ECAL hit position at the chosen layer
    TVector3 v_nhit(allParticles[index]->cal(detlayer)->getX(), allParticles[index]->cal(detlayer)->getY(), allParticles[index]->cal(detlayer)->getZ());

    // Basic kinematics/angles used for quality checks (and for optional historical physics cuts)
    const double beta = allParticles[index]->par()->getBeta();
    const double gamma = 1.0 / sqrt(1.0 - (beta * beta));  // not used in the veto decision; kept for legacy parity
    const double theta_n = analysis_math::RadToDeg(p_n_Angles.Theta());
    const double theta_q = analysis_math::RadToDeg(p_q.Theta());
    const double theta_nq = analysis_math::RadToDeg(p_n_Angles.Angle(p_q));

    // (1) Beta sanity: negative beta is non-physical (bad time/path association)
    if (beta < 0) { return false; }

    // Optional legacy checks (kept commented intentionally):
    //   - beta > 1.1 : strongly unphysical candidates
    //   - theta_nq, theta_q : historical topology/physics cuts (disabled per Larry)
    // if (beta > 1.1) { return false; }
    // if (theta_nq > 25) { return false; }
    // if (theta_q > 40) { return false; }

    // (2) Theta sanity: reject degenerate candidates with theta ~ 0 degrees
    if (theta_n < 1.0) { return false; }  // degrees

    // (3) ECAL-layer requirement: must hit ECIN or ECOUT (legacy neutron-like definition)
    if (!(IC || OC)) { return false; }

    // (4) PCAL veto: reject any candidate with PCAL activity (photon-like contamination in this legacy selection)
    if (PC) { return false; }

    // (5) Charged proximity veto: reject the neutral if any charged particle is too close in ECAL
    bool Veto = false;
    for (int j = 0; j < allParticles.size(); j++) {
        // Consider charged particles only
        if (allParticles[j]->par()->getCharge() == 0) { continue; }
        // v_chit is the charged reference position in ECAL (measured hit when available; otherwise trajectory projection)
        TVector3 v_chit;

        if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            // Both neutral and charged have a measured ECIN hit: compare using ECIN measured positions
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist = v_nhit - v_chit;
            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            // Both neutral and charged have a measured ECOUT hit: compare using ECOUT measured positions
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist = v_nhit - v_chit;
            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else {
            // Charged particle does not have a measured hit in the same layer as the neutral.
            // Fall back to the charged particle's ECAL trajectory projection at a reference layer:
            //   ECIN neutral -> trajlayer 4, ECOUT neutral -> trajlayer 7 (legacy mapping).
            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
            v_chit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                          allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist = v_nhit - v_chit;
            if (v_dist.Mag() < veto_cut) { Veto = true; }
        }
    }

    // If any charged particle was found within veto_cut, reject the neutral candidate
    if (Veto) { return false; }

    // Survived all criteria: accept this neutral candidate
    return true;
}
#pragma endregion

// ChargedECALveto function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* ChargedECALveto function */
/**
 * @brief Charged ECAL proximity veto for a given FD neutral candidate.
 *
 * Purpose:
 *  Reject a neutral candidate (neutron or photon) if there is nearby charged-particle activity
 *  in ECAL that could indicate the neutral is a mis-associated cluster or contamination from a
 *  charged shower.
 *
 * High level algorithm:
 *  1) Determine the earliest ECAL layer hit by the neutral: PCAL, ECIN, or ECOUT.
 *  2) Define the neutral hit position vector v_nhit at that layer.
 *  3) Apply basic validity and PID related veto criteria (see below).
 *  4) Loop over all reconstructed charged particles and build a charged reference position:
 *     - If the charged particle has a measured hit in the same ECAL layer, use that measured hit.
 *     - Otherwise, use the charged particle trajectory projection at an ECAL reference layer.
 *  5) Veto the neutral if any charged reference position lies within `veto_cut` of v_nhit.
 *
 * Veto criteria and motivation:
 *  - Neutrality requirement (charge == 0):
 *    This function is intended only for neutrals. A charged candidate is rejected immediately.
 *
 *  - Optional PCAL based neutral definition (`apply_PCAL_veto`):
 *    If enabled, require the neutral to have an ECIN or ECOUT hit, and reject any neutral with a
 *    PCAL hit. This implements the analysis policy used to suppress photon-like candidates, since
 *    photons commonly deposit in PCAL while the neutron definition used here excludes PCAL.
 *
 *  - Beta sanity (`0 <= beta <= 1.1`):
 *    Reject non-physical beta (negative) and strongly unphysical beta above 1.1. The upper bound
 *    is intentionally loose to tolerate resolution effects while still rejecting obvious failures.
 *
 *  - Theta sanity (`theta > 1 deg`):
 *    Remove degenerate or malformed candidates with theta near 0 that can lead to undefined
 *    geometry and unstable comparisons.
 *
 *  - Charged proximity veto (`|v_nhit - v_ch| < veto_cut`):
 *    If any charged particle is spatially close to the neutral hit at ECAL, veto the neutral.
 *    This suppresses cases where a charged shower can mimic or seed a neutral cluster.
 *
 * Notes on start time and recalculation:
 *  - If `recalc_P_nFD` is enabled, beta (and momentum) are recomputed from path and time-of-flight
 *    relative to `starttime` rather than using the stored particle values.
 *  - In this implementation, the helper flag `recalc_P_nFD` is derived from `starttime` and
 *    `Recalc_P_nFD`. Ensure the calling code uses a meaningful `starttime` when requesting a
 *    recalculation.
 *
 * @param allParticles Reconstructed event particles; scanned for charged ECAL activity.
 * @param electron Reconstructed electron track; used as the reference vertex when recalculating path.
 * @param NeutronFD Neutral candidate being tested.
 * @param veto_cut Maximum allowed 3D distance between neutral hit and charged hit/projection.
 * @param starttime Event start time used for ToF when recalculating; default 9999 is a sentinel.
 * @param Recalc_P_nFD If true, request recalculation of beta/momentum from path and ToF.
 * @param apply_PCAL_veto If true, enforce the PCAL based neutral definition (IC/OC required and PC veto).
 * @return True if the neutral survives the charged ECAL proximity veto; false if vetoed.
 */
bool ChargedECALveto(vector<clas12::region_part_ptr>& allParticles, clas12::region_part_ptr& electron, clas12::region_part_ptr NeutronFD, const double& veto_cut,
                     const double& starttime = 9999, const bool& Recalc_P_nFD = false, const bool& apply_PCAL_veto = false) {
    // Veto setup: define how (and whether) we will use reconstructed timing and geometry information.
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // If requested, we can recompute beta (and momentum) using a path and a time-of-flight relative to `starttime`.
    // Here we derive a local switch `recalc_P_nFD` from the user flag and the start-time sentinel.
    bool recalc_P_nFD = (starttime == 9999) ? Recalc_P_nFD : false;

    // If `recalc_P_nFD` is enabled, compute the neutral path from the electron vertex to the ECAL hit,
    // and compute ToF from the ECAL hit time relative to `starttime`. These are then converted to beta.
    // If disabled, use the stored particle beta from the reconstruction banks.
    const double reco_Path_nFD = CalcPathnFD(NeutronFD, electron, recalc_P_nFD);
    const double reco_ToF_nFD = CalcToFnFD(NeutronFD, starttime, recalc_P_nFD);
    const double reco_Beta_nFD = recalc_P_nFD ? reco_Path_nFD / (reco_ToF_nFD * constants::c) : NeutronFD->par()->getBeta();

    // Gamma is the usual relativistic factor. It is computed here for completeness and debugging.
    // It is not used by the veto decision itself.
    const double reco_Gamma_nFD = 1 / sqrt(1 - (reco_Beta_nFD * reco_Beta_nFD));

    // Determine which ECAL layers were hit by the neutral and choose the earliest hit layer.
    // detlayer is used consistently to define the neutral hit location and the charged comparison strategy.
    const auto [PC, IC, OC, detlayer] = CheckForECALHits(NeutronFD);

    // Neutral ECAL hit position in the chosen layer.
    const TVector3 reco_nFD_hit_v(NeutronFD->cal(detlayer)->getX(), NeutronFD->cal(detlayer)->getY(), NeutronFD->cal(detlayer)->getZ());

    // Build a momentum direction vector for the neutral (angles from the reconstructed particle).
    // The magnitude is taken from CalcPnFD if recalculation is enabled; otherwise it uses the stored momentum.
    // This is only used for a theta sanity cut.
    TVector3 reco_P_nFD_v;
    reco_P_nFD_v.SetMagThetaPhi(CalcPnFD(NeutronFD, electron, starttime, recalc_P_nFD), NeutronFD->getTheta(), NeutronFD->getPhi());

    // Veto application: apply fast rejection criteria first, then scan charged particles for proximity.
    // ------------------------------------------------------------------------------------------------------------------------------------------------------------

    // (A) Optional PCAL based neutral definition:
    //     - Require ECIN or ECOUT (neutral must have an ECAL hit beyond PCAL)
    //     - Veto any PCAL hit (photon suppression policy in this analysis)
    if (apply_PCAL_veto && (!(IC || OC) || PC)) { return false; }

    // (B) Neutrality requirement: this veto is defined only for neutrals.
    if (NeutronFD->par()->getCharge() != 0) { return false; }

    // (C) Beta sanity: reject non-physical beta and strongly unphysical values.
    if (reco_Beta_nFD < 0.0 || reco_Beta_nFD > 1.1) { return false; }

    // (D) Theta sanity: avoid degenerate theta close to 0 degrees.
    if (am::RadToDeg(reco_P_nFD_v.Theta()) < 1.0) { return false; }

    // Now let's put a charge particle veto:
    bool Veto = false;
    for (int j = 0; j < allParticles.size(); j++) {
        if (allParticles[j]->par()->getCharge() == 0) { continue; }  // Only consider charged particles

        // Charged reference position in ECAL.
        // Prefer a measured hit in the same layer as the neutral; otherwise fall back to the trajectory
        // projection at a reference ECAL layer to estimate where the charged particle would intersect ECAL.
        TVector3 reco_ChargedFD_hit_v;

        if ((detlayer == clas12::PCAL) && (allParticles[j]->cal(clas12::PCAL)->getZ() != 0)) {
            // If both particles hit the pre-shower calorimeter, use the pre-shower calorimeter to determine reco_ChargedFD_hit_v
            reco_ChargedFD_hit_v.SetXYZ(allParticles[j]->cal(clas12::PCAL)->getX(), allParticles[j]->cal(clas12::PCAL)->getY(), allParticles[j]->cal(clas12::PCAL)->getZ());
            TVector3 v_dist = reco_nFD_hit_v - reco_ChargedFD_hit_v;
            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            // If both particles hit the inner calorimeter, use the inner calorimeter to determine reco_ChargedFD_hit_v
            reco_ChargedFD_hit_v.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist = reco_nFD_hit_v - reco_ChargedFD_hit_v;
            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            // If both particles hit the outer calorimeter, use the outer calorimeter to determine reco_ChargedFD_hit_v
            reco_ChargedFD_hit_v.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist = reco_nFD_hit_v - reco_ChargedFD_hit_v;
            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else {
            // Trajectory layer mapping used in this project:
            //   PCAL -> 1, ECIN -> 4, ECOUT -> 7
            const int trajlayer = (detlayer == clas12::PCAL) ? 1 : (detlayer == clas12::ECIN) ? 4 : 7;
            reco_ChargedFD_hit_v.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                                        allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist = reco_nFD_hit_v - reco_ChargedFD_hit_v;
            if (v_dist.Mag() < veto_cut) { Veto = true; }
        }
    }

    if (Veto) { return false; }  // If any of the vetoes are true, return false
    return true;                 // We survived up to this point, we do have a neutral particle
}
#pragma endregion

// NeutralECALveto function ---------------------------------------------------------------------------------------------------------------------------------------------

#pragma region /* NeutralECALveto function */
/**
 * @brief Apply a neutral-particle proximity veto in ECAL using a solenoid-shifted electron reference.
 *
 * This function rejects a neutral candidate if its ECAL hit position lies within a configurable 3D
 * distance `veto_cut` from the *expected* charged-particle (typically electron) hit position, after
 * applying an approximate solenoidal-field azimuthal rotation Δφ.
 *
 * The key idea is to compare the neutral hit location `v_nhit` to a shifted electron reference
 * direction `shifted_e_unit_v`, where the electron’s φ is rotated by Δφ and the resulting vector is
 * placed on the same radius |v_nhit|.
 *
 * Workflow:
 *  - Define solenoid parameters (B, L) used for the Δφ estimate.
 *  - Build the beam vector P_b and reconstructed electron momentum vector P_e.
 *  - Form the 3-momentum transfer q = P_b - P_e (currently computed for context; not used in the veto).
 *  - Build the neutral direction angles (θ, φ) from the neutral hit position.
 *  - Construct the neutral hit position vector `v_nhit` from (xHit, yHit, zHit).
 *  - Reject trivially non-physical cases (|v_nhit| == 0, non-finite θ, or θ < 1 deg).
 *  - Compute the shifted electron reference vector using Get_shifted_e_unit_v_NeutVeto(...).
 *  - Compute the spatial separation v_dist = v_nhit - shifted_e_unit_v.
 *  - If |v_dist| < veto_cut, veto the neutral (return false); otherwise return true.
 *
 * Robustness:
 *  - Returns false on non-physical neutral geometry (zero magnitude, non-finite θ, θ < 1 deg).
 *  - Emits optional debug prints when compiled with -DDEBUG_VETO.
 *  - The solenoid parameters (B, L) are fixed constants here; if a run-dependent or field-map-based
 *    correction is needed, refactor to pass them in or compute Δφ from a proper ∫Bz dz.
 *
 * @param allParticles Reconstructed event particles (present for API symmetry; not currently used).
 * @param electrons Reconstructed electron vector (electrons[0] is assumed to be valid).
 * @param beamE Beam energy used to build the beam 3-vector P_b.
 * @param neut The NeutralHit candidate being tested.
 * @param veto_cut Maximum allowed 3D distance (same length units as hit positions) between `v_nhit`
 *        and the shifted electron reference before vetoing.
 * @return True if the neutral survives the neutral proximity veto; false if vetoed.
 */
bool NeutralECALveto(vector<clas12::region_part_ptr>& allParticles, vector<clas12::region_part_ptr>& electrons, const double& beamE, clas12::region_part_ptr neut, const double& veto_cut) {
    /* The veto flag */
    bool Veto = false;

    /* Solenoid field parameters */
    double B = 5.0;  // Tesla
    double L = 0.5;  // meters

    /* 3-momentum transfer */
    TVector3 P_b(0, 0, beamE);  // Beam energy
    TVector3 P_e;               // Reco electron momentum vector
    P_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 q = P_b - P_e;  // 3-momentum transfer

    /* Safety check - determine if the particle is neutral or not */
    if (neut->par()->getCharge() != 0) { return false; }

    bool PC = (neut->cal(clas12::PCAL)->getDetector() == 7);
    bool IC = (neut->cal(clas12::ECIN)->getDetector() == 7);
    bool OC = (neut->cal(clas12::ECOUT)->getDetector() == 7);
    // if (!(IC || OC)) { return false; }                 /* must hit ECIN or ECOUT */
    // if (PC) { return false; }                          /* veto photons */
    auto detlayer = IC ? clas12::ECIN : clas12::ECOUT; /* earliest valid calorimeter layer among ECIN/ECOUT */

    // calculate the angles of the neutral particle
    TVector3 P_n_Angles;
    P_n_Angles.SetMagThetaPhi(1, neut->getTheta(), neut->getPhi());

    // v_nhit = location of neutral particle hit
    TVector3 v_nhit(neut->cal(detlayer)->getX(), neut->cal(detlayer)->getY(), neut->cal(detlayer)->getZ());
    if (v_nhit.Mag() == 0.0 || !std::isfinite(v_nhit.Mag())) { return false; }
    double Beta = neut->par()->getBeta();  // Beta of the neutral particle
    double Theta_n = am::RadToDeg(P_n_Angles.Theta());

    if (Beta < 0) { return false; }
    if (Theta_n < 1) { return false; } /* to avoid events with Theta_n = 0 (the "1" is in deg) */
    if (!std::isfinite(Theta_n)) { return false; }

#ifdef DEBUG_VETO
    {
        const double dphi = Calc_Sol_dPhi(electrons, B, L);
        std::cerr << "[NeutronECAL_Cut_NeutVeto] dphi(rad)=" << dphi << ", |v_nhit|=" << v_nhit.Mag() << ", veto_cut=" << veto_cut << "\n";
    }
#endif

    TVector3 shifted_e_unit_v = Get_shifted_e_unit_v_NeutVeto(electrons, v_nhit, B, L);

    TVector3 v_dist = v_nhit - shifted_e_unit_v;

    if (v_dist.Mag() < veto_cut) { Veto = true; }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}
#pragma endregion

};  // namespace reco_analysis_functions

#endif  // RECO_ANALYSIS_FUNCTIONS_H
