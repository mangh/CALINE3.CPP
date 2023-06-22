#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <cmath>
#include <numeric>
#include <vector>

namespace Bullet
{
    struct Benchmark
    {
        std::vector<double> samples;

        Benchmark()
            : samples({})
        {
        }

        void Add(double sample) { samples.push_back(sample); }
        void Reset() { samples.clear(); }
        size_t Count() { return samples.size(); }

        double Average()
        {
            return std::accumulate(samples.begin(), samples.end(), 0.0) / samples.size();
        }
        double StdDev()
        {
            double avg = Average();
            auto add_sqr_dev = [avg](double total, double r) { return total + (r - avg) * (r - avg); };
            return std::sqrt(std::accumulate(samples.begin(), samples.end(), 0.0, add_sqr_dev) / Count());
        }

        double StdDevPercentage()
        {
            return 100.0 * StdDev() / Average();
        }
    };
}
#endif /* !BENCHMARK_H */
