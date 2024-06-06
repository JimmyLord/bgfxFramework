//
// Copyright (c) 2024 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "CoreComponents.h"
#include "CoreSystems.h"
#include "Components/CoreComponents.h"
#include "Components/ComponentManager.h"
#include "Resources/Mesh.h"

namespace fw {

void System_UpdateAllTransforms(fw::ComponentManager* pComponentManager)
{
    flecs::world& world = pComponentManager->GetFlecsWorld();

    world.each<>(
        [](TransformData& transformData, TransformMatrixData& transformMatrixData)
        {
            transformMatrixData.transform.CreateSRT( transformData.scale, transformData.rotation, transformData.position );
        }
    );
}

void System_DrawAllMeshes(fw::ComponentManager* pComponentManager, int viewID, fw::Uniforms* pUniforms)
{
    flecs::world& world = pComponentManager->GetFlecsWorld();

    world.each<>(
        [viewID, pUniforms](TransformMatrixData& transformMatrixData, MeshData& meshData)
        {
            if( meshData.pMesh )
            {
                meshData.pMesh->Draw( viewID, pUniforms, meshData.pMaterial, &transformMatrixData.transform );
            }
        }
    );
}

} // namespace fw
