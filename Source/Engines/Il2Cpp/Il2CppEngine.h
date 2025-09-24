#pragma once

#include <string>
#include <memory>
#include <vector>
#include "../../Core/Interfaces/IDumperEngine.h"
#include "Il2CppApiWrapper.h"

namespace EclipseKit::Engines::Il2Cpp {

    class Il2CppEngine final : public Core::Interfaces::IDumperEngine {
    public:
        explicit Il2CppEngine(const Core::Configuration::GameConfiguration* config);
        ~Il2CppEngine() override = default;

        bool Initialize() override;
        bool IsCompatible() const override;
        uint64_t GetTotalTypeCount() const override;
        std::string DumpTypeByIndex(uint64_t index) const override;
        std::string GetEngineInfo() const override;
        void Shutdown() override;

    private:
        const Core::Configuration::GameConfiguration* config_;

        uint64_t ProbeTotalTypes(uint64_t maxProbe) const;
    };

    using Il2CppEnginePtr = std::unique_ptr<Il2CppEngine>;
}
