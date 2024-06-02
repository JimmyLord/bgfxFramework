//
// Copyright (c) 2022-2023 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"
#include "Random.h"

namespace fw {
namespace Random {

// Global min/max defines conflict with pcg_random.
#undef max
#undef min

static Generator g_RNGObject;

Generator& GetGlobalGenerator()
{
    return g_RNGObject;
}

void SetSeed(uint32 seed)               { g_RNGObject.SetSeed( seed ); }
int32 Int(int32 min, int32 max)         { return g_RNGObject.Int( min, max ); }
int32 Int(int32 max)                    { return g_RNGObject.Int( max ); }
float32 Float(float32 min, float32 max) { return g_RNGObject.Float( min, max ); }
float32 Float(float32 max)              { return g_RNGObject.Float( max ); }

Generator::Generator()
    : m_RNGEngine( pcg_extras::seed_seq_from<std::random_device>() )
{
}

Generator::Generator(uint32 seed)
    : m_RNGEngine( seed )
{
}

void Generator::SetSeed(uint32 seed)
{
    m_RNGEngine.seed( seed );
}

// Min and max are inclusive.
int32 Generator::Int(int32 min, int32 max)
{
    return min + m_RNGEngine( max-min+1 );
}

// Max is inclusive.
int32 Generator::Int(int32 max)
{
    return m_RNGEngine( max+1 );
}

// Min is inclusive, max is inclusive.
float32 Generator::Float(float32 min, float32 max)
{
    float64 rand01 = m_RNGEngine()/(float64)pcg32::max();
    return (float32)(min + rand01 * (max-min));
}

// Max is inclusive.
float32 Generator::Float(float32 max)
{
    float64 rand01 = m_RNGEngine()/(float64)pcg32::max();
    return (float32)(rand01 * max);
}

} // namespace Random
} // namespace fw
