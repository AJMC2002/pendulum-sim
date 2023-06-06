#include "RandomGen.hpp"

#include <algorithm>

namespace pendulum {
RandomGen::RandomGen(double mean, double stddev, double min, double max)
    : distrib_(mean, stddev)
    , min_(min)
    , max_(max)
{
}

[[nodiscard]] double RandomGen::gen()
{
    return std::clamp(distrib_(e_), min_, max_);
}
} // namespace pendulum