#ifndef MANUALVETOHISTOGRAMS_H
#define MANUALVETOHISTOGRAMS_H

#include <cstdlib>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"

// ======================================================================================================================================================================
// Andrew's histograms
// ======================================================================================================================================================================

#pragma region /* Veto histograms - start */

    /////////////////////////////////////
    // Prepare histograms
    /////////////////////////////////////

    vector<TH1 *> HistoList;

    // gStyle->SetTitleXSize(0.05);
    // gStyle->SetTitleYSize(0.05);
    //
    // gStyle->SetTitleXOffset(0.8);
    // gStyle->SetTitleYOffset(0.8);

    char temp_name[300];
    char temp_title[300];
    // char temp_name[100];
    // char temp_title[100];

    // (e,e'p) plots
    // ======================================================================================================================================================================

#pragma region /* (e,e'p) plots - start */

    /* Proton histograms (from Erin) */
    TH1D *h_p_multiplicity_BPID_epCD;
    TH1D *h_P_p_BPID_epCD;
    TH2D *h_theta_p_VS_phi_p_BPID_epCD;
    TH1D *h_p_multiplicity_APID_epCD;
    TH1D *h_P_p_APID_epCD;
    TH2D *h_theta_p_VS_phi_p_APID_epCD;

    TH1D *h_p_multiplicity_BPID_epFD;
    TH1D *h_P_p_BPID_epFD;
    TH2D *h_theta_p_VS_phi_p_BPID_epFD;
    TH1D *h_p_multiplicity_APID_epFD;
    TH1D *h_P_p_APID_epFD;
    TH2D *h_theta_p_VS_phi_p_APID_epFD;

    TH2D *h_dbeta_p_VS_P_p_BPID_epCD;
    TH1D *h_dVz_p_BPID_epCD;
    TH1D *h_Chi2pid_p_BPID_epCD;
    TH2D *h_dbeta_p_VS_P_p_APID_epCD;
    TH1D *h_dVz_p_APID_epCD;
    TH1D *h_Chi2pid_p_APID_epCD;

    TH2D *h_dbeta_p_VS_P_p_BPID_epFD;
    TH1D *h_dVz_p_BPID_epFD;
    TH1D *h_Chi2pid_p_BPID_epFD;
    TH2D *h_dbeta_p_VS_P_p_APID_epFD;
    TH1D *h_dVz_p_APID_epFD;
    TH1D *h_Chi2pid_p_APID_epFD;

    /* Missing variabels */
    TH1D *h_P_miss_BmissC_epCD;
    TH1D *h_theta_miss_BmissC_epCD;
    TH2D *h_P_miss_VS_theta_miss_BmissC_epCD;
    TH1D *h_P_miss_AmissC_epCD;
    TH1D *h_theta_miss_AmissC_epCD;
    TH2D *h_P_miss_VS_theta_miss_AmissC_epCD;

    TH1D *h_P_miss_BmissC_epFD;
    TH1D *h_theta_miss_BmissC_epFD;
    TH2D *h_P_miss_VS_theta_miss_BmissC_epFD;
    TH1D *h_P_miss_AmissC_epFD;
    TH1D *h_theta_miss_AmissC_epFD;
    TH2D *h_P_miss_VS_theta_miss_AmissC_epFD;

    // // TODO: change "Before/After P_{miss}, #theta_{miss}, and M_{miss} Cuts" to something else!
    // TH1D *h_beta_n_BmissC_epCD;
    // TH1D *h_beta_n_AmissC_epCD;

    // // TODO: change "Before/After P_{miss}, #theta_{miss}, and M_{miss} Cuts" to something else!
    // TH1D *h_beta_n_BmissC_epFD;
    // TH1D *h_beta_n_AmissC_epFD;

    TH1D *h_E_p_BmissC_epCD;
    TH1D *h_E_miss_BmissC_epCD;
    TH1D *h_M_miss_BmissC_epCD;
    TH1D *h_E_p_AmissC_epCD;
    TH1D *h_E_miss_AmissC_epCD;
    TH1D *h_M_miss_AmissC_epCD;

    TH1D *h_E_p_BmissC_epFD;
    TH1D *h_E_miss_BmissC_epFD;
    TH1D *h_M_miss_BmissC_epFD;
    TH1D *h_E_p_AmissC_epFD;
    TH1D *h_E_miss_AmissC_epFD;
    TH1D *h_M_miss_AmissC_epFD;

    /* Checks on which events have neutrons (Andrew) */
    TH1D *h_xB_BmissC_epCD;
    TH2D *h_xB_VS_M_miss_BmissC_epCD;
    TH1D *h_xB_AmissC_epCD;
    TH2D *h_xB_VS_M_miss_AmissC_epCD;

    TH1D *h_xB_BmissC_epFD;
    TH2D *h_xB_VS_M_miss_BmissC_epFD;
    TH1D *h_xB_AmissC_epFD;
    TH2D *h_xB_VS_M_miss_AmissC_epFD;

    TH2D *h_xB_VS_M_miss_epCDn;
    TH2D *h_xB_VS_M_miss_epFDn;

    TH2D *h_xB_VS_M_miss_goodN_epCDn;
    TH2D *h_xB_VS_M_miss_badN_epCDn;

    TH2D *h_xB_VS_M_miss_goodN_epFDn;
    TH2D *h_xB_VS_M_miss_badN_epFDn;

    /* Kinematical variables */
    TH1D *h_theta_n_epCDn;
    TH1D *h_phi_n_epCDn;
    TH2D *h_theta_n_VS_phi_n_epCDn;
    TH2D *h_theta_n_VS_beta_n_epCDn;

    TH1D *h_theta_n_epFDn;
    TH1D *h_phi_n_epFDn;
    TH2D *h_theta_n_VS_phi_n_epFDn;
    TH2D *h_theta_n_VS_beta_n_epFDn;

    TH1D *h_P_n_epCDn;
    TH2D *h_P_n_VS_theta_n_epCDn;

    TH1D *h_P_n_epFDn;
    TH2D *h_P_n_VS_theta_n_epFDn;

    TH1D *h_P_miss_epCDn;
    TH2D *h_P_miss_VS_theta_miss_epCDn;

    TH1D *h_P_miss_epFDn;
    TH2D *h_P_miss_VS_theta_miss_epFDn;

    TH1D *h_dpp_allN_epCDn;
    TH1D *h_dpp_goodN_epCDn;
    TH1D *h_dpp_badN_epCDn;

    TH1D *h_dpp_allN_epFDn;
    TH1D *h_dpp_goodN_epFDn;
    TH1D *h_dpp_badN_epFDn;

    TH1D *h_theta_n_miss_allN_epCDn;
    TH1D *h_theta_n_miss_goodN_epCDn;
    TH1D *h_theta_n_miss_badN_epCDn;

    TH1D *h_theta_n_miss_allN_epFDn;
    TH1D *h_theta_n_miss_goodN_epFDn;
    TH1D *h_theta_n_miss_badN_epFDn;

    TH2D *h_dpp_VS_theta_n_miss_epCDn;

    TH2D *h_dpp_VS_theta_n_miss_epFDn;

    TH1D *h_E_p_epCDn;
    TH1D *h_E_miss_epCDn;
    TH1D *h_M_miss_epCDn;
    TH2D *h_M_miss_VS_P_n_epCDn;
    TH2D *h_M_miss_VS_theta_n_epCDn;
    TH2D *h_M_miss_VS_phi_n_epCDn;
    TH2D *h_M_miss_VS_P_miss_epCDn;
    TH2D *h_M_miss_VS_theta_miss_epCDn;
    TH2D *h_M_miss_VS_phi_miss_epCDn;

    TH1D *h_E_p_epFDn;
    TH1D *h_E_miss_epFDn;
    TH1D *h_M_miss_epFDn;
    TH2D *h_M_miss_VS_P_n_epFDn;
    TH2D *h_M_miss_VS_theta_n_epFDn;
    TH2D *h_M_miss_VS_phi_n_epFDn;
    TH2D *h_M_miss_VS_P_miss_epFDn;
    TH2D *h_M_miss_VS_theta_miss_epFDn;
    TH2D *h_M_miss_VS_phi_miss_epFDn;

    TH1D *h_P_n_minus_P_miss_epCDn;
    TH1D *h_P_n_x_minus_P_miss_x_epCDn;
    TH1D *h_P_n_y_minus_P_miss_y_epCDn;
    TH1D *h_P_n_z_minus_P_miss_z_epCDn;

    TH1D *h_P_n_minus_P_miss_epFDn;
    TH1D *h_P_n_x_minus_P_miss_x_epFDn;
    TH1D *h_P_n_y_minus_P_miss_y_epFDn;
    TH1D *h_P_n_z_minus_P_miss_z_epFDn;

    TH2D *h_P_n_VS_P_miss_epCDn;
    TH2D *h_P_n_x_VS_P_miss_x_epCDn;
    TH2D *h_P_n_y_VS_P_miss_y_epCDn;
    TH2D *h_P_n_z_VS_P_miss_z_epCDn;

    TH2D *h_P_n_VS_P_miss_epFDn;
    TH2D *h_P_n_x_VS_P_miss_x_epFDn;
    TH2D *h_P_n_y_VS_P_miss_y_epFDn;
    TH2D *h_P_n_z_VS_P_miss_z_epFDn;

    TH1D *h_theta_n_p_epCDn;
    TH2D *h_theta_p_n_VS_P_p_epCDn;

    TH1D *h_theta_n_p_epFDn;
    TH2D *h_theta_p_n_VS_P_p_epFDn;

    TH1D *h_xB_epCDn;

    TH1D *h_xB_epFDn;

    /* Detector responses */
    TH1D *h_Edep_CND_epCDn;
    TH2D *h_P_n_VS_Edep_CND_epCDn;
    TH2D *h_theta_n_VS_Edep_CND_epCDn;
    TH2D *h_phi_n_VS_Edep_CND_epCDn;
    TH2D *h_P_miss_VS_Edep_CND_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND_epCDn;
    TH2D *h_dpp_VS_Edep_CND_epCDn;
    TH2D *h_beta_n_VS_Edep_CND_epCDn;
    TH2D *h_E_miss_VS_Edep_CND_epCDn;
    TH2D *h_M_miss_VS_Edep_CND_epCDn;
    TH2D *h_path_VS_Edep_CND_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_epCDn;
    TH2D *h_ToF_VS_Edep_CND_epCDn;
    TH2D *h_nSector_VS_Edep_CND_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_epCDn;

    TH1D *h_Edep_CND_epFDn;
    TH2D *h_P_n_VS_Edep_CND_epFDn;
    TH2D *h_theta_n_VS_Edep_CND_epFDn;
    TH2D *h_phi_n_VS_Edep_CND_epFDn;
    TH2D *h_P_miss_VS_Edep_CND_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND_epFDn;
    TH2D *h_dpp_VS_Edep_CND_epFDn;
    TH2D *h_beta_n_VS_Edep_CND_epFDn;
    TH2D *h_E_miss_VS_Edep_CND_epFDn;
    TH2D *h_M_miss_VS_Edep_CND_epFDn;
    TH2D *h_path_VS_Edep_CND_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_epFDn;
    TH2D *h_ToF_VS_Edep_CND_epFDn;
    TH2D *h_nSector_VS_Edep_CND_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_epFDn;

    TH1D *h_Edep_CTOF_epCDn;
    TH2D *h_P_n_VS_Edep_CTOF_epCDn;
    TH2D *h_theta_n_VS_Edep_CTOF_epCDn;
    TH2D *h_phi_n_VS_Edep_CTOF_epCDn;
    TH2D *h_P_miss_VS_Edep_CTOF_epCDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_epCDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_epCDn;
    TH2D *h_dpp_VS_Edep_CTOF_epCDn;
    TH2D *h_beta_n_VS_Edep_CTOF_epCDn;
    TH2D *h_E_miss_VS_Edep_CTOF_epCDn;
    TH2D *h_M_miss_VS_Edep_CTOF_epCDn;
    TH2D *h_path_VS_Edep_CTOF_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_epCDn;
    TH2D *h_ToF_VS_Edep_CTOF_epCDn;
    TH2D *h_nSector_VS_Edep_CTOF_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_epCDn;

    TH1D *h_Edep_CTOF_epFDn;
    TH2D *h_P_n_VS_Edep_CTOF_epFDn;
    TH2D *h_theta_n_VS_Edep_CTOF_epFDn;
    TH2D *h_phi_n_VS_Edep_CTOF_epFDn;
    TH2D *h_P_miss_VS_Edep_CTOF_epFDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_epFDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_epFDn;
    TH2D *h_dpp_VS_Edep_CTOF_epFDn;
    TH2D *h_beta_n_VS_Edep_CTOF_epFDn;
    TH2D *h_E_miss_VS_Edep_CTOF_epFDn;
    TH2D *h_M_miss_VS_Edep_CTOF_epFDn;
    TH2D *h_path_VS_Edep_CTOF_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_epFDn;
    TH2D *h_ToF_VS_Edep_CTOF_epFDn;
    TH2D *h_nSector_VS_Edep_CTOF_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_epFDn;

    TH1D *h_Edep_single_epCDn;
    TH2D *h_P_n_VS_Edep_single_epCDn;
    TH2D *h_theta_n_VS_Edep_single_epCDn;
    TH2D *h_phi_n_VS_Edep_single_epCDn;
    TH2D *h_P_miss_VS_Edep_single_epCDn;
    TH2D *h_theta_miss_VS_Edep_single_epCDn;
    TH2D *h_phi_miss_VS_Edep_single_epCDn;
    TH2D *h_dpp_VS_Edep_single_epCDn;
    TH2D *h_beta_n_VS_Edep_single_epCDn;
    TH2D *h_E_miss_VS_Edep_single_epCDn;
    TH2D *h_M_miss_VS_Edep_single_epCDn;
    TH2D *h_path_VS_Edep_single_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_single_epCDn;
    TH2D *h_ToF_VS_Edep_single_epCDn;
    TH2D *h_nSector_VS_Edep_single_epCDn;

    TH1D *h_Edep_single_epFDn;
    TH2D *h_P_n_VS_Edep_single_epFDn;
    TH2D *h_theta_n_VS_Edep_single_epFDn;
    TH2D *h_phi_n_VS_Edep_single_epFDn;
    TH2D *h_P_miss_VS_Edep_single_epFDn;
    TH2D *h_theta_miss_VS_Edep_single_epFDn;
    TH2D *h_phi_miss_VS_Edep_single_epFDn;
    TH2D *h_dpp_VS_Edep_single_epFDn;
    TH2D *h_beta_n_VS_Edep_single_epFDn;
    TH2D *h_E_miss_VS_Edep_single_epFDn;
    TH2D *h_M_miss_VS_Edep_single_epFDn;
    TH2D *h_path_VS_Edep_single_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_single_epFDn;
    TH2D *h_ToF_VS_Edep_single_epFDn;
    TH2D *h_nSector_VS_Edep_single_epFDn;

    TH1D *h_Edep_CND1_epCDn;
    TH2D *h_P_n_VS_Edep_CND1_epCDn;
    TH2D *h_theta_n_VS_Edep_CND1_epCDn;
    TH2D *h_phi_n_VS_Edep_CND1_epCDn;
    TH2D *h_P_miss_VS_Edep_CND1_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND1_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND1_epCDn;
    TH2D *h_dpp_VS_Edep_CND1_epCDn;
    TH2D *h_beta_n_VS_Edep_CND1_epCDn;
    TH2D *h_E_miss_VS_Edep_CND1_epCDn;
    TH2D *h_M_miss_VS_Edep_CND1_epCDn;
    TH2D *h_path_VS_Edep_CND1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_epCDn;
    TH2D *h_ToF_VS_Edep_CND1_epCDn;
    TH2D *h_nSector_VS_Edep_CND1_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_epCDn;

    TH1D *h_Edep_CND1_epFDn;
    TH2D *h_P_n_VS_Edep_CND1_epFDn;
    TH2D *h_theta_n_VS_Edep_CND1_epFDn;
    TH2D *h_phi_n_VS_Edep_CND1_epFDn;
    TH2D *h_P_miss_VS_Edep_CND1_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND1_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND1_epFDn;
    TH2D *h_dpp_VS_Edep_CND1_epFDn;
    TH2D *h_beta_n_VS_Edep_CND1_epFDn;
    TH2D *h_E_miss_VS_Edep_CND1_epFDn;
    TH2D *h_M_miss_VS_Edep_CND1_epFDn;
    TH2D *h_path_VS_Edep_CND1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_epFDn;
    TH2D *h_ToF_VS_Edep_CND1_epFDn;
    TH2D *h_nSector_VS_Edep_CND1_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_epFDn;

    TH1D *h_Edep_CND2_epCDn;
    TH2D *h_P_n_VS_Edep_CND2_epCDn;
    TH2D *h_theta_n_VS_Edep_CND2_epCDn;
    TH2D *h_phi_n_VS_Edep_CND2_epCDn;
    TH2D *h_P_miss_VS_Edep_CND2_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND2_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND2_epCDn;
    TH2D *h_dpp_VS_Edep_CND2_epCDn;
    TH2D *h_beta_n_VS_Edep_CND2_epCDn;
    TH2D *h_E_miss_VS_Edep_CND2_epCDn;
    TH2D *h_M_miss_VS_Edep_CND2_epCDn;
    TH2D *h_path_VS_Edep_CND2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_epCDn;
    TH2D *h_ToF_VS_Edep_CND2_epCDn;
    TH2D *h_nSector_VS_Edep_CND2_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_epCDn;

    TH1D *h_Edep_CND2_epFDn;
    TH2D *h_P_n_VS_Edep_CND2_epFDn;
    TH2D *h_theta_n_VS_Edep_CND2_epFDn;
    TH2D *h_phi_n_VS_Edep_CND2_epFDn;
    TH2D *h_P_miss_VS_Edep_CND2_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND2_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND2_epFDn;
    TH2D *h_dpp_VS_Edep_CND2_epFDn;
    TH2D *h_beta_n_VS_Edep_CND2_epFDn;
    TH2D *h_E_miss_VS_Edep_CND2_epFDn;
    TH2D *h_M_miss_VS_Edep_CND2_epFDn;
    TH2D *h_path_VS_Edep_CND2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_epFDn;
    TH2D *h_ToF_VS_Edep_CND2_epFDn;
    TH2D *h_nSector_VS_Edep_CND2_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_epFDn;

    TH1D *h_Edep_CND3_epCDn;
    TH2D *h_P_n_VS_Edep_CND3_epCDn;
    TH2D *h_theta_n_VS_Edep_CND3_epCDn;
    TH2D *h_phi_n_VS_Edep_CND3_epCDn;
    TH2D *h_P_miss_VS_Edep_CND3_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND3_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND3_epCDn;
    TH2D *h_dpp_VS_Edep_CND3_epCDn;
    TH2D *h_beta_n_VS_Edep_CND3_epCDn;
    TH2D *h_E_miss_VS_Edep_CND3_epCDn;
    TH2D *h_M_miss_VS_Edep_CND3_epCDn;
    TH2D *h_path_VS_Edep_CND3_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_epCDn;
    TH2D *h_ToF_VS_Edep_CND3_epCDn;
    TH2D *h_nSector_VS_Edep_CND3_epCDn;

    TH1D *h_Edep_CND3_epFDn;
    TH2D *h_P_n_VS_Edep_CND3_epFDn;
    TH2D *h_theta_n_VS_Edep_CND3_epFDn;
    TH2D *h_phi_n_VS_Edep_CND3_epFDn;
    TH2D *h_P_miss_VS_Edep_CND3_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND3_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND3_epFDn;
    TH2D *h_dpp_VS_Edep_CND3_epFDn;
    TH2D *h_beta_n_VS_Edep_CND3_epFDn;
    TH2D *h_E_miss_VS_Edep_CND3_epFDn;
    TH2D *h_M_miss_VS_Edep_CND3_epFDn;
    TH2D *h_path_VS_Edep_CND3_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_epFDn;
    TH2D *h_ToF_VS_Edep_CND3_epFDn;
    TH2D *h_nSector_VS_Edep_CND3_epFDn;

    TH1D *h_Size_CND1_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND1_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_epCDn;
    TH2D *h_P_n_VS_Size_CND1_epCDn;
    TH2D *h_theta_n_VS_Size_CND1_epCDn;
    TH2D *h_phi_n_VS_Size_CND1_epCDn;
    TH2D *h_P_miss_VS_Size_CND1_epCDn;
    TH2D *h_theta_miss_VS_Size_CND1_epCDn;
    TH2D *h_phi_miss_VS_Size_CND1_epCDn;
    TH2D *h_dpp_VS_Size_CND1_epCDn;
    TH2D *h_beta_n_VS_Size_CND1_epCDn;
    TH2D *h_E_miss_VS_Size_CND1_epCDn;
    TH2D *h_M_miss_VS_Size_CND1_epCDn;
    TH2D *h_path_VS_Size_CND1_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_epCDn;
    TH2D *h_ToF_VS_Size_CND1_epCDn;
    TH2D *h_nSector_VS_Size_CND1_epCDn;

    TH1D *h_Size_CND1_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND1_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_epFDn;
    TH2D *h_P_n_VS_Size_CND1_epFDn;
    TH2D *h_theta_n_VS_Size_CND1_epFDn;
    TH2D *h_phi_n_VS_Size_CND1_epFDn;
    TH2D *h_P_miss_VS_Size_CND1_epFDn;
    TH2D *h_theta_miss_VS_Size_CND1_epFDn;
    TH2D *h_phi_miss_VS_Size_CND1_epFDn;
    TH2D *h_dpp_VS_Size_CND1_epFDn;
    TH2D *h_beta_n_VS_Size_CND1_epFDn;
    TH2D *h_E_miss_VS_Size_CND1_epFDn;
    TH2D *h_M_miss_VS_Size_CND1_epFDn;
    TH2D *h_path_VS_Size_CND1_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_epFDn;
    TH2D *h_ToF_VS_Size_CND1_epFDn;
    TH2D *h_nSector_VS_Size_CND1_epFDn;

    TH1D *h_Size_CND2_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND2_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_epCDn;
    TH2D *h_P_n_VS_Size_CND2_epCDn;
    TH2D *h_theta_n_VS_Size_CND2_epCDn;
    TH2D *h_phi_n_VS_Size_CND2_epCDn;
    TH2D *h_P_miss_VS_Size_CND2_epCDn;
    TH2D *h_theta_miss_VS_Size_CND2_epCDn;
    TH2D *h_phi_miss_VS_Size_CND2_epCDn;
    TH2D *h_dpp_VS_Size_CND2_epCDn;
    TH2D *h_beta_n_VS_Size_CND2_epCDn;
    TH2D *h_E_miss_VS_Size_CND2_epCDn;
    TH2D *h_M_miss_VS_Size_CND2_epCDn;
    TH2D *h_path_VS_Size_CND2_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_epCDn;
    TH2D *h_ToF_VS_Size_CND2_epCDn;
    TH2D *h_nSector_VS_Size_CND2_epCDn;

    TH1D *h_Size_CND2_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND2_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_epFDn;
    TH2D *h_P_n_VS_Size_CND2_epFDn;
    TH2D *h_theta_n_VS_Size_CND2_epFDn;
    TH2D *h_phi_n_VS_Size_CND2_epFDn;
    TH2D *h_P_miss_VS_Size_CND2_epFDn;
    TH2D *h_theta_miss_VS_Size_CND2_epFDn;
    TH2D *h_phi_miss_VS_Size_CND2_epFDn;
    TH2D *h_dpp_VS_Size_CND2_epFDn;
    TH2D *h_beta_n_VS_Size_CND2_epFDn;
    TH2D *h_E_miss_VS_Size_CND2_epFDn;
    TH2D *h_M_miss_VS_Size_CND2_epFDn;
    TH2D *h_path_VS_Size_CND2_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_epFDn;
    TH2D *h_ToF_VS_Size_CND2_epFDn;
    TH2D *h_nSector_VS_Size_CND2_epFDn;

    TH1D *h_Size_CND3_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND3_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_epCDn;
    TH2D *h_P_n_VS_Size_CND3_epCDn;
    TH2D *h_theta_n_VS_Size_CND3_epCDn;
    TH2D *h_phi_n_VS_Size_CND3_epCDn;
    TH2D *h_P_miss_VS_Size_CND3_epCDn;
    TH2D *h_theta_miss_VS_Size_CND3_epCDn;
    TH2D *h_phi_miss_VS_Size_CND3_epCDn;
    TH2D *h_dpp_VS_Size_CND3_epCDn;
    TH2D *h_beta_n_VS_Size_CND3_epCDn;
    TH2D *h_E_miss_VS_Size_CND3_epCDn;
    TH2D *h_M_miss_VS_Size_CND3_epCDn;
    TH2D *h_path_VS_Size_CND3_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_epCDn;
    TH2D *h_ToF_VS_Size_CND3_epCDn;
    TH2D *h_nSector_VS_Size_CND3_epCDn;

    TH1D *h_Size_CND3_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND3_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_epFDn;
    TH2D *h_P_n_VS_Size_CND3_epFDn;
    TH2D *h_theta_n_VS_Size_CND3_epFDn;
    TH2D *h_phi_n_VS_Size_CND3_epFDn;
    TH2D *h_P_miss_VS_Size_CND3_epFDn;
    TH2D *h_theta_miss_VS_Size_CND3_epFDn;
    TH2D *h_phi_miss_VS_Size_CND3_epFDn;
    TH2D *h_dpp_VS_Size_CND3_epFDn;
    TH2D *h_beta_n_VS_Size_CND3_epFDn;
    TH2D *h_E_miss_VS_Size_CND3_epFDn;
    TH2D *h_M_miss_VS_Size_CND3_epFDn;
    TH2D *h_path_VS_Size_CND3_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_epFDn;
    TH2D *h_ToF_VS_Size_CND3_epFDn;
    TH2D *h_nSector_VS_Size_CND3_epFDn;

    TH2D *h_Size_CND1_VS_Size_CND2_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND3_epCDn;
    TH2D *h_Size_CND2_VS_Size_CND3_epCDn;

    TH2D *h_Size_CND1_VS_Size_CND2_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND3_epFDn;
    TH2D *h_Size_CND2_VS_Size_CND3_epFDn;

    TH1D *h_LayerMult_CND1_epCDn;
    TH1D *h_LayerMult_CND2_epCDn;
    TH1D *h_LayerMult_CND3_epCDn;

    TH1D *h_LayerMult_CND1_epFDn;
    TH1D *h_LayerMult_CND2_epFDn;
    TH1D *h_LayerMult_CND3_epFDn;

    TH2D *h_LayerMult_CND1_VS_LayerMult_CND2_epCDn;
    TH2D *h_LayerMult_CND1_VS_LayerMult_CND3_epCDn;
    TH2D *h_LayerMult_CND2_VS_LayerMult_CND3_epCDn;

    TH2D *h_LayerMult_CND1_VS_LayerMult_CND2_epFDn;
    TH2D *h_LayerMult_CND1_VS_LayerMult_CND3_epFDn;
    TH2D *h_LayerMult_CND2_VS_LayerMult_CND3_epFDn;

    TH1D *h_ToF_epCDn;
    TH1D *h_ToF_zoomout_epCDn;
    TH2D *h_P_n_VS_ToF_epCDn;
    TH2D *h_theta_n_VS_ToF_epCDn;
    TH2D *h_phi_n_VS_ToF_epCDn;
    TH2D *h_P_miss_VS_ToF_epCDn;
    TH2D *h_theta_miss_VS_ToF_epCDn;
    TH2D *h_phi_miss_VS_ToF_epCDn;
    TH2D *h_dpp_VS_ToF_epCDn;
    TH2D *h_beta_n_VS_ToF_epCDn;
    TH2D *h_E_miss_VS_ToF_epCDn;
    TH2D *h_M_miss_VS_ToF_epCDn;
    TH2D *h_path_VS_ToF_epCDn;
    TH2D *h_theta_n_miss_VS_ToF_epCDn;
    TH2D *h_nSector_VS_ToF_epCDn;

    TH1D *h_ToF_epFDn;
    TH1D *h_ToF_zoomout_epFDn;
    TH2D *h_P_n_VS_ToF_epFDn;
    TH2D *h_theta_n_VS_ToF_epFDn;
    TH2D *h_phi_n_VS_ToF_epFDn;
    TH2D *h_P_miss_VS_ToF_epFDn;
    TH2D *h_theta_miss_VS_ToF_epFDn;
    TH2D *h_phi_miss_VS_ToF_epFDn;
    TH2D *h_dpp_VS_ToF_epFDn;
    TH2D *h_beta_n_VS_ToF_epFDn;
    TH2D *h_E_miss_VS_ToF_epFDn;
    TH2D *h_M_miss_VS_ToF_epFDn;
    TH2D *h_path_VS_ToF_epFDn;
    TH2D *h_theta_n_miss_VS_ToF_epFDn;
    TH2D *h_nSector_VS_ToF_epFDn;

#pragma endregion /* (e,e'p) plots - end */

    // Step Zero (Andrew)
    // ======================================================================================================================================================================

#pragma region /* Step Zero (Andrew) - start */

    /* Neutron histograms (from Erin) */
    TH1D *h_n_multiplicity_allN_epCDn_Step0;
    TH1D *h_n_multiplicity_goodN_epCDn_Step0;
    TH1D *h_n_multiplicity_badN_epCDn_Step0;

    TH1D *h_n_multiplicity_allN_epFDn_Step0;
    TH1D *h_n_multiplicity_goodN_epFDn_Step0;
    TH1D *h_n_multiplicity_badN_epFDn_Step0;

    /* Step0 cuts */
    TH1D *h_dbeta_n_BS0C_Step0_epCDn;
    TH2D *h_dbeta_n_VS_P_n_BS0C_Step0_epCDn;
    TH2D *h_dbeta_n_VS_ToF_BS0C_Step0_epCDn;
    TH1D *h_dbeta_n_AS0C_Step0_epCDn;
    TH2D *h_dbeta_n_VS_P_n_AS0C_Step0_epCDn;
    TH2D *h_dbeta_n_VS_ToF_AS0C_Step0_epCDn;

    TH1D *h_dbeta_n_BS0C_Step0_epFDn;
    TH2D *h_dbeta_n_VS_P_n_BS0C_Step0_epFDn;
    TH2D *h_dbeta_n_VS_ToF_BS0C_Step0_epFDn;
    TH1D *h_dbeta_n_AS0C_Step0_epFDn;
    TH2D *h_dbeta_n_VS_P_n_AS0C_Step0_epFDn;
    TH2D *h_dbeta_n_VS_ToF_AS0C_Step0_epFDn;

    TH1D *h_Vhit_z_n_BS0C_Step0_epCDn;
    TH1D *h_Vhit_z_n_AS0C_Step0_epCDn;

    TH1D *h_Vhit_z_n_BS0C_Step0_epFDn;
    TH1D *h_Vhit_z_n_AS0C_Step0_epFDn;

    TH1D *h_ToF_n_BS0C_Step0_epCDn;
    TH1D *h_ToF_n_AS0C_Step0_epCDn;

    TH1D *h_ToF_n_BS0C_Step0_epFDn;
    TH1D *h_ToF_n_AS0C_Step0_epFDn;

    /* Kinematical variables */
    TH1D *h_theta_n_goodN_Step0_epCDn;
    TH1D *h_theta_n_badN_Step0_epCDn;
    TH1D *h_phi_n_goodN_Step0_epCDn;
    TH1D *h_phi_n_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_phi_n_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_phi_n_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_beta_n_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_beta_n_badN_Step0_epCDn;

    TH1D *h_theta_n_goodN_Step0_epFDn;
    TH1D *h_theta_n_badN_Step0_epFDn;
    TH1D *h_phi_n_goodN_Step0_epFDn;
    TH1D *h_phi_n_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_phi_n_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_phi_n_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_beta_n_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_beta_n_badN_Step0_epFDn;

    TH1D *h_P_n_goodN_Step0_epCDn;
    TH1D *h_P_n_badN_Step0_epCDn;
    TH2D *h_P_n_VS_theta_n_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_theta_n_badN_Step0_epCDn;

    TH1D *h_P_n_goodN_Step0_epFDn;
    TH1D *h_P_n_badN_Step0_epFDn;
    TH2D *h_P_n_VS_theta_n_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_theta_n_badN_Step0_epFDn;

    TH1D *h_P_miss_goodN_Step0_epCDn;
    TH1D *h_P_miss_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_theta_miss_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_theta_miss_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_phi_miss_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_phi_miss_badN_Step0_epCDn;

    TH1D *h_P_miss_goodN_Step0_epFDn;
    TH1D *h_P_miss_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_theta_miss_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_theta_miss_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_phi_miss_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_phi_miss_badN_Step0_epFDn;

    TH1D *h_dpp_allN_Step0_epCDn;
    TH1D *h_dpp_goodN_Step0_epCDn;
    TH1D *h_dpp_badN_Step0_epCDn;

    TH1D *h_dpp_allN_Step0_epFDn;
    TH1D *h_dpp_goodN_Step0_epFDn;
    TH1D *h_dpp_badN_Step0_epFDn;

    TH1D *h_theta_n_miss_allN_Step0_epCDn;
    TH1D *h_theta_n_miss_goodN_Step0_epCDn;
    TH1D *h_theta_n_miss_badN_Step0_epCDn;

    TH1D *h_theta_n_miss_allN_Step0_epFDn;
    TH1D *h_theta_n_miss_goodN_Step0_epFDn;
    TH1D *h_theta_n_miss_badN_Step0_epFDn;

    TH2D *h_dpp_VS_theta_n_miss_allN_Step0_epCDn;

    TH2D *h_dpp_VS_theta_n_miss_allN_Step0_epFDn;

    TH1D *h_E_p_goodN_Step0_epCDn;
    TH1D *h_E_p_badN_Step0_epCDn;
    TH1D *h_E_miss_goodN_Step0_epCDn;
    TH1D *h_E_miss_badN_Step0_epCDn;
    TH1D *h_M_miss_goodN_Step0_epCDn;
    TH1D *h_M_miss_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_P_n_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_P_n_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_theta_n_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_theta_n_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_phi_n_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_phi_n_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_P_miss_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_P_miss_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_theta_miss_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_theta_miss_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_phi_miss_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_phi_miss_badN_Step0_epCDn;

    TH1D *h_E_p_goodN_Step0_epFDn;
    TH1D *h_E_p_badN_Step0_epFDn;
    TH1D *h_E_miss_goodN_Step0_epFDn;
    TH1D *h_E_miss_badN_Step0_epFDn;
    TH1D *h_M_miss_goodN_Step0_epFDn;
    TH1D *h_M_miss_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_P_n_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_P_n_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_theta_n_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_theta_n_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_phi_n_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_phi_n_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_P_miss_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_P_miss_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_theta_miss_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_theta_miss_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_phi_miss_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_phi_miss_badN_Step0_epFDn;

    TH1D *h_P_n_minus_P_miss_goodN_Step0_epCDn;
    TH1D *h_P_n_minus_P_miss_badN_Step0_epCDn;
    TH1D *h_P_n_x_minus_P_miss_x_goodN_Step0_epCDn;
    TH1D *h_P_n_x_minus_P_miss_x_badN_Step0_epCDn;
    TH1D *h_P_n_y_minus_P_miss_y_goodN_Step0_epCDn;
    TH1D *h_P_n_y_minus_P_miss_y_badN_Step0_epCDn;
    TH1D *h_P_n_z_minus_P_miss_z_goodN_Step0_epCDn;
    TH1D *h_P_n_z_minus_P_miss_z_badN_Step0_epCDn;

    TH1D *h_P_n_minus_P_miss_goodN_Step0_epFDn;
    TH1D *h_P_n_minus_P_miss_badN_Step0_epFDn;
    TH1D *h_P_n_x_minus_P_miss_x_goodN_Step0_epFDn;
    TH1D *h_P_n_x_minus_P_miss_x_badN_Step0_epFDn;
    TH1D *h_P_n_y_minus_P_miss_y_goodN_Step0_epFDn;
    TH1D *h_P_n_y_minus_P_miss_y_badN_Step0_epFDn;
    TH1D *h_P_n_z_minus_P_miss_z_goodN_Step0_epFDn;
    TH1D *h_P_n_z_minus_P_miss_z_badN_Step0_epFDn;

    TH2D *h_P_n_VS_P_miss_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_P_miss_badN_Step0_epCDn;
    TH2D *h_P_n_x_VS_P_miss_x_goodN_Step0_epCDn;
    TH2D *h_P_n_x_VS_P_miss_x_badN_Step0_epCDn;
    TH2D *h_P_n_y_VS_P_miss_y_goodN_Step0_epCDn;
    TH2D *h_P_n_y_VS_P_miss_y_badN_Step0_epCDn;
    TH2D *h_P_n_z_VS_P_miss_z_goodN_Step0_epCDn;
    TH2D *h_P_n_z_VS_P_miss_z_badN_Step0_epCDn;

    TH2D *h_P_n_VS_P_miss_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_P_miss_badN_Step0_epFDn;
    TH2D *h_P_n_x_VS_P_miss_x_goodN_Step0_epFDn;
    TH2D *h_P_n_x_VS_P_miss_x_badN_Step0_epFDn;
    TH2D *h_P_n_y_VS_P_miss_y_goodN_Step0_epFDn;
    TH2D *h_P_n_y_VS_P_miss_y_badN_Step0_epFDn;
    TH2D *h_P_n_z_VS_P_miss_z_goodN_Step0_epFDn;
    TH2D *h_P_n_z_VS_P_miss_z_badN_Step0_epFDn;

    TH1D *h_theta_n_p_goodN_Step0_epCDn;
    TH1D *h_theta_n_p_badN_Step0_epCDn;
    TH2D *h_theta_n_p_VS_P_p_goodN_Step0_epCDn;
    TH2D *h_theta_n_p_VS_P_p_badN_Step0_epCDn;

    TH1D *h_theta_n_p_goodN_Step0_epFDn;
    TH1D *h_theta_n_p_badN_Step0_epFDn;
    TH2D *h_theta_n_p_VS_P_p_goodN_Step0_epFDn;
    TH2D *h_theta_n_p_VS_P_p_badN_Step0_epFDn;

    TH1D *h_xB_goodN_Step0_epCDn;
    TH1D *h_xB_badN_Step0_epCDn;

    TH1D *h_xB_goodN_Step0_epFDn;
    TH1D *h_xB_badN_Step0_epFDn;

    /* Detector responses */
    TH1D *h_Edep_CND_goodN_Step0_epCDn;
    TH1D *h_Edep_CND_badN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_path_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_path_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_badN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_goodN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_badN_Step0_epCDn;

    TH1D *h_Edep_CND_goodN_Step0_epFDn;
    TH1D *h_Edep_CND_badN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_path_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_path_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_badN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_goodN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_badN_Step0_epFDn;

    TH1D *h_Edep_CTOF_goodN_Step0_epCDn;
    TH1D *h_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_path_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_path_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_badN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_goodN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_badN_Step0_epCDn;

    TH1D *h_Edep_CTOF_goodN_Step0_epFDn;
    TH1D *h_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_path_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_path_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_badN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_goodN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_badN_Step0_epFDn;

    TH1D *h_Edep_single_goodN_Step0_epCDn;
    TH1D *h_Edep_single_badN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_path_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_path_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_single_badN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_single_goodN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_single_badN_Step0_epCDn;

    TH1D *h_Edep_single_goodN_Step0_epFDn;
    TH1D *h_Edep_single_badN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_path_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_path_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_single_badN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_single_goodN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_single_badN_Step0_epFDn;

    TH1D *h_Edep_CND1_goodN_Step0_epCDn;
    TH1D *h_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_path_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_path_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_badN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_goodN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_badN_Step0_epCDn;

    TH1D *h_Edep_CND1_goodN_Step0_epFDn;
    TH1D *h_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_path_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_path_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_badN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_goodN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_badN_Step0_epFDn;

    TH1D *h_Edep_CND2_goodN_Step0_epCDn;
    TH1D *h_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_path_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_path_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_CND2_badN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_goodN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_badN_Step0_epCDn;

    TH1D *h_Edep_CND2_goodN_Step0_epFDn;
    TH1D *h_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_path_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_path_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_CND2_badN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_goodN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_badN_Step0_epFDn;

    TH1D *h_Edep_CND3_goodN_Step0_epCDn;
    TH1D *h_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_phi_n_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_dpp_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_beta_n_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_E_p_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_E_miss_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_path_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_path_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_ToF_VS_Edep_CND3_badN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_CND3_goodN_Step0_epCDn;
    TH2D *h_nSector_VS_Edep_CND3_badN_Step0_epCDn;

    TH1D *h_Edep_CND3_goodN_Step0_epFDn;
    TH1D *h_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_phi_n_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_dpp_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_beta_n_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_E_p_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_E_miss_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_path_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_path_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_ToF_VS_Edep_CND3_badN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_CND3_goodN_Step0_epFDn;
    TH2D *h_nSector_VS_Edep_CND3_badN_Step0_epFDn;

    TH1D *h_Size_CND1_goodN_Step0_epCDn;
    TH1D *h_Size_CND1_badN_Step0_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_P_n_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_phi_n_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_phi_n_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_dpp_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_dpp_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_beta_n_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_beta_n_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_E_p_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_E_p_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_E_miss_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_E_miss_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_path_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_path_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_ToF_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_ToF_VS_Size_CND1_badN_Step0_epCDn;
    TH2D *h_nSector_VS_Size_CND1_goodN_Step0_epCDn;
    TH2D *h_nSector_VS_Size_CND1_badN_Step0_epCDn;

    TH1D *h_Size_CND1_goodN_Step0_epFDn;
    TH1D *h_Size_CND1_badN_Step0_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_P_n_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_phi_n_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_phi_n_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_dpp_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_dpp_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_beta_n_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_beta_n_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_E_p_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_E_p_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_E_miss_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_E_miss_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_path_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_path_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_ToF_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_ToF_VS_Size_CND1_badN_Step0_epFDn;
    TH2D *h_nSector_VS_Size_CND1_goodN_Step0_epFDn;
    TH2D *h_nSector_VS_Size_CND1_badN_Step0_epFDn;

    TH1D *h_Size_CND2_goodN_Step0_epCDn;
    TH1D *h_Size_CND2_badN_Step0_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_P_n_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_phi_n_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_phi_n_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_dpp_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_dpp_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_beta_n_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_beta_n_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_E_p_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_E_p_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_E_miss_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_E_miss_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_path_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_path_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_ToF_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_ToF_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_nSector_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_nSector_VS_Size_CND2_badN_Step0_epCDn;

    TH1D *h_Size_CND2_goodN_Step0_epFDn;
    TH1D *h_Size_CND2_badN_Step0_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_P_n_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_phi_n_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_phi_n_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_dpp_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_dpp_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_beta_n_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_beta_n_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_E_p_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_E_p_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_E_miss_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_E_miss_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_path_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_path_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_ToF_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_ToF_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_nSector_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_nSector_VS_Size_CND2_badN_Step0_epFDn;

    TH1D *h_Size_CND3_goodN_Step0_epCDn;
    TH1D *h_Size_CND3_badN_Step0_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_P_n_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_phi_n_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_phi_n_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_theta_miss_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_phi_miss_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_dpp_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_dpp_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_beta_n_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_beta_n_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_E_p_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_E_p_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_E_miss_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_E_miss_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_path_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_path_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_ToF_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_ToF_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_nSector_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_nSector_VS_Size_CND3_badN_Step0_epCDn;

    TH1D *h_Size_CND3_goodN_Step0_epFDn;
    TH1D *h_Size_CND3_badN_Step0_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_P_n_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_phi_n_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_phi_n_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_theta_miss_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_phi_miss_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_dpp_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_dpp_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_beta_n_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_beta_n_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_E_p_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_E_p_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_E_miss_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_E_miss_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_path_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_path_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_ToF_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_ToF_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_nSector_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_nSector_VS_Size_CND3_badN_Step0_epFDn;

    TH2D *h_Size_CND1_VS_Size_CND2_goodN_Step0_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND2_badN_Step0_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND3_badN_Step0_epCDn;
    TH2D *h_Size_CND2_VS_Size_CND3_goodN_Step0_epCDn;
    TH2D *h_Size_CND2_VS_Size_CND3_badN_Step0_epCDn;

    TH2D *h_Size_CND1_VS_Size_CND2_goodN_Step0_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND2_badN_Step0_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND3_badN_Step0_epFDn;
    TH2D *h_Size_CND2_VS_Size_CND3_goodN_Step0_epFDn;
    TH2D *h_Size_CND2_VS_Size_CND3_badN_Step0_epFDn;

    TH1D *h_ToF_goodN_Step0_epCDn;
    TH1D *h_ToF_badN_Step0_epCDn;
    TH2D *h_P_n_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_P_n_VS_ToF_badN_Step0_epCDn;
    TH2D *h_theta_n_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_theta_n_VS_ToF_badN_Step0_epCDn;
    TH2D *h_phi_n_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_phi_n_VS_ToF_badN_Step0_epCDn;
    TH2D *h_P_miss_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_P_miss_VS_ToF_badN_Step0_epCDn;
    TH2D *h_theta_miss_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_theta_miss_VS_ToF_badN_Step0_epCDn;
    TH2D *h_phi_miss_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_phi_miss_VS_ToF_badN_Step0_epCDn;
    TH2D *h_dpp_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_dpp_VS_ToF_badN_Step0_epCDn;
    TH2D *h_beta_n_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_beta_n_VS_ToF_badN_Step0_epCDn;
    TH2D *h_E_p_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_E_p_VS_ToF_badN_Step0_epCDn;
    TH2D *h_E_miss_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_E_miss_VS_ToF_badN_Step0_epCDn;
    TH2D *h_M_miss_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_M_miss_VS_ToF_badN_Step0_epCDn;
    TH2D *h_path_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_path_VS_ToF_badN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_theta_n_miss_VS_ToF_badN_Step0_epCDn;
    TH2D *h_nSector_VS_ToF_goodN_Step0_epCDn;
    TH2D *h_nSector_VS_ToF_badN_Step0_epCDn;

    TH1D *h_ToF_goodN_Step0_epFDn;
    TH1D *h_ToF_badN_Step0_epFDn;
    TH2D *h_P_n_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_P_n_VS_ToF_badN_Step0_epFDn;
    TH2D *h_theta_n_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_theta_n_VS_ToF_badN_Step0_epFDn;
    TH2D *h_phi_n_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_phi_n_VS_ToF_badN_Step0_epFDn;
    TH2D *h_P_miss_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_P_miss_VS_ToF_badN_Step0_epFDn;
    TH2D *h_theta_miss_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_theta_miss_VS_ToF_badN_Step0_epFDn;
    TH2D *h_phi_miss_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_phi_miss_VS_ToF_badN_Step0_epFDn;
    TH2D *h_dpp_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_dpp_VS_ToF_badN_Step0_epFDn;
    TH2D *h_beta_n_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_beta_n_VS_ToF_badN_Step0_epFDn;
    TH2D *h_E_p_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_E_p_VS_ToF_badN_Step0_epFDn;
    TH2D *h_E_miss_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_E_miss_VS_ToF_badN_Step0_epFDn;
    TH2D *h_M_miss_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_M_miss_VS_ToF_badN_Step0_epFDn;
    TH2D *h_path_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_path_VS_ToF_badN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_theta_n_miss_VS_ToF_badN_Step0_epFDn;
    TH2D *h_nSector_VS_ToF_goodN_Step0_epFDn;
    TH2D *h_nSector_VS_ToF_badN_Step0_epFDn;

    TH1D *h_beta_n_goodN_Step0_epCDn;
    TH1D *h_beta_n_badN_Step0_epCDn;

    TH1D *h_beta_n_goodN_Step0_epFDn;
    TH1D *h_beta_n_badN_Step0_epFDn;

    /*
    // TODO: add these to code below
    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_goodN_Step0_epCDn;
    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_badN_Step0_epCDn;

    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_goodN_Step0_epFDn;
    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_badN_Step0_epFDn;

    TH1D *h_Edep_CND_goodN_withNearbyPos_Step0_epCDn;
    TH1D *h_Edep_CND_badN_withNearbyPos_Step0_epCDn;

    TH1D *h_Edep_CND_goodN_withNearbyPos_Step0_epFDn;
    TH1D *h_Edep_CND_badN_withNearbyPos_Step0_epFDn;

    TH1D *h_sdiff_pos_goodN_Step0_layer_epCDn[7];
    TH1D *h_sdiff_pos_badN_Step0_layer_epCDn[7];

    TH1D *h_sdiff_pos_goodN_Step0_layer_epFDn[7];
    TH1D *h_sdiff_pos_badN_Step0_layer_epFDn[7];

    TH2D *h_sdiff_pos_mom_goodN_Step0_layer_epCDn[7];
    TH2D *h_sdiff_pos_mom_badN_Step0_layer_epCDn[7];

    TH2D *h_sdiff_pos_mom_goodN_Step0_layer_epFDn[7];
    TH2D *h_sdiff_pos_mom_badN_Step0_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_VhitZ_goodN_Step0_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_VhitZ_badN_Step0_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_VhitZ_goodN_Step0_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_VhitZ_badN_Step0_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step0_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step0_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step0_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step0_layer_epFDn[7];

    for (int k = 0; k < 7; k++)
    {
        sprintf(temp_name, "sdiff_pos_goodN_Step0_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts", k - 3);
        h_sdiff_pos_goodN_Step0_layer_epCDn[k] = new TH1D(temp_name, temp_title, 24,  -11.5, 12.5);
        sprintf(temp_name, "sdiff_pos_badN_Step0_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts", k - 3);
        h_sdiff_pos_badN_Step0_layer_epCDn[k] = new TH1D(temp_name, temp_title, 24,  -11.5, 12.5);

        sprintf(temp_name, "sdiff_pos_goodN_Step0_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts", k - 3);
        h_sdiff_pos_goodN_Step0_layer_epFDn[k] = new TH1D(temp_name, temp_title, 24,  -11.5, 12.5);
        sprintf(temp_name, "sdiff_pos_badN_Step0_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts", k - 3);
        h_sdiff_pos_badN_Step0_layer_epFDn[k] = new TH1D(temp_name, temp_title, 24,  -11.5, 12.5);

        sprintf(temp_name, "sdiff_pos_mom_goodN_Step0_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]", k - 3);
        h_sdiff_pos_mom_goodN_Step0_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, 0.3, 1.5);
        sprintf(temp_name, "sdiff_pos_mom_badN_Step0_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]", k - 3);
        h_sdiff_pos_mom_badN_Step0_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, 0.3, 1.5);

        sprintf(temp_name, "sdiff_pos_mom_goodN_Step0_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]", k - 3);
        h_sdiff_pos_mom_goodN_Step0_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, 0.4, 3.);
        sprintf(temp_name, "sdiff_pos_mom_badN_Step0_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]", k - 3);
        h_sdiff_pos_mom_badN_Step0_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, 0.4, 3.);

        sprintf(temp_name, "sdiff_pos_z_goodN_Step0_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]", k - 3);
        h_sdiff_pos_VS_VhitZ_goodN_Step0_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, -40.0, 40.0);
        sprintf(temp_name, "sdiff_pos_z_badN_Step0_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]", k - 3);
        h_sdiff_pos_VS_VhitZ_badN_Step0_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, -40.0, 40.0);

        sprintf(temp_name, "sdiff_pos_z_goodN_Step0_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]", k - 3);
        h_sdiff_pos_VS_VhitZ_goodN_Step0_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, -40.0, 40.0);
        sprintf(temp_name, "sdiff_pos_z_badN_Step0_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]", k - 3);
        h_sdiff_pos_VS_VhitZ_badN_Step0_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, -40.0, 40.0);

        sprintf(temp_name, "sdiff_pos_diff_ToFc_z_goodN_Step0_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]", k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step0_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, 0, 300);
        sprintf(temp_name, "sdiff_pos_diff_ToFc_z_badN_Step0_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]", k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step0_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, 0, 300);

        sprintf(temp_name, "sdiff_pos_diff_ToFc_z_goodN_Step0_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]", k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step0_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, 0, 300);
        sprintf(temp_name, "sdiff_pos_diff_ToFc_z_badN_Step0_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]", k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step0_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24,  -11.5, 12.5, 50, 0, 300);
    }

    TH2D *h_diff_ToFc_z_VS_Edep_noNear_goodN_Step0_epCDn;
    TH2D *h_diff_ToFc_z_VS_Edep_noNear_badN_Step0_epCDn;

    TH2D *h_diff_ToFc_z_VS_Edep_noNear_goodN_Step0_epFDn;
    TH2D *h_diff_ToFc_z_VS_Edep_noNear_badN_Step0_epFDn;

    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_goodN_Step0_epCDn;
    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_badN_Step0_epCDn;

    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_goodN_Step0_epFDn;
    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_badN_Step0_epFDn;



    for (int k = 0; k < 3; k++)
    {
        sprintf(temp_name, "diff_ToFc_z_goodN_Step0_layer_%d_epCDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_goodN_Step0_layer_epCDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);
        sprintf(temp_name, "diff_ToFc_z_badN_Step0_layer_%d_epCDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_badN_Step0_layer_epCDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);

        sprintf(temp_name, "diff_ToFc_z_goodN_Step0_layer_%d_epFDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_goodN_Step0_layer_epFDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);
        sprintf(temp_name, "diff_ToFc_z_badN_Step0_layer_%d_epFDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_badN_Step0_layer_epFDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);
    }

    TH2D *h_sdiff_ldiff_allhit_goodN_Step0_epCDn;
    TH2D *h_sdiff_ldiff_allhit_badN_Step0_epCDn;

    TH2D *h_sdiff_ldiff_allhit_goodN_Step0_epFDn;
    TH2D *h_sdiff_ldiff_allhit_badN_Step0_epFDn;

    TH1D *h_numberNearby_goodN_Step0_epCDn;
    TH1D *h_numberNearby_badN_Step0_epCDn;

    TH1D *h_numberNearby_goodN_Step0_epFDn;
    TH1D *h_numberNearby_badN_Step0_epFDn;

    TH2D *h_numberNearby_momN_goodN_Step0_epCDn;
    TH2D *h_numberNearby_momN_badN_Step0_epCDn;

    TH2D *h_numberNearby_momN_goodN_Step0_epFDn;
    TH2D *h_numberNearby_momN_badN_Step0_epFDn;

    TH1D *h_NearbyEdep_goodN_Step0_epCDn;
    TH1D *h_NearbyEdep_badN_Step0_epCDn;

    TH1D *h_NearbyEdep_goodN_Step0_epFDn;
    TH1D *h_NearbyEdep_badN_Step0_epFDn;

    TH1D *h_nsector_goodN_Step0_epCDn;
    TH1D *h_nsector_badN_Step0_epCDn;

    TH1D *h_nsector_goodN_Step0_epFDn;
    TH1D *h_nsector_badN_Step0_epFDn;

    TH1D *h_phidiff_en_goodN_Step0_epCDn;
    TH1D *h_phidiff_en_badN_Step0_epCDn;

    TH1D *h_phidiff_en_goodN_Step0_epFDn;
    TH1D *h_phidiff_en_badN_Step0_epFDn;

    TH1D *h_TP_goodN_Step0_epCDn;
    TH1D *h_TP_badN_Step0_epCDn;

    TH1D *h_TP_goodN_Step0_epFDn;
    TH1D *h_TP_badN_Step0_epFDn;

    TH1D *h_Z_goodN_Step0_epCDn;
    TH1D *h_Z_badN_Step0_epCDn;

    TH1D *h_Z_goodN_Step0_epFDn;
    TH1D *h_Z_badN_Step0_epFDn;
    */

#pragma endregion /* Step Zero (Andrew) - end */

    // Step One (After Edep_CND Cut) (Andrew)
    // ======================================================================================================================================================================

#pragma region /* Step One (After Edep_CND Cut) (Andrew) - start */

    /* Neutron histograms (from Erin) */
    TH1D *h_n_multiplicity_allN_epCDn_Step1;
    TH1D *h_n_multiplicity_goodN_epCDn_Step1;
    TH1D *h_n_multiplicity_badN_epCDn_Step1;

    TH1D *h_n_multiplicity_allN_epFDn_Step1;
    TH1D *h_n_multiplicity_goodN_epFDn_Step1;
    TH1D *h_n_multiplicity_badN_epFDn_Step1;

    /* Step1 cuts */
    /*
    TH2D *h_dbeta_n_VS_P_n_BS1C_Step1_epCDn;
    TH2D *h_dbeta_n_VS_ToF_BS1C_Step1_epCDn;
    TH2D *h_dbeta_n_VS_P_n_AS1C_Step1_epCDn;
    TH2D *h_dbeta_n_VS_ToF_AS1C_Step1_epCDn;

    TH2D *h_dbeta_n_VS_P_n_BS1C_Step1_epFDn;
    TH2D *h_dbeta_n_VS_ToF_BS1C_Step1_epFDn;
    TH2D *h_dbeta_n_VS_P_n_AS1C_Step1_epFDn;
    TH2D *h_dbeta_n_VS_ToF_AS1C_Step1_epFDn;

    TH1D *h_Vhit_z_n_BS1C_Step1_epCDn;
    TH1D *h_Vhit_z_n_AS1C_Step1_epCDn;

    TH1D *h_Vhit_z_n_BS1C_Step1_epFDn;
    TH1D *h_Vhit_z_n_AS1C_Step1_epFDn;

    TH1D *h_ToF_n_BS1C_Step1_epCDn;
    TH1D *h_ToF_n_AS1C_Step1_epCDn;

    TH1D *h_ToF_n_BS1C_Step1_epFDn;
    TH1D *h_ToF_n_AS1C_Step1_epFDn;

    TH1D *h_beta_n_BS1C_Step1_epFDn;
    TH1D *h_beta_n_AS1C_Step1_epFDn;
 */

    /* Kinematical variables */
    TH1D *h_theta_n_goodN_Step1_epCDn;
    TH1D *h_theta_n_badN_Step1_epCDn;
    TH1D *h_phi_n_goodN_Step1_epCDn;
    TH1D *h_phi_n_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_phi_n_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_phi_n_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_beta_n_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_beta_n_badN_Step1_epCDn;

    TH1D *h_theta_n_goodN_Step1_epFDn;
    TH1D *h_theta_n_badN_Step1_epFDn;
    TH1D *h_phi_n_goodN_Step1_epFDn;
    TH1D *h_phi_n_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_phi_n_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_phi_n_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_beta_n_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_beta_n_badN_Step1_epFDn;

    TH1D *h_P_n_goodN_Step1_epCDn;
    TH1D *h_P_n_badN_Step1_epCDn;
    TH2D *h_P_n_VS_theta_n_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_theta_n_badN_Step1_epCDn;

    TH1D *h_P_n_goodN_Step1_epFDn;
    TH1D *h_P_n_badN_Step1_epFDn;
    TH2D *h_P_n_VS_theta_n_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_theta_n_badN_Step1_epFDn;

    TH1D *h_P_miss_goodN_Step1_epCDn;
    TH1D *h_P_miss_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_theta_miss_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_theta_miss_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_phi_miss_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_phi_miss_badN_Step1_epCDn;

    TH1D *h_P_miss_goodN_Step1_epFDn;
    TH1D *h_P_miss_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_theta_miss_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_theta_miss_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_phi_miss_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_phi_miss_badN_Step1_epFDn;

    TH1D *h_dpp_allN_Step1_epCDn;
    TH1D *h_dpp_goodN_Step1_epCDn;
    TH1D *h_dpp_badN_Step1_epCDn;

    TH1D *h_dpp_allN_Step1_epFDn;
    TH1D *h_dpp_goodN_Step1_epFDn;
    TH1D *h_dpp_badN_Step1_epFDn;

    TH1D *h_theta_n_miss_allN_Step1_epCDn;
    TH1D *h_theta_n_miss_goodN_Step1_epCDn;
    TH1D *h_theta_n_miss_badN_Step1_epCDn;

    TH1D *h_theta_n_miss_allN_Step1_epFDn;
    TH1D *h_theta_n_miss_goodN_Step1_epFDn;
    TH1D *h_theta_n_miss_badN_Step1_epFDn;

    TH2D *h_dpp_VS_theta_n_miss_allN_Step1_epCDn;

    TH2D *h_dpp_VS_theta_n_miss_allN_Step1_epFDn;

    TH1D *h_E_p_goodN_Step1_epCDn;
    TH1D *h_E_p_badN_Step1_epCDn;
    TH1D *h_E_miss_goodN_Step1_epCDn;
    TH1D *h_E_miss_badN_Step1_epCDn;
    TH1D *h_M_miss_goodN_Step1_epCDn;
    TH1D *h_M_miss_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_P_n_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_P_n_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_theta_n_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_theta_n_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_phi_n_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_phi_n_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_P_miss_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_P_miss_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_theta_miss_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_theta_miss_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_phi_miss_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_phi_miss_badN_Step1_epCDn;

    TH1D *h_E_p_goodN_Step1_epFDn;
    TH1D *h_E_p_badN_Step1_epFDn;
    TH1D *h_E_miss_goodN_Step1_epFDn;
    TH1D *h_E_miss_badN_Step1_epFDn;
    TH1D *h_M_miss_goodN_Step1_epFDn;
    TH1D *h_M_miss_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_P_n_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_P_n_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_theta_n_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_theta_n_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_phi_n_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_phi_n_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_P_miss_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_P_miss_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_theta_miss_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_theta_miss_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_phi_miss_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_phi_miss_badN_Step1_epFDn;

    TH1D *h_P_n_minus_P_miss_goodN_Step1_epCDn;
    TH1D *h_P_n_minus_P_miss_badN_Step1_epCDn;
    TH1D *h_P_n_x_minus_P_miss_x_goodN_Step1_epCDn;
    TH1D *h_P_n_x_minus_P_miss_x_badN_Step1_epCDn;
    TH1D *h_P_n_y_minus_P_miss_y_goodN_Step1_epCDn;
    TH1D *h_P_n_y_minus_P_miss_y_badN_Step1_epCDn;
    TH1D *h_P_n_z_minus_P_miss_z_goodN_Step1_epCDn;
    TH1D *h_P_n_z_minus_P_miss_z_badN_Step1_epCDn;

    TH1D *h_P_n_minus_P_miss_goodN_Step1_epFDn;
    TH1D *h_P_n_minus_P_miss_badN_Step1_epFDn;
    TH1D *h_P_n_x_minus_P_miss_x_goodN_Step1_epFDn;
    TH1D *h_P_n_x_minus_P_miss_x_badN_Step1_epFDn;
    TH1D *h_P_n_y_minus_P_miss_y_goodN_Step1_epFDn;
    TH1D *h_P_n_y_minus_P_miss_y_badN_Step1_epFDn;
    TH1D *h_P_n_z_minus_P_miss_z_goodN_Step1_epFDn;
    TH1D *h_P_n_z_minus_P_miss_z_badN_Step1_epFDn;

    TH2D *h_P_n_VS_P_miss_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_P_miss_badN_Step1_epCDn;
    TH2D *h_P_n_x_VS_P_miss_x_goodN_Step1_epCDn;
    TH2D *h_P_n_x_VS_P_miss_x_badN_Step1_epCDn;
    TH2D *h_P_n_y_VS_P_miss_y_goodN_Step1_epCDn;
    TH2D *h_P_n_y_VS_P_miss_y_badN_Step1_epCDn;
    TH2D *h_P_n_z_VS_P_miss_z_goodN_Step1_epCDn;
    TH2D *h_P_n_z_VS_P_miss_z_badN_Step1_epCDn;

    TH2D *h_P_n_VS_P_miss_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_P_miss_badN_Step1_epFDn;
    TH2D *h_P_n_x_VS_P_miss_x_goodN_Step1_epFDn;
    TH2D *h_P_n_x_VS_P_miss_x_badN_Step1_epFDn;
    TH2D *h_P_n_y_VS_P_miss_y_goodN_Step1_epFDn;
    TH2D *h_P_n_y_VS_P_miss_y_badN_Step1_epFDn;
    TH2D *h_P_n_z_VS_P_miss_z_goodN_Step1_epFDn;
    TH2D *h_P_n_z_VS_P_miss_z_badN_Step1_epFDn;

    TH1D *h_theta_n_p_goodN_Step1_epCDn;
    TH1D *h_theta_n_p_badN_Step1_epCDn;
    TH2D *h_theta_n_p_VS_P_p_goodN_Step1_epCDn;
    TH2D *h_theta_n_p_VS_P_p_badN_Step1_epCDn;

    TH1D *h_theta_n_p_goodN_Step1_epFDn;
    TH1D *h_theta_n_p_badN_Step1_epFDn;
    TH2D *h_theta_n_p_VS_P_p_goodN_Step1_epFDn;
    TH2D *h_theta_n_p_VS_P_p_badN_Step1_epFDn;

    TH1D *h_xB_goodN_Step1_epCDn;
    TH1D *h_xB_badN_Step1_epCDn;

    TH1D *h_xB_goodN_Step1_epFDn;
    TH1D *h_xB_badN_Step1_epFDn;

    /* Detector responses */
    TH1D *h_Edep_CND_goodN_Step1_epCDn;
    TH1D *h_Edep_CND_badN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_path_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_path_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_badN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_goodN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_badN_Step1_epCDn;

    TH1D *h_Edep_CND_goodN_Step1_epFDn;
    TH1D *h_Edep_CND_badN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_path_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_path_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_badN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_goodN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_badN_Step1_epFDn;

    TH1D *h_Edep_CTOF_goodN_Step1_epCDn;
    TH1D *h_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_path_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_path_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_badN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_goodN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_badN_Step1_epCDn;

    TH1D *h_Edep_CTOF_goodN_Step1_epFDn;
    TH1D *h_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_path_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_path_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_badN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_goodN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_badN_Step1_epFDn;

    TH1D *h_Edep_single_goodN_Step1_epCDn;
    TH1D *h_Edep_single_badN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_path_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_path_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_single_badN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_single_goodN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_single_badN_Step1_epCDn;

    TH1D *h_Edep_single_goodN_Step1_epFDn;
    TH1D *h_Edep_single_badN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_path_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_path_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_single_badN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_single_goodN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_single_badN_Step1_epFDn;

    TH1D *h_Edep_CND1_goodN_Step1_epCDn;
    TH1D *h_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_path_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_path_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_badN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_goodN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_badN_Step1_epCDn;

    TH1D *h_Edep_CND1_goodN_Step1_epFDn;
    TH1D *h_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_path_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_path_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_badN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_goodN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_badN_Step1_epFDn;

    TH1D *h_Edep_CND2_goodN_Step1_epCDn;
    TH1D *h_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_path_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_path_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_CND2_badN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_goodN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_badN_Step1_epCDn;

    TH1D *h_Edep_CND2_goodN_Step1_epFDn;
    TH1D *h_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_path_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_path_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_CND2_badN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_goodN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_badN_Step1_epFDn;

    TH1D *h_Edep_CND3_goodN_Step1_epCDn;
    TH1D *h_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_phi_n_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_dpp_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_beta_n_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_E_p_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_E_miss_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_path_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_path_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_ToF_VS_Edep_CND3_badN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_CND3_goodN_Step1_epCDn;
    TH2D *h_nSector_VS_Edep_CND3_badN_Step1_epCDn;

    TH1D *h_Edep_CND3_goodN_Step1_epFDn;
    TH1D *h_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_phi_n_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_dpp_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_beta_n_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_E_p_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_E_miss_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_path_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_path_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_ToF_VS_Edep_CND3_badN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_CND3_goodN_Step1_epFDn;
    TH2D *h_nSector_VS_Edep_CND3_badN_Step1_epFDn;

    TH1D *h_Size_CND1_goodN_Step1_epCDn;
    TH1D *h_Size_CND1_badN_Step1_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_P_n_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_phi_n_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_phi_n_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_dpp_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_dpp_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_beta_n_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_beta_n_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_E_p_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_E_p_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_E_miss_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_E_miss_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_path_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_path_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_ToF_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_ToF_VS_Size_CND1_badN_Step1_epCDn;
    TH2D *h_nSector_VS_Size_CND1_goodN_Step1_epCDn;
    TH2D *h_nSector_VS_Size_CND1_badN_Step1_epCDn;

    TH1D *h_Size_CND1_goodN_Step1_epFDn;
    TH1D *h_Size_CND1_badN_Step1_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_P_n_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_phi_n_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_phi_n_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_dpp_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_dpp_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_beta_n_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_beta_n_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_E_p_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_E_p_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_E_miss_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_E_miss_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_path_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_path_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_ToF_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_ToF_VS_Size_CND1_badN_Step1_epFDn;
    TH2D *h_nSector_VS_Size_CND1_goodN_Step1_epFDn;
    TH2D *h_nSector_VS_Size_CND1_badN_Step1_epFDn;

    TH1D *h_Size_CND2_goodN_Step1_epCDn;
    TH1D *h_Size_CND2_badN_Step1_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_P_n_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_phi_n_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_phi_n_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_dpp_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_dpp_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_beta_n_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_beta_n_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_E_p_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_E_p_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_E_miss_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_E_miss_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_path_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_path_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_ToF_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_ToF_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_nSector_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_nSector_VS_Size_CND2_badN_Step1_epCDn;

    TH1D *h_Size_CND2_goodN_Step1_epFDn;
    TH1D *h_Size_CND2_badN_Step1_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_P_n_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_phi_n_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_phi_n_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_dpp_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_dpp_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_beta_n_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_beta_n_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_E_p_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_E_p_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_E_miss_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_E_miss_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_path_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_path_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_ToF_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_ToF_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_nSector_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_nSector_VS_Size_CND2_badN_Step1_epFDn;

    TH1D *h_Size_CND3_goodN_Step1_epCDn;
    TH1D *h_Size_CND3_badN_Step1_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_P_n_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_phi_n_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_phi_n_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_theta_miss_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_phi_miss_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_dpp_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_dpp_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_beta_n_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_beta_n_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_E_p_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_E_p_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_E_miss_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_E_miss_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_path_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_path_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_ToF_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_ToF_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_nSector_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_nSector_VS_Size_CND3_badN_Step1_epCDn;

    TH1D *h_Size_CND3_goodN_Step1_epFDn;
    TH1D *h_Size_CND3_badN_Step1_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_P_n_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_phi_n_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_phi_n_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_theta_miss_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_phi_miss_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_dpp_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_dpp_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_beta_n_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_beta_n_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_E_p_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_E_p_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_E_miss_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_E_miss_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_path_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_path_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_ToF_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_ToF_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_nSector_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_nSector_VS_Size_CND3_badN_Step1_epFDn;

    TH2D *h_Size_CND1_VS_Size_CND2_goodN_Step1_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND2_badN_Step1_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND3_badN_Step1_epCDn;
    TH2D *h_Size_CND2_VS_Size_CND3_goodN_Step1_epCDn;
    TH2D *h_Size_CND2_VS_Size_CND3_badN_Step1_epCDn;

    TH2D *h_Size_CND1_VS_Size_CND2_goodN_Step1_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND2_badN_Step1_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND3_badN_Step1_epFDn;
    TH2D *h_Size_CND2_VS_Size_CND3_goodN_Step1_epFDn;
    TH2D *h_Size_CND2_VS_Size_CND3_badN_Step1_epFDn;

    TH1D *h_ToF_goodN_Step1_epCDn;
    TH1D *h_ToF_badN_Step1_epCDn;
    TH2D *h_P_n_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_P_n_VS_ToF_badN_Step1_epCDn;
    TH2D *h_theta_n_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_theta_n_VS_ToF_badN_Step1_epCDn;
    TH2D *h_phi_n_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_phi_n_VS_ToF_badN_Step1_epCDn;
    TH2D *h_P_miss_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_P_miss_VS_ToF_badN_Step1_epCDn;
    TH2D *h_theta_miss_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_theta_miss_VS_ToF_badN_Step1_epCDn;
    TH2D *h_phi_miss_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_phi_miss_VS_ToF_badN_Step1_epCDn;
    TH2D *h_dpp_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_dpp_VS_ToF_badN_Step1_epCDn;
    TH2D *h_beta_n_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_beta_n_VS_ToF_badN_Step1_epCDn;
    TH2D *h_E_p_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_E_p_VS_ToF_badN_Step1_epCDn;
    TH2D *h_E_miss_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_E_miss_VS_ToF_badN_Step1_epCDn;
    TH2D *h_M_miss_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_M_miss_VS_ToF_badN_Step1_epCDn;
    TH2D *h_path_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_path_VS_ToF_badN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_theta_n_miss_VS_ToF_badN_Step1_epCDn;
    TH2D *h_nSector_VS_ToF_goodN_Step1_epCDn;
    TH2D *h_nSector_VS_ToF_badN_Step1_epCDn;

    TH1D *h_ToF_goodN_Step1_epFDn;
    TH1D *h_ToF_badN_Step1_epFDn;
    TH2D *h_P_n_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_P_n_VS_ToF_badN_Step1_epFDn;
    TH2D *h_theta_n_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_theta_n_VS_ToF_badN_Step1_epFDn;
    TH2D *h_phi_n_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_phi_n_VS_ToF_badN_Step1_epFDn;
    TH2D *h_P_miss_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_P_miss_VS_ToF_badN_Step1_epFDn;
    TH2D *h_theta_miss_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_theta_miss_VS_ToF_badN_Step1_epFDn;
    TH2D *h_phi_miss_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_phi_miss_VS_ToF_badN_Step1_epFDn;
    TH2D *h_dpp_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_dpp_VS_ToF_badN_Step1_epFDn;
    TH2D *h_beta_n_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_beta_n_VS_ToF_badN_Step1_epFDn;
    TH2D *h_E_p_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_E_p_VS_ToF_badN_Step1_epFDn;
    TH2D *h_E_miss_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_E_miss_VS_ToF_badN_Step1_epFDn;
    TH2D *h_M_miss_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_M_miss_VS_ToF_badN_Step1_epFDn;
    TH2D *h_path_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_path_VS_ToF_badN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_theta_n_miss_VS_ToF_badN_Step1_epFDn;
    TH2D *h_nSector_VS_ToF_goodN_Step1_epFDn;
    TH2D *h_nSector_VS_ToF_badN_Step1_epFDn;

    TH1D *h_beta_n_goodN_Step1_epCDn;
    TH1D *h_beta_n_badN_Step1_epCDn;

    TH1D *h_beta_n_goodN_Step1_epFDn;
    TH1D *h_beta_n_badN_Step1_epFDn;

    /*
    // TODO: add these to code below


    for (int k = 0; k < 3; k++)
    {
        sprintf(temp_name, "diff_ToFc_z_goodN_Step1_layer_%d_epCDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_goodN_Step1_layer_epCDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);
        sprintf(temp_name, "diff_ToFc_z_badN_Step1_layer_%d_epCDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_badN_Step1_layer_epCDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);

        sprintf(temp_name, "diff_ToFc_z_goodN_Step1_layer_%d_epFDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_goodN_Step1_layer_epFDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);
        sprintf(temp_name, "diff_ToFc_z_badN_Step1_layer_%d_epFDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_badN_Step1_layer_epFDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);
    }

    TH2D *h_sdiff_ldiff_allhit_goodN_Step1_epCDn;
    TH2D *h_sdiff_ldiff_allhit_badN_Step1_epCDn;

    TH2D *h_sdiff_ldiff_allhit_goodN_Step1_epFDn;
    TH2D *h_sdiff_ldiff_allhit_badN_Step1_epFDn;

    TH1D *h_numberNearby_goodN_Step1_epCDn;
    TH1D *h_numberNearby_badN_Step1_epCDn;

    TH1D *h_numberNearby_goodN_Step1_epFDn;
    TH1D *h_numberNearby_badN_Step1_epFDn;

    TH2D *h_numberNearby_momN_goodN_Step1_epCDn;
    TH2D *h_numberNearby_momN_badN_Step1_epCDn;

    TH2D *h_numberNearby_momN_goodN_Step1_epFDn;
    TH2D *h_numberNearby_momN_badN_Step1_epFDn;

    TH1D *h_NearbyEdep_goodN_Step1_epCDn;
    TH1D *h_NearbyEdep_badN_Step1_epCDn;

    TH1D *h_NearbyEdep_goodN_Step1_epFDn;
    TH1D *h_NearbyEdep_badN_Step1_epFDn;

    TH1D *h_nsector_goodN_Step1_epCDn;
    TH1D *h_nsector_badN_Step1_epCDn;

    TH1D *h_nsector_goodN_Step1_epFDn;
    TH1D *h_nsector_badN_Step1_epFDn;

    TH1D *h_phidiff_en_goodN_Step1_epCDn;
    TH1D *h_phidiff_en_badN_Step1_epCDn;

    TH1D *h_phidiff_en_goodN_Step1_epFDn;
    TH1D *h_phidiff_en_badN_Step1_epFDn;

    TH1D *h_TP_goodN_Step1_epCDn;
    TH1D *h_TP_badN_Step1_epCDn;

    TH1D *h_TP_goodN_Step1_epFDn;
    TH1D *h_TP_badN_Step1_epFDn;

    TH1D *h_Z_goodN_Step1_epCDn;
    TH1D *h_Z_badN_Step1_epCDn;

    TH1D *h_Z_goodN_Step1_epFDn;
    TH1D *h_Z_badN_Step1_epFDn;
    */

#pragma endregion /* Step One (After Edep_CND Cut) (Andrew) - end */

    // Step Two (After applying Phi Diff Charge Track cut) (Andrew)
    // ======================================================================================================================================================================

#pragma region /* Step Two (After applying Phi Diff Charge Track cut) (Andrew) - start */

    /* Neutron histograms (from Erin) */
    TH1D *h_n_multiplicity_allN_epCDn_Step2;
    TH1D *h_n_multiplicity_goodN_epCDn_Step2;
    TH1D *h_n_multiplicity_badN_epCDn_Step2;

    TH1D *h_n_multiplicity_allN_epFDn_Step2;
    TH1D *h_n_multiplicity_goodN_epFDn_Step2;
    TH1D *h_n_multiplicity_badN_epFDn_Step2;

    /* Step2 prep plots */
    /* ToF * c - v_hit_3v.Z() plots */
    TH1D *h_ToF_c_minus_VhitZ_BC_allN_Step2prep_epCDn;
    TH1D *h_ToF_c_minus_VhitZ_BC_goodN_Step2prep_epCDn;
    TH1D *h_ToF_c_minus_VhitZ_BC_badN_Step2prep_epCDn;
    // TH1D *h_ToF_c_minus_VhitZ_AC_allN_Step2prep_epCDn;
    // TH1D *h_ToF_c_minus_VhitZ_AC_goodN_Step2prep_epCDn;
    // TH1D *h_ToF_c_minus_VhitZ_AC_badN_Step2prep_epCDn;

    TH1D *h_ToF_c_minus_VhitZ_BC_allN_Step2prep_epFDn;
    TH1D *h_ToF_c_minus_VhitZ_BC_goodN_Step2prep_epFDn;
    TH1D *h_ToF_c_minus_VhitZ_BC_badN_Step2prep_epFDn;
    // TH1D *h_ToF_c_minus_VhitZ_AC_allN_Step2prep_epFDn;
    // TH1D *h_ToF_c_minus_VhitZ_AC_goodN_Step2prep_epFDn;
    // TH1D *h_ToF_c_minus_VhitZ_AC_badN_Step2prep_epFDn;

    TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_BC_allN_Step2prep_epCDn;
    TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_BC_goodN_Step2prep_epCDn;
    TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_BC_badN_Step2prep_epCDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_AC_allN_Step2prep_epCDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_AC_goodN_Step2prep_epCDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_AC_badN_Step2prep_epCDn;

    TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_BC_allN_Step2prep_epFDn;
    TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_BC_goodN_Step2prep_epFDn;
    TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_BC_badN_Step2prep_epFDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_AC_allN_Step2prep_epFDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_AC_goodN_Step2prep_epFDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_VhitZ_AC_badN_Step2prep_epFDn;

    TH2D *h_ToF_c_minus_VhitZ_VS_ToF_BC_allN_Step2prep_epCDn;
    TH2D *h_ToF_c_minus_VhitZ_VS_ToF_BC_goodN_Step2prep_epCDn;
    TH2D *h_ToF_c_minus_VhitZ_VS_ToF_BC_badN_Step2prep_epCDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_ToF_AC_allN_Step2prep_epCDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_ToF_AC_goodN_Step2prep_epCDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_ToF_AC_badN_Step2prep_epCDn;

    TH2D *h_ToF_c_minus_VhitZ_VS_ToF_BC_allN_Step2prep_epFDn;
    TH2D *h_ToF_c_minus_VhitZ_VS_ToF_BC_goodN_Step2prep_epFDn;
    TH2D *h_ToF_c_minus_VhitZ_VS_ToF_BC_badN_Step2prep_epFDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_ToF_AC_allN_Step2prep_epFDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_ToF_AC_goodN_Step2prep_epFDn;
    // TH2D *h_ToF_c_minus_VhitZ_VS_ToF_AC_badN_Step2prep_epFDn;

    // TH1D *h_Edep_CND_goodN_Step2prep_epCDn;
    // TH1D *h_Edep_CND_badN_Step2prep_epCDn;

    // TH1D *h_Edep_CND_goodN_Step2prep_epFDn;
    // TH1D *h_Edep_CND_badN_Step2prep_epFDn;

    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_goodN_Step2prep_epCDn;
    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_badN_Step2prep_epCDn;

    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_goodN_Step2prep_epFDn;
    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_badN_Step2prep_epFDn;

    TH1D *h_Edep_CND_goodN_withNearbyPos_Step2prep_epCDn;
    TH1D *h_Edep_CND_badN_withNearbyPos_Step2prep_epCDn;

    TH1D *h_Edep_CND_goodN_withNearbyPos_Step2prep_epFDn;
    TH1D *h_Edep_CND_badN_withNearbyPos_Step2prep_epFDn;

    TH1D *h_sdiff_pos_goodN_Step2prep_layer_epCDn[7];
    TH1D *h_sdiff_pos_badN_Step2prep_layer_epCDn[7];

    TH1D *h_sdiff_pos_goodN_Step2prep_layer_epFDn[7];
    TH1D *h_sdiff_pos_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_mom_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_mom_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_mom_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_mom_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_VhitZ_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_VhitZ_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_VhitZ_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_VhitZ_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2prep_layer_epFDn[7];

    TH1D *h_theta_n_goodN_Step2prep_layer_epCDn[7];
    TH1D *h_theta_n_badN_Step2prep_layer_epCDn[7];

    TH1D *h_theta_n_goodN_Step2prep_layer_epFDn[7];
    TH1D *h_theta_n_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_theta_n_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_theta_n_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_theta_n_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_theta_n_badN_Step2prep_layer_epFDn[7];

    TH1D *h_phi_n_goodN_Step2prep_layer_epCDn[7];
    TH1D *h_phi_n_badN_Step2prep_layer_epCDn[7];

    TH1D *h_phi_n_goodN_Step2prep_layer_epFDn[7];
    TH1D *h_phi_n_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_phi_n_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_phi_n_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_phi_n_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_phi_n_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_ToF_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_ToF_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_ToF_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_ToF_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_path_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_path_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_path_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_path_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_beta_n_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_beta_n_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_beta_n_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_beta_n_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_Edep_CND_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_Edep_CND_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_Edep_CND_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_Edep_CND_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_theta_n_miss_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_theta_n_miss_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_theta_n_miss_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_theta_n_miss_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_dpp_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_dpp_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_dpp_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_dpp_badN_Step2prep_layer_epFDn[7];

    TH1D *h_dToF_goodN_Step2prep_layer_epCDn[7];
    TH1D *h_dToF_badN_Step2prep_layer_epCDn[7];

    TH1D *h_dToF_goodN_Step2prep_layer_epFDn[7];
    TH1D *h_dToF_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_dToF_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_dToF_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_dToF_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_dToF_badN_Step2prep_layer_epFDn[7];

    TH1D *h_dToF_rel_pos_goodN_Step2prep_layer_epCDn[7];
    TH1D *h_dToF_rel_pos_badN_Step2prep_layer_epCDn[7];

    TH1D *h_dToF_rel_pos_goodN_Step2prep_layer_epFDn[7];
    TH1D *h_dToF_rel_pos_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_dToF_rel_pos_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_dToF_rel_pos_badN_Step2prep_layer_epFDn[7];

    TH1D *h_dToF_rel_n_goodN_Step2prep_layer_epCDn[7];
    TH1D *h_dToF_rel_n_badN_Step2prep_layer_epCDn[7];

    TH1D *h_dToF_rel_n_goodN_Step2prep_layer_epFDn[7];
    TH1D *h_dToF_rel_n_badN_Step2prep_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_dToF_rel_n_goodN_Step2prep_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_dToF_rel_n_badN_Step2prep_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_dToF_rel_n_goodN_Step2prep_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_dToF_rel_n_badN_Step2prep_layer_epFDn[7];

    // for (int k = 0; k < 7; k++) {
    //     sprintf(temp_name, "sdiff_pos_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",//             k - 3);
    //     h_sdiff_pos_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);
    //     sprintf(temp_name, "sdiff_pos_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",//             k - 3);
    //     h_sdiff_pos_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);
    //
    //     sprintf(temp_name, "sdiff_pos_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",//             k - 3);
    //     h_sdiff_pos_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);
    //     sprintf(temp_name, "sdiff_pos_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",//             k - 3);
    //     h_sdiff_pos_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);
    //
    //     sprintf(temp_name, "sdiff_pos_mom_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",//             k - 3);
    //     h_sdiff_pos_mom_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.3, 1.5);
    //     sprintf(temp_name, "sdiff_pos_mom_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",//             k - 3);
    //     h_sdiff_pos_mom_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.3, 1.5);
    //
    //     sprintf(temp_name, "sdiff_pos_mom_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",//             k - 3);
    //     h_sdiff_pos_mom_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.4, 3.);
    //     sprintf(temp_name, "sdiff_pos_mom_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",//             k - 3);
    //     h_sdiff_pos_mom_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.4, 3.);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_VhitZ_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",//             k - 3);
    //     h_sdiff_pos_VS_VhitZ_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50,//                                                                    -40.0, 40.0);
    //     sprintf(temp_name, "sdiff_pos_VS_VhitZ_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",//             k - 3);
    //     h_sdiff_pos_VS_VhitZ_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,//                                                                   40.0);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_VhitZ_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",//             k - 3);
    //     h_sdiff_pos_VS_VhitZ_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50,//                                                                    -40.0, 40.0);
    //     sprintf(temp_name, "sdiff_pos_VS_VhitZ_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",//             k - 3);
    //     h_sdiff_pos_VS_VhitZ_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,//                                                                   40.0);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",//             k - 3);
    //     h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2prep_layer_epCDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);
    //     sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",//             k - 3);
    //     h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2prep_layer_epCDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",//             k - 3);
    //     h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2prep_layer_epFDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);
    //     sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",//             k - 3);
    //     h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2prep_layer_epFDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);
    //
    //     sprintf(temp_name, "theta_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
    //     h_theta_n_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);
    //     sprintf(temp_name, "theta_n_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
    //     h_theta_n_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);
    //
    //     sprintf(temp_name, "theta_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
    //     h_theta_n_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);
    //     sprintf(temp_name, "theta_n_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
    //     h_theta_n_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_theta_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_theta_n_goodN_Step2prep_layer_epCDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 35, 135);
    //     sprintf(temp_name, "sdiff_pos_VS_theta_n_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_theta_n_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35,//                                                                     135);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_theta_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_theta_n_goodN_Step2prep_layer_epFDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 35, 135);
    //     sprintf(temp_name, "sdiff_pos_VS_theta_n_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_theta_n_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35,//                                                                     135);
    //
    //     sprintf(temp_name, "phi_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
    //     h_phi_n_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);
    //     sprintf(temp_name, "phi_n_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
    //     h_phi_n_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);
    //
    //     sprintf(temp_name, "phi_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
    //     h_phi_n_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);
    //     sprintf(temp_name, "phi_n_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
    //     h_phi_n_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_phi_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_phi_n_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,//                                                                    180);
    //     sprintf(temp_name, "sdiff_pos_VS_phi_n_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_phi_n_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,//                                                                   180);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_phi_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_phi_n_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,//                                                                    180);
    //     sprintf(temp_name, "sdiff_pos_VS_phi_n_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_phi_n_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,//                                                                   180);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_ToF_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",//             k - 3);
    //     h_sdiff_pos_VS_ToF_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.,//                                                                  20.);
    //     sprintf(temp_name, "sdiff_pos_VS_ToF_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",//             k - 3);
    //     h_sdiff_pos_VS_ToF_badN_Step2prep_layer_epCDn[k] = new //             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_ToF_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",//             k - 3);
    //     h_sdiff_pos_VS_ToF_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.,//                                                                  20.);
    //     sprintf(temp_name, "sdiff_pos_VS_ToF_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",//             k - 3);
    //     h_sdiff_pos_VS_ToF_badN_Step2prep_layer_epFDn[k] = new //             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_path_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",//             k - 3);
    //     h_sdiff_pos_VS_path_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20.,//                                                                   60.);
    //     sprintf(temp_name, "sdiff_pos_VS_path_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",//             k - 3);
    //     h_sdiff_pos_VS_path_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20.,//                                                                  60.);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_path_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",//             k - 3);
    //     h_sdiff_pos_VS_path_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20.,//                                                                   60.);
    //     sprintf(temp_name, "sdiff_pos_VS_path_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",//             k - 3);
    //     h_sdiff_pos_VS_path_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20.,//                                                                  60.);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_beta_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",//             k - 3);
    //     h_sdiff_pos_VS_beta_n_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50,//                                                                     -0.1, 1.1);
    //     sprintf(temp_name, "sdiff_pos_VS_beta_n_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",//             k - 3);
    //     h_sdiff_pos_VS_beta_n_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,//                                                                    1.1);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_beta_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",//             k - 3);
    //     h_sdiff_pos_VS_beta_n_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50,//                                                                     -0.1, 1.1);
    //     sprintf(temp_name, "sdiff_pos_VS_beta_n_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",//             k - 3);
    //     h_sdiff_pos_VS_beta_n_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,//                                                                    1.1);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_Edep_CND_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",//             k - 3);
    //     h_sdiff_pos_VS_Edep_CND_goodN_Step2prep_layer_epCDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);
    //     sprintf(temp_name, "sdiff_pos_VS_Edep_CND_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",//             k - 3);
    //     h_sdiff_pos_VS_Edep_CND_badN_Step2prep_layer_epCDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_Edep_CND_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",//             k - 3);
    //     h_sdiff_pos_VS_Edep_CND_goodN_Step2prep_layer_epFDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);
    //     sprintf(temp_name, "sdiff_pos_VS_Edep_CND_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",//             k - 3);
    //     h_sdiff_pos_VS_Edep_CND_badN_Step2prep_layer_epFDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_theta_n_miss_goodN_Step2prep_layer_epCDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);
    //     sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_theta_n_miss_badN_Step2prep_layer_epCDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_theta_n_miss_goodN_Step2prep_layer_epFDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);
    //     sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",//             k - 3);
    //     h_sdiff_pos_VS_theta_n_miss_badN_Step2prep_layer_epFDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_dpp_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",//             k - 3);
    //     h_sdiff_pos_VS_dpp_goodN_Step2prep_layer_epCDn[k] = new //             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);
    //     sprintf(temp_name, "sdiff_pos_VS_dpp_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",//             k - 3);
    //     h_sdiff_pos_VS_dpp_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_dpp_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",//             k - 3);
    //     h_sdiff_pos_VS_dpp_goodN_Step2prep_layer_epFDn[k] = new //             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);
    //     sprintf(temp_name, "sdiff_pos_VS_dpp_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",//             k - 3);
    //     h_sdiff_pos_VS_dpp_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);
    //
    //     sprintf(temp_name, "dToF_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",//             k - 3);
    //     h_dToF_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);
    //     sprintf(temp_name, "dToF_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",//             k - 3);
    //     h_dToF_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);
    //
    //     sprintf(temp_name, "dToF_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",//             k - 3);
    //     h_dToF_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);
    //     sprintf(temp_name, "dToF_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",//             k - 3);
    //     h_dToF_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",//             k - 3);
    //     h_sdiff_pos_VS_dToF_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3.,//                                                                   9.);
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",//             k - 3);
    //     h_sdiff_pos_VS_dToF_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3.,//                                                                  9.);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",//             k - 3);
    //     h_sdiff_pos_VS_dToF_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3.,//                                                                   9.);
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",//             k - 3);
    //     h_sdiff_pos_VS_dToF_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3.,//                                                                  9.);
    //
    //     sprintf(temp_name, "dToF_rel_pos_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",//             k - 3);
    //     h_dToF_rel_pos_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);
    //     sprintf(temp_name, "dToF_rel_pos_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",//             k - 3);
    //     h_dToF_rel_pos_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);
    //
    //     sprintf(temp_name, "dToF_rel_pos_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",//             k - 3);
    //     h_dToF_rel_pos_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);
    //     sprintf(temp_name, "dToF_rel_pos_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",//             k - 3);
    //     h_dToF_rel_pos_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",//             k - 3);
    //     h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2prep_layer_epCDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",//             k - 3);
    //     h_sdiff_pos_VS_dToF_rel_pos_badN_Step2prep_layer_epCDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",//             k - 3);
    //     h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2prep_layer_epFDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",//             k - 3);
    //     h_sdiff_pos_VS_dToF_rel_pos_badN_Step2prep_layer_epFDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);
    //
    //     sprintf(temp_name, "dToF_rel_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",//             k - 3);
    //     h_dToF_rel_n_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);
    //     sprintf(temp_name, "dToF_rel_n_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",//             k - 3);
    //     h_dToF_rel_n_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);
    //
    //     sprintf(temp_name, "dToF_rel_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",//             k - 3);
    //     h_dToF_rel_n_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);
    //     sprintf(temp_name, "dToF_rel_n_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",//             k - 3);
    //     h_dToF_rel_n_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",//             k - 3);
    //     h_sdiff_pos_VS_dToF_rel_n_goodN_Step2prep_layer_epCDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 2);
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_badN_Step2prep_layer_%d_epCDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",//             k - 3);
    //     h_sdiff_pos_VS_dToF_rel_n_badN_Step2prep_layer_epCDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 2);
    //
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",//             k - 3);
    //     h_sdiff_pos_VS_dToF_rel_n_goodN_Step2prep_layer_epFDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 2);
    //     sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_badN_Step2prep_layer_%d_epFDn", k - 3);
    //     sprintf(temp_title,//             "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",//             k - 3);
    //     h_sdiff_pos_VS_dToF_rel_n_badN_Step2prep_layer_epFDn[k] = new TH2D(//         temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 2);
    // }

    TH2D *h_diff_ToFc_z_VS_Edep_noNear_goodN_Step2prep_epCDn;
    TH2D *h_diff_ToFc_z_VS_Edep_noNear_badN_Step2prep_epCDn;

    TH2D *h_diff_ToFc_z_VS_Edep_noNear_goodN_Step2prep_epFDn;
    TH2D *h_diff_ToFc_z_VS_Edep_noNear_badN_Step2prep_epFDn;

    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_goodN_Step2prep_epCDn;
    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_badN_Step2prep_epCDn;

    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_goodN_Step2prep_epFDn;
    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_badN_Step2prep_epFDn;

    /* Step2 cuts */
    TH1D *h_Size_CND1_BS2C_Step2_epCDn;
    TH1D *h_Size_CND1_AS2C_Step2_epCDn;
    TH1D *h_Size_CND2_BS2C_Step2_epCDn;
    TH1D *h_Size_CND2_AS2C_Step2_epCDn;
    TH1D *h_Size_CND3_BS2C_Step2_epCDn;
    TH1D *h_Size_CND3_AS2C_Step2_epCDn;

    TH1D *h_Size_CND1_BS2C_Step2_epFDn;
    TH1D *h_Size_CND1_AS2C_Step2_epFDn;
    TH1D *h_Size_CND2_BS2C_Step2_epFDn;
    TH1D *h_Size_CND2_AS2C_Step2_epFDn;
    TH1D *h_Size_CND3_BS2C_Step2_epFDn;
    TH1D *h_Size_CND3_AS2C_Step2_epFDn;

    TH2D *h_Size_CND1_VS_Size_CND2_BS2C_Step2_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND2_AS2C_Step2_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND3_BS2C_Step2_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND3_AS2C_Step2_epCDn;
    TH2D *h_Size_CND2_VS_Size_CND3_BS2C_Step2_epCDn;
    TH2D *h_Size_CND2_VS_Size_CND3_AS2C_Step2_epCDn;

    TH2D *h_Size_CND1_VS_Size_CND2_BS2C_Step2_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND2_AS2C_Step2_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND3_BS2C_Step2_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND3_AS2C_Step2_epFDn;
    TH2D *h_Size_CND2_VS_Size_CND3_BS2C_Step2_epFDn;
    TH2D *h_Size_CND2_VS_Size_CND3_AS2C_Step2_epFDn;

    TH1D *h_LayerMult_CND1_BS2C_Step2_epCDn;
    TH1D *h_LayerMult_CND1_AS2C_Step2_epCDn;
    TH1D *h_LayerMult_CND2_BS2C_Step2_epCDn;
    TH1D *h_LayerMult_CND2_AS2C_Step2_epCDn;
    TH1D *h_LayerMult_CND3_BS2C_Step2_epCDn;
    TH1D *h_LayerMult_CND3_AS2C_Step2_epCDn;

    TH1D *h_LayerMult_CND1_BS2C_Step2_epFDn;
    TH1D *h_LayerMult_CND1_AS2C_Step2_epFDn;
    TH1D *h_LayerMult_CND2_BS2C_Step2_epFDn;
    TH1D *h_LayerMult_CND2_AS2C_Step2_epFDn;
    TH1D *h_LayerMult_CND3_BS2C_Step2_epFDn;
    TH1D *h_LayerMult_CND3_AS2C_Step2_epFDn;

    TH2D *h_LayerMult_CND1_VS_LayerMult_CND2_BS2C_Step2_epCDn;
    TH2D *h_LayerMult_CND1_VS_LayerMult_CND2_AS2C_Step2_epCDn;
    TH2D *h_LayerMult_CND1_VS_LayerMult_CND3_BS2C_Step2_epCDn;
    TH2D *h_LayerMult_CND1_VS_LayerMult_CND3_AS2C_Step2_epCDn;
    TH2D *h_LayerMult_CND2_VS_LayerMult_CND3_BS2C_Step2_epCDn;
    TH2D *h_LayerMult_CND2_VS_LayerMult_CND3_AS2C_Step2_epCDn;

    TH2D *h_LayerMult_CND1_VS_LayerMult_CND2_BS2C_Step2_epFDn;
    TH2D *h_LayerMult_CND1_VS_LayerMult_CND2_AS2C_Step2_epFDn;
    TH2D *h_LayerMult_CND1_VS_LayerMult_CND3_BS2C_Step2_epFDn;
    TH2D *h_LayerMult_CND1_VS_LayerMult_CND3_AS2C_Step2_epFDn;
    TH2D *h_LayerMult_CND2_VS_LayerMult_CND3_BS2C_Step2_epFDn;
    TH2D *h_LayerMult_CND2_VS_LayerMult_CND3_AS2C_Step2_epFDn;
    /*
    TH2D *h_dbeta_n_VS_P_n_BS1C_Step2_epCDn;
    TH2D *h_dbeta_n_VS_ToF_BS1C_Step2_epCDn;
    TH2D *h_dbeta_n_VS_P_n_AS1C_Step2_epCDn;
    TH2D *h_dbeta_n_VS_ToF_AS1C_Step2_epCDn;

    TH2D *h_dbeta_n_VS_P_n_BS1C_Step2_epFDn;
    TH2D *h_dbeta_n_VS_ToF_BS1C_Step2_epFDn;
    TH2D *h_dbeta_n_VS_P_n_AS1C_Step2_epFDn;
    TH2D *h_dbeta_n_VS_ToF_AS1C_Step2_epFDn;

    TH1D *h_Vhit_z_n_BS1C_Step2_epCDn;
    TH1D *h_Vhit_z_n_AS1C_Step2_epCDn;

    TH1D *h_Vhit_z_n_BS1C_Step2_epFDn;
    TH1D *h_Vhit_z_n_AS1C_Step2_epFDn;

    TH1D *h_ToF_n_BS1C_Step2_epCDn;
    TH1D *h_ToF_n_AS1C_Step2_epCDn;

    TH1D *h_ToF_n_BS1C_Step2_epFDn;
    TH1D *h_ToF_n_AS1C_Step2_epFDn;

    TH1D *h_beta_n_BS1C_Step2_epFDn;
    TH1D *h_beta_n_AS1C_Step2_epFDn;
 */

    /* ToF * c - v_hit_3v.Z() plots */
    // TODO: move from here!
    // TH1D *h_ToF_c_minus_VhitZ_BC_allN_Step2_epCDn;
    // TH1D *h_ToF_c_minus_VhitZ_BC_goodN_Step2_epCDn;
    // TH1D *h_ToF_c_minus_VhitZ_BC_badN_Step2_epCDn;
    TH1D *h_ToF_c_minus_VhitZ_AC_allN_Step2_epCDn;
    TH1D *h_ToF_c_minus_VhitZ_AC_goodN_Step2_epCDn;
    TH1D *h_ToF_c_minus_VhitZ_AC_badN_Step2_epCDn;

    // TH1D *h_ToF_c_minus_VhitZ_BC_allN_Step2_epFDn;
    // TH1D *h_ToF_c_minus_VhitZ_BC_goodN_Step2_epFDn;
    // TH1D *h_ToF_c_minus_VhitZ_BC_badN_Step2_epFDn;
    TH1D *h_ToF_c_minus_VhitZ_AC_allN_Step2_epFDn;
    TH1D *h_ToF_c_minus_VhitZ_AC_goodN_Step2_epFDn;
    TH1D *h_ToF_c_minus_VhitZ_AC_badN_Step2_epFDn;

    // TH1D *h_Edep_CND_goodN_Step2_test_epCDn;
    // TH1D *h_Edep_CND_badN_Step2_test_epCDn;

    // TH1D *h_Edep_CND_goodN_Step2_test_epFDn;
    // TH1D *h_Edep_CND_badN_Step2_test_epFDn;

    /* Kinematical variables */
    TH1D *h_theta_n_goodN_Step2_epCDn;
    TH1D *h_theta_n_badN_Step2_epCDn;
    TH1D *h_phi_n_goodN_Step2_epCDn;
    TH1D *h_phi_n_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_phi_n_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_phi_n_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_beta_n_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_beta_n_badN_Step2_epCDn;

    TH1D *h_theta_n_goodN_Step2_epFDn;
    TH1D *h_theta_n_badN_Step2_epFDn;
    TH1D *h_phi_n_goodN_Step2_epFDn;
    TH1D *h_phi_n_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_phi_n_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_phi_n_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_beta_n_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_beta_n_badN_Step2_epFDn;

    TH1D *h_P_n_goodN_Step2_epCDn;
    TH1D *h_P_n_badN_Step2_epCDn;
    TH2D *h_P_n_VS_theta_n_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_theta_n_badN_Step2_epCDn;

    TH1D *h_P_n_goodN_Step2_epFDn;
    TH1D *h_P_n_badN_Step2_epFDn;
    TH2D *h_P_n_VS_theta_n_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_theta_n_badN_Step2_epFDn;

    TH1D *h_P_miss_goodN_Step2_epCDn;
    TH1D *h_P_miss_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_theta_miss_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_theta_miss_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_phi_miss_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_phi_miss_badN_Step2_epCDn;

    TH1D *h_P_miss_goodN_Step2_epFDn;
    TH1D *h_P_miss_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_theta_miss_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_theta_miss_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_phi_miss_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_phi_miss_badN_Step2_epFDn;

    TH1D *h_dpp_allN_Step2_epCDn;
    TH1D *h_dpp_goodN_Step2_epCDn;
    TH1D *h_dpp_badN_Step2_epCDn;

    TH1D *h_dpp_allN_Step2_epFDn;
    TH1D *h_dpp_goodN_Step2_epFDn;
    TH1D *h_dpp_badN_Step2_epFDn;

    TH1D *h_theta_n_miss_allN_Step2_epCDn;
    TH1D *h_theta_n_miss_goodN_Step2_epCDn;
    TH1D *h_theta_n_miss_badN_Step2_epCDn;

    TH1D *h_theta_n_miss_allN_Step2_epFDn;
    TH1D *h_theta_n_miss_goodN_Step2_epFDn;
    TH1D *h_theta_n_miss_badN_Step2_epFDn;

    TH2D *h_dpp_VS_theta_n_miss_allN_Step2_epCDn;

    TH2D *h_dpp_VS_theta_n_miss_allN_Step2_epFDn;

    TH1D *h_E_p_goodN_Step2_epCDn;
    TH1D *h_E_p_badN_Step2_epCDn;
    TH1D *h_E_miss_goodN_Step2_epCDn;
    TH1D *h_E_miss_badN_Step2_epCDn;
    TH1D *h_M_miss_goodN_Step2_epCDn;
    TH1D *h_M_miss_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_P_n_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_P_n_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_theta_n_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_theta_n_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_phi_n_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_phi_n_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_P_miss_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_P_miss_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_theta_miss_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_theta_miss_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_phi_miss_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_phi_miss_badN_Step2_epCDn;

    TH1D *h_E_p_goodN_Step2_epFDn;
    TH1D *h_E_p_badN_Step2_epFDn;
    TH1D *h_E_miss_goodN_Step2_epFDn;
    TH1D *h_E_miss_badN_Step2_epFDn;
    TH1D *h_M_miss_goodN_Step2_epFDn;
    TH1D *h_M_miss_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_P_n_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_P_n_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_theta_n_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_theta_n_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_phi_n_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_phi_n_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_P_miss_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_P_miss_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_theta_miss_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_theta_miss_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_phi_miss_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_phi_miss_badN_Step2_epFDn;

    TH1D *h_P_n_minus_P_miss_goodN_Step2_epCDn;
    TH1D *h_P_n_minus_P_miss_badN_Step2_epCDn;
    TH1D *h_P_n_x_minus_P_miss_x_goodN_Step2_epCDn;
    TH1D *h_P_n_x_minus_P_miss_x_badN_Step2_epCDn;
    TH1D *h_P_n_y_minus_P_miss_y_goodN_Step2_epCDn;
    TH1D *h_P_n_y_minus_P_miss_y_badN_Step2_epCDn;
    TH1D *h_P_n_z_minus_P_miss_z_goodN_Step2_epCDn;
    TH1D *h_P_n_z_minus_P_miss_z_badN_Step2_epCDn;

    TH1D *h_P_n_minus_P_miss_goodN_Step2_epFDn;
    TH1D *h_P_n_minus_P_miss_badN_Step2_epFDn;
    TH1D *h_P_n_x_minus_P_miss_x_goodN_Step2_epFDn;
    TH1D *h_P_n_x_minus_P_miss_x_badN_Step2_epFDn;
    TH1D *h_P_n_y_minus_P_miss_y_goodN_Step2_epFDn;
    TH1D *h_P_n_y_minus_P_miss_y_badN_Step2_epFDn;
    TH1D *h_P_n_z_minus_P_miss_z_goodN_Step2_epFDn;
    TH1D *h_P_n_z_minus_P_miss_z_badN_Step2_epFDn;

    TH2D *h_P_n_VS_P_miss_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_P_miss_badN_Step2_epCDn;
    TH2D *h_P_n_x_VS_P_miss_x_goodN_Step2_epCDn;
    TH2D *h_P_n_x_VS_P_miss_x_badN_Step2_epCDn;
    TH2D *h_P_n_y_VS_P_miss_y_goodN_Step2_epCDn;
    TH2D *h_P_n_y_VS_P_miss_y_badN_Step2_epCDn;
    TH2D *h_P_n_z_VS_P_miss_z_goodN_Step2_epCDn;
    TH2D *h_P_n_z_VS_P_miss_z_badN_Step2_epCDn;

    TH2D *h_P_n_VS_P_miss_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_P_miss_badN_Step2_epFDn;
    TH2D *h_P_n_x_VS_P_miss_x_goodN_Step2_epFDn;
    TH2D *h_P_n_x_VS_P_miss_x_badN_Step2_epFDn;
    TH2D *h_P_n_y_VS_P_miss_y_goodN_Step2_epFDn;
    TH2D *h_P_n_y_VS_P_miss_y_badN_Step2_epFDn;
    TH2D *h_P_n_z_VS_P_miss_z_goodN_Step2_epFDn;
    TH2D *h_P_n_z_VS_P_miss_z_badN_Step2_epFDn;

    TH1D *h_theta_n_p_goodN_Step2_epCDn;
    TH1D *h_theta_n_p_badN_Step2_epCDn;
    TH2D *h_theta_n_p_VS_P_p_goodN_Step2_epCDn;
    TH2D *h_theta_n_p_VS_P_p_badN_Step2_epCDn;

    TH1D *h_theta_n_p_goodN_Step2_epFDn;
    TH1D *h_theta_n_p_badN_Step2_epFDn;
    TH2D *h_theta_n_p_VS_P_p_goodN_Step2_epFDn;
    TH2D *h_theta_n_p_VS_P_p_badN_Step2_epFDn;

    TH1D *h_xB_goodN_Step2_epCDn;
    TH1D *h_xB_badN_Step2_epCDn;

    TH1D *h_xB_goodN_Step2_epFDn;
    TH1D *h_xB_badN_Step2_epFDn;

    /* Detector responses */
    TH1D *h_Edep_CND_goodN_Step2_epCDn;
    TH1D *h_Edep_CND_badN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_path_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_path_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_badN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_goodN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_badN_Step2_epCDn;

    TH1D *h_Edep_CND_goodN_Step2_epFDn;
    TH1D *h_Edep_CND_badN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_path_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_path_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND_badN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_goodN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND_badN_Step2_epFDn;

    TH1D *h_Edep_CTOF_goodN_Step2_epCDn;
    TH1D *h_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_path_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_path_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_badN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_goodN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_badN_Step2_epCDn;

    TH1D *h_Edep_CTOF_goodN_Step2_epFDn;
    TH1D *h_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_path_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_path_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_Edep_CND1_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CTOF_badN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_goodN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CTOF_badN_Step2_epFDn;

    TH1D *h_Edep_single_goodN_Step2_epCDn;
    TH1D *h_Edep_single_badN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_path_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_path_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_single_badN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_single_goodN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_single_badN_Step2_epCDn;

    TH1D *h_Edep_single_goodN_Step2_epFDn;
    TH1D *h_Edep_single_badN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_path_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_path_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_single_badN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_single_goodN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_single_badN_Step2_epFDn;

    TH1D *h_Edep_CND1_goodN_Step2_epCDn;
    TH1D *h_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_path_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_path_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_badN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_goodN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_badN_Step2_epCDn;

    TH1D *h_Edep_CND1_goodN_Step2_epFDn;
    TH1D *h_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_path_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_path_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Edep_CND1_badN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_goodN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND1_badN_Step2_epFDn;

    TH1D *h_Edep_CND2_goodN_Step2_epCDn;
    TH1D *h_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_path_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_path_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_CND2_badN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_goodN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_badN_Step2_epCDn;

    TH1D *h_Edep_CND2_goodN_Step2_epFDn;
    TH1D *h_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_path_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_path_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_CND2_badN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_goodN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Edep_CND2_badN_Step2_epFDn;

    TH1D *h_Edep_CND3_goodN_Step2_epCDn;
    TH1D *h_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_phi_n_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_dpp_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_beta_n_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_E_p_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_E_miss_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_path_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_path_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_ToF_VS_Edep_CND3_badN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_CND3_goodN_Step2_epCDn;
    TH2D *h_nSector_VS_Edep_CND3_badN_Step2_epCDn;

    TH1D *h_Edep_CND3_goodN_Step2_epFDn;
    TH1D *h_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_phi_n_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_dpp_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_beta_n_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_E_p_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_E_miss_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_path_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_path_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_ToF_VS_Edep_CND3_badN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_CND3_goodN_Step2_epFDn;
    TH2D *h_nSector_VS_Edep_CND3_badN_Step2_epFDn;

    TH1D *h_Size_CND1_goodN_Step2_epCDn;
    TH1D *h_Size_CND1_badN_Step2_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_P_n_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_phi_n_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_phi_n_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_dpp_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_dpp_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_beta_n_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_beta_n_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_E_p_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_E_p_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_E_miss_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_E_miss_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_path_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_path_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_ToF_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_ToF_VS_Size_CND1_badN_Step2_epCDn;
    TH2D *h_nSector_VS_Size_CND1_goodN_Step2_epCDn;
    TH2D *h_nSector_VS_Size_CND1_badN_Step2_epCDn;

    TH1D *h_Size_CND1_goodN_Step2_epFDn;
    TH1D *h_Size_CND1_badN_Step2_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_P_n_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_phi_n_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_phi_n_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_dpp_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_dpp_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_beta_n_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_beta_n_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_E_p_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_E_p_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_E_miss_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_E_miss_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_path_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_path_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_ToF_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_ToF_VS_Size_CND1_badN_Step2_epFDn;
    TH2D *h_nSector_VS_Size_CND1_goodN_Step2_epFDn;
    TH2D *h_nSector_VS_Size_CND1_badN_Step2_epFDn;

    TH1D *h_Size_CND2_goodN_Step2_epCDn;
    TH1D *h_Size_CND2_badN_Step2_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_P_n_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_phi_n_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_phi_n_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_dpp_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_dpp_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_beta_n_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_beta_n_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_E_p_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_E_p_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_E_miss_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_E_miss_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_path_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_path_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_ToF_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_ToF_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_nSector_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_nSector_VS_Size_CND2_badN_Step2_epCDn;

    TH1D *h_Size_CND2_goodN_Step2_epFDn;
    TH1D *h_Size_CND2_badN_Step2_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_P_n_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_phi_n_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_phi_n_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_dpp_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_dpp_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_beta_n_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_beta_n_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_E_p_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_E_p_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_E_miss_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_E_miss_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_path_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_path_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_ToF_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_ToF_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_nSector_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_nSector_VS_Size_CND2_badN_Step2_epFDn;

    TH1D *h_Size_CND3_goodN_Step2_epCDn;
    TH1D *h_Size_CND3_badN_Step2_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_Edep_CND_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_P_n_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_phi_n_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_phi_n_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_theta_miss_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_phi_miss_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_dpp_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_dpp_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_beta_n_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_beta_n_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_E_p_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_E_p_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_E_miss_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_E_miss_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_path_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_path_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_ToF_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_ToF_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_nSector_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_nSector_VS_Size_CND3_badN_Step2_epCDn;

    TH1D *h_Size_CND3_goodN_Step2_epFDn;
    TH1D *h_Size_CND3_badN_Step2_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_Edep_CND_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_Edep_CND1_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_Edep_CND2_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_Edep_CND3_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_P_n_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_phi_n_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_phi_n_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_theta_miss_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_phi_miss_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_dpp_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_dpp_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_beta_n_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_beta_n_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_E_p_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_E_p_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_E_miss_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_E_miss_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_path_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_path_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_ToF_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_ToF_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_nSector_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_nSector_VS_Size_CND3_badN_Step2_epFDn;

    TH2D *h_Size_CND1_VS_Size_CND2_goodN_Step2_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND2_badN_Step2_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_Size_CND1_VS_Size_CND3_badN_Step2_epCDn;
    TH2D *h_Size_CND2_VS_Size_CND3_goodN_Step2_epCDn;
    TH2D *h_Size_CND2_VS_Size_CND3_badN_Step2_epCDn;

    TH2D *h_Size_CND1_VS_Size_CND2_goodN_Step2_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND2_badN_Step2_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_Size_CND1_VS_Size_CND3_badN_Step2_epFDn;
    TH2D *h_Size_CND2_VS_Size_CND3_goodN_Step2_epFDn;
    TH2D *h_Size_CND2_VS_Size_CND3_badN_Step2_epFDn;

    TH1D *h_ToF_goodN_Step2_epCDn;
    TH1D *h_ToF_badN_Step2_epCDn;
    TH2D *h_P_n_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_P_n_VS_ToF_badN_Step2_epCDn;
    TH2D *h_theta_n_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_theta_n_VS_ToF_badN_Step2_epCDn;
    TH2D *h_phi_n_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_phi_n_VS_ToF_badN_Step2_epCDn;
    TH2D *h_P_miss_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_P_miss_VS_ToF_badN_Step2_epCDn;
    TH2D *h_theta_miss_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_theta_miss_VS_ToF_badN_Step2_epCDn;
    TH2D *h_phi_miss_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_phi_miss_VS_ToF_badN_Step2_epCDn;
    TH2D *h_dpp_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_dpp_VS_ToF_badN_Step2_epCDn;
    TH2D *h_beta_n_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_beta_n_VS_ToF_badN_Step2_epCDn;
    TH2D *h_E_p_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_E_p_VS_ToF_badN_Step2_epCDn;
    TH2D *h_E_miss_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_E_miss_VS_ToF_badN_Step2_epCDn;
    TH2D *h_M_miss_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_M_miss_VS_ToF_badN_Step2_epCDn;
    TH2D *h_path_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_path_VS_ToF_badN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_theta_n_miss_VS_ToF_badN_Step2_epCDn;
    TH2D *h_nSector_VS_ToF_goodN_Step2_epCDn;
    TH2D *h_nSector_VS_ToF_badN_Step2_epCDn;

    TH1D *h_ToF_goodN_Step2_epFDn;
    TH1D *h_ToF_badN_Step2_epFDn;
    TH2D *h_P_n_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_P_n_VS_ToF_badN_Step2_epFDn;
    TH2D *h_theta_n_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_theta_n_VS_ToF_badN_Step2_epFDn;
    TH2D *h_phi_n_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_phi_n_VS_ToF_badN_Step2_epFDn;
    TH2D *h_P_miss_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_P_miss_VS_ToF_badN_Step2_epFDn;
    TH2D *h_theta_miss_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_theta_miss_VS_ToF_badN_Step2_epFDn;
    TH2D *h_phi_miss_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_phi_miss_VS_ToF_badN_Step2_epFDn;
    TH2D *h_dpp_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_dpp_VS_ToF_badN_Step2_epFDn;
    TH2D *h_beta_n_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_beta_n_VS_ToF_badN_Step2_epFDn;
    TH2D *h_E_p_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_E_p_VS_ToF_badN_Step2_epFDn;
    TH2D *h_E_miss_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_E_miss_VS_ToF_badN_Step2_epFDn;
    TH2D *h_M_miss_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_M_miss_VS_ToF_badN_Step2_epFDn;
    TH2D *h_path_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_path_VS_ToF_badN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_theta_n_miss_VS_ToF_badN_Step2_epFDn;
    TH2D *h_nSector_VS_ToF_goodN_Step2_epFDn;
    TH2D *h_nSector_VS_ToF_badN_Step2_epFDn;

    TH1D *h_beta_n_goodN_Step2_epCDn;
    TH1D *h_beta_n_badN_Step2_epCDn;

    TH1D *h_beta_n_goodN_Step2_epFDn;
    TH1D *h_beta_n_badN_Step2_epFDn;

    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_goodN_Step2_epCDn;
    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_badN_Step2_epCDn;

    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_goodN_Step2_epFDn;
    TH1D *h_neut_Edep_CND_over_pos_Edep_CTOF_badN_Step2_epFDn;

    TH1D *h_Edep_CND_goodN_withNearbyPos_Step2_epCDn;
    TH1D *h_Edep_CND_badN_withNearbyPos_Step2_epCDn;

    TH1D *h_Edep_CND_goodN_withNearbyPos_Step2_epFDn;
    TH1D *h_Edep_CND_badN_withNearbyPos_Step2_epFDn;

    TH1D *h_sdiff_pos_goodN_Step2_layer_epCDn[7];
    TH1D *h_sdiff_pos_badN_Step2_layer_epCDn[7];

    TH1D *h_sdiff_pos_goodN_Step2_layer_epFDn[7];
    TH1D *h_sdiff_pos_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_mom_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_mom_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_mom_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_mom_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_VhitZ_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_VhitZ_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_VhitZ_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_VhitZ_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2_layer_epFDn[7];

    TH1D *h_theta_n_goodN_Step2_layer_epCDn[7];
    TH1D *h_theta_n_badN_Step2_layer_epCDn[7];

    TH1D *h_theta_n_goodN_Step2_layer_epFDn[7];
    TH1D *h_theta_n_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_theta_n_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_theta_n_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_theta_n_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_theta_n_badN_Step2_layer_epFDn[7];

    TH1D *h_phi_n_goodN_Step2_layer_epCDn[7];
    TH1D *h_phi_n_badN_Step2_layer_epCDn[7];

    TH1D *h_phi_n_goodN_Step2_layer_epFDn[7];
    TH1D *h_phi_n_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_phi_n_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_phi_n_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_phi_n_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_phi_n_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_ToF_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_ToF_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_ToF_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_ToF_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_path_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_path_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_path_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_path_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_beta_n_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_beta_n_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_beta_n_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_beta_n_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_Edep_CND_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_Edep_CND_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_Edep_CND_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_Edep_CND_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_theta_n_miss_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_theta_n_miss_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_theta_n_miss_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_theta_n_miss_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_dpp_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_dpp_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_dpp_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_dpp_badN_Step2_layer_epFDn[7];

    TH1D *h_dToF_goodN_Step2_layer_epCDn[7];
    TH1D *h_dToF_badN_Step2_layer_epCDn[7];

    TH1D *h_dToF_goodN_Step2_layer_epFDn[7];
    TH1D *h_dToF_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_dToF_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_dToF_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_dToF_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_dToF_badN_Step2_layer_epFDn[7];

    TH1D *h_dToF_rel_pos_goodN_Step2_layer_epCDn[7];
    TH1D *h_dToF_rel_pos_badN_Step2_layer_epCDn[7];

    TH1D *h_dToF_rel_pos_goodN_Step2_layer_epFDn[7];
    TH1D *h_dToF_rel_pos_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_dToF_rel_pos_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_dToF_rel_pos_badN_Step2_layer_epFDn[7];

    TH1D *h_dToF_rel_n_goodN_Step2_layer_epCDn[7];
    TH1D *h_dToF_rel_n_badN_Step2_layer_epCDn[7];

    TH1D *h_dToF_rel_n_goodN_Step2_layer_epFDn[7];
    TH1D *h_dToF_rel_n_badN_Step2_layer_epFDn[7];

    TH2D *h_sdiff_pos_VS_dToF_rel_n_goodN_Step2_layer_epCDn[7];
    TH2D *h_sdiff_pos_VS_dToF_rel_n_badN_Step2_layer_epCDn[7];

    TH2D *h_sdiff_pos_VS_dToF_rel_n_goodN_Step2_layer_epFDn[7];
    TH2D *h_sdiff_pos_VS_dToF_rel_n_badN_Step2_layer_epFDn[7];

    /*
    for (int k = 0; k < 7; k++) {
        sprintf(temp_name, "sdiff_pos_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);
        sprintf(temp_name, "sdiff_pos_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);

        sprintf(temp_name, "sdiff_pos_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);
        sprintf(temp_name, "sdiff_pos_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);

        sprintf(temp_name, "sdiff_pos_mom_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.3, 1.5);
        sprintf(temp_name, "sdiff_pos_mom_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.3, 1.5);

        sprintf(temp_name, "sdiff_pos_mom_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.4, 3.);
        sprintf(temp_name, "sdiff_pos_mom_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.4, 3.);

        sprintf(temp_name, "sdiff_pos_VS_VhitZ_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,                                                               40.0);
        sprintf(temp_name, "sdiff_pos_VS_VhitZ_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,                                                              40.0);

        sprintf(temp_name, "sdiff_pos_VS_VhitZ_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,                                                               40.0);
        sprintf(temp_name, "sdiff_pos_VS_VhitZ_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,                                                              40.0);

        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);
        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);

        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);
        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);

        sprintf(temp_name, "theta_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);
        sprintf(temp_name, "theta_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);

        sprintf(temp_name, "theta_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);
        sprintf(temp_name, "theta_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35,                                                                 135);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_badN_Step2_layer_epCDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35, 135);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35,                                                                 135);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_badN_Step2_layer_epFDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35, 135);

        sprintf(temp_name, "phi_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);
        sprintf(temp_name, "phi_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);

        sprintf(temp_name, "phi_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);
        sprintf(temp_name, "phi_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);

        sprintf(temp_name, "sdiff_pos_VS_phi_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,                                                               180);
        sprintf(temp_name, "sdiff_pos_VS_phi_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_badN_Step2_layer_epCDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180, 180);

        sprintf(temp_name, "sdiff_pos_VS_phi_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,                                                               180);
        sprintf(temp_name, "sdiff_pos_VS_phi_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_badN_Step2_layer_epFDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180, 180);

        sprintf(temp_name, "sdiff_pos_VS_ToF_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);
        sprintf(temp_name, "sdiff_pos_VS_ToF_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);

        sprintf(temp_name, "sdiff_pos_VS_ToF_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);
        sprintf(temp_name, "sdiff_pos_VS_ToF_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);

        sprintf(temp_name, "sdiff_pos_VS_path_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20., 60.);
        sprintf(temp_name, "sdiff_pos_VS_path_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20., 60.);

        sprintf(temp_name, "sdiff_pos_VS_path_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20., 60.);
        sprintf(temp_name, "sdiff_pos_VS_path_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20., 60.);

        sprintf(temp_name, "sdiff_pos_VS_beta_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,                                                                1.1);
        sprintf(temp_name, "sdiff_pos_VS_beta_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,                                                               1.1);

        sprintf(temp_name, "sdiff_pos_VS_beta_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,                                                                1.1);
        sprintf(temp_name, "sdiff_pos_VS_beta_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,                                                               1.1);

        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0,                                                                  100);
        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_badN_Step2_layer_epCDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);

        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0,                                                                  100);
        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_badN_Step2_layer_epFDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_goodN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_badN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_goodN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_badN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);

        sprintf(temp_name, "sdiff_pos_VS_dpp_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);
        sprintf(temp_name, "sdiff_pos_VS_dpp_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);

        sprintf(temp_name, "sdiff_pos_VS_dpp_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);
        sprintf(temp_name, "sdiff_pos_VS_dpp_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);

        sprintf(temp_name, "dToF_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);
        sprintf(temp_name, "dToF_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);

        sprintf(temp_name, "dToF_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);
        sprintf(temp_name, "dToF_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);

        sprintf(temp_name, "sdiff_pos_VS_dToF_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3., 9.);
        sprintf(temp_name, "sdiff_pos_VS_dToF_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3., 9.);

        sprintf(temp_name, "sdiff_pos_VS_dToF_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3., 9.);
        sprintf(temp_name, "sdiff_pos_VS_dToF_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3., 9.);

        sprintf(temp_name, "dToF_rel_pos_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);
        sprintf(temp_name, "dToF_rel_pos_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);

        sprintf(temp_name, "dToF_rel_pos_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);
        sprintf(temp_name, "dToF_rel_pos_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_badN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_badN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);

        sprintf(temp_name, "dToF_rel_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);
        sprintf(temp_name, "dToF_rel_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);

        sprintf(temp_name, "dToF_rel_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);
        sprintf(temp_name, "dToF_rel_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -1,                                                                    2);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -1,                                                                   2);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -1,                                                                    2);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -1,                                                                   2);
    }
    */

    TH2D *h_diff_ToFc_z_VS_Edep_noNear_goodN_Step2_epCDn;
    TH2D *h_diff_ToFc_z_VS_Edep_noNear_badN_Step2_epCDn;

    TH2D *h_diff_ToFc_z_VS_Edep_noNear_goodN_Step2_epFDn;
    TH2D *h_diff_ToFc_z_VS_Edep_noNear_badN_Step2_epFDn;

    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_goodN_Step2_epCDn;
    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_badN_Step2_epCDn;

    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_goodN_Step2_epFDn;
    TH2D *h_diff_ToFc_z_VS_Edep_yesNear_badN_Step2_epFDn;

    /*
    // TODO: add these to code below


    for (int k = 0; k < 3; k++)
    {
        sprintf(temp_name, "diff_ToFc_z_goodN_Step2_layer_%d_epCDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);
        sprintf(temp_name, "diff_ToFc_z_badN_Step2_layer_%d_epCDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);

        sprintf(temp_name, "diff_ToFc_z_goodN_Step2_layer_%d_epFDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);
        sprintf(temp_name, "diff_ToFc_z_badN_Step2_layer_%d_epFDn", k + 1);
        sprintf(temp_title, "ToF*c - V_{hit,z} vs. E_{dep}^{CND} of CND Neutrons (#DeltaL_{n,+} = %d);ToF*c-V_{hit,z} [cm];E_{dep}^{CND} [MeV]", k + 1);
        h_diff_ToFc_z_Edep_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 50, 0, 300, 50, 0, 100);
    }

    TH2D *h_sdiff_ldiff_allhit_goodN_Step2_epCDn;
    TH2D *h_sdiff_ldiff_allhit_badN_Step2_epCDn;

    TH2D *h_sdiff_ldiff_allhit_goodN_Step2_epFDn;
    TH2D *h_sdiff_ldiff_allhit_badN_Step2_epFDn;
    */

    TH1D *h_numberNearby_goodN_Step2_epCDn;
    TH1D *h_numberNearby_badN_Step2_epCDn;

    TH1D *h_numberNearby_goodN_Step2_epFDn;
    TH1D *h_numberNearby_badN_Step2_epFDn;

    TH2D *h_numberNearby_momN_goodN_Step2_epCDn;
    TH2D *h_numberNearby_momN_badN_Step2_epCDn;

    TH2D *h_numberNearby_momN_goodN_Step2_epFDn;
    TH2D *h_numberNearby_momN_badN_Step2_epFDn;

    TH1D *h_NearbyEdep_goodN_Step2_epCDn;
    TH1D *h_NearbyEdep_badN_Step2_epCDn;

    TH1D *h_NearbyEdep_goodN_Step2_epFDn;
    TH1D *h_NearbyEdep_badN_Step2_epFDn;

    TH1D *h_nsector_goodN_Step2_epCDn;
    TH1D *h_nsector_badN_Step2_epCDn;

    TH1D *h_nsector_goodN_Step2_epFDn;
    TH1D *h_nsector_badN_Step2_epFDn;

    /*
    TH1D *h_phidiff_en_goodN_Step2_epCDn;
    TH1D *h_phidiff_en_badN_Step2_epCDn;

    TH1D *h_phidiff_en_goodN_Step2_epFDn;
    TH1D *h_phidiff_en_badN_Step2_epFDn;

    TH1D *h_TP_goodN_Step2_epCDn;
    TH1D *h_TP_badN_Step2_epCDn;

    TH1D *h_TP_goodN_Step2_epFDn;
    TH1D *h_TP_badN_Step2_epFDn;

    TH1D *h_Z_goodN_Step2_epCDn;
    TH1D *h_Z_badN_Step2_epCDn;

    TH1D *h_Z_goodN_Step2_epFDn;
    TH1D *h_Z_badN_Step2_epFDn;
    */

#pragma endregion /* Step Two (After applying Phi Diff Charge Track cut) (Andrew) - end */

    // Step Three (After applying Phi Diff Charge Track cut) (Andrew)
    // ======================================================================================================================================================================

    /* Neutron histograms (from Erin) */
    TH1D *h_n_multiplicity_allN_epCDn_Step3;
    TH1D *h_n_multiplicity_goodN_epCDn_Step3;
    TH1D *h_n_multiplicity_badN_epCDn_Step3;

    TH1D *h_n_multiplicity_allN_epFDn_Step3;
    TH1D *h_n_multiplicity_goodN_epFDn_Step3;
    TH1D *h_n_multiplicity_badN_epFDn_Step3;

    // Step Four (After applying Phi Diff CND hit cut) (Andrew)
    // ======================================================================================================================================================================

    /* Neutron histograms (from Erin) */
    TH1D *h_n_multiplicity_allN_epCDn_Step4;
    TH1D *h_n_multiplicity_goodN_epCDn_Step4;
    TH1D *h_n_multiplicity_badN_epCDn_Step4;

    TH1D *h_n_multiplicity_allN_epFDn_Step4;
    TH1D *h_n_multiplicity_goodN_epFDn_Step4;
    TH1D *h_n_multiplicity_badN_epFDn_Step4;

    // Step Five (After event selection cuts) (Andrew)
    // ======================================================================================================================================================================

    /* Neutron histograms (from Erin) */
    TH1D *h_n_multiplicity_allN_epCDn_Step5;
    TH1D *h_n_multiplicity_goodN_epCDn_Step5;
    TH1D *h_n_multiplicity_badN_epCDn_Step5;

    TH1D *h_n_multiplicity_allN_epFDn_Step5;
    TH1D *h_n_multiplicity_goodN_epFDn_Step5;
    TH1D *h_n_multiplicity_badN_epFDn_Step5;


void InitHistograms() {
    gStyle->SetTitleXSize(0.05);
    gStyle->SetTitleYSize(0.05);

    gStyle->SetTitleXOffset(0.8);
    gStyle->SetTitleYOffset(0.8);

    // Step Two histograms
        for (int k = 0; k < 7; k++) {
        sprintf(temp_name, "sdiff_pos_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);
        sprintf(temp_name, "sdiff_pos_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);

        sprintf(temp_name, "sdiff_pos_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);
        sprintf(temp_name, "sdiff_pos_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);

        sprintf(temp_name, "sdiff_pos_mom_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.3, 1.5);
        sprintf(temp_name, "sdiff_pos_mom_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.3, 1.5);

        sprintf(temp_name, "sdiff_pos_mom_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.4, 3.);
        sprintf(temp_name, "sdiff_pos_mom_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.4, 3.);

        sprintf(temp_name, "sdiff_pos_VS_VhitZ_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50,                                                                   -40.0, 40.0);
        sprintf(temp_name, "sdiff_pos_VS_VhitZ_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,                                                                  40.0);

        sprintf(temp_name, "sdiff_pos_VS_VhitZ_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50,                                                                   -40.0, 40.0);
        sprintf(temp_name, "sdiff_pos_VS_VhitZ_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,                                                                  40.0);

        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2prep_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);
        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2prep_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);

        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2prep_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);
        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2prep_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);

        sprintf(temp_name, "theta_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);
        sprintf(temp_name, "theta_n_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);

        sprintf(temp_name, "theta_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);
        sprintf(temp_name, "theta_n_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_goodN_Step2prep_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 35, 135);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35,                                                                    135);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_goodN_Step2prep_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 35, 135);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35,                                                                    135);

        sprintf(temp_name, "phi_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);
        sprintf(temp_name, "phi_n_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);

        sprintf(temp_name, "phi_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);
        sprintf(temp_name, "phi_n_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);

        sprintf(temp_name, "sdiff_pos_VS_phi_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,                                                                   180);
        sprintf(temp_name, "sdiff_pos_VS_phi_n_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,                                                                  180);

        sprintf(temp_name, "sdiff_pos_VS_phi_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,                                                                   180);
        sprintf(temp_name, "sdiff_pos_VS_phi_n_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,                                                                  180);

        sprintf(temp_name, "sdiff_pos_VS_ToF_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.,                                                                 20.);
        sprintf(temp_name, "sdiff_pos_VS_ToF_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_badN_Step2prep_layer_epCDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);

        sprintf(temp_name, "sdiff_pos_VS_ToF_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.,                                                                 20.);
        sprintf(temp_name, "sdiff_pos_VS_ToF_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_badN_Step2prep_layer_epFDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);

        sprintf(temp_name, "sdiff_pos_VS_path_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20.,                                                                  60.);
        sprintf(temp_name, "sdiff_pos_VS_path_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20.,                                                                 60.);

        sprintf(temp_name, "sdiff_pos_VS_path_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20.,                                                                  60.);
        sprintf(temp_name, "sdiff_pos_VS_path_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20.,                                                                 60.);

        sprintf(temp_name, "sdiff_pos_VS_beta_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50,                                                                    -0.1, 1.1);
        sprintf(temp_name, "sdiff_pos_VS_beta_n_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,                                                                   1.1);

        sprintf(temp_name, "sdiff_pos_VS_beta_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50,                                                                    -0.1, 1.1);
        sprintf(temp_name, "sdiff_pos_VS_beta_n_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,                                                                   1.1);

        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_goodN_Step2prep_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);
        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_badN_Step2prep_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);

        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_goodN_Step2prep_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);
        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_badN_Step2prep_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_goodN_Step2prep_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_badN_Step2prep_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_goodN_Step2prep_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_badN_Step2prep_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);

        sprintf(temp_name, "sdiff_pos_VS_dpp_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_goodN_Step2prep_layer_epCDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);
        sprintf(temp_name, "sdiff_pos_VS_dpp_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);

        sprintf(temp_name, "sdiff_pos_VS_dpp_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_goodN_Step2prep_layer_epFDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);
        sprintf(temp_name, "sdiff_pos_VS_dpp_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);

        sprintf(temp_name, "dToF_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);
        sprintf(temp_name, "dToF_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);

        sprintf(temp_name, "dToF_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);
        sprintf(temp_name, "dToF_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);

        sprintf(temp_name, "sdiff_pos_VS_dToF_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_goodN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3.,                                                                  9.);
        sprintf(temp_name, "sdiff_pos_VS_dToF_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_badN_Step2prep_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3.,                                                                 9.);

        sprintf(temp_name, "sdiff_pos_VS_dToF_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_goodN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3.,                                                                  9.);
        sprintf(temp_name, "sdiff_pos_VS_dToF_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_badN_Step2prep_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3.,                                                                 9.);

        sprintf(temp_name, "dToF_rel_pos_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);
        sprintf(temp_name, "dToF_rel_pos_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);

        sprintf(temp_name, "dToF_rel_pos_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);
        sprintf(temp_name, "dToF_rel_pos_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2prep_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_badN_Step2prep_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2prep_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_badN_Step2prep_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);

        sprintf(temp_name, "dToF_rel_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_goodN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);
        sprintf(temp_name, "dToF_rel_n_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_badN_Step2prep_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);

        sprintf(temp_name, "dToF_rel_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_goodN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);
        sprintf(temp_name, "dToF_rel_n_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_badN_Step2prep_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_goodN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_goodN_Step2prep_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 2);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_badN_Step2prep_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_badN_Step2prep_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 2);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_goodN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_goodN_Step2prep_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 2);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_badN_Step2prep_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_badN_Step2prep_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 2);
    }

        for (int k = 0; k < 7; k++) {
        sprintf(temp_name, "sdiff_pos_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);
        sprintf(temp_name, "sdiff_pos_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);

        sprintf(temp_name, "sdiff_pos_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);
        sprintf(temp_name, "sdiff_pos_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Counts",            k - 3);
        h_sdiff_pos_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 24, -11.5, 12.5);

        sprintf(temp_name, "sdiff_pos_mom_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.3, 1.5);
        sprintf(temp_name, "sdiff_pos_mom_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.3, 1.5);

        sprintf(temp_name, "sdiff_pos_mom_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.4, 3.);
        sprintf(temp_name, "sdiff_pos_mom_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Proton Momentum (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};P_{p} [GeV/c]",            k - 3);
        h_sdiff_pos_mom_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0.4, 3.);

        sprintf(temp_name, "sdiff_pos_VS_VhitZ_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,                                                               40.0);
        sprintf(temp_name, "sdiff_pos_VS_VhitZ_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,                                                              40.0);

        sprintf(temp_name, "sdiff_pos_VS_VhitZ_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,                                                               40.0);
        sprintf(temp_name, "sdiff_pos_VS_VhitZ_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_VhitZ_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -40.0,                                                              40.0);

        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);
        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);

        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_goodN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);
        sprintf(temp_name, "sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. ToF*c-V_{hit,z} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};ToF*c-V_{hit,z} [cm]",            k - 3);
        h_sdiff_pos_VS_ToF_c_minus_VhitZ_badN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 300);

        sprintf(temp_name, "theta_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);
        sprintf(temp_name, "theta_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);

        sprintf(temp_name, "theta_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);
        sprintf(temp_name, "theta_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#theta_{n} Distribution (#DeltaL_{n,+} = %d);#theta_{n} [#circ]", k - 3);
        h_theta_n_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, 35, 135);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35,                                                                 135);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_badN_Step2_layer_epCDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35, 135);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35,                                                                 135);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_badN_Step2_layer_epFDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 35, 135);

        sprintf(temp_name, "phi_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);
        sprintf(temp_name, "phi_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);

        sprintf(temp_name, "phi_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);
        sprintf(temp_name, "phi_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title, "#phi_{n} Distribution (#DeltaL_{n,+} = %d);#phi_{n} [#circ]", k - 3);
        h_phi_n_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 48, -180, 180);

        sprintf(temp_name, "sdiff_pos_VS_phi_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,                                                               180);
        sprintf(temp_name, "sdiff_pos_VS_phi_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_badN_Step2_layer_epCDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180, 180);

        sprintf(temp_name, "sdiff_pos_VS_phi_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180,                                                               180);
        sprintf(temp_name, "sdiff_pos_VS_phi_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #phi_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#phi_{n} [#circ]",            k - 3);
        h_sdiff_pos_VS_phi_n_badN_Step2_layer_epFDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 48, -180, 180);

        sprintf(temp_name, "sdiff_pos_VS_ToF_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);
        sprintf(temp_name, "sdiff_pos_VS_ToF_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);

        sprintf(temp_name, "sdiff_pos_VS_ToF_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);
        sprintf(temp_name, "sdiff_pos_VS_ToF_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron ToF (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};t_{ToF}^{n} [ns]",            k - 3);
        h_sdiff_pos_VS_ToF_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0., 20.);

        sprintf(temp_name, "sdiff_pos_VS_path_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20., 60.);
        sprintf(temp_name, "sdiff_pos_VS_path_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20., 60.);

        sprintf(temp_name, "sdiff_pos_VS_path_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20., 60.);
        sprintf(temp_name, "sdiff_pos_VS_path_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. Neutron path length (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};Path length [cm]",            k - 3);
        h_sdiff_pos_VS_path_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 20., 60.);

        sprintf(temp_name, "sdiff_pos_VS_beta_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,                                                                1.1);
        sprintf(temp_name, "sdiff_pos_VS_beta_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,                                                               1.1);

        sprintf(temp_name, "sdiff_pos_VS_beta_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,                                                                1.1);
        sprintf(temp_name, "sdiff_pos_VS_beta_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #beta_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#beta_{n}",            k - 3);
        h_sdiff_pos_VS_beta_n_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -0.1,                                                               1.1);

        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0,                                                                  100);
        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_badN_Step2_layer_epCDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);

        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0,                                                                  100);
        sprintf(temp_name, "sdiff_pos_VS_Edep_CND_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. E^{CND}_{dep} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};E^{CND}_{dep} [MeV]",            k - 3);
        h_sdiff_pos_VS_Edep_CND_badN_Step2_layer_epFDn[k] = new             TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 100);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_goodN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_badN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);

        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_goodN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);
        sprintf(temp_name, "sdiff_pos_VS_theta_n_miss_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #theta_{n,miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#theta_{n,miss} [#circ]",            k - 3);
        h_sdiff_pos_VS_theta_n_miss_badN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, 0, 25);

        sprintf(temp_name, "sdiff_pos_VS_dpp_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);
        sprintf(temp_name, "sdiff_pos_VS_dpp_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);

        sprintf(temp_name, "sdiff_pos_VS_dpp_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);
        sprintf(temp_name, "sdiff_pos_VS_dpp_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. (|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};(|#vec{P}_{miss}|-|#vec{P}_{n}|)/P_{miss}",            k - 3);
        h_sdiff_pos_VS_dpp_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3, 1.);

        sprintf(temp_name, "dToF_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);
        sprintf(temp_name, "dToF_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);

        sprintf(temp_name, "dToF_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);
        sprintf(temp_name, "dToF_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_dToF_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -3., 9.);

        sprintf(temp_name, "sdiff_pos_VS_dToF_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3., 9.);
        sprintf(temp_name, "sdiff_pos_VS_dToF_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3., 9.);

        sprintf(temp_name, "sdiff_pos_VS_dToF_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3., 9.);
        sprintf(temp_name, "sdiff_pos_VS_dToF_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+} = t^{ToF}_{n} - t^{ToF}_{+} [ns]",            k - 3);
        h_sdiff_pos_VS_dToF_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -3., 9.);

        sprintf(temp_name, "dToF_rel_pos_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);
        sprintf(temp_name, "dToF_rel_pos_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);

        sprintf(temp_name, "dToF_rel_pos_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);
        sprintf(temp_name, "dToF_rel_pos_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{+} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_dToF_rel_pos_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 8);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_badN_Step2_layer_epCDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_goodN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_pos_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{+} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{+} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{+}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_pos_badN_Step2_layer_epFDn[k] = new TH2D(        temp_name, temp_title, 24, -11.5, 12.5, 50, -1, 8);

        sprintf(temp_name, "dToF_rel_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_goodN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);
        sprintf(temp_name, "dToF_rel_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_badN_Step2_layer_epCDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);

        sprintf(temp_name, "dToF_rel_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_goodN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);
        sprintf(temp_name, "dToF_rel_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "#Deltat^{ToF}_{n,+}/t^{ToF}_{n} Distribution (#DeltaL_{n,+} = %d);#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_dToF_rel_n_badN_Step2_layer_epFDn[k] = new TH1D(temp_name, temp_title, 50, -1, 2);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_goodN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_goodN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -1,                                                                    2);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_badN_Step2_layer_%d_epCDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_badN_Step2_layer_epCDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -1,                                                                   2);

        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_goodN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_goodN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -1,                                                                    2);
        sprintf(temp_name, "sdiff_pos_VS_dToF_rel_n_badN_Step2_layer_%d_epFDn", k - 3);
        sprintf(temp_title,            "Nuetral Sector minus +Charge Particle Sector vs. #Deltat^{ToF}_{n,+}/t^{ToF}_{n} (#DeltaL_{n,+} = %d);#DeltaS_{n,+} = S_{n} - S_{+};#Deltat^{ToF}_{n,+}/t^{ToF}_{n} = (t^{ToF}_{n} - t^{ToF}_{+})/t^{ToF}_{n}",            k - 3);
        h_sdiff_pos_VS_dToF_rel_n_badN_Step2_layer_epFDn[k] = new TH2D(temp_name, temp_title, 24, -11.5, 12.5, 50, -1,                                                                   2);
    }

    for (int i = 0; i < HistoList.size(); i++) {
        if (HistoList[i]->InheritsFrom("TH1D")) {
            HistoList[i]->Sumw2();
        }

        HistoList[i]->GetXaxis()->CenterTitle();
        HistoList[i]->GetYaxis()->CenterTitle();
    }

}

#pragma endregion /* Veto histograms - end */#endif // MANUALVETOHISTOGRAMS_H
