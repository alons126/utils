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
    system(("mkdir -p " + Target + "/" + GENIE_tune + "/" + BeamE + "/" + Q2_th).c_str());
    string SaveDir = Target + "/" + GENIE_tune + "/" + BeamE + "/" + Q2_th;

    double theta_e_min;

    if (BeamE == "2070MeV") {
        theta_e_min = 7;
    } else if (BeamE == "4029MeV") {
        theta_e_min = 10;
    } else if (BeamE == "5986MeV") {
        theta_e_min = 9;
    }

    TH1D *h_El_all_Int_inc = new TH1D("El_all_Int_inc", "E_{e} in (e,e');E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_inc = new TH1D("Q2_all_Int_inc", "Q2 in (e,e');Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_inc = new TH1D("El_QE_inc", "E_{e} in (e,e') and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_inc = new TH1D("Q2_QE_inc", "Q2 in (e,e') and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_inc = new TH1D("El_MEC_inc", "E_{e} in (e,e') and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_inc = new TH1D("Q2_MEC_inc", "Q2 in (e,e') and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_inc = new TH1D("El_RES_inc", "E_{e} in (e,e') and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_inc = new TH1D("Q2_RES_inc", "Q2 in (e,e') and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_inc = new TH1D("El_DIS_inc", "E_{e} in (e,e') and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_inc = new TH1D("Q2_DIS_inc", "Q2 in (e,e') and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    TH1D *h_El_all_Int_2N = new TH1D("El_all_Int_2N", "E_{e} in 2N;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_2N = new TH1D("Q2_all_Int_2N", "Q2 in 2N;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_2N = new TH1D("El_QE_2N", "E_{e} in 2N and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_2N = new TH1D("Q2_QE_2N", "Q2 in 2N and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_2N = new TH1D("El_MEC_2N", "E_{e} in 2N and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_2N = new TH1D("Q2_MEC_2N", "Q2 in 2N and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_2N = new TH1D("El_RES_2N", "E_{e} in 2N and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_2N = new TH1D("Q2_RES_2N", "Q2 in 2N and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_2N = new TH1D("El_DIS_2N", "E_{e} in 2N and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_2N = new TH1D("Q2_DIS_2N", "Q2 in 2N and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    TH1D *h_El_all_Int_2p = new TH1D("El_all_Int_2p", "E_{e} in 2p;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_2p = new TH1D("Q2_all_Int_2p", "Q2 in 2p;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_2p = new TH1D("El_QE_2p", "E_{e} in 2p and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_2p = new TH1D("Q2_QE_2p", "Q2 in 2p and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_2p = new TH1D("El_MEC_2p", "E_{e} in 2p and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_2p = new TH1D("Q2_MEC_2p", "Q2 in 2p and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_2p = new TH1D("El_RES_2p", "E_{e} in 2p and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_2p = new TH1D("Q2_RES_2p", "Q2 in 2p and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_2p = new TH1D("El_DIS_2p", "E_{e} in 2p and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_2p = new TH1D("Q2_DIS_2p", "Q2 in 2p and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    TH1D *h_El_all_Int_1n1p = new TH1D("El_all_Int_1n1p", "E_{e} in 1n1p;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_all_Int_1n1p = new TH1D("Q2_all_Int_1n1p", "Q2 in 1n1p;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_QE_1n1p = new TH1D("El_QE_1n1p", "E_{e} in 1n1p and QE;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_QE_1n1p = new TH1D("Q2_QE_1n1p", "Q2 in 1n1p and QE;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_MEC_1n1p = new TH1D("El_MEC_1n1p", "E_{e} in 1n1p and MEC;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_MEC_1n1p = new TH1D("Q2_MEC_1n1p", "Q2 in 1n1p and MEC;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_RES_1n1p = new TH1D("El_RES_1n1p", "E_{e} in 1n1p and RES;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_RES_1n1p = new TH1D("Q2_RES_1n1p", "Q2 in 1n1p and RES;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);
    TH1D *h_El_DIS_1n1p = new TH1D("El_DIS_1n1p", "E_{e} in 1n1p and DIS;E_{e} [GeV]", 50, 0, 6.5);
    TH1D *h_Q2_DIS_1n1p = new TH1D("Q2_DIS_1n1p", "Q2 in 1n1p and DIS;Q^{2} [GeV^{2}/c^{2}]", 50, 0, 6.5);

    int limiter = -1;
    // int limiter = 1000000;

    if (fChain == 0) return;

    Long64_t nentries = fChain->GetEntriesFast();

    Long64_t nbytes = 0, nb = 0;
    for (Long64_t jentry = 0; jentry < nentries; jentry++) {
        if ((limiter > 0) && (jentry > limiter)) { break; }

        Long64_t ientry = LoadTree(jentry);

        if (ientry < 0) break;

        nb = fChain->GetEntry(jentry);

        nbytes += nb;

        if (acos(pzl / sqrt(pxl * pxl + pyl * pyl + pzl * pzl)) * 180.0 / 3.14159265359 < theta_e_min) { continue; }

        bool Is2p = (nf == 2 && nfn == 0 && nfp == 2);
        bool Is1n1p = (nf == 2 && nfn == 1 && nfp == 1);

        h_El_all_Int_inc->Fill(El);
        h_Q2_all_Int_inc->Fill(Q2);

        if (qel) {
            h_El_QE_inc->Fill(El);
            h_Q2_QE_inc->Fill(Q2);
        } else if (mec) {
            h_El_MEC_inc->Fill(El);
            h_Q2_MEC_inc->Fill(Q2);
        } else if (res) {
            h_El_RES_inc->Fill(El);
            h_Q2_RES_inc->Fill(Q2);
        } else if (dis) {
            h_El_DIS_inc->Fill(El);
            h_Q2_DIS_inc->Fill(Q2);
        }

        if (!(Is2p || Is1n1p)) { continue; }

        if (Is2p) {
            double theta_p1 = acos(pzf[0] / sqrt(pxf[0] * pxf[0] + pyf[0] * pyf[0] + pzf[0] * pzf[0])) * 180.0 / 3.14159265359;
            double theta_p2 = acos(pzf[1] / sqrt(pxf[1] * pxf[1] + pyf[1] * pyf[1] + pzf[1] * pzf[1])) * 180.0 / 3.14159265359;
            bool isInAcceptance =
                (((theta_p1 >= 5. && theta_p1 <= 40.) && (theta_p2 >= 40. && theta_p2 <= 140.)) || ((theta_p2 >= 5. && theta_p2 <= 40.) && (theta_p1 >= 40. && theta_p1 <= 140.)));
            bool ispFDpCD = (isInAcceptance && (pf[0] >= 0.4) && (pf[1] >= 0.4));

            if (ispFDpCD) {
                h_El_all_Int_2p->Fill(El);
                h_Q2_all_Int_2p->Fill(Q2);

                if (qel) {
                    h_El_QE_2p->Fill(El);
                    h_Q2_QE_2p->Fill(Q2);
                } else if (mec) {
                    h_El_MEC_2p->Fill(El);
                    h_Q2_MEC_2p->Fill(Q2);
                } else if (res) {
                    h_El_RES_2p->Fill(El);
                    h_Q2_RES_2p->Fill(Q2);
                } else if (dis) {
                    h_El_DIS_2p->Fill(El);
                    h_Q2_DIS_2p->Fill(Q2);
                }
            }
        } else if (Is1n1p) {
            int neutron_ind, proton_ind;

            if (pdgf[0] == 2112) {
                neutron_ind = 0;
                proton_ind = 1;
            } else {
                neutron_ind = 1;
                proton_ind = 0;
            }

            double theta_n =
                acos(pzf[neutron_ind] / sqrt(pxf[neutron_ind] * pxf[neutron_ind] + pyf[neutron_ind] * pyf[neutron_ind] + pzf[neutron_ind] * pzf[neutron_ind])) * 180.0 / 3.14159265359;
            double theta_p = acos(pzf[proton_ind] / sqrt(pxf[proton_ind] * pxf[proton_ind] + pyf[proton_ind] * pyf[proton_ind] + pzf[proton_ind] * pzf[proton_ind])) * 180.0 / 3.14159265359;
            bool isInAcceptance = ((theta_n >= 5. && theta_n <= 40.) && (theta_p >= 40. && theta_p <= 140.));
            bool isnFDpCD = (isInAcceptance && (pf[0] >= 0.4) && (pf[1] >= 0.4));

            if (isnFDpCD) {
                h_El_all_Int_1n1p->Fill(El);
                h_Q2_all_Int_1n1p->Fill(Q2);

                if (qel) {
                    h_El_QE_1n1p->Fill(El);
                    h_Q2_QE_1n1p->Fill(Q2);
                } else if (mec) {
                    h_El_MEC_1n1p->Fill(El);
                    h_Q2_MEC_1n1p->Fill(Q2);
                } else if (res) {
                    h_El_RES_1n1p->Fill(El);
                    h_Q2_RES_1n1p->Fill(Q2);
                } else if (dis) {
                    h_El_DIS_1n1p->Fill(El);
                    h_Q2_DIS_1n1p->Fill(Q2);
                }
            }
        }

        h_El_all_Int_2N->Fill(El);
        h_Q2_all_Int_2N->Fill(Q2);

        if (qel) {
            h_El_QE_2N->Fill(El);
            h_Q2_QE_2N->Fill(Q2);
        } else if (mec) {
            h_El_MEC_2N->Fill(El);
            h_Q2_MEC_2N->Fill(Q2);
        } else if (res) {
            h_El_RES_2N->Fill(El);
            h_Q2_RES_2N->Fill(Q2);
        } else if (dis) {
            h_El_DIS_2N->Fill(El);
            h_Q2_DIS_2N->Fill(Q2);
        }
    }

    int pixelx = 1980, pixely = 1530;
    TCanvas *canvas = new TCanvas("myText", "myText", pixelx, pixely);
    canvas->cd();

    h_El_all_Int_inc->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_all_Int_inc.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_inc->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_all_Int_inc.pdf").c_str());
    canvas->Clear();

    h_El_QE_inc->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_QE_inc.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_inc->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_QE_inc.pdf").c_str());
    canvas->Clear();

    h_El_MEC_inc->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_MEC_inc.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_inc->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_MEC_inc.pdf").c_str());
    canvas->Clear();

    h_El_RES_inc->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_RES_inc.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_inc->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_RES_inc.pdf").c_str());
    canvas->Clear();

    h_El_DIS_inc->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_DIS_inc.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_inc->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_DIS_inc.pdf").c_str());
    canvas->Clear();

    h_El_all_Int_2N->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_all_Int_2N.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_2N->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_all_Int_2N.pdf").c_str());
    canvas->Clear();

    h_El_QE_2N->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_QE_2N.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_2N->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_QE_2N.pdf").c_str());
    canvas->Clear();

    h_El_MEC_2N->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_MEC_2N.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_2N->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_MEC_2N.pdf").c_str());
    canvas->Clear();

    h_El_RES_2N->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_RES_2N.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_2N->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_RES_2N.pdf").c_str());
    canvas->Clear();

    h_El_DIS_2N->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_DIS_2N.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_2N->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_DIS_2N.pdf").c_str());
    canvas->Clear();

    h_El_all_Int_2p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_all_Int_2p.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_2p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_all_Int_2p.pdf").c_str());
    canvas->Clear();

    h_El_QE_2p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_QE_2p.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_2p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_QE_2p.pdf").c_str());
    canvas->Clear();

    h_El_MEC_2p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_MEC_2p.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_2p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_MEC_2p.pdf").c_str());
    canvas->Clear();

    h_El_RES_2p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_RES_2p.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_2p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_RES_2p.pdf").c_str());
    canvas->Clear();

    h_El_DIS_2p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_DIS.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_2p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_DIS_2p.pdf").c_str());
    canvas->Clear();

    h_El_all_Int_1n1p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_all_Int_1n1p.pdf").c_str());
    canvas->Clear();

    h_Q2_all_Int_1n1p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_all_Int_1n1p.pdf").c_str());
    canvas->Clear();

    h_El_QE_1n1p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_QE_1n1p.pdf").c_str());
    canvas->Clear();

    h_Q2_QE_1n1p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_QE_1n1p.pdf").c_str());
    canvas->Clear();

    h_El_MEC_1n1p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_MEC_1n1p.pdf").c_str());
    canvas->Clear();

    h_Q2_MEC_1n1p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_MEC_1n1p.pdf").c_str());
    canvas->Clear();

    h_El_RES_1n1p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_RES_1n1p.pdf").c_str());
    canvas->Clear();

    h_Q2_RES_1n1p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_RES_1n1p.pdf").c_str());
    canvas->Clear();

    h_El_DIS_1n1p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_El_DIS.pdf").c_str());
    canvas->Clear();

    h_Q2_DIS_1n1p->Draw();
    canvas->SaveAs(("./" + SaveDir + "/h_Q2_DIS_1n1p.pdf").c_str());
    canvas->Clear();
}
