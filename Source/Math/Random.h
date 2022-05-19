#pragma once

#include <random>
#include "../Libraries/pcg-cpp/include/pcg_random.hpp"

namespace fw {
namespace Random {

// Get values from a global generator.
void SetSeed(unsigned int seed);
int Int(int min, int max);
int Int(int max);
float Float(float min, float max);
float Float(float max);

class Generator
{
public:
    Generator();
    Generator(unsigned int seed);

    void SetSeed(unsigned int seed);

    int Int(int min, int max);
    int Int(int max);
    float Float(float min, float max);
    float Float(float max);

protected:
    pcg32 m_RNGEngine;
};

} // namespace Random
} // namespace fw
