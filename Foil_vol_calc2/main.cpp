#include <iostream>
#include <string>  // std::string

/*
Foil_vol_calc2 — simple volume calculator

This program estimates the volume of a rectangular foil blank after subtracting two pairs of triangular cutouts.
Geometry model:
  - Start from a rectangular box of thickness t, height h, width w.
  - Subtract two identical triangular prisms (V1) with legs (a1, b1).
  - Subtract two identical triangular prisms (V2) with legs (a2, b2).

All lengths are in centimeters.
Outputs:
  - V0: full rectangular volume
  - V1, V2: removed volumes from the two cutout types
  - V: remaining foil volume
  - w_eff: effective width equivalent, defined by V = t * h * w_eff

Notes / fixes:
  - Added the missing <string> include (required for std::string).
  - Added an explicit error path if Size is neither "small" nor "large".
  - Left `separation` documented but unused (if you plan to extend the model).
*/

// Compute volume of a rectangular box (thickness × height × width).
// t: thickness [cm]
// h: height   [cm]
// w: width    [cm]
// returns: volume [cm^3]
double BoxVolume(double t, double h, double w) { return t * w * h; }

// Compute volume of a right-triangular prism.
// The triangle is assumed right-angled with legs a and b.
// t: prism thickness / extrusion length [cm]
// a: triangle leg a [cm]
// b: triangle leg b [cm]
// returns: volume [cm^3]
double PrismVolume(double t, double a, double b) {
    // Area of a right triangle: (1/2) * a * b
    const double triangular_area = 0.5 * a * b;

    // Extrude that area by thickness t to get a prism volume
    return triangular_area * t;
}

int main() {
    // Choose which geometry parameters to use.
    // Switch by commenting/uncommenting the line you want.
    // supported values: "small", "large"
    // std::string Size = "small";
    std::string Size = "large";

    // (Currently unused) separation between features; kept for potential future extensions.
    // If you don't intend to use it, you can delete this variable.
    const double separation = 0.127;
    (void)separation;  // suppress unused-variable warnings while keeping it documented

    // All lengths are in cm!
    // t: thickness, h: height, w: width of the starting rectangular blank.
    double t = 0.0;
    double h = 0.0;
    double w = 0.0;

    // Parameters for the first pair of triangular cutouts (two identical triangles).
    double a1 = 0.0;
    double b1 = 0.0;

    // Parameters for the second pair of triangular cutouts (two identical triangles).
    double a2 = 0.0;
    double b2 = 0.0;

    // Assign dimensions based on the requested size.
    if (Size == "small") {
        // Base rectangular blank dimensions
        t = 0.2;
        h = 0.81;
        w = 0.4;

        // Cutout type #1 (two identical right triangles)
        a1 = 0.1;
        b1 = 0.1;

        // Cutout type #2 (two identical right triangles)
        a2 = 0.41;
        b2 = 0.073;

    } else if (Size == "large") {
        // Base rectangular blank dimensions
        t = 0.2;
        h = 0.91;
        w = 0.6;

        // Cutout type #1 (two identical right triangles)
        a1 = 0.15;
        b1 = 0.15;

        // Cutout type #2 (two identical right triangles)
        a2 = 0.46;
        b2 = 0.173;

    } else {
        // Defensive programming: prevent using uninitialized geometry.
        std::cerr << "ERROR: Size must be \"small\" or \"large\". Got: " << Size << "\n";
        return 1;
    }

    // Compute volumes:
    // V0 = volume of the full rectangular blank
    // V1 = volume removed by the first pair of triangular cutouts
    // V2 = volume removed by the second pair of triangular cutouts
    const double V0 = BoxVolume(t, h, w);
    const double V1 = 2.0 * PrismVolume(t, a1, b1);
    const double V2 = 2.0 * PrismVolume(t, a2, b2);

    // Remaining volume after removing the cutouts.
    const double V = V0 - V1 - V2;

    // Print inputs/outputs.
    std::cout << "\nSize = " << Size << std::endl;
    std::cout << "\nV0 = " << V0 << " cm^3" << std::endl;
    std::cout << "V1 = " << V1 << " cm^3" << std::endl;
    std::cout << "V2 = " << V2 << " cm^3" << std::endl;
    std::cout << "\nV  = " << V << " cm^3" << std::endl;

    // Effective width:
    // If you compress the remaining volume into a perfect rectangle of the same thickness t and height h,
    // then w_eff is the width that rectangle would need.
    // V = t * h * w_eff  =>  w_eff = V / (t * h)
    const double w_eff = V / (t * h);

    std::cout << "\nw_eff      = " << w_eff << " cm" << std::endl;
    std::cout << "Half w_eff = " << 0.5 * w_eff << " cm" << std::endl;

    return 0;
}
