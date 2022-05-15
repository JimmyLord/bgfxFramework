//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreHeaders.h"
#include "GameCore.h"
#include "Components/CoreComponents.h"
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Renderer/Mesh.h"
#include "Renderer/Material.h"

namespace fw {

void GameCore::Draw()
{
    auto group = m_ECSRegistry.group<fw::TransformData>( entt::get<fw::MeshData> );
    for( auto entity : group )
    {
        auto& [transformData, meshData] = group.get<fw::TransformData, fw::MeshData>( entity );

        mat4 worldMat;
        worldMat.CreateSRT( transformData.scale, transformData.rotation, transformData.position );
        meshData.pMesh->Draw( m_pUniforms, meshData.pMaterial, &worldMat );
    }
}

entt::entity GameCore::CreateEntity()
{
    return m_ECSRegistry.create();
}

} // namespace fw