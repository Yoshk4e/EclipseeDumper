#include "pch.h"
#include "Il2CppEngine.h"
#include "Il2CppApiWrapper.h"
#include "../../Core/Configuration/EngineConfiguration.h"
#include "Il2CppMetadataRenderer.h"
#include <windows.h>

namespace EclipseKit::Engines::Il2Cpp {

    Il2CppEngine::Il2CppEngine(const Core::Configuration::GameConfiguration* config)
        : config_(config) {}

    bool Il2CppEngine::Initialize() {
        if (!config_) return false;
        return Il2CppApiWrapper::Initialize(config_);
    }

    bool Il2CppEngine::IsCompatible() const {
        if (!config_) return false;
        for (const auto& name : config_->ModuleNames) {
            if (GetModuleHandleA(name.c_str()) != nullptr) return true;
        }
        return false;
    }

    uint64_t Il2CppEngine::ProbeTotalTypes(uint64_t maxProbe) const {
        for (uint64_t i = 0; i < maxProbe; ++i) {
            Il2CppClass* klass = nullptr;
            __try {
                klass = Il2CppApiWrapper::GetTypeInfoFromIndex(static_cast<int>(i));
            }
            __except (EXCEPTION_EXECUTE_HANDLER) {
                return i;
            }
            if (!klass) {
                return i;
            }
        }
        return maxProbe;
    }

    uint64_t Il2CppEngine::GetTotalTypeCount() const {
        return ProbeTotalTypes(config_->MaxTypeProbeCount);
    }

    std::string Il2CppEngine::DumpTypeByIndex(uint64_t index) const {
        Il2CppClass* klass = Il2CppApiWrapper::GetTypeInfoFromIndex(static_cast<TypeDefinitionIndex>(index));
        if (!klass) return std::string();

        // Render using our metadata renderer to C#-like definitions
        return RenderClassCSharp(klass);
    }

    std::string Il2CppEngine::GetEngineInfo() const {
        return std::string("Il2Cpp Engine for ") + config_->GameName + " " + config_->Version;
    }

    void Il2CppEngine::Shutdown() {
        Il2CppApiWrapper::Shutdown();
    }
}
