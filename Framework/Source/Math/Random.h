#pragma once

#include <random>
#include "../Libraries/pcg-cpp/include/pcg_random.hpp"

namespace fw {
namespace Random {

// Get values from a global generator.
void SetSeed(unsigned int seed);
int GetInt(int min, int max);
int GetInt(int max);
float GetFloat(float min, float max);
float GetFloat(float max);

class Generator
{
public:
    Generator();
    Generator(unsigned int seed);

    void SetSeed(unsigned int seed);

    int GetInt(int min, int max);
    int GetInt(int max);
    float GetFloat(float min, float max);
    float GetFloat(float max);

protected:
    pcg32 m_RNGEngine;
};

} // namespace Random
} // namespace fw
