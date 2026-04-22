#ifndef RANDOM_HPP_
#define RANDOM_HPP_

// ============================================
// Random - Random number utilities
// ============================================
#include <random>

class Random {
public:
    static void Seed(unsigned int seed) { s_engine.seed(seed); }
    static void Seed() { s_engine.seed(std::random_device{}()); }

    // Integer range [min, max]
    static int Int(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(s_engine);
    }

    // Float range [min, max)
    static float Float(float min = 0.0f, float max = 1.0f) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(s_engine);
    }

    // Bool with probability
    static bool Bool(float probability = 0.5f) {
        std::bernoulli_distribution dist(probability);
        return dist(s_engine);
    }

private:
    static std::mt19937 s_engine;
};

#endif // RANDOM_HPP_
