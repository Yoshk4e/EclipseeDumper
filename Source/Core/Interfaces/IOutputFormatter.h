#pragma once

#include <string>
#include <vector>

namespace EclipseKit::Core::Interfaces {

    /// Interface for different output formats (C#, JSON, etc.)
    class IOutputFormatter {
    public:
        virtual ~IOutputFormatter() = default;

        /// Format the dumped content
        virtual std::string FormatOutput(const std::vector<std::string>& dumpedTypes) const = 0;

        /// Get the file extension for this format
        virtual std::string GetFileExtension() const = 0;

        /// Get formatter name
        virtual std::string GetFormatterName() const = 0;
    };

    using OutputFormatterPtr = std::unique_ptr<IOutputFormatter>;
}
