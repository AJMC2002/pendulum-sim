#include <math.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "NoiseFilter.hpp"

// We assume that no mass is lost and that k is constant for the rope
// during the simulation.
// We also consider our coordinates with respect to the position of the pivot
// of rotation
const double G = -9.81; // gravitational acceleration [m/s²]

const double M = 1; // mass [kg]
const double L_0 = 10; // initial length [m]
const double K = 100; // elasticity constant [N/m]
const double THETA_0 = M_PI / 4; // angle between the rope and the vertical [rad]
const double OMEGA_0 = 0; // initial angular velocity [rad/s]
const double ALPHA_0 = 0; // initial angular acceleration [rad/s²]
const double VX_0 = 0; // initial velocity tangent to movement [m/s]
const double VY_0 = 0; // initial velocity perpendicular to movement [m/s]

const double DT = 0.01; // time step [s]
const double T_MAX = 10; // seconds we want to run the sim [s]

double
modulus(double x, double y)
{
    return std::sqrt(x * x + y * y);
}

void write_header(std::ofstream& fout,
    std::map<std::string, std::vector<double>>& data)
{
    std::string row = "";
    for (auto const& [key, _] : data) {
        row += key;
        row += ",";
    }
    row.pop_back();
    fout << row << "\n";
}

void write_row(std::ofstream& fout, std::map<std::string, std::vector<double>>& data)
{
    std::string row = "";
    for (auto const& [_, val] : data) {
        row += std::to_string(val.back());
        row += ",";
    }
    row.pop_back();
    fout << row << "\n";
}

void create_csv(std::ofstream& fout,
    std::map<std::string, std::vector<double>>& data,
    pendulum::NoiseFilter& filter)
{
    write_header(fout, data);

    for (double t = DT; t <= T_MAX; t = t + DT) {
        write_row(fout, data);

        data["time"].push_back(t);

        data["theta"].push_back(data["theta"].back() + data["omega"].back() * DT + 1 / 2 * data["alfa"].back() * DT * DT);
        data["omega"].push_back(data["omega"].back() + data["alfa"].back() * DT);
        data["alfa"].push_back(G * sin(data["theta"].back()) / L_0);

        data["L"].push_back(L_0 + -M * G * cos(data["theta"].back()) / K);
        data["x"].push_back(data["L"].back() * sin(data["theta"].back()));
        data["y"].push_back(-data["L"].back() * cos(data["theta"].back()));

        data["v_x"].push_back(data["omega"].back() * data["L"].back() * cos(data["theta"].back()));
        data["v_y"].push_back(data["omega"].back() * data["L"].back() * sin(data["theta"].back()));
        data["||v||"].push_back(modulus(data["v_x"].back(), data["v_y"].back()));

        data["v_x_noisy"].push_back(filter.noisify(data["v_x"].back()));
        data["v_y_noisy"].push_back(filter.noisify(data["v_y"].back()));

        data["a_x"].push_back(data["alfa"].back() * data["L"].back() * cos(data["theta"].back()));
        data["a_y"].push_back(data["alfa"].back() * data["L"].back() * sin(data["theta"].back()));
        data["||a||"].push_back(modulus(data["a_x"].back(), data["a_y"].back()));
    }

    fout.close();
}

int main()
{
    std::filesystem::create_directories("../output");
    std::cout << "Creating directories...\n";

    std::ofstream fout("../output/pendulum.csv");
    std::map<std::string, std::vector<double>> data = {
        { "time", { 0 } },
        { "L", { L_0 - M * G * cos(THETA_0) / K } },
        { "theta", { THETA_0 } },
        { "omega", { OMEGA_0 } },
        { "alfa", { ALPHA_0 + G * sin(THETA_0) / L_0 } },
        { "x", { (L_0 - M * G * cos(THETA_0) / K) * sin(THETA_0) } },
        { "y", { (-L_0 + M * G * cos(THETA_0) / K) * cos(THETA_0) } },
        { "v_x", { VX_0 } },
        { "v_y", { VY_0 } },
        { "v_x_noisy", { VX_0 } },
        { "v_y_noisy", { VY_0 } },
        { "||v||", { modulus(VX_0, VY_0) } },
        { "a_x", { G * sin(THETA_0) * cos(THETA_0) } },
        { "a_y", { G * sin(THETA_0) * sin(THETA_0) } },
        { "||a||", { modulus(G * sin(THETA_0), 0) } },
    };
    pendulum::NoiseFilter filter;

    std::cout << "Creating your CSV...\n";
    create_csv(fout, data, filter);

    std::cout << "Your files are ready!\n";

    return 0;
}