//
// Created by alons on 13/05/2023.
//

#ifndef NEUTRALPARTICLEID_H
#define NEUTRALPARTICLEID_H

#include <TF1.h>
#include <math.h>

#include <iostream>
#include <map>
#include <vector>

#include "GetFDNeutronP.h"
#include "clas12reader.h"
#include "region_particle.h"

using namespace std;

/* The NeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */

// bool GetFDNeutrons(region_part_ptr &particle) {
vector<int> NeutralParticleID(vector<region_part_ptr> &allParticles, DSCuts &Momentum_th, const string &NeutralParticle2ID, bool apply_nucleon_cuts) {
    vector<int> Neutral_indices_in_allParticles;

    for (int i = 0; i < allParticles.size(); i++) {
        if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD)) {  // If particle is neutral and in the FD
            int ParticlePDG = allParticles[i]->par()->getPid();

            bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);    // PCAL hit
            bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);    // ECIN hit
            bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7);  // ECOUT hit

            if ((ParticlePDG == 2112) || (ParticlePDG == 22)) {
                if (ParticleInPCAL && (ParticlePDG == 22)) {  // if there's a 'photon' with a PCAL hit -> photon
                                                              //                    ID_Photons_FD.push_back(i);

                    /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */
                    double Momentum = allParticles[i]->getP();

                    /* Log photons above momentum cuts (given by Momentum_cuts): */
                    if (Momentum >= Photon_momentum_th.GetLowerCut()) { ID_Photons_FD.push_back(i); }
                } else if (!ParticleInPCAL) {                 // if there is a neutron or a 'photon' without a PCAL hit
                    if (ParticleInECIN || ParticleInECOUT) {  // if there is either a ECIN or ECOUT hit -> neutron
                                                              //                        ID_Neutrons_FD.push_back(i);
                                                              //
                        /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */
                        double Momentum = GetFDNeutronP(allParticles[i], apply_nucleon_cuts);

                        /* Log neutrons above momentum cuts (given by Momentum_th): */
                        if (Momentum >= Neutron_momentum_th.GetLowerCut()) { ID_Neutrons_FD.push_back(i); }
                    }
                }
            }  // end of clas12root neutron or 'photon' if
        }  // end of neutral and in the FD if
    }  // end of loop over allparticle vector

    /* Return a vector of the neutrons in allParticles, according to the definition. */
    return Neutral_indices_in_allParticles;
}

#endif  // NEUTRALPARTICLEID_H
