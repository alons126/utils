//
// Created by Alon Sportes on 15/03/2025.
//

#ifndef ANALYSIS_MATH_H
#define ANALYSIS_MATH_H

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace analysis_math {
// Mathematical constants -----------------------------------------------------------------------------------------------------------------------------------------------

const double pi = M_PI;

// RadToDeg function -----------------------------------------------------------------------------------------------------------------------------------------------------

double RadToDeg(const double &rad) { return rad * 180. / M_PI; }

// DegToRad function -----------------------------------------------------------------------------------------------------------------------------------------------------

double DegToRad(const double &deg) { return deg * M_PI / 180.; }

// CalcTheta_rad function ------------------------------------------------------------------------------------------------------------------------------------------------

double CalcTheta_rad(const double &x, const double &y, const double &z) { return acos(z / sqrt(x * x + y * y + z * z)); }

// CalcTheta_deg function ------------------------------------------------------------------------------------------------------------------------------------------------

double CalcTheta_deg(const double &x, const double &y, const double &z) { return RadToDeg(CalcTheta_rad(x, y, z)); }

// CalcPhi_rad function ------------------------------------------------------------------------------------------------------------------------------------------------

double CalcPhi_rad(const double &x, const double &y) { return atan2(y, x); }

// CalcPhi_deg function ------------------------------------------------------------------------------------------------------------------------------------------------

double CalcPhi_deg(const double &x, const double &y, const double &z) { return RadToDeg(CalcPhi_rad(x, y)); }

// GetPi0MomTh function --------------------------------------------------------------------------------------------------------------------------------------------------

double GetPi0MomTh(const double &ph_mom_th) {
    if (std::abs(ph_mom_th) == 9999) { return -9999; }
    double pi0_mom_th = std::sqrt(4 * ph_mom_th * ph_mom_th - m_pizero * m_pizero);
    return pi0_mom_th;
}

}  // namespace analysis_math

#endif  // ANALYSIS_MATH_H
