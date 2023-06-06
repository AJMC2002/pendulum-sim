#pragma once

#include <random>

namespace pendulum {
class RandomGen {
private:
    std::default_random_engine e_;
    std::normal_distribution<> distrib_;
    double min_;
    double max_;

public:
    RandomGen(double mean, double stddev, double min, double max);
    ~RandomGen() = default;

    [[nodiscard]] double gen();
};
} // namespace pendulum