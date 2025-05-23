//
// Created by Alon Sportes on 13/03/2025.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "../../classes/DSCuts/DSCuts.h"

namespace constants {
// Physical constants ---------------------------------------------------------------------------------------------------------------------------------------------------

// Particle masses (in GeV):
double m_n = 0.939565;
double m_p = 0.938272;
double m_Kminus = 0.493677;
double m_Kplus = 0.493677;
double m_Kzero = 0.497614;
double m_piplus = 0.13957;
double m_piminus = 0.13957;
double m_pizero = 0.134977;
double m_e = 0.000510999;
double m_d = 1.8756;

// Speed of light in vacuum (in cm/ns):
const double c = 29.9792458;

// Particle PDG ---------------------------------------------------------------------------------------------------------------------------------------------------------

int nPDG = 2112, pPDG = 2212;
int KplusPDG = 321, KminusPDG = -321, KzeroPDG = 311;
int piplusPDG = 211, piminusPDG = -211, pizeroPDG = 111;
int ePDG = 11, nuePDG = 12;
int muPDG = 13, numuPDG = 14;
int tauPDG = 15, nutauPDG = 16;
int phPDG = 22;

// CLAS12 constants -----------------------------------------------------------------------------------------------------------------------------------------------------

/* FD & CD theta range */
DSCuts ThetaFD = DSCuts("Theta FD range", "FD", "", "", 1, 5., 40.);
DSCuts ThetaCD = DSCuts("Theta CD range", "CD", "", "", 1, 40., 135.);

};  // namespace constants

#endif  // CONSTANTS_H
