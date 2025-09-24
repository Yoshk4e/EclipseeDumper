#pragma once

#include <memory>
#include <string>
#include "../Interfaces/IDumperEngine.h"
#include "../Configuration/EngineConfiguration.h"

namespace EclipseKit::Core::Factory {

    class EngineFactory {
    public:
        // Create an engine instance based on configuration.EngineId
        // Returns nullptr if the engine id is unsupported.
        static std::unique_ptr<Interfaces::IDumperEngine>
        Create(const Configuration::GameConfiguration* config);
    };
}
