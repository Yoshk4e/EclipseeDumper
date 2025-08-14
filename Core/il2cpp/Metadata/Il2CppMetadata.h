#pragma once
#include "../Api/Il2CppApi.h"

class Il2CppMetadata {
public:
    static std::string DumpClass(Il2CppClass* klass);
    static std::string DumpClassMethods(Il2CppClass* klass);
    static std::string DumpClassFields(Il2CppClass* klass);
};
