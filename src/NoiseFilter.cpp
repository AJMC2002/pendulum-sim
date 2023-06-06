#include "NoiseFilter.hpp"

namespace pendulum {
NoiseFilter::NoiseFilter()
    : noise_(1.0, 0.05, 0.5, 1.5)
{
}

double NoiseFilter::noisify(const double value) { return value * noise_.gen(); }

std::vector<double> NoiseFilter::noisify(const std::vector<double> data)
{
    std::vector<double> filtered_data;
    filtered_data.reserve(data.size());
    for (double value : data) {
        filtered_data.push_back(this->noisify(value));
    }
    return filtered_data;
}
} // namespace pendulum
