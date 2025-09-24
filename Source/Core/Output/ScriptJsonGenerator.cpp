#include "pch.h"
#include "ScriptJsonGenerator.h"
#include <sstream>
#include <vector>
#include <iomanip>

#include "../../Engines/Il2Cpp/Il2CppApiWrapper.h"
#include "../Configuration/EngineConfiguration.h"

namespace EclipseKit::Core::Output {
    using namespace EclipseKit::Engines::Il2Cpp;
    using EclipseKit::Core::Configuration::ConfigurationManager;

    static uintptr_t GetModuleBase() {
        const auto* cfg = ConfigurationManager::AutoDetectConfiguration();
        HMODULE baseMod = (cfg && cfg->UseMainModule)
            ? GetModuleHandleA(nullptr)
            : GetModuleHandleA("GameAssembly.dll");
        return reinterpret_cast<uintptr_t>(baseMod);
    }

    static uint64_t ReadMethodPointerVA(const uint8_t* method) {
        const auto* cfg = ConfigurationManager::AutoDetectConfiguration();
        const auto offset = cfg ? cfg->Offsets.MethodPointer : 0;
        // If offset is unknown (0), we still try +0 for safety like legacy, caller handles RVA subtraction
        const uint8_t* ptr = method + static_cast<size_t>(offset);
        return *reinterpret_cast<const uint64_t*>(ptr);
    }

    // --- ScriptJsonGenerator -------------------------------------------------

    std::string ScriptJsonGenerator::GenerateJson() {
        std::stringstream ss;
        ss << "{";

        // ScriptString (empty array)
        ss << "\"ScriptString\":[],";

        // ScriptMetadata
        ss << "\"ScriptMetadata\":";
        WriteTypeMetadata(ss);
        ss << ",";

        // ScriptMethod
        ss << "\"ScriptMethod\":";
        WriteMethods(ss);
        ss << ",";

        // ScriptMetadataMethod
        ss << "\"ScriptMetadataMethod\":";
        WriteMethodRefs(ss);

        ss << "}";
        return ss.str();
    }

    void ScriptJsonGenerator::WriteEscapedString(std::stringstream& ss, const std::string& value) {
        for (char c : value) {
            switch (c) {
                case '"':  ss << "\\\""; break;
                case '\\': ss << "\\\\"; break;
                case '\n': ss << "\\n";  break;
                case '\r': ss << "\\r";  break;
                case '\t': ss << "\\t";  break;
                default:   ss << c;      break;
            }
        }
    }

    void ScriptJsonGenerator::WriteMethods(std::stringstream& ss) {
        std::vector<std::string> entries;
        ss << "[";

        const auto* cfg = ConfigurationManager::AutoDetectConfiguration();
        const uint64_t maxProbe = cfg ? cfg->MaxTypeProbeCount : 2000000ULL;
        const uintptr_t moduleBase = GetModuleBase();

        for (uint64_t index = 0; index <= maxProbe; ++index) {
            Il2CppClass* klass = Il2CppApiWrapper::GetTypeInfoFromIndex(static_cast<TypeDefinitionIndex>(index));
            if (!klass) continue;

            void* methodIter = nullptr;
            while (const uint8_t* method = Il2CppApiWrapper::GetClassMethods(klass, &methodIter)) {
                const uint64_t va = ReadMethodPointerVA(method);
                const uint64_t rva = (va >= moduleBase) ? (va - moduleBase) : 0ULL;

                const char* className = Il2CppApiWrapper::GetClassName(klass);
                const char* methodName = Il2CppApiWrapper::GetMethodName(method);

                std::stringstream entry;
                entry << "{"
                      << "\"Address\":" << std::dec << rva << ","
                      << "\"Name\":\"";
                WriteEscapedString(entry, std::string(className) + "$$" + methodName);
                entry << "\",";
                entry << "\"Signature\":\"\",";      // left empty by design
                entry << "\"TypeSignature\":\"\"";    // left empty by design
                entry << "}";

                entries.push_back(entry.str());
            }
        }

        for (size_t i = 0; i < entries.size(); ++i) {
            if (i > 0) ss << ",";
            ss << entries[i];
        }
        ss << "]";
    }

    void ScriptJsonGenerator::WriteTypeMetadata(std::stringstream& ss) {
        std::vector<std::string> entries;
        ss << "[";

        const auto* cfg = ConfigurationManager::AutoDetectConfiguration();
        const uint64_t maxProbe = cfg ? cfg->MaxTypeProbeCount : 2000000ULL;
        const uintptr_t moduleBase = GetModuleBase();

        for (uint64_t index = 0; index <= maxProbe; ++index) {
            Il2CppClass* klass = Il2CppApiWrapper::GetTypeInfoFromIndex(static_cast<TypeDefinitionIndex>(index));
            if (!klass) continue;

            const uintptr_t addr = reinterpret_cast<uintptr_t>(klass);
            const uint64_t rva = (addr >= moduleBase) ? (addr - moduleBase) : 0ULL;
            const char* className = Il2CppApiWrapper::GetClassName(klass);

            std::stringstream entry;
            entry << "{"
                  << "\"Address\":" << std::dec << rva << ","
                  << "\"Name\":\"";
            WriteEscapedString(entry, std::string(className) + "_TypeInfo");
            entry << "\",";
            entry << "\"Signature\":\"";
            WriteEscapedString(entry, std::string(className) + "_c*");
            entry << "\"";
            entry << "}";

            entries.push_back(entry.str());
        }

        for (size_t i = 0; i < entries.size(); ++i) {
            if (i > 0) ss << ",";
            ss << entries[i];
        }
        ss << "]";
    }

    void ScriptJsonGenerator::WriteMethodRefs(std::stringstream& ss) {
        std::vector<std::string> entries;
        ss << "[";

        const auto* cfg = ConfigurationManager::AutoDetectConfiguration();
        const uint64_t maxProbe = cfg ? cfg->MaxTypeProbeCount : 2000000ULL;
        const uintptr_t moduleBase = GetModuleBase();

        for (uint64_t index = 0; index <= maxProbe; ++index) {
            Il2CppClass* klass = Il2CppApiWrapper::GetTypeInfoFromIndex(static_cast<TypeDefinitionIndex>(index));
            if (!klass) continue;

            void* methodIter = nullptr;
            while (const uint8_t* method = Il2CppApiWrapper::GetClassMethods(klass, &methodIter)) {
                const uint64_t va = ReadMethodPointerVA(method);
                const uint64_t methodRva = (va >= moduleBase) ? (va - moduleBase) : 0ULL;

                const uintptr_t methodAddr = reinterpret_cast<uintptr_t>(method);
                const uint64_t rva = (methodAddr >= moduleBase) ? (methodAddr - moduleBase) : 0ULL;

                const char* className = Il2CppApiWrapper::GetClassName(klass);
                const char* methodName = Il2CppApiWrapper::GetMethodName(method);

                std::stringstream entry;
                entry << "{"
                      << "\"Address\":" << std::dec << rva << ","
                      << "\"Name\":\"";
                WriteEscapedString(entry, std::string(className) + "_" + methodName);
                entry << "\",";
                entry << "\"MethodAddress\":" << std::dec << methodRva;
                entry << "}";

                entries.push_back(entry.str());
            }
        }

        for (size_t i = 0; i < entries.size(); ++i) {
            if (i > 0) ss << ",";
            ss << entries[i];
        }
        ss << "]";
    }
}
