#pragma once

#include "Math/Vector.h"

namespace fw {

class Mesh;
class Material;

struct NameData
{
    static const int8 c_MaxNameLength = 32;
    char m_Name[c_MaxNameLength];

    NameData(const char* name)
    {
        strncpy_s( m_Name, c_MaxNameLength, name, c_MaxNameLength );
    }
    //NameData(const NameData& other)
    //{
    //    strncpy_s( m_Name, c_MaxNameLength, other.m_Name, c_MaxNameLength );
    //}
    //NameData(NameData&& other)
    //{
    //    strncpy_s( m_Name, c_MaxNameLength, other.m_Name, c_MaxNameLength );
    //}
    //NameData& operator=(const NameData& other)
    //{
    //    strncpy_s( m_Name, c_MaxNameLength, other.m_Name, c_MaxNameLength );
    //    return *this;
    //}
    //NameData& operator=(const NameData&& other)
    //{
    //    strncpy_s( m_Name, c_MaxNameLength, other.m_Name, c_MaxNameLength );
    //    return *this;
    //}
};

struct TransformData
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
};

struct MeshData
{
    Mesh* pMesh;
    Material* pMaterial;
};

} // namespace fw