/*
/cvmfs/oasis.opensciencegrid.org/mis/apptainer/current/bin/apptainer shell --shell=/bin/bash -B /cvmfs,/exp,/nashome,/pnfs/genie,/opt,/run/user,/etc/hostname,/etc/hosts,/etc/krb5.conf --ipc
--pid /cvmfs/singularity.opensciencegrid.org/fermilab/fnal-dev-sl7:latest

cd $MY_USER_FOLDER/GENIE

source genie_env.sh

source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setup
setup fife_utils

cd /pnfs/genie/scratch/users/asportes/gst_looper

source run.sh
 */

#define gst_cxx

#include "gst.h"

#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>

#include "source/classes/AMaps/AMaps.cpp"
#include "source/namespaces/utilities/utilities.h"

using namespace std;
using namespace utilities;

void gst::Loop(string Target, string GENIE_tune, string BeamE, string Q2_th) {
    //   In a ROOT session, you can do:
    //      root> .L gst.C
    //      root> gst t
    //      root> t.GetEntry(12); // Fill t data members with entry number 12
    //      root> t.Show();       // Show values of entry 12
    //      root> t.Show(16);     // Read and show values of entry 16
    //      root> t.Loop();       // Loop on all entries
    //

    //     This is the loop skeleton where:
    //    jentry is the global entry number in the chain
    //    ientry is the entry number in the current Tree
    //  Note that the argument to GetEntry must be:
    //    jentry for TChain::GetEntry
    //    ientry for TTree::GetEntry and TBranch::GetEntry
    //
    //       To read only selected branches, Insert statements like:
    // METHOD1:
    //    fChain->SetBranchStatus("*",0);  // disable all branches
    //    fChain->SetBranchStatus("branchname",1);  // activate branchname
    // METHOD2: replace line
    //    fChain->GetEntry(jentry);       //read all branches
    // by  b_branchname->GetEntry(ientry); //read only this branch

    system(("mkdir -p " + Target).c_str());
    system(("mkdir -p " + Target + "/" + GENIE_tune).c_str());
    system(("mkdir -p " + Target + "/" + GENIE_tune + "/" + BeamE).c_str());

    string SaveDir = Target + "/" + GENIE_tune + "/" + BeamE + "/" + Q2_th;
    system(("rm -r " + SaveDir).c_str());
    system(("mkdir -p " + SaveDir).c_str());

    double theta_e_min = (BeamE == "2070MeV") ? 7 : (BeamE == "4029MeV") ? 10 : (BeamE == "5986MeV") ? 9 : 0;

    string SaveDir_inc = SaveDir + "/01_inc";
    system(("mkdir -p " + SaveDir_inc).c_str());

#pragma region /* Set histograms */

    TH2D *h_theta_e_VS_phi_e_all_Int_inc =
        new TH1D("theta_e_VS_phi_e_all_Int_inc",
                 ("#theta_{e} vs. #phi_{e} in (e,e') for #theta_{e}#geq" + ToStringWithPrecision(theta_e_min, 0) + "#circ;#phi_{e} [#circ];#theta_{e} [#circ]").c_str(), 100, Phi_lboundary,
                 Phi_uboundary, 100, Theta_lboundary_FD, Theta_uboundary_FD);

    TH1D *h_El_all_Int_inc = new TH1D("El_all_Int_inc", ("E_{e} in (e,e') for #theta_{e}#geq" + ToStringWithPrecision(theta_e_min, 0) + "#circ;E_{e} [GeV]").c_str(), 50, 0, 6.5);
    TH1D *h_Q2_all_Int_inc = new TH1D("Q2_all_Int_inc", ("Q2 in (e,e') for #theta_{e}#geq" + ToStringWithPrecision(theta_e_min, 0) + "#circ;Q^{2} [GeV^{2}/c^{2}]").c_str(), 50, 0, 6.5);
    TH1D *h_El_QE_inc = new TH1D("El_QE_inc", ("E_{e} in (e,e') for #theta_{e}#geq" + ToStringWithPrecision(theta_e_min, 0) + "#circ and QE;E_{e} [GeV]").c_str(), 50, 0, 6.5);
    TH1D *h_Q2_QE_inc = new TH1D("Q2_QE_inc", ("Q2 in (e,e') for #theta_{e}#geq" + ToStringWithPrecision(theta_e_min, 0) + "#circ and QE;Q^{2} [GeV^{2}/c^{2}]").c_str(), 50, 0, 6.5);
    TH1D *h_El_MEC_inc = new TH1D("El_MEC_inc", ("E_{e} in (e,e') for #theta_{e}#geq" + ToStringWithPrecision(theta_e_min, 0) + "#circ and MEC;E_{e} [GeV]").c_str(), 50, 0, 6.5);
    TH1D *h_Q2_MEC_inc = new TH1D("Q2_MEC_inc", ("Q2 in (e,e') for #theta_{e}#geq" + ToStringWithPrecision(theta_e_min, 0) + "#circ and MEC;Q^{2} [GeV^{2}/c^{2}]").c_str(), 50, 0, 6.5);
    TH1D *h_El_RES_inc = new TH1D("El_RES_inc", ("E_{e} in (e,e') for #theta_{e}#geq" + ToStringWithPrecision(theta_e_min, 0) + "#circ and RES;E_{e} [GeV]").c_str(), 50, 0, 6.5);
    TH1D *h_Q2_RES_inc = new TH1D("Q2_RES_inc", ("Q2 in (e,e') for #theta_{e}#geq" + ToStringWithPrecision(theta_e_min, 0) + "#circ and RES;Q^{2} [GeV^{2}/c^{2}]").c_str(), 50, 0, 6.5);
    TH1D *h_El_DIS_inc = new TH1D("El_DIS_inc", ("E_{e} in (e,e') for #theta_{e}#geq" + ToStringWithPrecision(theta_e_min, 0) + "#circ and DIS;E_{e} [GeV]").c_str(), 50, 0, 6.5);
    TH1D *h_Q2_DIS_inc = new TH1D("Q2_DIS_inc", ("Q2 in (e,e') for #theta_{e}#geq" + ToStringWithPrecision(theta_e_min, 0) + "#circ and DIS;Q^{2} [GeV^{2}/c^{2}]").c_str(), 50, 0, 6.5);

    string SaveDir_2N_no_cuts = SaveDir + "/02a_2N_no_cuts";
    system(("mkdir -p " + SaveDir_2N_no_cuts).c_str());

    TH1D *h_El_all_Int_2N_no_cuts = new TH1D("El_all_Int_2N_no_cuts", "E_{e} in 2N without nucleon cuts;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_2N_no_cuts = new TH1D("Q2_all_Int_2N_no_cuts", "Q2 in 2N without nucleon cuts;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_2N_no_cuts = new TH1D("El_QE_2N_no_cuts", "E_{e} in 2N without nucleon cuts and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_2N_no_cuts = new TH1D("Q2_QE_2N_no_cuts", "Q2 in 2N without nucleon cuts and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_2N_no_cuts = new TH1D("El_MEC_2N_no_cuts", "E_{e} in 2N without nucleon cuts and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_2N_no_cuts = new TH1D("Q2_MEC_2N_no_cuts", "Q2 in 2N without nucleon cuts and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_2N_no_cuts = new TH1D("El_RES_2N_no_cuts", "E_{e} in 2N without nucleon cuts and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_2N_no_cuts = new TH1D("Q2_RES_2N_no_cuts", "Q2 in 2N without nucleon cuts and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_2N_no_cuts = new TH1D("El_DIS_2N_no_cuts", "E_{e} in 2N without nucleon cuts and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_2N_no_cuts = new TH1D("Q2_DIS_2N_no_cuts", "Q2 in 2N without nucleon cuts and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    string SaveDir_2p_no_cuts = SaveDir + "/02b_2p_no_cuts";
    system(("mkdir -p " + SaveDir_2p_no_cuts).c_str());

    TH1D *h_El_all_Int_2p_no_cuts = new TH1D("El_all_Int_2p_no_cuts", "E_{e} in 2p without nucleon cuts;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_2p_no_cuts = new TH1D("Q2_all_Int_2p_no_cuts", "Q2 in 2p without nucleon cuts;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_2p_no_cuts = new TH1D("El_QE_2p_no_cuts", "E_{e} in 2p without nucleon cuts and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_2p_no_cuts = new TH1D("Q2_QE_2p_no_cuts", "Q2 in 2p without nucleon cuts and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_2p_no_cuts = new TH1D("El_MEC_2p_no_cuts", "E_{e} in 2p without nucleon cuts and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_2p_no_cuts = new TH1D("Q2_MEC_2p_no_cuts", "Q2 in 2p without nucleon cuts and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_2p_no_cuts = new TH1D("El_RES_2p_no_cuts", "E_{e} in 2p without nucleon cuts and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_2p_no_cuts = new TH1D("Q2_RES_2p_no_cuts", "Q2 in 2p without nucleon cuts and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_2p_no_cuts = new TH1D("El_DIS_2p_no_cuts", "E_{e} in 2p without nucleon cuts and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_2p_no_cuts = new TH1D("Q2_DIS_2p_no_cuts", "Q2 in 2p without nucleon cuts and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    string SaveDir_1n1p_no_cuts = SaveDir + "/02c_1n1p_no_cuts";
    system(("mkdir -p " + SaveDir_1n1p_no_cuts).c_str());

    TH1D *h_El_all_Int_1n1p_no_cuts = new TH1D("El_all_Int_1n1p_no_cuts", "E_{e} in 1n1p without nucleon cuts;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_1n1p_no_cuts = new TH1D("Q2_all_Int_1n1p_no_cuts", "Q2 in 1n1p without nucleon cuts;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_1n1p_no_cuts = new TH1D("El_QE_1n1p_no_cuts", "E_{e} in 1n1p without nucleon cuts and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_1n1p_no_cuts = new TH1D("Q2_QE_1n1p_no_cuts", "Q2 in 1n1p without nucleon cuts and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_1n1p_no_cuts = new TH1D("El_MEC_1n1p_no_cuts", "E_{e} in 1n1p without nucleon cuts and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_1n1p_no_cuts = new TH1D("Q2_MEC_1n1p_no_cuts", "Q2 in 1n1p without nucleon cuts and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_1n1p_no_cuts = new TH1D("El_RES_1n1p_no_cuts", "E_{e} in 1n1p without nucleon cuts and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_1n1p_no_cuts = new TH1D("Q2_RES_1n1p_no_cuts", "Q2 in 1n1p without nucleon cuts and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_1n1p_no_cuts = new TH1D("El_DIS_1n1p_no_cuts", "E_{e} in 1n1p without nucleon cuts and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_1n1p_no_cuts = new TH1D("Q2_DIS_1n1p_no_cuts", "Q2 in 1n1p without nucleon cuts and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    string SaveDir_2N_with_mom_cuts = SaveDir + "/03a_2N_with_mom_cuts";
    system(("mkdir -p " + SaveDir_2N_with_mom_cuts).c_str());

    TH1D *h_El_all_Int_2N_with_mom_cuts = new TH1D("El_all_Int_2N_with_mom_cuts", "E_{e} in 2N with only momentum cuts;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_2N_with_mom_cuts = new TH1D("Q2_all_Int_2N_with_mom_cuts", "Q2 in 2N with only momentum cuts;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_2N_with_mom_cuts = new TH1D("El_QE_2N_with_mom_cuts", "E_{e} in 2N with only momentum cuts and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_2N_with_mom_cuts = new TH1D("Q2_QE_2N_with_mom_cuts", "Q2 in 2N with only momentum cuts and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_2N_with_mom_cuts = new TH1D("El_MEC_2N_with_mom_cuts", "E_{e} in 2N with only momentum cuts and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_2N_with_mom_cuts = new TH1D("Q2_MEC_2N_with_mom_cuts", "Q2 in 2N with only momentum cuts and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_2N_with_mom_cuts = new TH1D("El_RES_2N_with_mom_cuts", "E_{e} in 2N with only momentum cuts and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_2N_with_mom_cuts = new TH1D("Q2_RES_2N_with_mom_cuts", "Q2 in 2N with only momentum cuts and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_2N_with_mom_cuts = new TH1D("El_DIS_2N_with_mom_cuts", "E_{e} in 2N with only momentum cuts and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_2N_with_mom_cuts = new TH1D("Q2_DIS_2N_with_mom_cuts", "Q2 in 2N with only momentum cuts and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    string SaveDir_2p_with_mom_cuts = SaveDir + "/03b_2p_with_mom_cuts";
    system(("mkdir -p " + SaveDir_2p_with_mom_cuts).c_str());

    TH1D *h_El_all_Int_2p_with_mom_cuts = new TH1D("El_all_Int_2p_with_mom_cuts", "E_{e} in 2p with only momentum cuts;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_2p_with_mom_cuts = new TH1D("Q2_all_Int_2p_with_mom_cuts", "Q2 in 2p with only momentum cuts;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_2p_with_mom_cuts = new TH1D("El_QE_2p_with_mom_cuts", "E_{e} in 2p with only momentum cuts and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_2p_with_mom_cuts = new TH1D("Q2_QE_2p_with_mom_cuts", "Q2 in 2p with only momentum cuts and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_2p_with_mom_cuts = new TH1D("El_MEC_2p_with_mom_cuts", "E_{e} in 2p with only momentum cuts and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_2p_with_mom_cuts = new TH1D("Q2_MEC_2p_with_mom_cuts", "Q2 in 2p with only momentum cuts and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_2p_with_mom_cuts = new TH1D("El_RES_2p_with_mom_cuts", "E_{e} in 2p with only momentum cuts and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_2p_with_mom_cuts = new TH1D("Q2_RES_2p_with_mom_cuts", "Q2 in 2p with only momentum cuts and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_2p_with_mom_cuts = new TH1D("El_DIS_2p_with_mom_cuts", "E_{e} in 2p with only momentum cuts and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_2p_with_mom_cuts = new TH1D("Q2_DIS_2p_with_mom_cuts", "Q2 in 2p with only momentum cuts and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    string SaveDir_1n1p_with_mom_cuts = SaveDir + "/03c_1n1p_with_mom_cuts";
    system(("mkdir -p " + SaveDir_1n1p_with_mom_cuts).c_str());

    TH1D *h_El_all_Int_1n1p_with_mom_cuts = new TH1D("El_all_Int_1n1p_with_mom_cuts", "E_{e} in 1n1p with only momentum cuts;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_1n1p_with_mom_cuts = new TH1D("Q2_all_Int_1n1p_with_mom_cuts", "Q2 in 1n1p with only momentum cuts;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_1n1p_with_mom_cuts = new TH1D("El_QE_1n1p_with_mom_cuts", "E_{e} in 1n1p with only momentum cuts and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_1n1p_with_mom_cuts = new TH1D("Q2_QE_1n1p_with_mom_cuts", "Q2 in 1n1p with only momentum cuts and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_1n1p_with_mom_cuts = new TH1D("El_MEC_1n1p_with_mom_cuts", "E_{e} in 1n1p with only momentum cuts and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_1n1p_with_mom_cuts = new TH1D("Q2_MEC_1n1p_with_mom_cuts", "Q2 in 1n1p with only momentum cuts and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_1n1p_with_mom_cuts = new TH1D("El_RES_1n1p_with_mom_cuts", "E_{e} in 1n1p with only momentum cuts and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_1n1p_with_mom_cuts = new TH1D("Q2_RES_1n1p_with_mom_cuts", "Q2 in 1n1p with only momentum cuts and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_1n1p_with_mom_cuts = new TH1D("El_DIS_1n1p_with_mom_cuts", "E_{e} in 1n1p with only momentum cuts and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_1n1p_with_mom_cuts = new TH1D("Q2_DIS_1n1p_with_mom_cuts", "Q2 in 1n1p with only momentum cuts and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    string SaveDir_2N_with_theta_nuc_cuts = SaveDir + "/04a_2N_with_theta_nuc_cuts";
    system(("mkdir -p " + SaveDir_2N_with_theta_nuc_cuts).c_str());

    TH1D *h_El_all_Int_2N_with_theta_nuc_cuts = new TH1D("El_all_Int_2N_with_theta_nuc_cuts", "E_{e} in 2N with only #theta_{nuc} cuts;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_2N_with_theta_nuc_cuts = new TH1D("Q2_all_Int_2N_with_theta_nuc_cuts", "Q2 in 2N with only #theta_{nuc} cuts;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_2N_with_theta_nuc_cuts = new TH1D("El_QE_2N_with_theta_nuc_cuts", "E_{e} in 2N with only #theta_{nuc} cuts and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_2N_with_theta_nuc_cuts = new TH1D("Q2_QE_2N_with_theta_nuc_cuts", "Q2 in 2N with only #theta_{nuc} cuts and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_2N_with_theta_nuc_cuts = new TH1D("El_MEC_2N_with_theta_nuc_cuts", "E_{e} in 2N with only #theta_{nuc} cuts and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_2N_with_theta_nuc_cuts = new TH1D("Q2_MEC_2N_with_theta_nuc_cuts", "Q2 in 2N with only #theta_{nuc} cuts and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_2N_with_theta_nuc_cuts = new TH1D("El_RES_2N_with_theta_nuc_cuts", "E_{e} in 2N with only #theta_{nuc} cuts and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_2N_with_theta_nuc_cuts = new TH1D("Q2_RES_2N_with_theta_nuc_cuts", "Q2 in 2N with only #theta_{nuc} cuts and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_2N_with_theta_nuc_cuts = new TH1D("El_DIS_2N_with_theta_nuc_cuts", "E_{e} in 2N with only #theta_{nuc} cuts and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_2N_with_theta_nuc_cuts = new TH1D("Q2_DIS_2N_with_theta_nuc_cuts", "Q2 in 2N with only #theta_{nuc} cuts and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    string SaveDir_2p_with_theta_nuc_cuts = SaveDir + "/04b_2p_with_theta_nuc_cuts";
    system(("mkdir -p " + SaveDir_2p_with_theta_nuc_cuts).c_str());

    TH1D *h_El_all_Int_2p_with_theta_nuc_cuts = new TH1D("El_all_Int_2p_with_theta_nuc_cuts", "E_{e} in 2p with only #theta_{nuc} cuts;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_2p_with_theta_nuc_cuts = new TH1D("Q2_all_Int_2p_with_theta_nuc_cuts", "Q2 in 2p with only #theta_{nuc} cuts;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_2p_with_theta_nuc_cuts = new TH1D("El_QE_2p_with_theta_nuc_cuts", "E_{e} in 2p with only #theta_{nuc} cuts and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_2p_with_theta_nuc_cuts = new TH1D("Q2_QE_2p_with_theta_nuc_cuts", "Q2 in 2p with only #theta_{nuc} cuts and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_2p_with_theta_nuc_cuts = new TH1D("El_MEC_2p_with_theta_nuc_cuts", "E_{e} in 2p with only #theta_{nuc} cuts and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_2p_with_theta_nuc_cuts = new TH1D("Q2_MEC_2p_with_theta_nuc_cuts", "Q2 in 2p with only #theta_{nuc} cuts and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_2p_with_theta_nuc_cuts = new TH1D("El_RES_2p_with_theta_nuc_cuts", "E_{e} in 2p with only #theta_{nuc} cuts and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_2p_with_theta_nuc_cuts = new TH1D("Q2_RES_2p_with_theta_nuc_cuts", "Q2 in 2p with only #theta_{nuc} cuts and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_2p_with_theta_nuc_cuts = new TH1D("El_DIS_2p_with_theta_nuc_cuts", "E_{e} in 2p with only #theta_{nuc} cuts and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_2p_with_theta_nuc_cuts = new TH1D("Q2_DIS_2p_with_theta_nuc_cuts", "Q2 in 2p with only #theta_{nuc} cuts and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    string SaveDir_1n1p_with_theta_nuc_cuts = SaveDir + "/04c_1n1p_with_theta_nuc_cuts";
    system(("mkdir -p " + SaveDir_1n1p_with_theta_nuc_cuts).c_str());

    TH1D *h_El_all_Int_1n1p_with_theta_nuc_cuts = new TH1D("El_all_Int_1n1p_with_theta_nuc_cuts", "E_{e} in 1n1p with only #theta_{nuc} cuts;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_1n1p_with_theta_nuc_cuts = new TH1D("Q2_all_Int_1n1p_with_theta_nuc_cuts", "Q2 in 1n1p with only #theta_{nuc} cuts;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_1n1p_with_theta_nuc_cuts = new TH1D("El_QE_1n1p_with_theta_nuc_cuts", "E_{e} in 1n1p with only #theta_{nuc} cuts and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_1n1p_with_theta_nuc_cuts = new TH1D("Q2_QE_1n1p_with_theta_nuc_cuts", "Q2 in 1n1p with only #theta_{nuc} cuts and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_1n1p_with_theta_nuc_cuts = new TH1D("El_MEC_1n1p_with_theta_nuc_cuts", "E_{e} in 1n1p with only #theta_{nuc} cuts and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_1n1p_with_theta_nuc_cuts = new TH1D("Q2_MEC_1n1p_with_theta_nuc_cuts", "Q2 in 1n1p with only #theta_{nuc} cuts and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_1n1p_with_theta_nuc_cuts = new TH1D("El_RES_1n1p_with_theta_nuc_cuts", "E_{e} in 1n1p with only #theta_{nuc} cuts and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_1n1p_with_theta_nuc_cuts = new TH1D("Q2_RES_1n1p_with_theta_nuc_cuts", "Q2 in 1n1p with only #theta_{nuc} cuts and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_1n1p_with_theta_nuc_cuts = new TH1D("El_DIS_1n1p_with_theta_nuc_cuts", "E_{e} in 1n1p with only #theta_{nuc} cuts and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_1n1p_with_theta_nuc_cuts = new TH1D("Q2_DIS_1n1p_with_theta_nuc_cuts", "Q2 in 1n1p with only #theta_{nuc} cuts and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    string SaveDir_2p_all_cuts = SaveDir + "/05b_2p_all_cuts";
    system(("mkdir -p " + SaveDir_2p_all_cuts).c_str());

    TH1D *h_El_all_Int_2p_all_cuts = new TH1D("El_all_Int_2p_all_cuts", "E_{e} in 2p with all cuts;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_2p_all_cuts = new TH1D("Q2_all_Int_2p_all_cuts", "Q2 in 2p with all cuts;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_2p_all_cuts = new TH1D("El_QE_2p_all_cuts", "E_{e} in 2p with all cuts and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_2p_all_cuts = new TH1D("Q2_QE_2p_all_cuts", "Q2 in 2p with all cuts and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_2p_all_cuts = new TH1D("El_MEC_2p_all_cuts", "E_{e} in 2p with all cuts and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_2p_all_cuts = new TH1D("Q2_MEC_2p_all_cuts", "Q2 in 2p with all cuts and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_2p_all_cuts = new TH1D("El_RES_2p_all_cuts", "E_{e} in 2p with all cuts and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_2p_all_cuts = new TH1D("Q2_RES_2p_all_cuts", "Q2 in 2p with all cuts and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_2p_all_cuts = new TH1D("El_DIS_2p_all_cuts", "E_{e} in 2p with all cuts and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_2p_all_cuts = new TH1D("Q2_DIS_2p_all_cuts", "Q2 in 2p with all cuts and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    string SaveDir_1n1p_all_cuts = SaveDir + "/05c_1n1p_all_cuts";
    system(("mkdir -p " + SaveDir_1n1p_all_cuts).c_str());

    TH1D *h_El_all_Int_1n1p_all_cuts = new TH1D("El_all_Int_1n1p_all_cuts", "E_{e} in 1n1p with all cuts;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_1n1p_all_cuts = new TH1D("Q2_all_Int_1n1p_all_cuts", "Q2 in 1n1p with all cuts;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_1n1p_all_cuts = new TH1D("El_QE_1n1p_all_cuts", "E_{e} in 1n1p with all cuts and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_1n1p_all_cuts = new TH1D("Q2_QE_1n1p_all_cuts", "Q2 in 1n1p with all cuts and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_1n1p_all_cuts = new TH1D("El_MEC_1n1p_all_cuts", "E_{e} in 1n1p with all cuts and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_1n1p_all_cuts = new TH1D("Q2_MEC_1n1p_all_cuts", "Q2 in 1n1p with all cuts and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_1n1p_all_cuts = new TH1D("El_RES_1n1p_all_cuts", "E_{e} in 1n1p with all cuts and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_1n1p_all_cuts = new TH1D("Q2_RES_1n1p_all_cuts", "Q2 in 1n1p with all cuts and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_1n1p_all_cuts = new TH1D("El_DIS_1n1p_all_cuts", "E_{e} in 1n1p with all cuts and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_1n1p_all_cuts = new TH1D("Q2_DIS_1n1p_all_cuts", "Q2 in 1n1p with all cuts and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

#pragma endregion

#pragma region /* Set acceptance maps */

    string AcceptanceMapsDirectory = "source/data/amaps";
    AMaps aMaps_master = AMaps(AcceptanceMapsDirectory, BeamE);

#pragma endregion

#pragma region /* Loop over events */

    int limiter = -1;
    // int limiter = 1000000;

    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntries();
    // Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;

    bool TL_fiducial_cuts = true;

    double weight = 1.0;

    for (Long64_t jentry = 0; jentry < nentries; jentry++) {
        if ((limiter > 0) && (jentry > limiter)) { break; }

        if (jentry % 100000 == 0) { cout << "FillHistogramByProcessing event " << jentry << " of " << nentries << " (" << 100 * (jentry / nentries) << "%)" << endl; }

        Long64_t ientry = LoadTree(jentry);

        if (ientry < 0) break;

        nb = fChain->GetEntry(jentry);

        nbytes += nb;

        double P_e = pl;
        double theta_e = CalcTheta_deg(pxl, pyl, pzl);
        double phi_e = CalcPhi_deg(pxl, pyl);

        if (theta_e < theta_e_min) { continue; }

        bool e_inFD = aMaps_master.IsInFDQuery((!TL_fiducial_cuts), ThetaFD, "Electron", P_e, theta_e, phi_e);

        bool Is2p = (nf == 2 && nfn == 0 && nfp == 2);
        bool Is1n1p = (nf == 2 && nfn == 1 && nfp == 1);

        h_theta_e_VS_phi_e_all_Int_inc->Fill(phi_e, theta_e, weight);
        
        FillHistogramByProcess(El, h_El_all_Int_inc, h_El_QE_inc, h_El_MEC_inc, h_El_RES_inc, h_El_DIS_inc, weight, qel, mec, res, dis);
        FillHistogramByProcess(Q2, h_Q2_all_Int_inc, h_Q2_QE_inc, h_Q2_MEC_inc, h_Q2_RES_inc, h_Q2_DIS_inc, weight, qel, mec, res, dis);

        if (!(Is2p || Is1n1p)) { continue; }

        FillHistogramByProcess(El, h_El_all_Int_2N_no_cuts, h_El_QE_2N_no_cuts, h_El_MEC_2N_no_cuts, h_El_RES_2N_no_cuts, h_El_DIS_2N_no_cuts, weight, qel, mec, res, dis);
        FillHistogramByProcess(Q2, h_Q2_all_Int_2N_no_cuts, h_Q2_QE_2N_no_cuts, h_Q2_MEC_2N_no_cuts, h_Q2_RES_2N_no_cuts, h_Q2_DIS_2N_no_cuts, weight, qel, mec, res, dis);

        if (Is2p) {
            FillHistogramByProcess(El, h_El_all_Int_2p_no_cuts, h_El_QE_2p_no_cuts, h_El_MEC_2p_no_cuts, h_El_RES_2p_no_cuts, h_El_DIS_2p_no_cuts, weight, qel, mec, res, dis);
            FillHistogramByProcess(Q2, h_Q2_all_Int_2p_no_cuts, h_Q2_QE_2p_no_cuts, h_Q2_MEC_2p_no_cuts, h_Q2_RES_2p_no_cuts, h_Q2_DIS_2p_no_cuts, weight, qel, mec, res, dis);

            double theta_p1 = CalcTheta_deg(pxf[0], pyf[0], pzf[0]);
            double theta_p2 = CalcTheta_deg(pxf[1], pyf[1], pzf[1]);
            bool PassMomCuts = ((pf[0] >= 0.4) && (pf[1] >= 0.4));
            bool PassAcceptance =
                (((theta_p1 >= 5. && theta_p1 <= 40.) && (theta_p2 >= 40. && theta_p2 <= 140.)) || ((theta_p2 >= 5. && theta_p2 <= 40.) && (theta_p1 >= 40. && theta_p1 <= 140.)));
            bool ispFDpCD = (PassMomCuts && PassAcceptance);

            if (PassMomCuts) {
                FillHistogramByProcess(El, h_El_all_Int_2N_with_mom_cuts, h_El_QE_2N_with_mom_cuts, h_El_MEC_2N_with_mom_cuts, h_El_RES_2N_with_mom_cuts, h_El_DIS_2N_with_mom_cuts, qel, mec,
                                       res, dis);
                FillHistogramByProcess(Q2, h_Q2_all_Int_2N_with_mom_cuts, h_Q2_QE_2N_with_mom_cuts, h_Q2_MEC_2N_with_mom_cuts, h_Q2_RES_2N_with_mom_cuts, h_Q2_DIS_2N_with_mom_cuts, qel, mec,
                                       res, dis);
                FillHistogramByProcess(El, h_El_all_Int_2p_with_mom_cuts, h_El_QE_2p_with_mom_cuts, h_El_MEC_2p_with_mom_cuts, h_El_RES_2p_with_mom_cuts, h_El_DIS_2p_with_mom_cuts, qel, mec,
                                       res, dis);
                FillHistogramByProcess(Q2, h_Q2_all_Int_2p_with_mom_cuts, h_Q2_QE_2p_with_mom_cuts, h_Q2_MEC_2p_with_mom_cuts, h_Q2_RES_2p_with_mom_cuts, h_Q2_DIS_2p_with_mom_cuts, qel, mec,
                                       res, dis);
            }

            if (PassAcceptance) {
                FillHistogramByProcess(El, h_El_all_Int_2N_with_theta_nuc_cuts, h_El_QE_2N_with_theta_nuc_cuts, h_El_MEC_2N_with_theta_nuc_cuts, h_El_RES_2N_with_theta_nuc_cuts,
                                       h_El_DIS_2N_with_theta_nuc_cuts, weight, qel, mec, res, dis);
                FillHistogramByProcess(Q2, h_Q2_all_Int_2N_with_theta_nuc_cuts, h_Q2_QE_2N_with_theta_nuc_cuts, h_Q2_MEC_2N_with_theta_nuc_cuts, h_Q2_RES_2N_with_theta_nuc_cuts,
                                       h_Q2_DIS_2N_with_theta_nuc_cuts, weight, qel, mec, res, dis);
                FillHistogramByProcess(El, h_El_all_Int_2p_with_theta_nuc_cuts, h_El_QE_2p_with_theta_nuc_cuts, h_El_MEC_2p_with_theta_nuc_cuts, h_El_RES_2p_with_theta_nuc_cuts,
                                       h_El_DIS_2p_with_theta_nuc_cuts, weight, qel, mec, res, dis);
                FillHistogramByProcess(Q2, h_Q2_all_Int_2p_with_theta_nuc_cuts, h_Q2_QE_2p_with_theta_nuc_cuts, h_Q2_MEC_2p_with_theta_nuc_cuts, h_Q2_RES_2p_with_theta_nuc_cuts,
                                       h_Q2_DIS_2p_with_theta_nuc_cuts, weight, qel, mec, res, dis);
            }

            if (ispFDpCD) {
                FillHistogramByProcess(El, h_El_all_Int_2p_all_cuts, h_El_QE_2p_all_cuts, h_El_MEC_2p_all_cuts, h_El_RES_2p_all_cuts, h_El_DIS_2p_all_cuts, weight, qel, mec, res, dis);
                FillHistogramByProcess(Q2, h_Q2_all_Int_2p_all_cuts, h_Q2_QE_2p_all_cuts, h_Q2_MEC_2p_all_cuts, h_Q2_RES_2p_all_cuts, h_Q2_DIS_2p_all_cuts, weight, qel, mec, res, dis);
            }
        } else if (Is1n1p) {
            FillHistogramByProcess(El, h_El_all_Int_1n1p_no_cuts, h_El_QE_1n1p_no_cuts, h_El_MEC_1n1p_no_cuts, h_El_RES_1n1p_no_cuts, h_El_DIS_1n1p_no_cuts, weight, qel, mec, res, dis);
            FillHistogramByProcess(Q2, h_Q2_all_Int_1n1p_no_cuts, h_Q2_QE_1n1p_no_cuts, h_Q2_MEC_1n1p_no_cuts, h_Q2_RES_1n1p_no_cuts, h_Q2_DIS_1n1p_no_cuts, weight, qel, mec, res, dis);

            int neutron_ind = (pdgf[0] == 2112) ? 0 : 1;
            int proton_ind = 1 - neutron_ind;

            double theta_n = CalcTheta_deg(pxf[neutron_ind], pyf[neutron_ind], pzf[neutron_ind]);
            double theta_p = CalcTheta_deg(pxf[proton_ind], pyf[proton_ind], pzf[proton_ind]);
            bool PassMomCuts = ((pf[0] >= 0.4) && (pf[1] >= 0.4));
            bool PassAcceptance = ((theta_n >= 5. && theta_n <= 40.) && (theta_p >= 40. && theta_p <= 140.));
            bool isnFDpCD = (PassMomCuts && PassAcceptance);

            if (PassMomCuts) {
                FillHistogramByProcess(El, h_El_all_Int_2N_with_mom_cuts, h_El_QE_2N_with_mom_cuts, h_El_MEC_2N_with_mom_cuts, h_El_RES_2N_with_mom_cuts, h_El_DIS_2N_with_mom_cuts, qel, mec,
                                       res, dis);
                FillHistogramByProcess(Q2, h_Q2_all_Int_2N_with_mom_cuts, h_Q2_QE_2N_with_mom_cuts, h_Q2_MEC_2N_with_mom_cuts, h_Q2_RES_2N_with_mom_cuts, h_Q2_DIS_2N_with_mom_cuts, qel, mec,
                                       res, dis);
                FillHistogramByProcess(El, h_El_all_Int_1n1p_with_mom_cuts, h_El_QE_1n1p_with_mom_cuts, h_El_MEC_1n1p_with_mom_cuts, h_El_RES_1n1p_with_mom_cuts, h_El_DIS_1n1p_with_mom_cuts,
                                       qel, mec, res, dis);
                FillHistogramByProcess(Q2, h_Q2_all_Int_1n1p_with_mom_cuts, h_Q2_QE_1n1p_with_mom_cuts, h_Q2_MEC_1n1p_with_mom_cuts, h_Q2_RES_1n1p_with_mom_cuts, h_Q2_DIS_1n1p_with_mom_cuts,
                                       qel, mec, res, dis);
            }

            if (PassAcceptance) {
                FillHistogramByProcess(El, h_El_all_Int_2N_with_theta_nuc_cuts, h_El_QE_2N_with_theta_nuc_cuts, h_El_MEC_2N_with_theta_nuc_cuts, h_El_RES_2N_with_theta_nuc_cuts,
                                       h_El_DIS_2N_with_theta_nuc_cuts, weight, qel, mec, res, dis);
                FillHistogramByProcess(Q2, h_Q2_all_Int_2N_with_theta_nuc_cuts, h_Q2_QE_2N_with_theta_nuc_cuts, h_Q2_MEC_2N_with_theta_nuc_cuts, h_Q2_RES_2N_with_theta_nuc_cuts,
                                       h_Q2_DIS_2N_with_theta_nuc_cuts, weight, qel, mec, res, dis);
                FillHistogramByProcess(El, h_El_all_Int_1n1p_with_theta_nuc_cuts, h_El_QE_1n1p_with_theta_nuc_cuts, h_El_MEC_1n1p_with_theta_nuc_cuts, h_El_RES_1n1p_with_theta_nuc_cuts,
                                       h_El_DIS_1n1p_with_theta_nuc_cuts, weight, qel, mec, res, dis);
                FillHistogramByProcess(Q2, h_Q2_all_Int_1n1p_with_theta_nuc_cuts, h_Q2_QE_1n1p_with_theta_nuc_cuts, h_Q2_MEC_1n1p_with_theta_nuc_cuts, h_Q2_RES_1n1p_with_theta_nuc_cuts,
                                       h_Q2_DIS_1n1p_with_theta_nuc_cuts, weight, qel, mec, res, dis);
            }

            if (isnFDpCD) {
                FillHistogramByProcess(El, h_El_all_Int_1n1p_all_cuts, h_El_QE_1n1p_all_cuts, h_El_MEC_1n1p_all_cuts, h_El_RES_1n1p_all_cuts, h_El_DIS_1n1p_all_cuts, weight, qel, mec, res,
                                       dis);
                FillHistogramByProcess(Q2, h_Q2_all_Int_1n1p_all_cuts, h_Q2_QE_1n1p_all_cuts, h_Q2_MEC_1n1p_all_cuts, h_Q2_RES_1n1p_all_cuts, h_Q2_DIS_1n1p_all_cuts, weight, qel, mec, res,
                                       dis);
            }
        }
    }

#pragma endregion

    cout << "\n";

    int pixelx = 1980, pixely = 1530;
    TCanvas *canvas = new TCanvas("myText", "myText", pixelx, pixely);
    canvas->cd();

    h_El_all_Int_inc->Draw();
    cout << "\n\nh_El_all_Int_inc->GetEntries() = " << h_El_all_Int_inc->GetEntries() << "\n";
    canvas->SaveAs(("./" + SaveDir_inc + "/h_El_all_Int_inc.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_inc->Draw();
    cout << "h_Q2_all_Int_inc->GetEntries() = " << h_Q2_all_Int_inc->GetEntries() << "\n";
    canvas->SaveAs(("./" + SaveDir_inc + "/h_Q2_all_Int_inc.pdf").c_str());
    canvas->Clear();

    h_El_QE_inc->Draw();
    cout << "h_El_QE_inc->GetEntries() = " << h_El_QE_inc->GetEntries() << "\n";
    canvas->SaveAs(("./" + SaveDir_inc + "/h_El_QE_inc.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_inc->Draw();
    cout << "h_Q2_QE_inc->GetEntries() = " << h_Q2_QE_inc->GetEntries() << "\n";
    canvas->SaveAs(("./" + SaveDir_inc + "/h_Q2_QE_inc.pdf").c_str());
    canvas->Clear();

    h_El_MEC_inc->Draw();
    cout << "h_El_MEC_inc->GetEntries() = " << h_El_MEC_inc->GetEntries() << "\n";
    canvas->SaveAs(("./" + SaveDir_inc + "/h_El_MEC_inc.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_inc->Draw();
    cout << "h_Q2_MEC_inc->GetEntries() = " << h_Q2_MEC_inc->GetEntries() << "\n";
    canvas->SaveAs(("./" + SaveDir_inc + "/h_Q2_MEC_inc.pdf").c_str());
    canvas->Clear();

    h_El_RES_inc->Draw();
    cout << "h_El_RES_inc->GetEntries() = " << h_El_RES_inc->GetEntries() << "\n";
    canvas->SaveAs(("./" + SaveDir_inc + "/h_El_RES_inc.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_inc->Draw();
    cout << "h_Q2_RES_inc->GetEntries() = " << h_Q2_RES_inc->GetEntries() << "\n";
    canvas->SaveAs(("./" + SaveDir_inc + "/h_Q2_RES_inc.pdf").c_str());
    canvas->Clear();

    h_El_DIS_inc->Draw();
    cout << "h_El_DIS_inc->GetEntries() = " << h_El_DIS_inc->GetEntries() << "\n";
    canvas->SaveAs(("./" + SaveDir_inc + "/h_El_DIS_inc.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_inc->Draw();
    cout << "h_Q2_DIS_inc->GetEntries() = " << h_Q2_DIS_inc->GetEntries() << "\n";
    canvas->SaveAs(("./" + SaveDir_inc + "/h_Q2_DIS_inc.pdf").c_str());
    canvas->Clear();

    // 2N_no_cuts

    h_El_all_Int_2N_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_no_cuts + "/h_El_all_Int_2N_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_2N_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_no_cuts + "/h_Q2_all_Int_2N_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_QE_2N_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_no_cuts + "/h_El_QE_2N_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_2N_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_no_cuts + "/h_Q2_QE_2N_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_MEC_2N_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_no_cuts + "/h_El_MEC_2N_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_2N_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_no_cuts + "/h_Q2_MEC_2N_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_RES_2N_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_no_cuts + "/h_El_RES_2N_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_2N_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_no_cuts + "/h_Q2_RES_2N_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_DIS_2N_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_no_cuts + "/h_El_DIS_2N_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_2N_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_no_cuts + "/h_Q2_DIS_2N_no_cuts.pdf").c_str());
    canvas->Clear();

    // 2p_no_cuts

    h_El_all_Int_2p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_no_cuts + "/h_El_all_Int_2p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_2p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_no_cuts + "/h_Q2_all_Int_2p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_QE_2p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_no_cuts + "/h_El_QE_2p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_2p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_no_cuts + "/h_Q2_QE_2p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_MEC_2p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_no_cuts + "/h_El_MEC_2p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_2p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_no_cuts + "/h_Q2_MEC_2p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_RES_2p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_no_cuts + "/h_El_RES_2p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_2p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_no_cuts + "/h_Q2_RES_2p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_DIS_2p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_no_cuts + "/h_El_DIS_2p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_2p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_no_cuts + "/h_Q2_DIS_2p_no_cuts.pdf").c_str());
    canvas->Clear();

    // 1n1p_no_cuts

    h_El_all_Int_1n1p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_no_cuts + "/h_El_all_Int_1n1p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_1n1p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_no_cuts + "/h_Q2_all_Int_1n1p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_QE_1n1p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_no_cuts + "/h_El_QE_1n1p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_1n1p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_no_cuts + "/h_Q2_QE_1n1p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_MEC_1n1p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_no_cuts + "/h_El_MEC_1n1p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_1n1p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_no_cuts + "/h_Q2_MEC_1n1p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_RES_1n1p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_no_cuts + "/h_El_RES_1n1p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_1n1p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_no_cuts + "/h_Q2_RES_1n1p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_El_DIS_1n1p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_no_cuts + "/h_El_DIS_1n1p_no_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_1n1p_no_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_no_cuts + "/h_Q2_DIS_1n1p_no_cuts.pdf").c_str());
    canvas->Clear();

    // 2N_with_mom_cuts

    h_El_all_Int_2N_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_mom_cuts + "/h_El_all_Int_2N_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_2N_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_mom_cuts + "/h_Q2_all_Int_2N_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_QE_2N_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_mom_cuts + "/h_El_QE_2N_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_2N_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_mom_cuts + "/h_Q2_QE_2N_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_MEC_2N_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_mom_cuts + "/h_El_MEC_2N_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_2N_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_mom_cuts + "/h_Q2_MEC_2N_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_RES_2N_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_mom_cuts + "/h_El_RES_2N_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_2N_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_mom_cuts + "/h_Q2_RES_2N_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_DIS_2N_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_mom_cuts + "/h_El_DIS_2N_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_2N_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_mom_cuts + "/h_Q2_DIS_2N_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    // 2p_with_mom_cuts

    h_El_all_Int_2p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_mom_cuts + "/h_El_all_Int_2p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_2p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_mom_cuts + "/h_Q2_all_Int_2p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_QE_2p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_mom_cuts + "/h_El_QE_2p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_2p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_mom_cuts + "/h_Q2_QE_2p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_MEC_2p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_mom_cuts + "/h_El_MEC_2p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_2p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_mom_cuts + "/h_Q2_MEC_2p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_RES_2p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_mom_cuts + "/h_El_RES_2p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_2p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_mom_cuts + "/h_Q2_RES_2p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_DIS_2p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_mom_cuts + "/h_El_DIS_2p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_2p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_mom_cuts + "/h_Q2_DIS_2p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    // 1n1p_with_mom_cuts

    h_El_all_Int_1n1p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_mom_cuts + "/h_El_all_Int_1n1p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_1n1p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_mom_cuts + "/h_Q2_all_Int_1n1p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_QE_1n1p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_mom_cuts + "/h_El_QE_1n1p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_1n1p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_mom_cuts + "/h_Q2_QE_1n1p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_MEC_1n1p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_mom_cuts + "/h_El_MEC_1n1p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_1n1p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_mom_cuts + "/h_Q2_MEC_1n1p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_RES_1n1p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_mom_cuts + "/h_El_RES_1n1p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_1n1p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_mom_cuts + "/h_Q2_RES_1n1p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_El_DIS_1n1p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_mom_cuts + "/h_El_DIS_1n1p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_1n1p_with_mom_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_mom_cuts + "/h_Q2_DIS_1n1p_with_mom_cuts.pdf").c_str());
    canvas->Clear();

    // 2N_with_theta_nuc_cuts

    h_El_all_Int_2N_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_theta_nuc_cuts + "/h_El_all_Int_2N_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_2N_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_theta_nuc_cuts + "/h_Q2_all_Int_2N_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_QE_2N_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_theta_nuc_cuts + "/h_El_QE_2N_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_2N_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_theta_nuc_cuts + "/h_Q2_QE_2N_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_MEC_2N_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_theta_nuc_cuts + "/h_El_MEC_2N_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_2N_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_theta_nuc_cuts + "/h_Q2_MEC_2N_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_RES_2N_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_theta_nuc_cuts + "/h_El_RES_2N_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_2N_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_theta_nuc_cuts + "/h_Q2_RES_2N_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_DIS_2N_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_theta_nuc_cuts + "/h_El_DIS_2N_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_2N_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2N_with_theta_nuc_cuts + "/h_Q2_DIS_2N_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    // 2p_with_theta_nuc_cuts

    h_El_all_Int_2p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_theta_nuc_cuts + "/h_El_all_Int_2p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_2p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_theta_nuc_cuts + "/h_Q2_all_Int_2p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_QE_2p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_theta_nuc_cuts + "/h_El_QE_2p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_2p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_theta_nuc_cuts + "/h_Q2_QE_2p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_MEC_2p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_theta_nuc_cuts + "/h_El_MEC_2p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_2p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_theta_nuc_cuts + "/h_Q2_MEC_2p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_RES_2p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_theta_nuc_cuts + "/h_El_RES_2p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_2p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_theta_nuc_cuts + "/h_Q2_RES_2p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_DIS_2p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_theta_nuc_cuts + "/h_El_DIS_2p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_2p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_with_theta_nuc_cuts + "/h_Q2_DIS_2p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    // 1n1p_with_theta_nuc_cuts

    h_El_all_Int_1n1p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_theta_nuc_cuts + "/h_El_all_Int_1n1p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_1n1p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_theta_nuc_cuts + "/h_Q2_all_Int_1n1p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_QE_1n1p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_theta_nuc_cuts + "/h_El_QE_1n1p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_1n1p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_theta_nuc_cuts + "/h_Q2_QE_1n1p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_MEC_1n1p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_theta_nuc_cuts + "/h_El_MEC_1n1p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_1n1p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_theta_nuc_cuts + "/h_Q2_MEC_1n1p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_RES_1n1p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_theta_nuc_cuts + "/h_El_RES_1n1p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_1n1p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_theta_nuc_cuts + "/h_Q2_RES_1n1p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_El_DIS_1n1p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_theta_nuc_cuts + "/h_El_DIS_1n1p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_1n1p_with_theta_nuc_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_with_theta_nuc_cuts + "/h_Q2_DIS_1n1p_with_theta_nuc_cuts.pdf").c_str());
    canvas->Clear();

    // 2p_all_cuts

    h_El_all_Int_2p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_all_cuts + "/h_El_all_Int_2p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_2p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_all_cuts + "/h_Q2_all_Int_2p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_El_QE_2p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_all_cuts + "/h_El_QE_2p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_2p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_all_cuts + "/h_Q2_QE_2p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_El_MEC_2p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_all_cuts + "/h_El_MEC_2p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_2p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_all_cuts + "/h_Q2_MEC_2p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_El_RES_2p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_all_cuts + "/h_El_RES_2p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_2p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_all_cuts + "/h_Q2_RES_2p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_El_DIS_2p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_all_cuts + "/h_El_DIS_2p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_2p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_2p_all_cuts + "/h_Q2_DIS_2p_all_cuts.pdf").c_str());
    canvas->Clear();

    // 1n1p_all_cuts

    h_El_all_Int_1n1p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_all_cuts + "/h_El_all_Int_1n1p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_1n1p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_all_cuts + "/h_Q2_all_Int_1n1p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_El_QE_1n1p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_all_cuts + "/h_El_QE_1n1p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_1n1p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_all_cuts + "/h_Q2_QE_1n1p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_El_MEC_1n1p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_all_cuts + "/h_El_MEC_1n1p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_1n1p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_all_cuts + "/h_Q2_MEC_1n1p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_El_RES_1n1p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_all_cuts + "/h_El_RES_1n1p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_1n1p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_all_cuts + "/h_Q2_RES_1n1p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_El_DIS_1n1p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_all_cuts + "/h_El_DIS_1n1p_all_cuts.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_1n1p_all_cuts->Draw();
    canvas->SaveAs(("./" + SaveDir_1n1p_all_cuts + "/h_Q2_DIS_1n1p_all_cuts.pdf").c_str());
    canvas->Clear();
}
