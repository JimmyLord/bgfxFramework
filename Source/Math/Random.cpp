#include "CoreHeaders.h"
#include "Random.h"

namespace fw {
namespace Random {

// Global min/max defines conflict with pcg_random.
#undef max
#undef min

static Generator g_RNGObject;

void SetSeed(unsigned int seed)     { g_RNGObject.SetSeed( seed ); }
int Int(int min, int max)           { return g_RNGObject.Int( min, max ); }
int Int(int max)                    { return g_RNGObject.Int( max ); }
float Float(float min, float max)   { return g_RNGObject.Float( min, max ); }
float Float(float max)              { return g_RNGObject.Float( max ); }

Generator::Generator()
    : m_RNGEngine( pcg_extras::seed_seq_from<std::random_device>() )
{
}

Generator::Generator(unsigned int seed)
    : m_RNGEngine( seed )
{
}

void Generator::SetSeed(unsigned int seed)
{
    m_RNGEngine.seed( seed );
}

// Min and max are inclusive.
int Generator::Int(int min, int max)
{
    assert( min < max );
    return min + m_RNGEngine( max-min+1 );
}

// Max is inclusive.
int Generator::Int(int max)
{
    return m_RNGEngine( max+1 );
}

// Min is inclusive, max is inclusive.
float Generator::Float(float min, float max)
{
    assert( min < max );

    double rand01 = m_RNGEngine()/(double)pcg32::max();
    return (float)(min + rand01 * (max-min));
}

// Max is inclusive.
float Generator::Float(float max)
{
    double rand01 = m_RNGEngine()/(double)pcg32::max();
    return (float)(rand01 * max);
}

} // namespace Random
} // namespace fw
