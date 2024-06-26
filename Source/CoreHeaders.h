//
// Copyright (c) 2022 Jimmy Lord
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

// This file includes many static headers.
// Any changes to any of these headers will likely cause a full rebuild of the
//   Framework project, which isn't desirable.

// Platform headers.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>

// Core c++ headers.
#include <assert.h>
#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>

// Framework headers.
#include "DataTypes.h"
#include "ForwardDeclarations.h"

// 3rd party library headers.
#include "bgfx/bgfx.h"
#include "enkiTS/src/TaskScheduler.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui.h"
#include "ImFileDialog/ImFileDialog.h"
#include "ImGuizmo/ImGuizmo.h"
#include "nlohmann-json/single_include/nlohmann/json.hpp"
#include "nlohmann-json/single_include/nlohmann/json_fwd.hpp"
#include "pcg-cpp/include/pcg_random.hpp"

#pragma warning( push )
#pragma warning( disable : 4005 ) // 'flecs_STATIC': macro redefinition
#include "../Libraries/flecs/flecs.h"
#pragma warning( pop )
