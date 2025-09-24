#include "pch.h"
#include "EngineFactory.h"
#include "../Configuration/EngineConfiguration.h"
#include "../../Engines/Il2Cpp/Il2CppEngine.h"

namespace EclipseKit::Core::Factory {

    std::unique_ptr<Interfaces::IDumperEngine>
    EngineFactory::Create(const Configuration::GameConfiguration* config) {
        if (!config) return nullptr;

        if (config->EngineId == "il2cpp") {
            return std::make_unique<Engines::Il2Cpp::Il2CppEngine>(config);
        }

        return nullptr;
    }
}
