#pragma once

#include <string>
#include <memory>
#include <vector>

namespace EclipseKit::Core::Interfaces {

    /// Base interface for all dumper engines
    /// Provides extensibility for different game engines and IL2CPP versions
    class IDumperEngine {
    public:
        virtual ~IDumperEngine() = default;

        /// Initialize the dumper engine with the target process
        virtual bool Initialize() = 0;

        /// Check if the engine is compatible with the current target
        virtual bool IsCompatible() const = 0;

        /// Get the total number of types available for dumping
        virtual uint64_t GetTotalTypeCount() const = 0;

        /// Dump a specific type by index
        virtual std::string DumpTypeByIndex(uint64_t index) const = 0;

        /// Get engine name and version info
        virtual std::string GetEngineInfo() const = 0;

        /// Cleanup resources
        virtual void Shutdown() = 0;
    };

    using DumperEnginePtr = std::unique_ptr<IDumperEngine>;
}
