#pragma once

#include <RandomGen.hpp>

namespace pendulum {
class NoiseFilter {
private:
    RandomGen noise_;

public:
    NoiseFilter();
    ~NoiseFilter() = default;

    double noisify(const double data);
    std::vector<double> noisify(const std::vector<double> data);
};
} // namespace pendulum
