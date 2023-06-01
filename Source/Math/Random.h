#pragma once

#include <random>
#include "../Libraries/pcg-cpp/include/pcg_random.hpp"

namespace fw {
namespace Random {

class Generator;
Generator& GetGlobalGenerator();

// Get values from a global generator.
void SetSeed(uint32 seed);
int32 Int(int32 min, int32 max);
int32 Int(int32 max);
float32 Float(float32 min, float32 max);
float32 Float(float32 max);

class Generator
{
public:
    Generator();
    Generator(uint32 seed);

    void SetSeed(uint32 seed);

    int32 Int(int32 min, int32 max);
    int32 Int(int32 max);
    float32 Float(float32 min, float32 max);
    float32 Float(float32 max);

protected:
    pcg32 m_RNGEngine;
};

} // namespace Random
} // namespace fw
