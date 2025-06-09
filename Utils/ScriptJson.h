#pragma once
#include <string>
#include "../Core/il2cpp/Api/Il2CppApi.h"
#include "../Core/il2cpp/Metadata/Il2CppMetadata.h"

class ScriptJsonGenerator {
public:
    static std::string GenerateJson();

private:
    static void WriteEscapedString(std::stringstream& ss, const std::string& value);
    static void WriteMethods(std::stringstream& ss);
    static void WriteTypeMetadata(std::stringstream& ss);
    static void WriteMethodRefs(std::stringstream& ss);
};