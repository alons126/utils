#include <iostream>

double BoxVolume(double t, double h, double w) { return t * w * h; }

double PrismVolume(double t, double a, double b) {
    double triangular_area = 0.5 * a * b;
    return triangular_area * t;
}

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // std::string Size = "small";
    std::string Size = "large";

    double separation = 0.127;

    // All lengths are in cm!
    double t, h, w;

    double a1, b1;

    double a2, b2;

    if (Size == "small") {
        t = 0.2, h = 0.81, w = 0.4;

        a1 = 0.1, b1 = 0.1;

        a2 = 0.41, b2 = 0.073;
    } else if (Size == "large") {
        t = 0.2, h = 0.91, w = 0.6;

        a1 = 0.15, b1 = 0.15;

        a2 = 0.46, b2 = 0.173;
    }

    double V0 = BoxVolume(t, h, w), V1 = 2 * PrismVolume(t, a1, b1), V2 = 2 * PrismVolume(t, a2, b2);

    double V = V0 - V1 - V2;

    std::cout << "\nSize = " << Size << std::endl;
    std::cout << "\nV0 = " << V0 << " cm^3" << std::endl;
    std::cout << "V1 = " << V1 << " cm^3" << std::endl;
    std::cout << "V2 = " << V2 << " cm^3" << std::endl;

    std::cout << "\nV = " << V << " cm^3" << std::endl;

    double w_eff = V / (t * h);

    std::cout << "\nw_eff = " << w_eff << " cm" << std::endl;
    std::cout << "Half w_eff = " << 0.5 * w_eff << " cm" << std::endl;

    return 0;
}
