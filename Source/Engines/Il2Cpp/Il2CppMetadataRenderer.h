#pragma once

#include <string>
#include "Il2CppApiWrapper.h"

namespace EclipseKit::Engines::Il2Cpp {
    // Render Il2Cpp class into a C#-style definition string
    std::string RenderClassCSharp(Il2CppClass* klass);
}
