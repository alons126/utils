
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "../../classes/DSCuts/DSCuts.h"

/**
 * @namespace constants
 * @brief A namespace for various constants used in the analysis, including physical constants, particle masses, PDG codes, and CLAS12-specific constants such as theta ranges for the forward
 * and central detectors.
 * @details This namespace contains various constants that are used throughout the analysis code. The physical constants include particle masses (in GeV) for neutrons, protons, kaons, pions,
 * and electrons, as well as the speed of light in vacuum (in cm/ns). The particle PDG codes are also defined for neutrons, protons, kaons, pions, electrons, neutrinos, and photons.
 * Additionally, the namespace includes CLAS12-specific constants such as the theta ranges for the forward detector (FD) and central detector (CD), which are defined as DSCuts objects. These
 * constants are used in the main code for performing various calculations and applying cuts based on the characteristics of the particles and the detector geometry.
 * @note The constants defined in this namespace should be used consistently throughout the code to ensure that calculations are performed correctly and that the results are consistent
 * across different parts of the analysis. For example, the particle masses should be used in calculations of kinematic quantities, while the PDG codes should be used for identifying
 * particles in the analysis. The CLAS12-specific constants such as the theta ranges should be used when applying cuts based on the detector geometry, ensuring that the cuts are applied
 * correctly for particles detected in the forward or central detectors.
 */
namespace constants {
// Physical constants ---------------------------------------------------------------------------------------------------------------------------------------------------

// Particle masses (in GeV):
constexpr double m_n = 0.939565;
constexpr double m_p = 0.938272;
constexpr double m_Kminus = 0.493677;
constexpr double m_Kplus = 0.493677;
constexpr double m_Kzero = 0.497614;
constexpr double m_piplus = 0.13957;
constexpr double m_piminus = 0.13957;
constexpr double m_pizero = 0.134977;
constexpr double m_e = 0.000511;
constexpr double m_d = 1.8756;

// Speed of light in vacuum (in cm/ns):
constexpr double c = 29.9792458;

// Particle PDG ---------------------------------------------------------------------------------------------------------------------------------------------------------
constexpr int nPDG = 2112, pPDG = 2212;
constexpr int KplusPDG = 321, KminusPDG = -321, KzeroPDG = 311;
constexpr int piplusPDG = 211, piminusPDG = -211, pizeroPDG = 111;
constexpr int ePDG = 11, nuePDG = 12;
constexpr int muPDG = 13, numuPDG = 14;
constexpr int tauPDG = 15, nutauPDG = 16;
constexpr int phPDG = 22;

// CLAS12 constants -----------------------------------------------------------------------------------------------------------------------------------------------------
/* FD & CD theta range */
DSCuts ThetaFD = DSCuts("Theta FD range", "FD", "", "", 1, 5., 40.);
DSCuts ThetaCD = DSCuts("Theta CD range", "CD", "", "", 1, 40., 135.);
};  // namespace constants

#endif  // CONSTANTS_H
