//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

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
