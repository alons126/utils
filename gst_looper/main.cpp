/*
/cvmfs/oasis.opensciencegrid.org/mis/apptainer/current/bin/apptainer shell --shell=/bin/bash -B /cvmfs,/exp,/nashome,/pnfs/genie,/opt,/run/user,/etc/hostname,/etc/hosts,/etc/krb5.conf --ipc --pid /cvmfs/singularity.opensciencegrid.org/fermilab/fnal-dev-sl7:latest

cd $MY_USER_FOLDER/GENIE

source genie_env.sh

source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setup
setup fife_utils

cd /pnfs/genie/scratch/users/asportes/gst_looper

source run.sh
 */

#include <iostream>

#include "gst.cpp"
#include "source/namespaces/utilities/utilities.cpp"

using namespace std;
using namespace utilities;

int main() {
    auto start = std::chrono::system_clock::now();  // Start counting running time

    system("rm ./*.pdf");

    string BaseDir = "/pnfs/genie/persistent/users/asportes/2N_Analysis_Samples";

    vector<string> Targets_v = {"C12"};
    // vector<string> Targets_v = {"D2", "C12", "Ar40"};
    vector<string> GENIE_tune_v = {"G18_10a_00_000", "GEM21_11a_00_000"};
    vector<string> BeamE_v = {"2070MeV", "4029MeV", "5986MeV"};

    for (int Targets_v_ind = 0; Targets_v_ind < Targets_v.size(); Targets_v_ind++) {
        for (int GENIE_tune_v_ind = 0; GENIE_tune_v_ind < GENIE_tune_v.size(); GENIE_tune_v_ind++) {
            for (int BeamE_v_ind = 0; BeamE_v_ind < BeamE_v.size(); BeamE_v_ind++) {
                string Target = Targets_v.at(Targets_v_ind);
                string GENIE_tune = GENIE_tune_v.at(GENIE_tune_v_ind);
                string BeamE = BeamE_v.at(BeamE_v_ind);
                string Q2_th = (BeamE == "2070MeV") ? "Q2_0_02" : (BeamE == "4029MeV") ? "Q2_0_25" : "Q2_0_40";
                string filesPath = BaseDir + "/" + Target + "/" + GENIE_tune + "/" + BeamE + "_" + Q2_th + "/master-routine_validation_01-eScattering/" + Target + "_" + GENIE_tune + "_" +
                                   Q2_th + "_" + BeamE + ".root";

                const char *files = filesPath.c_str();
                // const char *files = "*.root";

                gst loopTree = gst(files);

                loopTree.Loop(Target, GENIE_tune, BeamE, Q2_th);
            }
        }
    }

    auto end = std::chrono::system_clock::now();
    auto elapsed_time_seconds = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    double elapsed_time_minutes = elapsed_time_seconds.count() / 60;

    if (elapsed_time_seconds.count() < 60) {
        std::cout << "Running time:\t" << elapsed_time_seconds.count() << " seconds\n\n";
    } else {
        std::cout << "Running time:\t" << ToStringWithPrecision(elapsed_time_minutes, 3) << " minutes\n\n";
    }
}