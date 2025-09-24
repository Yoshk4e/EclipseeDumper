#pragma once

#include <memory>
#include <vector>
#include <string>
#include "../Core/Interfaces/IDumperEngine.h"
#include "../Core/Interfaces/IOutputFormatter.h"
#include "../Core/Interfaces/IProgressReporter.h"
#include "../Core/Configuration/EngineConfiguration.h"

namespace EclipseKit::App {

    class DumpController {
    public:
        DumpController(const Core::Configuration::GameConfiguration* config,
                       std::unique_ptr<Core::Interfaces::IDumperEngine> engine,
                       std::unique_ptr<Core::Interfaces::IOutputFormatter> formatter,
                       std::unique_ptr<Core::Interfaces::IProgressReporter> progress);

        // Run the dump process and write to the specified file path.
        // Returns true on success.
        bool Run(const std::string& outputFilePath);

    private:
        const Core::Configuration::GameConfiguration* config_;
        std::unique_ptr<Core::Interfaces::IDumperEngine> engine_;
        std::unique_ptr<Core::Interfaces::IOutputFormatter> formatter_;
        std::unique_ptr<Core::Interfaces::IProgressReporter> progress_;
    };
}
